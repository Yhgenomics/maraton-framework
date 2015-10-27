#include "ExecutorSession.h"
#include "json.hpp"
#include "ExecutorManager.h"
#include "Executor.h"
#include "json.hpp"
#include <string.h>
#include <memory>
#include "SessionManager.hpp"

ExecutorSession::ExecutorSession(uv_tcp_t * conn)
    : ClusterSession::ClusterSession(conn)
{
}

ExecutorSession::~ExecutorSession()
{
}

void ExecutorSession::message( Message * message )
{
}

void ExecutorSession::shutdown()
{
    SessionManager<ExecutorSession>::instance()->remove( this );
}
