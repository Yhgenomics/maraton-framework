/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef MANAGER_HPP_
#define MANAGER_HPP_ 

#include <vector>
#include <mutex>
#include "Macros.h"

template<class T>
class Manager
{
public:

    virtual void push( T* instance )
    {
        this->instance_.push_back( instance );
    }

    virtual bool pop( T* instance )
    {
        // Rework and improve the code 
        typename std::vector<T*>::iterator itr = this->instance_.begin();

        while ( itr != this->instance_.end() )
        {
            if ( *itr == instance )
            {
                itr = this->instance_.erase( itr );
                return true;
            }
            else
            {
                itr++;
            }
        }

        return false;
    }

protected:

    std::vector<T*> instances() 
    {
        std::vector<T*> vec_return( this->instance_.begin(), this->instance_.end() );

        return vec_return;
    }

    std::vector<T*> instance_;

private:

    std::mutex mtx;
};
 
#endif //!MANAGER_HPP_ 