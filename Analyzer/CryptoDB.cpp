#include "stdafx.h"
#include "CryptoDB.h"
#include "general.h"

CCryptoDB::CCryptoDB()
{
	m_db = NULL;
}

CCryptoDB::~CCryptoDB()
{
	CloseDB();
}

int CCryptoDB::GetInfo(std::string name, int& total, int& hit)
{
	sqlite3_stmt *res;
	char buffer[512];
	sprintf(buffer,"SELECT TOTAL,HIT FROM %s WHERE name = '%s'",TABLE_NAME,name.c_str());

	m_rc = sqlite3_prepare_v2(m_db, buffer, -1, &res, 0);

	if (m_rc == SQLITE_OK) 
	{
		sqlite3_bind_int(res, 1, 3);
	}
	else 
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(m_db));
		return FAILURE;
	}

	int step = sqlite3_step(res);

	if (step == SQLITE_ROW) 
	{
		printf("%s: ", sqlite3_column_text(res, 0));
		printf("%s\n", sqlite3_column_text(res, 1));
		total = sqlite3_column_int(res, 0);
		hit   = sqlite3_column_int(res, 1);
	}
	else
	{
		// No data found.
		total = 0; 
		hit = 0;
		UpdateInfo(name, total, hit);
	}
	sqlite3_finalize(res);
	return SUCCESS;
}

int CCryptoDB::UpdateInfo(std::string name,int total,int hit)
{
	char buffer[512];
	sprintf(buffer, "INSERT OR REPLACE INTO %s VALUES('%s',%d, %d);", TABLE_NAME, name.c_str(),total,hit);
	m_rc = sqlite3_exec(m_db, buffer, 0, 0, &m_zErrMsg);
	if (m_rc != SQLITE_OK) 
	{
		fprintf(stderr, "SQL error: %s\n", m_zErrMsg);
		sqlite3_free(m_zErrMsg);
		return FAILURE;
	}
	return SUCCESS;
}

int CCryptoDB::OpenDB()
{
	char buffer[512];
	
	std::string stemp = GetRunningPath();
	stemp += "Crypto.db";
	/* Open database */
	m_rc = sqlite3_open(stemp.c_str(), &m_db);

	if (m_rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));		
		return FAILURE;
	}
	else 
	{
		fprintf(stdout, "Opened database successfully\n");
	}

	sprintf(buffer, "CREATE TABLE IF NOT EXISTS %s(NAME TEXT  PRIMARY KEY  NOT NULL,TOTAL INT NOT NULL,HIT INT NOT NULL);", TABLE_NAME);

	m_rc = sqlite3_exec(m_db,buffer, 0, 0, &m_zErrMsg);

	if (m_rc != SQLITE_OK) 
	{
		fprintf(stderr, "SQL error: %s\n", m_zErrMsg);
		sqlite3_free(m_zErrMsg);
	}
	else 
	{
		fprintf(stdout, "Table created successfully\n");
	}
	return SUCCESS;	
}

int CCryptoDB::CloseDB()
{
	if (m_db)
	{
		sqlite3_close(m_db);
		m_db = NULL;
	}
	return SUCCESS;
}