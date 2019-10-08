/**
 * @file Util.h
 * @brief 
 * @author tuyungang
 * @version 1.0
 * @date 2019-09-25
 */
#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <vector>
#include <memory>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include "ProcDataDef.h"

//using namespace std;

#if _WIN32
#pragma comment(lib, "libcurl.lib")
#endif


//ftp
#define FTP_RUN   0

#if _WIN32
#define msleep_c(t) Sleep(t)
#define timegm _mkgmtime
#define gmtime_r gmtime_s
#else
#define msleep_c(t) usleep(t * 1000)
#endif


#define DISALLOW_ASSIGN(TypeName) \
void operator=(const TypeName&){}

#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
TypeName(const TypeName&);                    \
DISALLOW_ASSIGN(TypeName)

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) \
DISALLOW_COPY_AND_ASSIGN(TypeName)

#undef DISALLOW_IMPLICIT_CONSTRUCTORS
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
TypeName();                                    \
DISALLOW_EVIL_CONSTRUCTORS(TypeName)

#define LIBJINGLE_DEFINE_STATIC_LOCAL(type, name, arguments) \
static type& name = *new type arguments


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x)
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


#ifndef SAFE_DELETE 
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p){delete[] (p);  (p)=NULL;} }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p){ (p)->Release();  (p)=NULL;} }
#endif


#endif
