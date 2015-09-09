#include "dial.h"
#ifdef WIN32
BOOL RasCreate(char* entryName)
{
    LPRASENTRYA lpRasEntry = NULL;
    //DWORD cb = sizeof(RASENTRYA);
    DWORD dwBufferSize = 0;

    RasGetEntryPropertiesA(NULL,"", NULL, &dwBufferSize, NULL, NULL);
    if (dwBufferSize == 0)
        return FALSE;

    lpRasEntry = (LPRASENTRYA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
    if (lpRasEntry == NULL)
        return FALSE;

    ZeroMemory(lpRasEntry, sizeof(RASENTRYA));
    lpRasEntry->dwSize = dwBufferSize;
    lpRasEntry->dwfOptions = RASEO_RemoteDefaultGateway | RASEO_ShowDialingProgress | RASEO_PreviewUserPw;
    lpRasEntry->dwType = RASET_Broadband;

    strcpy(lpRasEntry->szDeviceType, RASDT_PPPoE);
    strcpy(lpRasEntry->szDeviceName, entryName);
    lpRasEntry->dwfNetProtocols = RASNP_Ip;
    lpRasEntry->dwFramingProtocol = RASFP_Ppp;

    DWORD dwRet;
    dwRet = RasSetEntryPropertiesA(NULL, entryName, lpRasEntry, dwBufferSize, NULL, 0); // 创建连接
    HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasEntry);

    if (dwRet != 0)
    {
        //MessageBox(NULL, "无法创建网络连接", "错误", 0);
        return FALSE;
    }

    return TRUE;
}


int RasConnect(char* entryName,char *userName, char *passWord, WCHAR *error, char *strIP)
{

    //HRASCONN *pRC用于记录拨号信息存放的地址
    HRASCONN hRasConn = NULL;
    RASDIALPARAMSA RasParams;
    ZeroMemory(&RasParams, sizeof(RASDIALPARAMSA));


    RasParams.dwSize = sizeof(RASDIALPARAMSA);
    strcpy(RasParams.szEntryName, entryName);
    //getPIN((byte*)userName,PIN);
    strcpy(RasParams.szUserName,userName);
    strcpy(RasParams.szPassword, passWord);

    RasCreate(entryName);
    DWORD dwRet;
    dwRet = RasDialA(NULL, NULL, &RasParams, 0L, NULL , &hRasConn);  //如果函数成功, 则返回0
    if (dwRet != 0)
    {
        RasGetErrorStringA((UINT)dwRet,(LPSTR)error, 256);
        return (int)dwRet;
    }

    //-----获得ＩＰ-----
    RASPPPIPA RasIp;
    memset(&RasIp, 0, sizeof(RasIp));
    RasIp.dwSize=sizeof(RASPPPIPA);
    DWORD rasSize=sizeof(RASPPPIPA);
    RasGetProjectionInfoA(hRasConn, RASP_PppIp, &RasIp, &rasSize);

    strcpy(strIP, RasIp.szIpAddress);
    //strcat(strIP, RasIp.szIpAddress);

    return 0;
}

BOOL RasClose(HRASCONN  hrasconn)
{
    if(RasHangUp(hrasconn) == 0)	//断开成功
        return TRUE;
    else
        return FALSE;
}

void RasCloseAll()
{
    RASCONN rc[10];
    DWORD dwSize;
    DWORD dwNumber = 0;
    int i;
    rc[0].dwSize=sizeof(RASCONN);
    dwSize =sizeof(rc);

    DWORD dwRet;
    dwRet = RasEnumConnections(rc, &dwSize, &dwNumber);		//枚举已连接的网络
    if(dwRet == 0)
    {
        for(i=0; i<dwNumber; i++)
           RasHangUp(rc[i].hrasconn);		//返回"我的连接"对应的句柄
    }

    return ;
}

HRASCONN RasConnecting(char* entryName)
{

    RASCONN rc[10];
    DWORD dwSize;
    DWORD dwNumber = 0;
    int i;
    rc[0].dwSize=sizeof(RASCONN);
    dwSize =sizeof(rc);

    DWORD dwRet;
    dwRet = RasEnumConnections(rc, &dwSize, &dwNumber);		//枚举已连接的网络
    if(dwRet == 0)
    {
        for(i=0; i<dwNumber; i++)
            if(strcmp(rc[i].szEntryName, entryName) == 0)
            {
                return rc[i].hrasconn;		//返回"我的连接"对应的句柄
            }
    }

    return NULL;
}
#endif
