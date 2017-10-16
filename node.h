#ifndef NODE_H
#define NODE_H

#include <opendht.h>
#include <QString>
#include <QFile>
#include <QObject>
#include <cstdlib>

#define BOOTSTRAP "192.168.98.194"

class Node: public QObject
{
    Q_OBJECT

public:
    Node();
    ~Node();
    std::string getId();
    static int signup(QString);
    int checkid(QString);
    void conn();
    void search_user(QString);
    void add_friend(QString);
    void sendMessage(std::string, QString);
    void listenMessage();
    std::vector<std::string> g_friends();

private:
    dht::DhtRunner *node;
    std::string id;
    std::shared_ptr<dht::crypto::PrivateKey> pk;
    std::shared_ptr<dht::crypto::Certificate> cert;
    bool registered;
    bool user_found;
    std::vector<std::string> friends;

signals:
    void sig_registered(bool);
    void sig_friends(bool);
    void sig_user_found(bool, QString);
    void sig_addFriend(bool);
    void sig_send_message(bool);
    void sig_recv_msg(std::pair<dht::InfoHash, std::string>);

private slots:
    void get_friends(bool);
};

#endif // NODE_H
