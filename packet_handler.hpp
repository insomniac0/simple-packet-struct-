#ifndef PACKET_HANDLER_HPP_INCLUDED
#define PACKET_HANDLER_HPP_INCLUDED

#include "packet.hpp"

bool HandlePackets(PacketReader reader);

bool Connection(PacketReader reader);
bool Character(PacketReader reader);
#endif // PACKET_HPP_INCLUDED
