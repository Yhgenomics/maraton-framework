/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_ 

#include "maraton.h"
#include "json.hpp"
#include <string>

class Configuration
    : public Singleton<Configuration>
{
public:

    std::string get( std::string key );

protected:

    Configuration();
private:

    nlohmann::json config;
    friend class Singleton<Configuration>;
    
    static const int MAX_PATH_LEN = 512;
};

#endif //!CONFIGURATION_H_ 