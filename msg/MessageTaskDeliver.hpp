#ifndef MESSAGE_TASK_DELIVER_HPP_
#define MESSAGE_TASK_DELIVER_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageTaskDeliver : public Message
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
        
        // Getter of cpu_
        string cpu()
        {
            return cpu_;
        }
        
        // Setter of cpu_
        void cpu( string value )
        {
            cpu_ = value;
            raw_data_[ "data" ][ "cpu" ] = value;
        }
        
        // Getter of ram_
        string ram()
        {
            return ram_;
        }
        
        // Setter of ram_
        void ram( string value )
        {
            ram_ = value;
            raw_data_[ "data" ][ "ram" ] = value;
        }
        
        // Getter of command_lines_
        string command_lines()
        {
            return command_lines_;
        }
        
        // Setter of command_lines_
        void command_lines( string value )
        {
            command_lines_ = value;
            raw_data_[ "data" ][ "command_lines" ] = value;
        }
        
        // Getter of uri_
        string uri()
        {
            return uri_;
        }
        
        // Setter of uri_
        void uri( string value )
        {
            uri_ = value;
            raw_data_[ "data" ][ "uri" ] = value;
        }
        
        // Serilize Constructor
        MessageTaskDeliver()
            : Message( PROTOCOL_VERSION , 131 , 0 )
        {
            task_id( "" );
            cpu( "" );
            ram( "" );
            command_lines( "" );
            uri( "" );
        }
        
        // Deserilize Constructor
        MessageTaskDeliver( Message* message )
            : Message( *message )
        {
            this->task_id_ = raw_data_[ "data" ][ "task_id" ].get<string>();
            this->cpu_ = raw_data_[ "data" ][ "cpu" ].get<string>();
            this->ram_ = raw_data_[ "data" ][ "ram" ].get<string>();
            this->command_lines_ = raw_data_[ "data" ][ "command_lines" ].get<string>();
            this->uri_ = raw_data_[ "data" ][ "uri" ].get<string>();
        }
    
    private:
    
        string task_id_;
        string cpu_;
        string ram_;
        string command_lines_;
        string uri_;
    
    }; // End of class define of MessageTaskDeliver

} // End of namespace Protocol
#endif // !Message_Task_Deliver_HPP_
