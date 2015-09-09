#include "postdial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
inline int sxUrlData(char* outdata,char* ip,char* pppoe_user,char* pppoe_pwd)
{
    sprintf(outdata,
          "http://%s/userRpm/PPPoECfgRpm.htm?wantype=2&acc=%s&psw=%s&VnetPap=0&linktype=2&Save=%%B1%%A3+%%B4%%E6",
          ip,pppoe_user,pppoe_pwd);
    return 1;
}

inline int routersLoginData(char* outdata,char* login_user,char* login_pwd)
{
    char tmp[64];
    char* ba64;
    sprintf(tmp,"%s:%s",login_user,login_pwd);
    ba64=http_base64_encode(tmp);
    sprintf(outdata,"Basic %s",ba64);
    free(ba64);
    return 1;
}

int postDial(char* pppoe_user,char* pppoe_pwd,char* login_user,char* login_pwd,char* ip)
{
    int code=0;
    ghttp_request *request = NULL;
    ghttp_status status;
    char *buf;
    int bytes_read;
    char url[128];
    char login[128];
#ifdef WIN32
    WORD wVersionRequested=MAKEWORD(1,1);
    WSADATA wsaData;
    if(WSAStartup(wVersionRequested,&wsaData))
        return -1;
#endif
    sxUrlData(url,ip,pppoe_user,pppoe_pwd);
    //routersLoginData(login,login_user,login_pwd);

    request = ghttp_request_new();
    if(ghttp_set_uri(request, url) == -1)
        return -2;
    if(ghttp_set_type(request, ghttp_type_get) == -1)
        return -3;

    //puts(url);
    //puts(login);

    ghttp_set_authinfo(request,login_user,login_pwd);
    ghttp_set_header(request,http_hdr_Referer,url);
    //ghttp_set_header(request,http_hdr_Authorization,login);
    ghttp_set_header(request,http_hdr_Connection,"close");
    ghttp_set_header(request,http_hdr_Accept_Encoding,"identity");

    ghttp_prepare(request);
    status = ghttp_process(request);

    if(status == ghttp_error)
      return -4;
    /* OK, done */
    code=ghttp_status_code(request);

    //buf = ghttp_get_body(request);
    bytes_read = ghttp_get_body_len(request);

    //puts(buf);
    //putchar(bytes_read);


    ghttp_close(request);
#ifdef WIN32
    WSACleanup();
#endif
    if(bytes_read>10000)
    {
        return 1;
    }
    else
    {
        return code;
    }
}
