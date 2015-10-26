/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef UUID_HPP_
#define UUID_HPP_

#include <random>
#include <string>

using namespace std;

class UUID
{
public:

    static string create() 
    {
        char buf[UUID_LEN];
        const char *c = "yhgenomics";
        char *p = buf;
        int n;

        for ( n = 0; n < 16; ++n )
        {
            int seed = rand() % 255;
            switch ( n )
            {
            case 6:
                sprintf( p, "4%x", seed % 15 );
                break;

            case 8:
                sprintf( p, "%c%x", c[rand() % strlen( c )], seed % 15 );
                break;

            default:
                sprintf( p, "%02x", seed );
                break;
            }

            p += 2;
        }

        *p = 0;
        return string( buf );
    } 

private:

    const static int UUID_LEN = 37;

    UUID() {};
    ~UUID() {};
};

#endif // !UUID_HPP_