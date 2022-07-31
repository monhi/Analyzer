#pragma  once 
#define		WIN_OS					1

#include "plog\log.h"

//#include <openssl/ssl.h>
//#include <openssl/err.h>
//#include <openssl/pem.h>
//#include <openssl/pkcs12.h>
#include <process.h>

#include <string>
#include <map>
#include "stdafx.h"

#ifdef	 WIN_OS
#define _WINSOCKAPI_
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
//#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <vector>

#elif defined LINUX_OS

#endif


typedef std::vector<std::string> stringvec;


typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef std::map<std::string,std::string>::iterator string_iterator;

#define 	SUCCESS					 0
#define 	FAILURE					-1


#define		MAX_FONT_NUM				10

#define		PROGRAM_MAJOR_VERSION		0
#define		PROGRAM_MINOR_VERSION		0


#define		MAX_PACKET_SIZE				(4*1024*1024)
#define		NETWORK_Q_SIZE				(MAX_PACKET_SIZE*4)

struct IPv4
{
	unsigned char b1, b2, b3, b4;
};

typedef unsigned char				u8;
typedef unsigned short				u16;
typedef unsigned int				u32;
typedef unsigned long long			u64;

#define  BLACK			0
#define  DARKBLUE		FOREGROUND_BLUE
#define  DARKGREEN		FOREGROUND_GREEN
#define  DARKCYAN		FOREGROUND_GREEN | FOREGROUND_BLUE
#define  DARKRED		FOREGROUND_RED
#define  DARKMAGENTA	FOREGROUND_RED | FOREGROUND_BLUE
#define  DARKYELLOW		FOREGROUND_RED | FOREGROUND_GREEN
#define  DARKGRAY		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define  GRAY			FOREGROUND_INTENSITY
#define  BLUE			FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define  GREEN			FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define  CYAN			FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define  RED			FOREGROUND_INTENSITY | FOREGROUND_RED
#define  MAGENTA		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define  YELLOW			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define  WHITE			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE


#define	NAMED_MUTEX_CONSTANT_TEXT	L"MyAnalyzerProgramThatRunsEverydayandEverytime.blahblahblah"

///////////////////////////////////////////////
//			Client to Server Events			 //
///////////////////////////////////////////////
//bool			getMyIP						(IPv4 & myIP);

void			LOGIData					(char* buffer, int color);
int				color_printf				(int color, char* string, ...);
int				GetProgramVersionInfo		(int &maj,int &min, int &build , int &revision);
std::string		utf8_encode					(const std::wstring &wstr);
std::wstring	utf8_decode					(const std::string &str);
bool			DirExists					(const std::string& dirName_in);
bool			FileExists					(const std::string& dirName_in);
std::string		ExtractJSON					(std::string&	beta);
void			read_directory				(const std::string name, stringvec& v,std::string pan);
std::wstring	my_utf8_to_utf16			(const std::string& utf8);
std::string		GetRunningPath				();
void			gen_random					(char *s, size_t len);
void			DisableQuickEdit			();