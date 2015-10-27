#ifndef MESSAGE_EXCEPTION_REPORT_HPP_
#define MESSAGE_EXCEPTION_REPORT_HPP_

#include "Message.h"
#include "json.hpp"
#include <string>
using namespace std;

namespace Protocol
{
    class MessageExceptionReport : public Message
    {
    public:
    
        // Getter of exception_content_
        string exception_content()
        {
            return exception_content_;
        }
        
        // Setter of exception_content_
        void exception_content( string value )
        {
            exception_content_ = value;
            raw_data_[ "data" ][ "exception_content" ] = value;
        }
        
        // Serilize Constructor
        MessageExceptionReport()
            : Message( PROTOCOL_VERSION , 151 , 0 )
        {
            exception_content( "" );
        }
        
        // Deserilize Constructor
        MessageExceptionReport( Message* message )
            : Message( *message )
        {
            this->exception_content_ = raw_data_[ "data" ][ "exception_content" ].get<string>();
        }
    
    private:
    
        string exception_content_;
    
    }; // End of class define of MessageExceptionReport

} // End of namespace Protocol
#endif // !Message_Exception_Report_HPP_
