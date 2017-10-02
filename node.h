#ifndef NODE_H
#define NODE_H

#include <opendht.h>
#include <QString>
#include <QFile>
#include <QDebug>

class Node
{
public:
    Node();
    ~Node();
    static int signup(QString);
    int checkid(QString);

private:
    dht::crypto::Identity *identity;
};

#endif // NODE_H
