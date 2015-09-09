#ifndef DIAL_H
#define DIAL_H
#ifdef WIN32
# include <windows.h>
# include <ras.h>
# include <locale.h>

BOOL RasCreate(char* entryName);

int RasConnect(char* entryName,char *userName, char *passWord, WCHAR *error, char *strIP);

BOOL RasClose(HRASCONN  hrasconn);

HRASCONN RasConnecting(char* entryName);
#endif


#endif // DIAL_H
