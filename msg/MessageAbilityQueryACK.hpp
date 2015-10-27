#ifndef MESSAGE_ABILITY_QUERY_ACK_HPP_
#define MESSAGE_ABILITY_QUERY_ACK_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageAbilityQueryACK : public Message
    {
    public:
    
        // Getter of test_100m_
        string test_100m()
        {
            return test_100m_;
        }
        
        // Setter of test_100m_
        void test_100m( string value )
        {
            test_100m_ = value;
            raw_data_[ "data" ][ "test_100m" ] = value;
        }
        
        // Serilize Constructor
        MessageAbilityQueryACK()
            : Message( PROTOCOL_VERSION , 102 , 0 )
        {
            test_100m( "Donot have a report yet!" );
        }
        
        // Deserilize Constructor
        MessageAbilityQueryACK( Message* message )
            : Message( *message )
        {
            this->test_100m_ = raw_data_[ "data" ][ "test_100m" ].get<string>();
        }
    
    private:
    
        string test_100m_;
    
    }; // End of class define of MessageAbilityQueryACK

} // End of namespace Protocol
#endif // !Message_Ability_Query_ACK_HPP_
