/*************************************************************************
	> File Name: myshell.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月26日 星期二 14时16分16秒
 ************************************************************************/
#include<stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <string.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <linux/limits.h>

# define P 3 // 管道
# define I 0 // 输入重定向
# define V 2 // 两个>>输出流重定向
# define O 1 // 输出重定向
# define N 4 // 一般的命令
void output( char argv[][256], int sum );
void analyse(int , char fen[][256]);//执行参数
void fenge( char *buf, char fen[][256], int *len );
int find_mingli(char *argv);//一次只传进来一个，所以用一维的

int main(void)
{
 // int *argc;
    int i=0, len,j;
    char buf[256];
    char argv[100][256];
    char fen[100][256];
/*
    char **argv = (char **)malloc( sizeof(char *)*100);
    for( i = 0; i<100; i++ )
    {
        argv[i] = (char *)malloc(sizeof(char )*256);
    }
 */ 
    
    while(1)
    {
        len =0;
        printf("mashell:");
        gets(argv[i]);
        strcpy(buf, argv[i]);
        buf[strlen(buf)] = '\0';

        if( (strcmp(buf, "exit") ==0 )  || (strcmp(buf, "logout" ) == 0))
        {
            break;
        }
        i++;
        if( strcmp( buf, "history" ) == 0 )
        {
            output(argv, i);
        }
        fenge(buf, fen, &len);
       // printf("%d\n", len);
       /* for( j = 0; j<len; ++j)
        {
            printf("%s\n", fen[j]);
        }
        */
        analyse(len, fen);


    }

}
void output( char argv[100][256], int sum )
{
    int i;
    for( i = 0; i<sum; i++ )
    {
        printf("%d:%s\n",i+1,argv[i]);
    }
}
void fenge( char * buf, char fen[100][256], int * len )
{
   // printf("%s\n", buf);
//    printf("nihao\n");
    int i, j=0;
    char p[256];//这块不能写成char *p的形式，否则出现段错误
    for( i = 0; buf[i]!= NULL; i++ )
    {
       // printf("hao\n");
        p[j] = buf[i];
        j++;

        if( buf[i] == ' ')
        {
           // printf("xiaomig\n");
            strcpy(fen[*len], p );
            fen[*len][j]= '\0';
     //       printf("%s\n", fen[*len]);
            j=0;
            *len = *len +1;
        }
        if( buf[i+1] == '\0' )
        {
           // printf("lihuasdfsdf\n");
            strcpy(fen[*len], p);
            fen[*len][j]= '\0';
       //     printf("%s\n", fen[*len]);
            *len= *len +1;// 这块不能写成*len++这样len的值不变
            j=0;
        }
    }

}

void analyse(int len, char fen[100][256])
{
    int i;
    int t=0;//收纳各种符号
    int sum;//以空格记录命令的个数
    int hou;//单个记录后台符的个数，是一个辅助命令，所以单个记录下来
    int flag=0;//记录各种输入的各种符号的个数
    pid_t pid;
    char *file;
    




   // char arg[100][256];
    char* arg[len+1];
    char* argnext[len+1];
    sum = len;
    hou = 0;
    for( i = 0; i<sum; i++ )
    {
//        printf("%s\n", fen[i]);
        arg[i] = (char*)fen[i];
    }
    for( i = 0; i< sum; i++ )
    {
        if( strcmp(arg[i], "&") ==0 )
        {
            if( i ==sum-1 )
            {
                hou = 1; 
                arg[i] == NULL;

            }
            else
            {
                printf("输入错误，后台符必须放到最后!\n");
                exit(-1);
            }
        }
    }
    //判断有没有输出重定向符号'>'并且必须让符号的后面必须要有文件名
    for( i = 0; arg[i] != NULL; i++ )
    {
        if( strcmp( arg[i], ">" ) == 0  && arg[i+1] != NULL)
        {
            flag +=1;
            
            if( flag ==1 )
            {
                t =O;
                file = arg[i+1];
                arg[i] ==NULL;
            }
               
           // else if( flag ==2 )
             //  t = V;
            else if( flag>1 )
            {
               printf("输入错误\n");
               return ;
            }      
        }
    
    //判断有没有输入重定向'<'并且必须让这个符号前面有东西
    
    
        if( strcmp( arg[i], "<" ) == 0 && i!=0)
        {
            flag +=1;
            if( flag ==1 )
            {
                t =I;
                file = arg[i+1];
                arg[i] =NULL;
            }
            else
            {
                printf("输入错误\n");
                return ;
            }
        }   

        //命令只有一个管道'|' 并且它的前后必须都要有东西，还要将它后面的命令记录下来
        if ( strcmp( arg[i], "|" ) == 0 && i!=0 && arg[i+1] != NULL )
        {
            flag +=1;
            if(flag ==1)
            {
                t = P;
              int j;
               for( j = i+1; arg[j] != NULL; j++ )
              {
                argnext[j-i-1] = arg[j];
              }
              argnext[j-i-1] = arg[i];
              break;

            }
            else
            {
                printf("输入错误！\n");
                return;
            }

        }

    }
    if( ( pid = fork() ) < 0 )
    {
        printf("fork error\n");
        return;
    }
  //  printf("%s\n",arg[0]);
    switch(t)
    {
        case 0:
        if(pid == 0)
        {
            if(!( find_mingli(arg[0])))
            {
            printf("系统目录下没有%s这个命令\n",arg[0]);
            exit(0);
            }
        execvp(arg[0],arg);
        exit(0);
        break;
        }

     }

     if( waitpid( pid, &status, 0) )





}//这个函数写完，有参数 t , arg , file ,  
int find_mingli(char *arg)//这个函数的作用是判断你输入的命令在系统的命令目录里只否有，有则返回1,无则返回0
{
    DIR *dir;
    struct dirent *ptr;
    int i=0;

    char *path[] = {"./", "/bin", "/usr/bin", NULL};

    //是当前目录下的程序可以正常运行
      if( strncmp( arg, "./", 2 ) == 0 )
    {
        arg = arg +2;//暂时不知道在什么情况下用
    }

    while(path[i]!= NULL)
    {
       if( ( dir = opendir(path[i])  ) <0 )
        {
            printf("没有这个参数 /bin\n");
        }
        while( (ptr = readdir(dir) ) != NULL )
        {
            if( strcmp( ptr->d_name,arg) == 0 )
            {
                closedir( dir);
                return 1;
            }
        }
        closedir(dir);
        i++;
    }
    return 0;
}






