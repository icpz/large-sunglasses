#include "node.h"

Node::Node() : registered(false), user_found(false), node(nullptr)
{
    connect(this, &Node::sig_registered, this, &Node::get_friends, Qt::DirectConnection);
}

Node::~Node()
{
    delete node;
}

std::string Node::getId()
{
    return this->id;
}

int Node::signup(QString pw)
{
    auto key = dht::crypto::PrivateKey::generate();
    std::vector<uint8_t> pk_s = key.serialize(pw.toStdString());

    auto cert = dht::crypto::Certificate::generate(key);
    std::vector<uint8_t> cert_s;
    cert.pack(cert_s);

    QFile file("privkey");
    if (!file.open(QIODevice::WriteOnly)) {
        return 0;
    }
    file.write((char *)&pk_s[0], pk_s.size());
    file.close();
    QFile file1("cert");
    if (!file1.open(QIODevice::WriteOnly)) {
        return 0;
    }
    file1.write ((char *)&cert_s[0], cert_s.size());
    file1.close();
    return 1;
}

int Node::checkid(QString pw)
{
    QFile file("cert");
    if (!file.open(QIODevice::ReadOnly)) {
        return 0;
    }
    QByteArray bytes = file.readAll();
    std::vector<uint8_t> data;
    data.resize(bytes.length());
    memcpy(&data[0], bytes.data(), bytes.length());
    this->cert = std::make_shared<dht::crypto::Certificate>(data);
    file.close();
    QFile file1("privkey");
    if (!file1.open(QIODevice::ReadOnly)) {
        return 0;
    }
    bytes = file1.readAll();
    file1.close();
    data.resize(bytes.length());
    memcpy(&data[0], bytes.data(), bytes.length());
    try {
        this->pk = std::make_shared<dht::crypto::PrivateKey>(data, pw.toStdString());
        return 1;
    } catch(std::exception) {
        return 0;
    }
}


void Node::conn()
{
    node = new dht::DhtRunner();

    node->run(4222, /*dht::crypto::Identity(this->pk, this->cert)*/dht::crypto::generateIdentity(), true);
    node->bootstrap(BOOTSTRAP, "4222");
    id = node->getId().toString();
    qDebug("%s", id.data());

    node->get("key_users",
              [this](const std::vector<std::shared_ptr<dht::Value>>& values) {
                  int found = 0;
                  for (const auto &value : values) {
                      if (value->data.size() != this->id.size()) {
                          continue;
                      }
                      int i;
                      for (i = 0; i < this->id.size() && value->data[i] == this->id[i]; i++);
                      if (i == this->id.size()) {
                          found = 1;
                          break;
                      }
                  }
                  if (found) {
                      this->registered = true;
                      return false;
                  }

                  return true;
              },
              [this](bool) {
                  if (this->registered) {
                      emit this->sig_registered(true);
                  } else {
                      emit this->sig_registered(false);
                  }
    });

}

void Node::get_friends(bool b)
{
    if (!b) {
        node->put("key_users", dht::Value(4, reinterpret_cast<const uint8_t*>(this->id.c_str()), this->id.size()), {}, {}, true);
    } else {
        node->get("key_friends",
                  [this](const std::vector<std::shared_ptr<dht::Value>>& values) {
                      for (const auto &value : values) {
                          int i;
                          for (i = 0; i < 40 && value->data[i] == this->id[i]; i++);
                          if (i == 40) {
                              this->friends.insert(this->friends.end(), std::string(value->data.begin() + 40, value->data.end()));
                          }
                          for (i = 40; i < 80 && value->data[i] == this->id[i - 40]; i++);
                          if (i == 80) {
                              this->friends.insert(this->friends.end(), std::string(value->data.begin(), value->data.begin() + 40));
                          }
                      }
                      return true;
                  },
            [this](bool ok) {
                emit sig_friends(ok);
            }
        );
    }
}

void Node::search_user(QString uid)
{
    node->get("key_users",
              [uid, this](const std::vector<std::shared_ptr<dht::Value>>& values) {
                  int found = 0;
                  for (const auto &value : values) {
                      if (value->data.size() != uid.toStdString().size()) {
                          continue;
                      }
                      int i;
                      for (i = 0; i < this->id.size() && value->data[i] == uid.toStdString()[i]; i++);
                      if (i == this->id.size()) {
                          found = 1;
                          break;
                      }
                  }
                  if (found) {
                      this->user_found = true;
                      return false;
                  }

                  return true;
              },
              [this, &uid](bool) {
                  if (this->user_found) {
                      this->user_found = false;
                      emit this->sig_user_found(true, uid.toStdString());
                  } else {
                      emit this->sig_user_found(false, uid.toStdString());
                  }
    });
}

void Node::add_friend(QString uid)
{
    std::string s = uid.toStdString() + this->id;
    node->put("key_friends",
              dht::Value(4, reinterpret_cast<const uint8_t*>(s.c_str()), s.size()),
              [this, &uid](bool ok) {
                  if (ok) {
                      this->friends.insert(this->friends.end(), uid.toStdString());
                  }
                  emit this->sig_addFriend(ok);
              },
              {},
              true
    );
}

void Node::sendMessage(std::string uid, QString m)
{
    node->putEncrypted(dht::InfoHash::get(uid), dht::InfoHash(uid), dht::ImMessage(rand(), m.toStdString()), [this](bool ok) {
        if (not ok) {
            emit this->sig_send_message(false);
        } else {
            emit this->sig_send_message(true);
        }
    });
}

void Node::listenMessage()
{
    node->listen<dht::ImMessage>(dht::InfoHash::get(this->id), [this](dht::ImMessage&& msg) {
        emit this->sig_recv_msg(std::pair<dht::InfoHash, std::string>(msg.from, msg.msg));
        return true;
    });
}
