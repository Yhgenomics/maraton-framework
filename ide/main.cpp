//#include "SysProcess.h"
//#include "AsyncWorker.h"
//#include "SyncWorker.h"
//
//static void CB_2( SysProcess* p , size_t status )
//{
//    printf( "CB_2\r\n" );
//}
//
//
//static void CB_1( SysProcess* p , size_t status )
//{
//    printf( "CB_1 %s \r\n" , p->std_out().c_str() );
//    auto r = SysProcess::create( "C:\\Windows\\notepad.exe" , "D:\\test.txt" , CB_2 );
//    r->start();
//}
//
//
//int main()
//{
//    SyncWorker::create_loop( 1000 , [] ( SyncWorker* p ) { 
//        printf( "%s for %ld \r\n", p->data(), p->loop_count() );
//
//        if ( p->loop_count() % 10 == 0 )
//        {
//            SyncWorker::create_once( [] ( SyncWorker* p )
//            {
//                printf( "%s for %ld \r\n" , p->data() , p->loop_count() );
//            } , "I'm once worker" );
//        }
//
//
//    } ,"I'm 100 loop worker");
//    
// 
//    /*auto r = SysProcess::create( "D:\\ModerCPPLearn.exe" ,"a b c e d f g", CB_1 );
//    r->start();*/
//
//    while ( 1 )
//    {
//        
//        uv_run( uv_default_loop() , UV_RUN_DEFAULT );
//    }
//
//    return 0;
//}