#ifndef MESSAGE_GREETING_HPP_
#define MESSAGE_GREETING_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageGreeting : public Message
    {
    public:
    
        // Serilize Constructor
        MessageGreeting()
            : Message( PROTOCOL_VERSION , 91 , 0 )
        {
        }
        
        // Deserilize Constructor
        MessageGreeting( Message* message )
            : Message( *message )
        {
        }
    
    private:
    
    }; // End of class define of MessageGreeting

} // End of namespace Protocol
#endif // !Message_Greeting_HPP_
