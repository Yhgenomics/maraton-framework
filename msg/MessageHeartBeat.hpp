#ifndef MESSAGE_HEART_BEAT_HPP_
#define MESSAGE_HEART_BEAT_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageHeartBeat : public Message
    {
    public:
    
        // Serilize Constructor
        MessageHeartBeat()
            : Message( PROTOCOL_VERSION , 161 , 0 )
        {
        }
        
        // Deserilize Constructor
        MessageHeartBeat( Message* message )
            : Message( *message )
        {
        }
    
    private:
    
    }; // End of class define of MessageHeartBeat

} // End of namespace Protocol
#endif // !Message_Heart_Beat_HPP_
