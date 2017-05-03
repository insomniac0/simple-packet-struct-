#ifndef PACKET_HPP_INCLUDED
#define PACKET_HPP_INCLUDED

#include "stdafx.h"
#include "fwd/packet.hpp"

enum PacketFamily UTIL_EXTEND_ENUM(unsigned char)
{
    PACKET_INTERNAL = 0,
    PACKET_CONNECTION = 1,
    PACKET_ACCOUNT = 2,
    PACKET_CHARACTER = 3,
    PACKET_LOGIN = 4
};

enum PacketAction UTIL_EXTEND_ENUM(unsigned char)
{
    PACKET_REQUEST = 1,
    PACKET_ACCEPT = 2,
    PACKET_REPLY = 3,
    PACKET_REMOVE = 4,
    PACKET_AGREE = 5,
    PACKET_CREATE = 6,
    PACKET_ADD = 7
};

class PacketInfo
{
    public:
   static std::string GetFamilyName(PacketFamily family)
{
    switch (family)
    {
    case PACKET_CONNECTION:
        return "Connection";
    case PACKET_ACCOUNT:
        return "Account";
    case PACKET_CHARACTER:
        return "Character";
    case PACKET_LOGIN:
        return "Login";
    default:
        return "UNKNOWN";
    }
}

static std::string GetActionName(PacketAction action)
{
    switch (action)
    {
    case PACKET_REQUEST:
        return "Request";
    case PACKET_ACCEPT:
        return "Accept";
    case PACKET_REPLY:
        return "Reply";
    case PACKET_REMOVE:
        return "Remove";
    case PACKET_AGREE:
        return "Agree";
    case PACKET_CREATE:
        return "Create";
    case PACKET_ADD:
        return "Add";
    default:
        return "UNKNOWN";
    }
}
};

class PacketReader
{
    public:

    std::string data;
    std::size_t length;
    PacketFamily family;
    PacketAction action;
    PacketReader(std::string data)
    {
        this->data = data;
        this->length = data.length();
    }

    std::size_t Length();
    std::size_t Remaining();
    unsigned char *GetBytes(int amount = 1);
    unsigned char GetChar();
    unsigned short GetShort();
    unsigned int GetThree();
    unsigned int GetInt();

    std::string GetFixedString(std::size_t length);
    std::string GetEndString();
    std::string GetBreakString(unsigned char breakchar = 0xFF);
    unsigned int ByteOrder(unsigned char a,unsigned char b = 0,unsigned char c = 0,unsigned char d = 0);

};

class PacketBuilder
{
    public:
    PacketFamily family;
    PacketAction action;
    std::string data;
    std::size_t size;//bytes

    PacketBuilder(PacketFamily family_, PacketAction action_) : family(family_),action(action_)
    {
        std::string wrn = "";

        if(!family)
        wrn += "Packet family is invalid ";

        if(!action)
        wrn += "Packet action is invalid ";

        this->AddChar(this->family);
        this->AddChar(this->action);

        if(!wrn.empty())
        {
            Console::Out(wrn);
            return;
        }
    }
    ~PacketBuilder();

    std::string AddString(std::string str);
    std::string AddBreakString(std::string str, unsigned char breakchar = 0xFF);
    unsigned char AddChar(unsigned char num);
    unsigned short AddShort(unsigned short num);
    unsigned int AddThree(unsigned int num);
    unsigned int AddInt(unsigned int num);
    void Reset();
    std::string Get();
    operator std::string();
    std::string ByteStream(unsigned char *bytes);
    std::string Construct(PacketFamily family,PacketAction action,std::string data);

};

#endif // PACKET_HPP_INCLUDED
