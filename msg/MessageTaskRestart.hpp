#ifndef MESSAGE_TASK_RESTART_HPP_
#define MESSAGE_TASK_RESTART_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageTaskRestart : public Message
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
        
        // Serilize Constructor
        MessageTaskRestart()
            : Message( PROTOCOL_VERSION , 145 , 0 )
        {
            task_id( "" );
        }
        
        // Deserilize Constructor
        MessageTaskRestart( Message* message )
            : Message( *message )
        {
            this->task_id_ = raw_data_[ "data" ][ "task_id" ].get<string>();
        }
    
    private:
    
        string task_id_;
    
    }; // End of class define of MessageTaskRestart

} // End of namespace Protocol
#endif // !Message_Task_Restart_HPP_
