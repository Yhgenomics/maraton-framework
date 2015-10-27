#ifndef MESSAGE_RESOURCES_QUERY_HPP_
#define MESSAGE_RESOURCES_QUERY_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageResourcesQuery : public Message
    {
    public:
    
        // Serilize Constructor
        MessageResourcesQuery()
            : Message( PROTOCOL_VERSION , 111 , 0 )
        {
        }
        
        // Deserilize Constructor
        MessageResourcesQuery( Message* message )
            : Message( *message )
        {
        }
    
    private:
    
    }; // End of class define of MessageResourcesQuery

} // End of namespace Protocol
#endif // !Message_Resources_Query_HPP_
