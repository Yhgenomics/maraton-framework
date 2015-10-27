#include "MasterSession.h"

#include <stdio.h>
#include <memory.h>

#include "SessionManager.hpp"
#include "Message.h"
#include "MessagesHandler.hpp"

MasterSession::MasterSession( uv_tcp_t * conn )
    : ClusterSession::ClusterSession( conn )
{
}

MasterSession::~MasterSession()
{
}
  
void MasterSession::message( Message * message )
{
    Protocol::MessagesHandler::process( message );
}

void MasterSession::shutdown()
{
    SessionManager<MasterSession>::instance()->remove( this );
}
 