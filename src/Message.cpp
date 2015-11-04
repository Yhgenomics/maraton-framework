#include "Message.h"
#include "ClusterSession.h"

Message::Message(const std::string json_str )
{ 
    this->raw_data_ = nlohmann::json::parse( json_str );;

    this->status_ = this->raw_data_["status"];
    this->command_ = this->raw_data_["command"];
}

Message::Message( const std::string version, size_t command, size_t status )
{
    this->raw_data_.clear();
    this->raw_data_["command"] = command;
    this->raw_data_["status"] = status;
    this->raw_data_["version"] = version;

    this->version_ = version;
    this->status_ = status;
    this->command_ = command;
}

Message::Message( Message & message )
{
    this->raw_data_ = nlohmann::json::parse( message.raw_data_.dump() );
    this->command_ = message.command_;
    this->status_ = message.status_;
    this->owner_ = message.owner_;
}

Message::Message( Message && message )
{
    this->raw_data_ = message.raw_data_;
    this->command_ = message.command_;
    this->status_ = message.status_;
    this->owner_ = message.owner_;

    message.raw_data_ = nullptr;
    message.command_ = 0;
    message.status_ = 0;
    message.owner_ = nullptr;
}

Message& Message::operator=( Message & message )
{
    this->raw_data_ = nlohmann::json::parse( message.raw_data_.dump() );
    this->command_ = message.command_;
    this->status_ = message.status_;
    this->owner_ = message.owner_;

    return *this;
}

Message & Message::operator=( Message && message )
{
    this->raw_data_ = message.raw_data_;
    this->command_ = message.command_;
    this->status_ = message.status_;
    this->owner_ = message.owner_;

    message.raw_data_ = nullptr;
    message.command_ = 0;
    message.status_ = 0;
    message.owner_ = nullptr;

    return *this;
}

Buffer Message::bytes()
{
    Buffer buffer;
    std::string json = this->raw_data_.dump();
    buffer.raw( json.c_str(), static_cast< int >( json.length() ) );

    return buffer;
}

void Message::owner( ClusterSession * session )
{
    this->owner_ = session;
}

ClusterSession * Message::owner()
{
    return this->owner_;
}
