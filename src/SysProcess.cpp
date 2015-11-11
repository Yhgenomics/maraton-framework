#include "SysProcess.h"
#include <thread>
#include <string.h>
#include <memory>
#include <thread>

#include "maraton.h"

SysProcess::~SysProcess()
{

    if ( this->file_ != NULL )
    {
        delete this->file_;
        this->file_ = NULL;
    }

    if ( this->args_ != NULL )
    {
        delete this->args_;
        this->args_ = NULL;
    }

    if ( this->directory_ != NULL )
    {
        delete this->directory_;
        this->directory_ = NULL;
    }
    
}
 
void SysProcess::desctroy( SysProcess ** process )
{
    if ( process != nullptr && *process != nullptr )
    {
        delete *process;
        *process = nullptr;
    }
}

SysProcess * SysProcess::create( std::string file , std::string args , std::string directry , prceoss_callback_t on_finish )
{
    auto ret = new SysProcess( file , args , directry , on_finish );
    return ret;
}

SysProcess * SysProcess::create( std::string file , std::string args , prceoss_callback_t on_finish )
{
    auto ret = new SysProcess( file , args ,  on_finish );
    return ret;
}

SysProcess * SysProcess::create( std::string file , prceoss_callback_t on_finish )
{
    auto ret = new SysProcess( file , on_finish );
    return ret;
}  

void SysProcess::uv_process_exit_callback( uv_process_t * process , int64_t exit_status , int term_signal )
{
    SysProcess* instance = static_cast< SysProcess* >( process->data );
    uv_close( ( uv_handle_t* )process , SysProcess::uv_process_close_callback );
    instance->callback( instance , exit_status ); 
}

void SysProcess::uv_process_close_callback( uv_handle_t * handle )
{
    SysProcess* instance = static_cast< SysProcess* >( handle->data );
    SAFE_DELETE( instance );
}

void SysProcess::uv_process_alloc_buffer( uv_handle_t * handle , size_t suggested_size , uv_buf_t * buf )
{
    *buf = uv_buf_init( ( char* )malloc( suggested_size ) , suggested_size );
}

void SysProcess::uv_prcoess_read_stream( uv_stream_t * stream , ssize_t nread , const uv_buf_t * buf )
{
    SysProcess* inst = static_cast< SysProcess* > ( stream->data );

    if ( nread < 0 )
    {
        if ( nread == UV_EOF )
        {
            // end of file
            uv_close( ( uv_handle_t * )&inst->pipe_ , NULL );
        }

        return;
    } 

    inst->std_out_ = std::string( buf->base , nread );

    if ( buf->base )
        delete buf->base;
}

SysProcess::SysProcess()
{
    //uv_sem_init( &this->sem , 0 );
}

SysProcess::SysProcess( std::string  file, std::string args, std::string  directry, prceoss_callback_t on_finish )
    : SysProcess()
{
    std::string newArgs = " " +  args;

    this->file_ = new char[this->STR_LENGTH];
    this->args_ = new char[this->STR_LENGTH];
    this->directory_ = new char[this->STR_LENGTH];
    memset( this->file_, 0, this->STR_LENGTH );
    memset( this->args_, 0, this->STR_LENGTH );
    memset( this->directory_, 0, this->STR_LENGTH );

    memcpy( this->file_, file.c_str() , file.length() );
    memcpy( this->args_, newArgs.c_str(), newArgs.length() );
    memcpy( this->directory_, directry.c_str(), directry.length() );
    this->callback = on_finish;
}

SysProcess::SysProcess( std::string  file, std::string args, prceoss_callback_t on_finish )
    : SysProcess()
{
    std::string newArgs = " " + args;

    this->file_ = new char[this->STR_LENGTH];
    this->args_ = new char[this->STR_LENGTH]; 
    memset( this->file_, 0, this->STR_LENGTH );
    memset( this->args_, 0, this->STR_LENGTH ); 

    memcpy( this->file_, file.c_str(), file.length() );
    memcpy( this->args_, newArgs.c_str(), newArgs.length() );

    this->callback = on_finish; 
}

SysProcess::SysProcess( std::string  file, prceoss_callback_t on_finish )
    : SysProcess()
{
    this->file_ = new char[this->STR_LENGTH]; 
    memset( this->file_, 0, this->STR_LENGTH );

    memcpy( this->file_, file.c_str(), file.length() ); 
    this->callback = on_finish;  
}

void SysProcess::invoke()
{ 
    int r;
    char** args = nullptr;

    if ( args_ == nullptr )
    {
        args = new char*[1];
        args[0] = NULL;
    } 
    else
    {
        args = new char*[128];
        for ( size_t i = 0; i < 128; i++ )
        {
            args[i] = new char[512];
            memset( args[i] , 0 , 512 );
            args[i][0] = ' ';
        }
        auto raw_args = this->args_;
        size_t len = strlen( this->args_ );

        int start_pos = 0; 

#ifdef _WIN32
        int row = 0;
#else
        int row = 0;
#endif

        int col = 0;
        for ( int e = start_pos; e < len; e++ )
        {
            if ( raw_args[e] == ' ')
            {
                col = 0;
                row++;
                //std::string str( raw_args + f , ( e - f + 1 ) );
                //list.push_back( str );
                //f = e;
            }
            else 
            {
                args[row][col] = raw_args[e];
                col++;
            }
        }
         
        args[row+1] = NULL;
    }
    auto loop = uv_default_loop();
    this->pipe_ = { 0 };

    r = uv_pipe_init( loop , &this->pipe_ , 0 );
     
    this->pipe_.data = this;
    
    this->options.exit_cb = SysProcess::uv_process_exit_callback;
    this->options.file = this->file_;
    this->options.args = args;
    this->options.cwd = this->directory_;
    this->child_req.data = this;

    this->options.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];

    child_stdio[0].flags = UV_IGNORE;// ( uv_stdio_flags )( UV_CREATE_PIPE | UV_READABLE_PIPE );

    child_stdio[1].flags = ( uv_stdio_flags )( UV_CREATE_PIPE | UV_WRITABLE_PIPE );
    child_stdio[1].data.stream = ( uv_stream_t* )&this->pipe_;

    child_stdio[2].flags = UV_IGNORE;

    this->options.stdio = child_stdio;
    
    r  = uv_spawn( loop , &this->child_req , &this->options );
    if ( r != 0 )
    {
        printf( "uv_spawn: %s\r\n" , uv_strerror( r ) );
    }

    r = uv_read_start( ( uv_stream_t* )&this->pipe_ , SysProcess::uv_process_alloc_buffer , SysProcess::uv_prcoess_read_stream );
    if ( r != 0 )
    {
        printf( "uv_read_start: %s\r\n" , uv_strerror( r ) );
    }

    delete[] args;
}

size_t SysProcess::wait_for_exit()
{ 
   // uv_sem_wait( &this->sem );
    //uv_sem_destroy( &this->sem );
    return this->result;
}

void SysProcess::start()
{
    this->invoke();
}

void SysProcess::kill()
{

    uv_process_kill( &this->child_req , 1 );

#ifdef _WIN32
    
    //TerminateProcess( this->pi_.hProcess, -1 );

#else

    pclose( p_stream );

#endif // _WIN32   
}

