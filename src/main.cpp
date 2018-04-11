#include <string>
#include <cstdio>

// should define SQLITE_HAS_CODEC to use sqlite3_key
#define SQLITE_HAS_CODEC
#include "sqlite3.h"

int execute(sqlite3 *pDb, const char *sqlMsg)
{
    char * errMsg = NULL;
    int ret = sqlite3_exec(pDb, sqlMsg, NULL, NULL, &errMsg);
    if (SQLITE_OK != ret)
    {
        printf("execute sql command failed:%d, command:%s\n", ret, sqlMsg);
    }
    sqlite3_free(errMsg);
    return ret;
}

int closeDB(sqlite3 *pDb)
{
    int ret = sqlite3_close(pDb);
    if (SQLITE_OK != ret)
    {
        printf("close db failed:%d\n", ret);
        return ret;
    }
    return ret;
}

int openDB(sqlite3 **ppDb, const std::wstring& dbFile, const std::string& key)
{
    int ret = sqlite3_open16(dbFile.c_str(), ppDb);
    if (SQLITE_OK != ret)
    {
        printf("open db failed:%d\n", ret);
        return ret;
    }
    if (!key.empty())
    {
        ret = sqlite3_key(*ppDb, key.c_str(), (int)key.size());
        if (SQLITE_OK != ret)
        {
            printf("db key failed:%d\n", ret);
            return ret;
        }
    }
    //ret = execute(*ppDb, "PRAGMA kdf_iter = 12345;");
    if (SQLITE_OK != ret)
    {
        closeDB(*ppDb);
        return ret;
    }
    return ret;
}

int createTable(sqlite3 *pDB, const char *str)
{
    return execute(pDB, str);
}

int main()
{
    sqlite3 *pDb;
    openDB(&pDb, L"myDB.db", "123");

    const char * str = R"|(
CREATE TABLE KeyValuePairs
(Key TEXT PRIMARY KEY,
Value TEXT);
)|";
    createTable(pDb, str);
    closeDB(pDb);
}
