#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "getpin.h"
#include "postdial.h"
#ifdef WIN32
#   include "dial.h"
#endif
static char entryname[64]="zsm_dial";
//static char entryname[64]="ChinaNetSNWide";
static char status[64]="username";
static char sxname[64]="chinanet@xy";
static char sxpwd[64]="123456";
static char routname[64]="admin";
static char routpwd[64]="admin";
static char ip[64]="192.168.1.1";

int stop()
{
    printf("%ls\n\n",L"按任意键 结束~~~~~");
    getchar();
    return 1;
}

int _stop()
{
    return 1;
}

int (*pStop)()=stop;

int help()
{
    printf("%ls",
         L"sx\n"
          "  闪讯拨号器 本软件仅用于学习\n"
          "  by zsm <wzshiming@foxmail.com>\n"
          "    --status    -s   [username|site|routers] 默认 username\n"
          "        username  us   获得加密后的闪讯账号\n"
          "        site      si   本机闪讯拨号\n"
          "        routers   ro   路由器闪讯拨号\n"
          "        close     cl   关闭拨号\n"
          "    --sxname    -sn  闪讯账号                默认 chinanet@xy\n"
          "    --sxpwd     -sp  闪讯密码                默认 123456\n"
          "    --routname  -rn  路由器账号              默认 admin\n"
          "    --routpwd   -rp  路由器密码              默认 admin\n"
          "    --ip        -i   路由器地址              默认 192.168.1.1\n"
          "    --stop      -st  运行结束暂停一下        默认\n"
          "    --nostop    -ns  运行结束不暂停\n"
          "    --help      -h   帮助\n"
          "    --               就这么多\n"
          "    示范\n"
          "      本机拨号\n"
          "         sx -s si -sn 闪讯账号 -sp 闪讯密码\n"
          "      路由器拨号\n"
          "         sx -s ro -sn 闪讯账号 -sp 闪讯密码 -rn 路由器账号 -rp 路由器密码\n"
          "");
    pStop();
    exit(-1);
}

// 替换字符串中特征字符串为指定字符串
int replaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr)
{
        int  StringLen;
        char caNewString[255];

        char *FindPos = strstr(sSrc, sMatchStr);
        if( (!FindPos) || (!sMatchStr) )
                return -1;

        while( FindPos )
        {
                memset(caNewString, 0, sizeof(caNewString));
                StringLen = FindPos - sSrc;
                strncpy(caNewString, sSrc, StringLen);
                strcat(caNewString, sReplaceStr);
                strcat(caNewString, FindPos + strlen(sMatchStr));
                strcpy(sSrc, caNewString);

                FindPos = strstr(sSrc, sMatchStr);
        }

        return 0;
}



int main(int argc,char *argv[])
{
    char error[64];
    char strIP[64];
    char urlname[64];
    byte PIN[30];
    int i;

    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#ifdef WIN32
    setlocale(LC_ALL,"");
#endif
    if(argc==1)
        help();
    else
        for(i=1;i<argc;++i)
            if(!strcmp("--status",argv[i])||!strcmp("-s",argv[i]))
                strcpy(status,argv[++i]);
            else if(!strcmp("--sxname",argv[i])||!strcmp("-sn",argv[i]))
                strcpy(sxname,argv[++i]);
            else if(!strcmp("--sxpwd",argv[i])||!strcmp("-sp",argv[i]))
                strcpy(sxpwd,argv[++i]);
            else if(!strcmp("--routname",argv[i])||!strcmp("-rn",argv[i]))
                strcpy(routname,argv[++i]);
            else if(!strcmp("--routpwd",argv[i])||!strcmp("-rp",argv[i]))
                strcpy(routpwd,argv[++i]);
            else if(!strcmp("--ip",argv[i])||!strcmp("-i",argv[i]))
                strcpy(ip,argv[++i]);
            else if(!strcmp("--stop",argv[i])||!strcmp("-st",argv[i]))
                pStop=stop;
            else if(!strcmp("--nostop",argv[i])||!strcmp("-ns",argv[i]))
                pStop=_stop;
            else if(!strcmp("--help",argv[i])||!strcmp("-h",argv[i]))
                help();
            else if(!strcmp("--",argv[i]))
                break;
            else
                help();


    if(!strcmp("username",status)||!strcmp("us",status))
    {
        getPIN(sxname,PIN);
        printf(&PIN[2]);
    }
    else if(!strcmp("close",status)||!strcmp("cl",status)){
        RasCloseAll();
    }
    else if(!strcmp("site",status)||!strcmp("si",status))
    {
#ifdef WIN32
        printf("%ls%s\n",L"闪讯拨号: ",sxname);
        getPIN(sxname,PIN);
        printf("%ls%s\n",L"动态账号: ",&PIN[2]);
        printf("%ls\n",L"拨号中...");
        //RasClose(RasConnecting(entryname));
        RasCloseAll();
        i=RasConnect(entryname,PIN,sxpwd,error,strIP);
        if(!i)
        {
            printf("IP: %s\n",strIP);
            printf("%ls\n",L"拨号成功");
        }
        else
        {
            printf("%ls %d: %s\n",L"错误",i,error);
            printf("%ls\n",L"拨号失败");
            RasCloseAll();
            //RasClose(RasConnecting(entryname));
        }

#endif
    }
    else if(!strcmp("routers",status)||!strcmp("ro",status))
    {
        printf("%ls%s\n",L"闪讯拨号: ",sxname);
        getPIN(sxname,PIN);
        printf("%ls%s\n",L"动态账号: ",&PIN[2]);
        sprintf(urlname,"%%0D%%0A%s",&PIN[2]);
        replaceStr(urlname," ","%20");

        printf("%ls\n",L"设置路由器中...");
        printf("%ls\n",L"如果没有成功请检查\n"
                        "  1.是否已经连上路由器\n"
                        "  2.路由器型号是否支持");
        i=postDial(urlname,sxpwd,routname,routpwd,ip);
        printf("%ls:%d\n",L"设置结束",i);
    }
    else
        help();

    return pStop();
}
#ifdef __cplusplus
}
#endif
