#ifndef MESSAGE_ABILITY_QUERY_HPP_
#define MESSAGE_ABILITY_QUERY_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageAbilityQuery : public Message
    {
    public:
    
        // Serilize Constructor
        MessageAbilityQuery()
            : Message( PROTOCOL_VERSION , 101 , 0 )
        {
        }
        
        // Deserilize Constructor
        MessageAbilityQuery( Message* message )
            : Message( *message )
        {
        }
    
    private:
    
    }; // End of class define of MessageAbilityQuery

} // End of namespace Protocol
#endif // !Message_Ability_Query_HPP_
