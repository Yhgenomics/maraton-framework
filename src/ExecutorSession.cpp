#include "ExecutorSession.h"
#include <string.h>
#include <memory>

#include "json.hpp"
#include "SessionManager.hpp"
#include "Message.h"
#include "MessagesHandler.hpp"

ExecutorSession::ExecutorSession(uv_tcp_t * conn)
    : ClusterSession::ClusterSession(conn)
{
}

ExecutorSession::~ExecutorSession()
{
}

void ExecutorSession::message( Message * message )
{
    Protocol::MessagesHandler::process( message );
}

void ExecutorSession::shutdown()
{
    SessionManager<ExecutorSession>::instance()->remove( this );
}
