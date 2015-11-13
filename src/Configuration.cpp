#include "Configuration.h"
#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

std::string Configuration::get( std::string key )
{
    return this->config[key];
}

Configuration::Configuration()
{
    // Magic number
    char path[MAX_PATH_LEN] = { 0 };
    getcwd( path , MAX_PATH_LEN );

    FILE* file;
    file = fopen( "maraton.conf" , "r" );

    if ( file == NULL )
    {
        Logger::error( "can't not open maraton.conf" );
        return;
    }

    fseek( file , 0 , SEEK_END );
    int len = ftell( file ) ;
    fseek( file , 0 , SEEK_SET );

    char* json_string = new char[len+1];
    memset( json_string , 0 , len + 1 );
    fread( json_string , 1 , len , file );
    fclose( file );

    this->config = nlohmann::json::parse( json_string );

    delete json_string;
}
