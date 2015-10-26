/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef MACROS_H_
#define MACROS_H_

#define SAFE_DELETE(__X__) if( __X__ != nullptr ) { delete __X__; __X__ = nullptr; }
#define SAFE_DELETE_ARRAY(__X__) if( __X__ != nullptr ) { delete __X__[]; __X__ = nullptr; }

//1G memory size
const int CIRCLEBUFFERBUFFER_LEN = 1024 * 1024 * 1024 * 1;
const int CIRCLE_BUFFER_SIZE = 5120;

const int SESSION_BUFFER_SIZE = 5120;

const int HTTP_RESPONSE_CONTENT_SIZE = 1024 * 1024;

#endif // !MACROS_H_
