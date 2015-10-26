#include "Executor.h"

void Executor::run()
{
}

void Executor::message( std::string json_str )
{
    printf( "[%d] %s\r\n", this->session_->id(), json_str.c_str() );

    this->send( Message( json_str ) );
}

void Executor::send( Message message )
{
    this->session_->send( &message );
}
