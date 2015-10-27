#ifndef MESSAGE_RESOURCES_QUERY_ACK_HPP_
#define MESSAGE_RESOURCES_QUERY_ACK_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageResourcesQueryACK : public Message
    {
    public:
    
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
        
        // Getter of cpufrequency_
        string cpufrequency()
        {
            return cpufrequency_;
        }
        
        // Setter of cpufrequency_
        void cpufrequency( string value )
        {
            cpufrequency_ = value;
            raw_data_[ "data" ][ "cpufrequency" ] = value;
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
        
        // Getter of gpu_
        string gpu()
        {
            return gpu_;
        }
        
        // Setter of gpu_
        void gpu( string value )
        {
            gpu_ = value;
            raw_data_[ "data" ][ "gpu" ] = value;
        }
        
        // Getter of video_ram_
        string video_ram()
        {
            return video_ram_;
        }
        
        // Setter of video_ram_
        void video_ram( string value )
        {
            video_ram_ = value;
            raw_data_[ "data" ][ "video_ram" ] = value;
        }
        
        // Serilize Constructor
        MessageResourcesQueryACK()
            : Message( PROTOCOL_VERSION , 112 , 0 )
        {
            cpu( "not clear" );
            cpufrequency( "not clear" );
            ram( "not clear" );
            gpu( "not clear" );
            video_ram( "not clear" );
        }
        
        // Deserilize Constructor
        MessageResourcesQueryACK( Message* message )
            : Message( *message )
        {
            this->cpu_ = raw_data_[ "data" ][ "cpu" ].get<string>();
            this->cpufrequency_ = raw_data_[ "data" ][ "cpufrequency" ].get<string>();
            this->ram_ = raw_data_[ "data" ][ "ram" ].get<string>();
            this->gpu_ = raw_data_[ "data" ][ "gpu" ].get<string>();
            this->video_ram_ = raw_data_[ "data" ][ "video_ram" ].get<string>();
        }
    
    private:
    
        string cpu_;
        string cpufrequency_;
        string ram_;
        string gpu_;
        string video_ram_;
    
    }; // End of class define of MessageResourcesQueryACK

} // End of namespace Protocol
#endif // !Message_Resources_Query_ACK_HPP_
