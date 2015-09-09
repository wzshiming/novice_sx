#ifndef POSTDIAL_H
#define POSTDIAL_H
#ifdef WINNT
#   include <WinSock2.h>
#endif
#include <ghttp.h>
inline int sxUrlData(char* outdata,char* ip,char* pppoe_user,char* pppoe_pwd);
inline int routersLoginData(char* outdata,char* login_user,char* login_pwd);
int postDial(char* pppoe_user,char* pppoe_pwd,char* login_user,char* login_pwd,char* ip);

#endif // POSTDIAL_H
