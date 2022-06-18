#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(void)
{
const char *filename = "/home/dokhebi/Source/C/tests/testdb.dbf";
char *ptr;
sqlite3 *ppDb;
sqlite3_stmt *res;
struct in_addr ip_addr;

int rc;

    rc = sqlite3_open(filename, &ppDb);
    if (rc != SQLITE_OK)
    {
        printf("Could not open database: %s\n", sqlite3_errmsg(ppDb));
        return(rc);
    }
    
    rc = sqlite3_prepare_v2(ppDb, "SELECT PCID, PCname, IPAddress FROM table1", -1, &res, 0);
    if (rc != SQLITE_OK)
    {
        printf("Failed to fetch data: %s\n", sqlite3_errmsg(ppDb));
        return(rc);
    }
    
    rc = sqlite3_step(res);
    while (rc == SQLITE_ROW)
    {
        ip_addr.s_addr = strtol((const char *)sqlite3_column_text(res, 2), &ptr, 0);
        printf("PC #%s PC Name %s IP Addr %s\n", sqlite3_column_text(res, 0), sqlite3_column_text(res, 1),
               inet_ntoa(ip_addr));
        rc = sqlite3_step(res);
    }
    
    rc = sqlite3_finalize(res);
    
    rc = sqlite3_close(ppDb);
    if (rc != SQLITE_OK)
    {
        printf("Could not close database: %s\n", sqlite3_errmsg(ppDb));
        return(rc);
    }
    
    return(0);
}