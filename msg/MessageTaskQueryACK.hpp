#ifndef MESSAGE_TASK_QUERY_ACK_HPP_
#define MESSAGE_TASK_QUERY_ACK_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageTaskQueryACK : public Message
    {
    public:
    
        // Getter of task_id_
        string task_id()
        {
            return task_id_;
        }
        
        // Setter of task_id_
        void task_id( string value )
        {
            task_id_ = value;
            raw_data_[ "data" ][ "task_id" ] = value;
        }
        
        // Getter of task_status_
        string task_status()
        {
            return task_status_;
        }
        
        // Setter of task_status_
        void task_status( string value )
        {
            task_status_ = value;
            raw_data_[ "data" ][ "task_status" ] = value;
        }
        
        // Serilize Constructor
        MessageTaskQueryACK()
            : Message( PROTOCOL_VERSION , 122 , 0 )
        {
            task_id( "" );
            task_status( "" );
        }
        
        // Deserilize Constructor
        MessageTaskQueryACK( Message* message )
            : Message( *message )
        {
            this->task_id_ = raw_data_[ "data" ][ "task_id" ].get<string>();
            this->task_status_ = raw_data_[ "data" ][ "task_status" ].get<string>();
        }
    
    private:
    
        string task_id_;
        string task_status_;
    
    }; // End of class define of MessageTaskQueryACK

} // End of namespace Protocol
#endif // !Message_Task_Query_ACK_HPP_
