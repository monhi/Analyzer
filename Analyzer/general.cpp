#include "general.h"
#include <stdarg.h>
#include <stdio.h>
#include <psapi.h>
#include "IniSingleton.h"
#include <Shlwapi.h>
#include <algorithm>
#include <string>

static bool				m_isUpdating	= false;
static std::string		m_DispensedCash = "";
static bool				m_processOffline= false;

int color_printf(int color, char* string, ...)
{
	va_list arg;
	int done;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	va_start(arg, string);
	done = vfprintf(stdout, string, arg);
	va_end(arg);
	return done;
}

int GetProgramVersionInfo(int &major, int &minor, int &build, int &revision)
{
	TCHAR filename[512];	
	GetModuleFileName(NULL, filename, 512);
	DWORD   verBufferSize;
	char    verBuffer[2048];
	//  Get the size of the version info block in the file
	verBufferSize = GetFileVersionInfoSize(filename, NULL);
	if (verBufferSize > 0 && verBufferSize <= sizeof(verBuffer))
	{
		//  get the version block from the file
		if (TRUE == GetFileVersionInfo(filename, NULL, verBufferSize, verBuffer))
		{
			UINT length;
			VS_FIXEDFILEINFO *verInfo = NULL;
			//  Query the version information for neutral language
			if (TRUE == VerQueryValueA(verBuffer, ("\\"), reinterpret_cast<LPVOID*>(&verInfo), &length))
			{
				//  Pull the version values.
				major = HIWORD(verInfo->dwProductVersionMS);
				minor = LOWORD(verInfo->dwProductVersionMS);
				build = HIWORD(verInfo->dwProductVersionLS);
				revision = LOWORD(verInfo->dwProductVersionLS);
				return SUCCESS;
			}
		}
	}
	major		= PROGRAM_MAJOR_VERSION;
	minor		= PROGRAM_MINOR_VERSION;
	build		= 0;
	revision	= 0;
	return FAILURE;
}

std::string utf8_encode(const std::wstring &wstr)
{
	if( wstr.empty() ) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo( size_needed, 0 );
	WideCharToMultiByte                  (CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if( str.empty() ) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo( size_needed, 0 );
	MultiByteToWideChar                  (CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


bool DirExists(const std::string& dirName_in)
{

	int retval;
	retval = PathFileExistsA(dirName_in.c_str());
	if(retval == 1)
	{
		FILE* f = fopen(dirName_in.c_str(),"r");
		if(f)
		{
			fclose(f);
			return false;
		}
		return true;
	}
	return false;
}

bool FileExists(const std::string& fileName_in)
{
	int retval;
	retval = PathFileExistsA(fileName_in.c_str());
	if(retval == 1)
	{
		FILE* f = fopen(fileName_in.c_str(),"r");
		if(f)
		{
			fclose(f);
			return true;
		}
		return false;
	}
	return false;
}

void read_directory(const std::string name, stringvec& v,std::string pan)
{
	std::string cellname;
	std::string stemp;
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(),&data)) != INVALID_HANDLE_VALUE ) 
	{
		do 
		{
			cellname = data.cFileName;
			stemp = name + "\\" + cellname;
			if( 
				DirExists(stemp) 
				&& 
				( cellname != "." ) 
				&& 
				( cellname != "..") 
				)
			{
				read_directory(stemp,v,pan);
			}
			else if(FileExists(stemp))
			{
				if(stemp.find(pan)!= std::string::npos)
				{
					if(stemp.find(" ")!=std::string::npos)
					{
						stemp = "\""+stemp+"\"";
					}
					v.push_back(stemp);
				}
			}			
		} 
		while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}
}

std::string	ExtractJSON(std::string&	beta)
{
	u32 cntr		= 0;
	std::string ret	= "";
	size_t		i	= 0;
	for( i=0; i < beta.length(); i++ )
	{
		if(beta[i] == '{')
		{
			cntr++;
		}
		if(cntr)
		{
			ret += beta[i];
		}
		if(beta[i] == '}')
		{
			cntr--;
		}
	}
	return ret;
}



std::string	GetRunningPath()
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	std::string runpath = path;
	runpath += "\\";
	return runpath;
}

std::wstring my_utf8_to_utf16(const std::string& utf8)
{
	std::vector<unsigned long> unicode;
	size_t i = 0;
	while (i < utf8.size())
	{
		unsigned long uni;
		size_t todo;
		bool error = false;
		unsigned char ch = utf8[i++];
		if (ch <= 0x7F)
		{
			uni = ch;
			todo = 0;
		}
		else if (ch <= 0xBF)
		{
			//throw std::logic_error("not a UTF-8 string");
			return std::wstring(L"Not a unicode string");
		}
		else if (ch <= 0xDF)
		{
			uni = ch&0x1F;
			todo = 1;
		}
		else if (ch <= 0xEF)
		{
			uni = ch&0x0F;
			todo = 2;
		}
		else if (ch <= 0xF7)
		{
			uni = ch&0x07;
			todo = 3;
		}
		else
		{
			//throw std::logic_error("not a UTF-8 string");
			return std::wstring(L"Not a unicode string");
		}
		for (size_t j = 0; j < todo; ++j)
		{
			if (i == utf8.size())
				//throw std::logic_error("not a UTF-8 string");
				return std::wstring(L"Not a unicode string");
			unsigned char ch = utf8[i++];
			if (ch < 0x80 || ch > 0xBF)
				//throw std::logic_error("not a UTF-8 string");
				return std::wstring(L"Not a unicode string");
			uni <<= 6;
			uni += ch & 0x3F;
		}
		if (uni >= 0xD800 && uni <= 0xDFFF)
			//throw std::logic_error("not a UTF-8 string");
			return std::wstring(L"Not a unicode string");
		if (uni > 0x10FFFF)
			//throw std::logic_error("not a UTF-8 string");
			return std::wstring(L"Not a unicode string");
		unicode.push_back(uni);
	}
	std::wstring utf16;
	for (size_t i = 0; i < unicode.size(); ++i)
	{
		unsigned long uni = unicode[i];
		if (uni <= 0xFFFF)
		{
			utf16 += (wchar_t)uni;
		}
		else
		{
			uni -= 0x10000;
			utf16 += (wchar_t)((uni >> 10) + 0xD800);
			utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
		}
	}
	return utf16;
}


void gen_random(char *s, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		int randomChar = rand() % (26 + 26 + 10);
		if (randomChar < 26)
			s[i] = 'a' + randomChar;
		else if (randomChar < 26 + 26)
			s[i] = 'A' + randomChar - 26;
		else
			s[i] = '0' + randomChar - 26 - 26;
	}
	s[len] = 0;
}


void DisableQuickEdit()
{
	const int	QuickEditMode = 64;
	const int	ExtendedFlags = 128;
	HANDLE		hInput;
	DWORD		prev_mode;

	hInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hInput, &prev_mode);
	prev_mode = prev_mode & ~(QuickEditMode | ExtendedFlags);
	SetConsoleMode(hInput, prev_mode);
}

void LOGIData(char* buffer, int color)
{
	color_printf(color, buffer);
	color_printf(color, "\n");
	LOGI << buffer;
}
