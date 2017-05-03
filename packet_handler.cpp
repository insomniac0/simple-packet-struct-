#include "packet_handler.hpp"

bool HandlePackets(PacketReader reader)
{
    bool handled_packet = false;

    if(!reader.Remaining())
    {
        //err msg
        return handled_packet;
    }

    handled_packet = Connection(reader);
    handled_packet = (handled_packet ? true : Character(reader));

    return handled_packet;
}

bool Connection(PacketReader reader)
{
    if(reader.family != PACKET_CONNECTION)
    {
        //err msg
        return false;
    }

    switch(reader.action)
    {
        case PACKET_ADD:
        {
            Console::Out("hi");
        }
        break;

        default: return false;
    }

    return true;
}

bool Character(PacketReader reader)
{
    if(reader.family != PACKET_CHARACTER)
    {
        //err msg
        return false;
    }

    switch(reader.action)
    {
        case PACKET_ADD:
        {
            Console::Out("hi");
        }
        break;

        default: return false;
    }

    return true;
}
