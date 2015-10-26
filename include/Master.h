/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */

#ifndef MASTER_H_
#define MASTER_H_

#include "MasterSession.h"
#include "maraton.h"
#include "Message.h"

class Master
{
public:

    Master( MasterSession* session );

    MasterSession* session() { return this->session_; };

    void run();
    void message( std::string json_str );
    void send( Message* message );

private:

    MasterSession* session_;
};

#endif // !MASTER_H_