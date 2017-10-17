#include "msglist.h"

MsgList::MsgList(std::string u) : user(u)
{

}

void MsgList::setUser(QString u)
{
    this->user = u.toStdString();
}

void MsgList::add_msg(QString msg, QString date)
{
    Msg m(msg.toStdString(), date.toStdString());
    msgs.push_back(m);
}

std::string MsgList::getUser()
{
    return user;
}

std::vector<MsgList::Msg> MsgList::getMsgs()
{
    return msgs;
}
