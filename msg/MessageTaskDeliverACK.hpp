#ifndef MESSAGE_TASK_DELIVER_ACK_HPP_
#define MESSAGE_TASK_DELIVER_ACK_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageTaskDeliverACK : public Message
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
        
        // Getter of result_
        string result()
        {
            return result_;
        }
        
        // Setter of result_
        void result( string value )
        {
            result_ = value;
            raw_data_[ "data" ][ "result" ] = value;
        }
        
        // Serilize Constructor
        MessageTaskDeliverACK()
            : Message( PROTOCOL_VERSION , 132 , 0 )
        {
            task_id( "" );
            result( "" );
        }
        
        // Deserilize Constructor
        MessageTaskDeliverACK( Message* message )
            : Message( *message )
        {
            this->task_id_ = raw_data_[ "data" ][ "task_id" ].get<string>();
            this->result_ = raw_data_[ "data" ][ "result" ].get<string>();
        }
    
    private:
    
        string task_id_;
        string result_;
    
    }; // End of class define of MessageTaskDeliverACK

} // End of namespace Protocol
#endif // !Message_Task_Deliver_ACK_HPP_
