#ifndef __ZASSERT_H__
#define __ZASSERT_H__

#define ENABLE_ASSERTS

#ifdef ENABLE_ASSERTS
//#if defined(__i386__) && defined( WIN32 )
#if defined( WIN32 )
#ifdef WIN32
#pragma warning( disable: 4793 )
#pragma warning( disable : 4800 )
#endif
#if !defined( ZASSERT )
#define ZASSERT(exp) if( !static_cast<bool>(exp) ) { _asm{ int 3 } }
#endif
#ifdef WIN32
#pragma warning( default: 4793 )
#endif
#else
#if !defined( ZASSERT )
#define ZASSERT( exp ) // TODO 
#endif
#endif
#else
#ifndef ZASSERT
#define ZASSERT( exp )
#endif
#endif

#endif
