/*************************************************************************
	> File Name: inet.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月02日 星期二 19时31分10秒
 ************************************************************************/

#include<stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <string.h>
# include <arpa/inet.h>


int main(void)
{
    char buffer[32];
    int ret = 0;
    int host = 0;
    int network =0;
    unsigned int  address =0;
    struct in_addr in;

    in.s_addr = 0;
    printf("请输入IP地址：");
    fgets(buffer, 31, stdin);
    buffer[32] = '\0';

    //将‘.’形式的IP地址转变成网络字节顺序（大端模式）存储在in中
    if( (ret = inet_aton(buffer, &in) )==0 )//执行成功返回非0
    {
        printf("error1\n");
    }
    else
    {
        printf("inet_aton: \t0x%x\n", in.s_addr);
    }
    //和上面一样，只是返回值为网络字节的二进制的IP地址
    if( (address = inet_addr(buffer)) == -1 )
    {
        printf("error2\n");
    }
    else{
        printf("inet_addr: \t0x%lx", address);
    }
    //将--转化为主机字节顺序的二进制的IP地址
    if( (address = inet_network(buffer)) == -1 )
    {
        printf("errno3\n");
    }
    else{
        printf("inet_network: \t0x%lx\n",address);
    }
    //
    if( inet_ntoa(in) == NULL )//将in中的网络字节顺序的二进制的IP地址转化为‘.’分隔的字符串
    {
        printf("errno4\n");
    }
    else{
        printf("inet_ntoa: \t%s\n",inet_ntoa(in));
    }
    host = inet_lnaof(in);//从in中提取主机地址，执行成功返回主机字节顺序的主机地址
    network = inet_netof(in);//从in中提取网络地址，执行成功返回主机字节顺序的网络地址
    printf("inet_lnaof: \t0x%x\n",host);
    printf("inet_netof: \t0x%x\n",network);

    in = inet_makeaddr(network, host);//将网络号的主机号结合起来组成一个网络地址
    printf("inet_makeaddr: \t0x%x\n",in.s_addr);



    return 0;
}
