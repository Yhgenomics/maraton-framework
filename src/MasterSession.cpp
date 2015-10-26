#include "MasterSession.h"

#include <stdio.h>
#include <memory.h>
#include "Master.h"

#include "SessionManager.hpp"

MasterSession::MasterSession( uv_tcp_t * conn )
    : ClusterSession::ClusterSession( conn )
{
}

MasterSession::~MasterSession()
{
}
 
void MasterSession::close()
{
    SessionManager<MasterSession>::instance()->remove( this );
}

void MasterSession::message( Message * message )
{
    
}

void MasterSession::shutdown()
{
    SessionManager<MasterSession>::instance()->remove( this );
}
 