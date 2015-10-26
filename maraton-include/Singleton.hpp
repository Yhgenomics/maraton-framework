/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */ 

#ifndef SINGLETON_H_
#define SINGLETON_H_

template<class T>
class Singleton
{
public:
    
    static T* instance();

protected:

    ~Singleton();
    Singleton();
    static T* instance_ ;
};

template<class T>
T* Singleton<T>::instance_ = nullptr;

template<class T>
Singleton<T>::~Singleton()
{

}

template<class T>
T * Singleton<T>::instance()
{
    if ( instance_ == nullptr )
    {
        instance_ = new T();
    }

    return instance_;
}

template<class T>
Singleton<T>::Singleton()
{

}

#endif // !SINGLETON_H_


 