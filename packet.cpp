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
    this->data += this->ByteStream(byte);

    return num;
}

unsigned short PacketBuilder::AddShort(unsigned short num)
{
    unsigned char bytes[2];
    bytes[0] = num & 0xFF;
    bytes[1] = (num >> 8) & 0xFF;

    //WML_Console::Out(" SHORT %d ", (bytes[1] << CHAR_BIT) | bytes[0]);   //this works to get the short out of array

    this->size += 2;
    //this->data += (bytes[1] << CHAR_BIT) | bytes[0];
    this->data += this->ByteStream(bytes);
    return num;
}

unsigned int PacketBuilder::AddThree(unsigned int num)
{
    unsigned char bytes[3];
    bytes[0] = num & 0xFF;
    bytes[1] = (num >> 8) & 0xFF;
    bytes[2] = (num >> 16) & 0xFF;

    this->size += 3;
    this->data += this->ByteStream(bytes);

    return num;
}

unsigned int PacketBuilder::AddInt(unsigned int num)
{
    unsigned char bytes[4];

    bytes[0] = num & 0xFF;
    bytes[1] = (num >> 8) & 0xFF;
    bytes[2] = (num >> 16) & 0xFF;
    bytes[3] = (num >> 24) & 0xFF;

    this->size += 4;
    this->data += this->ByteStream(bytes);

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

PacketBuilder::operator std::string()
{
	return this->Get();
}

std::string PacketBuilder::ByteStream(unsigned char *bytes)
{
    std::stringstream byte_stream;
    byte_stream << bytes;

    return byte_stream.str();
}

unsigned char PacketReader::GetChar()
{
	unsigned char ret = 0;

	if (this->data.length() < 1)
	{
		return ret;
	}

    ret = this->data[0];
	this->data.erase(0, 1);

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
    return ((d << 24) | (c << 16) | (b << 8) | (a));
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
	    ret = this->ByteOrder(this->data[0]);
	    this->data.erase(0, 1);
		return ret;
	}

    //ret =  ((unsigned char)this->data[1] << 8) | (unsigned char)this->data[0]; // WORKS HERE
    ret = this->ByteOrder(this->data[0],this->data[1]);


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
         ret = this->ByteOrder(this->data[0]);
		this->data.erase(0, 1);
		return ret;
	}

	if (this->data.length() < 3)
	{
         ret = this->ByteOrder(this->data[0],this->data[1]);
		this->data.erase(0, 2);
		return ret;
	}

    ret = this->ByteOrder(this->data[0],this->data[1],this->data[2]);
	this->data.erase(0, 3);

	return ret;
}

unsigned int PacketReader::GetInt()
{
	unsigned int ret = 0;

	if (this->data.length() < 1)
	{
		return 0;
	}

	if (this->data.length() < 2)
	{
		 ret = this->ByteOrder(this->data[0]);
		this->data.erase(0, 1);
		return ret;
	}

	if (this->data.length() < 3)
	{
         ret = this->ByteOrder(this->data[0],this->data[1]);
		this->data.erase(0, 2);
		return ret;
	}

	if (this->data.length() < 4)
	{
        ret = this->ByteOrder(this->data[0],this->data[1],this->data[2]);
		this->data.erase(0, 3);
		return ret;
	}

    ret = this->ByteOrder(this->data[0],this->data[1],this->data[2],this->data[3]);
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

unsigned char PacketReader::PeekChar()
{
	unsigned char ret;

	if (this->data.length() < 1)
	{
		return 0;
	}

    ret = (ret << 8) + this->data[0];

	return ret;
}

unsigned short PacketReader::PeekShort()
{
	unsigned short ret = 0;

	if (this->data.length() < 1)
	{
		return ret;
	}

	if (this->data.length() < 2)
	{
        ret = (ret << 8) + this->data[0];
		this->data.erase(0, 1);
		return ret;
	}

    ret = (ret << 8) + this->data[1];
    ret = (ret << 8) + this->data[0];

	return ret;
}

unsigned int PacketReader::PeekThree()
{
	unsigned int ret = 0;

	if (this->data.length() < 1)
	{
		return ret;
	}

	if (this->data.length() < 2)
	{
        ret = (ret << 8) + this->data[0];

		this->data.erase(0, 1);
		return ret;
	}

	if (this->data.length() < 3)
	{
        ret = (ret << 8) + this->data[1];
        ret = (ret << 8) + this->data[0];
		this->data.erase(0, 2);
		return ret;
	}

    ret = (ret << 8) + this->data[2];
    ret = (ret << 8) + this->data[1];
    ret = (ret << 8) + this->data[0];

	return ret;
}

unsigned int PacketReader::PeekInt()
{
	unsigned int ret = 0;

	if (this->data.length() < 1)
	{
		return 0;
	}

	if (this->data.length() < 2)
	{
        ret = (ret << 8) + this->data[0];
		this->data.erase(0, 1);
		return ret;
	}

	if (this->data.length() < 3)
	{
        ret = (ret << 8) + this->data[1];
        ret = (ret << 8) + this->data[0];
        this->data.erase(0, 2);
		return ret;
	}

	if (this->data.length() < 4)
	{
        ret = (ret << 8) + this->data[2];
        ret = (ret << 8) + this->data[1];
        ret = (ret << 8) + this->data[0];
		this->data.erase(0, 3);
		return ret;
	}

	ret = (ret << 8) + this->data[3];
    ret = (ret << 8) + this->data[2];
    ret = (ret << 8) + this->data[1];
    ret = (ret << 8) + this->data[0];

	return ret;
}

PacketBuilder::~PacketBuilder()
{
    //delete this;
}
