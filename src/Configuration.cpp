#include "Configuration.h"
#include <stdio.h>

std::string Configuration::get( std::string key )
{
    return this->config[key];
}

Configuration::Configuration()
{
    FILE* file;
    fopen_s(&file , "maraton.conf" , "r" );
    fseek( file , 0 , SEEK_END );
    int len = ftell( file );
    fseek( file , 0 , SEEK_SET );

    char* config_dat = new char[len];
    fread( config_dat , 1 , len , file );
    fclose( file );

    this->config = nlohmann::json::parse( config_dat );
}
