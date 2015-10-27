/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef MESSAGE_H_
#define MESSAGE_H_ 

#define PROTOCOL_VERSION "0.0.1"

#include <string.h>
#include "maraton.h"
class ClusterSession;

class Message
{
public :

    //deserilize
    Message( const std::string json_str );

    //serilize
    Message( const std::string version , size_t command, size_t status);

    Message( Message& message);
    Message( Message&& message );

    Message& operator = ( Message& message );
    Message& operator = ( Message&& message );

    virtual ~Message() {};

    virtual Buffer bytes();

    size_t command() { return this->command_; };
    size_t status() { return this->status_; };
    std::string version() { return this->version_; }

    nlohmann::basic_json<>* data() { return &this->raw_data_; };

    void owner( ClusterSession * session );
    ClusterSession* owner();

protected:

    std::string version_;
    size_t command_;
    size_t status_;

    ClusterSession* owner_ = nullptr;

    nlohmann::json raw_data_;

};

#endif //!MESSAGE_H_ 