#ifndef MSGLIST_H
#define MSGLIST_H

#include <QString>
#include <vector>

class MsgList
{
public:
    class Msg
    {
    public:
        Msg(std::string msg, std::string date) : msg(msg), date(date) {}
        std::string msg;
        std::string date;
    };

    MsgList(std::string);
    void setUser(QString);
    void add_msg(QString, QString);
    std::string getUser();
    std::vector<Msg> getMsgs();


private:
    std::string user;
    std::vector<Msg> msgs;
};

#endif // MSGLIST_H
