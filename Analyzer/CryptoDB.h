#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "sqlite\sqlite3.h" 
#include <string>

#define TABLE_NAME	"CRYPTO_TABLE"


class CCryptoDB
{
private:
	sqlite3		*m_db;
	char		*m_zErrMsg;
	int		 	 m_rc;
	char		*m_sql;
public:
			 CCryptoDB();
			~CCryptoDB();
	int		 OpenDB();
	int		 CloseDB();
	int		 UpdateInfo(std::string name, int total, int hit);
	int		 GetInfo(std::string name, int& total, int& hit);
};

