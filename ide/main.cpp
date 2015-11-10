//#include "SysProcess.h"
//
//int main()
//{
//    {
//        auto ret = SysProcess::create( "C:\\Windows\\notepad.exe" , "D:\\Test\\MiniLZO\\MiniLZO\\minilzo.c" , [] ( SysProcess*p , size_t status )
//        {
//            printf( "Process exist %d \r\n" , status );
//        } );
//
//        ret->start();
//
//    }
//  
//
//    while ( 1 )
//    {
//        uv_run( uv_default_loop() , UV_RUN_DEFAULT );
//    }
//
//    return 0;
//}