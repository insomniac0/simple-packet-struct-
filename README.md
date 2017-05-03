# simple-packet-struct-
simple packet structure can be reused in anything from simple chat to mmo's

This is a simple yet very effective packet structure will fit most server client situations. 
tested on windows 7,8,10. 

EX: 
PacketReader reader(string data recieved from socket);
PacketFamily family = static_cast<PacketFamily>(reader.GetChar());
PacketAction action = static_cast<PacketAction>(reader.GetChar());
//ex of functions just make sure you know what packet structure should be recieved in that family,action
std::string mystring = reader.GetBreakString();
char mychar = reader.GetChar();
short myshort = reader.GetShort();
int myint = reader.GetInt();

EX:
PacketBuilder packet(PACKET_CONNETION,PACKET_ADD);//family,action
packet.AddChar(char value);
packet.AddShort(short value);
packet.AddThree(int 3 byte value);
packetAddInt(int 4 byte value);
Send(packet,socket);
//create a Send() function here is an example useing SDL_net

int ServerSocket::Send(PacketBuilder builder,TCPsocket s)
{
    std::string data = builder.Get();
    return SDLNet_TCP_Send(s,data.c_str(), data.length()+1);
}

EX:
PacketInfo info;
std::string family = info.GetFamilyName(static_cast<PacketFamily>(unsigned char value));
std::string action = info.GetActionName(static_cast<PacketAction>(unsigned char value));
