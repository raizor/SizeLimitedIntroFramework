#include "debugHelpers.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZASSERT(exp) if( !static_cast<bool>(exp) ) { _asm{ int 3 } }

void zmsg( const char * format, ... )
{
	char buf[ 2048 ];
	va_list args;
	va_start(args,format);
	ZASSERT( format != 0);
	vsnprintf(buf,2048-1,format,args);
	va_end(args);

	printf( "%s\n", buf );
}
