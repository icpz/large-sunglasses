#include "node.h"

Node::Node()
{

}

Node::~Node()
{
    delete identity;
}

int Node::signup(QString pw)
{
    auto key = std::make_shared<dht::crypto::PrivateKey>(dht::crypto::PrivateKey::generateEC());
    auto cert = std::make_shared<dht::crypto::Certificate>(dht::crypto::Certificate::generate(*key));
    dht::crypto::Identity iden = {std::move(key), std::move(cert)};

    int len = sizeof(iden);
    std::vector<uint8_t> data;
    data.resize(len);
    memcpy(&data[0], &iden, len);
    std::vector<uint8_t> crypted = dht::crypto::aesEncrypt(data, pw.toStdString());
    QFile file("identity");
    if (!file.open(QIODevice::WriteOnly)) {
        return 0;
    }
    file.write((char *)&crypted[0], crypted.size());
    file.close();
    return 1;
}

int Node::checkid(QString pw)
{
    QFile file("identity");
    if (!file.open(QIODevice::ReadOnly)) {
        return 0;
    }
    QByteArray bytes = file.readAll();
    std::vector<uint8_t> data;
    data.resize(bytes.length());
    memcpy(&data[0], bytes.data(), bytes.length());
    try {
        std::vector<uint8_t> dec = dht::crypto::aesDecrypt(data, pw.toStdString());
        char *ptr = static_cast<char *>(malloc(sizeof(dht::crypto::Identity)));
        for (int i = 0; i < sizeof(dht::crypto::Identity); i++) {
            ptr[i] = dec[i];
        }
        this->identity = reinterpret_cast<dht::crypto::Identity *>(ptr);
        return 1;
    } catch(std::exception) {
        return 0;
    }
}
