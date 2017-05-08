#include "packet.hpp"

std::string PacketBuilder::AddString(std::string str)
{
    this->size += str.length();
    this->data += str;

    return str;
}

std::string PacketBuilder::AddBreakString(std::string str, unsigned char breakchar)
{
	std::string tempstr(str);
	std::size_t breakin = tempstr.find_first_of(breakchar);
	while (breakin != std::string::npos)
	{
		tempstr[breakin] = 'y';
		breakin = tempstr.find_first_of(breakchar, breakin+1);
	}

	this->size += tempstr.length()+1;
	this->data += tempstr;
	this->data += breakchar;

	return str;
}

unsigned char PacketBuilder::AddChar(unsigned char num)
{
    unsigned char byte[1];
    byte[0] = num & 0xFF;

    this->size++;
    this->data += byte[0];

    return num;
}

unsigned short PacketBuilder::AddShort(unsigned short num)
{
    unsigned char bytes[2];
    bytes[0] = num & 0xFF;
    bytes[1] = (num >> 8) & 0xFF;

    this->size += 2;
    this->data += bytes[0];
    this->data += bytes[1];
    return num;
}

unsigned int PacketBuilder::AddThree(unsigned int num)
{
    unsigned char bytes[3];
    bytes[0] = num & 0xFF;
    bytes[1] = (num >> 8) & 0xFF;
    bytes[2] = (num >> 16) & 0xFF;

    this->size += 3;
    this->data += bytes[0];
    this->data += bytes[1];
    this->data += bytes[2];

    return num;
}

unsigned int PacketBuilder::AddInt(unsigned int num)
{
    unsigned char bytes[4] = {0xff};

    bytes[0] = num & 0xff;
    bytes[1] = (num >> 8) & 0xff;
    bytes[2] = (num >> 16) & 0xff;
    bytes[3] = (num >> 24) & 0xff;

    this->size += 4;
    this->data += bytes[0];
    this->data += bytes[1];
    this->data += bytes[2];
    this->data += bytes[3];

    return num;
}

void PacketBuilder::Reset()
{
	this->size = 0;
	this->data.erase();
}

std::string PacketBuilder::Get()
{
    return this->Construct(this->family,this->action,this->data);
}

std::string PacketBuilder::Construct(PacketFamily family,PacketAction action,std::string data)
{
    if(!family || !action || data.empty()) return "";

    return data;
}

unsigned char PacketReader::GetChar()
{
	unsigned char ret = 0;

	if (this->data.length() < 1)
	{
		return ret;
	}

    ret = this->ByteOrder(this->data[0]);
	this->data.erase(0,1);

	return ret;
}

unsigned char *PacketReader::GetBytes(int amount)
{
    static unsigned char ret[4];

    for(int i = 0;i < amount; ++i)
    {
        if (this->data.length() < 1) return ret;

        ret[i] = this->GetChar();
    }

    return ret;
}

unsigned int PacketReader::ByteOrder(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
    unsigned int ret = ((unsigned int) a) << 24;
  ret |= ((unsigned int) c) << 16;
  ret |= ((unsigned int) b) << 8;
  ret |= ((unsigned int) a);
    //return ((d << 24) | (c << 16) | (b << 8) | a);
    return ret;
}
unsigned short PacketReader::GetShort()
{
    unsigned short ret = 0;

    if (this->data.length() < 1)
	{
		return ret;
	}

	if (this->data.length() < 2)
	{
	    //ret = (unsigned char)this->data[0];
	    ret = (unsigned char)this->ByteOrder(this->data[0]);
	    this->data.erase(0, 1);
		return ret;
	}

    //ret =  ((unsigned char)this->data[1] << 8) | (unsigned char)this->data[0]; // WORKS HERE
    ret = (unsigned short)this->ByteOrder(this->data[0],this->data[1]);


	this->data.erase(0, 2);

	return ret;
}

unsigned int PacketReader::GetThree()
{
	unsigned int ret = 0;

	if (this->data.length() < 1)
	{
		return 0;
	}

	if (this->data.length() < 2)
	{
         ret = (unsigned char)this->ByteOrder(this->data[0]);
		this->data.erase(0, 1);
		return ret;
	}

	if (this->data.length() < 3)
	{
         ret = (unsigned short)this->ByteOrder(this->data[0],this->data[1]);
		this->data.erase(0, 2);
		return ret;
	}

    ret = (unsigned int)this->ByteOrder(this->data[0],this->data[1],this->data[2]);
	this->data.erase(0, 3);

	return ret;
}

unsigned int PacketReader::GetInt()
{
	unsigned int ret = 0;

Console::Out("1");
	if (this->data.length() < 1)
	{
		return ret;
	}
Console::Out("2");
	if (this->data.length() < 2)
	{
		 ret = (unsigned char)this->ByteOrder(this->data[0]);
		this->data.erase(0, 1);
		return ret;
	}
Console::Out("3");
	if (this->data.length() < 3)
	{
         ret = (unsigned short)this->ByteOrder(this->data[0],this->data[1]);
		this->data.erase(0, 2);
		return ret;
	}
Console::Out("4");
	if (this->data.length() < 4)
	{
        ret = (unsigned int)this->ByteOrder(this->data[0],this->data[1],this->data[2]);
		this->data.erase(0, 3);
		return ret;
	}

    ret = (unsigned int)this->ByteOrder(this->data[0],this->data[1],this->data[2],this->data[3]);
	this->data.erase(0, 4);

	return ret;
}

std::string PacketReader::GetEndString()
{
	std::string ret = this->data;

	this->data.erase();

	return ret;
}

std::string PacketReader::GetBreakString(unsigned char breakchar)
{
	std::string ret;
	std::size_t length;

	length = this->data.find_first_of(breakchar);

	if (length == std::string::npos)
	{
		return ret;
	}

	ret = this->data.substr(0, length);
	this->data.erase(0, length+1);

	return ret;
}

std::size_t PacketReader::Length()
{
	return this->length;
}

std::size_t PacketReader::Remaining()
{
	return this->data.length();
}

PacketBuilder::~PacketBuilder()
{
    //delete this;
}
