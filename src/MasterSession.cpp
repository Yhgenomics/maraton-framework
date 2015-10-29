#include "MasterSession.h"

#include <stdio.h>
#include <memory.h>

#include "SessionManager.hpp"
#include "Message.h" 

MasterSession::MasterSession( uv_tcp_t * conn )
    : ClusterSession::ClusterSession( conn )
{
}

MasterSession::~MasterSession()
{
}
  
void MasterSession::message( Message * message )
{
}

void MasterSession::shutdown()
{
    ClusterSession::shutdown();
    SessionManager<MasterSession>::instance()->remove( this );
}