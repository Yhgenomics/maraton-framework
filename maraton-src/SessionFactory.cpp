#include "SessionFactory.h"
#include "MasterSession.h"
#include "ExecutorSession.h"
#include "HTTPSession.h"
#include "Executor.h"

Session * SessionFactory::create( uv_tcp_t* conn , int port )
{
    switch ( port )
    {
    case MASTER:
        return new MasterSession( conn );
        break;

    case RESTAPI:
        return new HTTPSession( conn );
        break;

    case EXECUTOR:
        return new ExecutorSession( conn );
        break;

    default:
        break;
    }
    
    return nullptr;
}

SessionFactory::SessionFactory()
{
}

SessionFactory::~SessionFactory()
{
}
