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
# include <errno.h>
# define P 3 // 管道
# define I 2 // 输入重定向
# define V 4 // 两个>>输出流重定向
# define O 1 // 输出重定向
# define N 0 // 一般的命令
void output( char argv[][256], int sum );
void analyse(int , char fen[][256]);//执行参数
void fenge( char *buf, char fen[][256], int *len );
int find_mingli(char *canshu[256]);//一次只传进来一个，所以用一维的
void my_error( const char * err_string, int line )
{
    fprintf(stderr,"line : %d\n", line);
    perror( err_string );
    exit(-1);
}

int main(void)
{
 // int *argc;
    int i=0, len,j;
    char buf[256];
    char argv[100][256];
    char fen[100][256];
    int fd;
  /*  if( (fd= open("history_command", O_RDWR|O_CREAT, 0644) ) <0)
    {
        my_error("open", __LINE__);
    }
    */
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
  //      write("history_command", buf, 30);

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
      /*  for( j = 0; j<len; ++j)
        {
            printf("--%s--\n", fen[j]);
        }
        */
        analyse(len, fen);


    }
    exit(0);

}
void output( char argv[100][256], int sum )
{
    int i;
    char buf[256];
    for( i = 0; i<sum; i++ )
    {
        printf("%d:%s\n",i+1,argv[i]);
    }
   /* while( read( "history_command",buf, 30) )
    {
        printf("%s\n", buf);
    }
*/
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
            fen[*len][j-1]= '\0';
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
    int i,fd, status, pid2,fd2,ccc;
    int t=0;//收纳各种符号
    int sum;//以空格记录命令的个数
    int hou;//单个记录后台符的个数，是一个辅助命令，所以单个记录下来
    int flag=0;//记录各种输入的各种符号的个数
    pid_t pid;
    pid_t child_pid;
    char *file;
   // char arg[100][256];
    char *arg[256];
    
    char* argnext[len+1];
    sum = len;
    hou = 0;
    for( i = 0; i<sum; i++ )
    {
       // printf("%s\n", fen[i]);
        arg[i] = (char*)fen[i];
    //    strcpy(arg[i], fen[i]);
        arg[i][strlen(arg[i])] = '\0';

    }
    arg[i] = NULL;
    
  /*()  for( i = 0; i<sum; i++ )
    {
        printf("--=%s=-==-\n",arg[i]);
    }*/
   /*
    if( execvp( arg[0], arg ) ==-1 )
    {
        perror("execvp");
    }*/
//    exit(0);
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
    for( i = 0; arg[i]!=NULL; i++ )
    {
        if( strcmp( arg[i], ">" ) == 0  && arg[i+1] != NULL)
        {
         //  printf("sdfsdfsdf\n");
            flag +=1;
            
            if( flag ==1 )
            {
                t =O;
                file = arg[i+1];
                arg[i] =NULL;
             //   printf("nxiasodef\n");
            }
           // printf("%s\n",file);
        puts(file);
            
           // else if( flag ==2 )
             //  t = V;
            if( flag>1 )
            {
               printf("输入错误\n");
                return ;
            }      
        }
       // printf("%s\n",arg[i]);
        
    
    //判断有没有输入重定向'<'并且必须让这个符号前面有东西
    
    
        if(arg[i] != NULL && strcmp( arg[i], "<" ) == 0 && i!=0)
        {
            flag +=1;
         //   printf("woshuji\n");
            if( flag ==1 )
            {
                t =I;
                file = arg[i+1];
                arg[i] =NULL;
              //  printf("ljiefdfef\n");
            }
            else
            {
                printf("输入错误\n");
                return ;
            }
        }   

        //命令只有一个管道'|' 并且它的前后必须都要有东西，还要将它后面的命令记录下来
        if ( arg[i] != NULL && strcmp( arg[i], "|" ) == 0 && i!=0 && arg[i+1] != NULL )
        {
            arg[i] = NULL;
            flag +=1;
            if(flag ==1)
            {
              t = P;
              int j;
               for( j = i+1; arg[j] != NULL; j++ )
              {
                argnext[j-i-1] = arg[j];
              }
              argnext[j-i-1] = arg[j];
              break;

            }
            else
            {
                printf("输入错误！\n");
                return;
            }

        }

    }
 //   printf("ewwere\n");
      //  printf("%s=-=-==-\n", file);
    /*    for( i = 0; arg[i] != NULL; i++ )
        {
            printf("%s\n", arg[i]);
        }*/
    
    if( ( pid = fork() ) < 0 )
    {
        printf("fork error\n");
        return;
    }
   // printf("%s\n",arg[0]);
    

    switch(t)
    {
        case 0:
        if(pid == 0)
        {
           // printf("%s\n", arg[0]);
            if(!( find_mingli(arg)))
            {
            printf("系统目录下没有%s这个命令\n",arg[0]);
            exit(0);
            }
           // printf("%s\n", arg[0]);
       if(   execvp(arg[0],arg) ==-1)
            {
                perror("execvp");
            }
          exit(0);
        
        }
        break;
        case 1://输出流重定向
        if( pid == 0 )
        {
            if( !( find_mingli(arg) ) )
            {
                printf("没有这个%s命令\n", arg[0]);
                exit(0);
            }
         fd = open(file, O_RDWR|O_CREAT|O_TRUNC, 0644);
            if( fd < 0 )
            {
                my_error("open", __LINE__);
            }

            dup2( fd, 1 );
            execvp(arg[0], arg);
            exit(0);

        }
        break;
        case 2://输入流重定向
        if( pid == 0 )
        {
            if( !find_mingli( arg ) )
            {
                printf("没有这个命令%s\n", arg[0]);
                exit(0);
            }
            if( (fd = open( file, O_RDONLY )) < 0 )
            {
                my_error("open", __LINE__);
            }
            dup2( fd, 0 );
            execvp(arg[0], arg);
            exit(0);
        }
    break;
       case 3://在子进程中先执行管道符前面的，在父进程中执行后面的，父进程等待子进程结束
        if( pid == 0 )
        {
        //   printf("sdfsadf\n"); 
             
            if( (pid2 = fork()) <0 )
            {
                printf("make process erron\n");
                exit(-1);
            }
            if( pid2==0 )//在子进程中先执行管道符前面的
            {
              if( !find_mingli(arg) )
                {
                    printf("没有这个命令%s\n",arg[0]);
                    exit(-1);
                }
                if( (fd2 = open("/tmp/xiaoming", O_RDWR|O_CREAT|O_TRUNC, 0644)) <0 )
                {
                    my_error("open", __LINE__);
                    exit(-1);
                }
                dup2( fd2, 1 );//将父进程文件的文件描述符复制给新的FD2，这样fd2中就包含了父进程的所有信息
                execvp( arg[0], arg );//在环境变量中查找可执行的程序，执行参数arg中的命令
                exit(0);//关闭进程，他有一个返回值，在8到15之间，这个值放到内核中等待父进程通过wait(&reslt)来取这个值

            }
            if( waitpid(pid2, &ccc, 0  ) <0 )//父进程等待子进程结束
            {
                printf("等待失败！\n");
            }
            if( !find_mingli(argnext) )
            {
                printf("没有这个命令%s\n", argnext[0]);
                exit(-1);
            }
            fd2 = open("/tmp/xiaoming", O_RDONLY);//打开子进程的那个文件，输入到当前的程序中，执行新的参数
            dup2(fd2, 0);
            execvp(argnext[0], argnext);
            if( remove( "/tmp/xiaoming" ) )//正确移除返回0, 没有移除返回-1
            {
                printf("remove error\n");
            }
            exit(0);
                   
        }
        break;
    default :
        break;



     }
     if( hou == 1 )
     {
         return ;
     }

     if( waitpid( pid, &status, 0) == -1 )
     {
         printf(" 等待错误  ！\n");
     }





}//这个函数写完，有参数 t , arg , file ,  
int find_mingli(char *arg[256])//这个函数的作用是判断你输入的命令在系统的命令目录里只否有，有则返回1,无则返回0
{
   // printf("%s---------\n",arg[0]);
    DIR *dir;
    struct dirent *ptr;
    int i=0;

    char *path[] = {"./", "/bin", "/usr/bin", NULL};

    //是当前目录下的程序可以正常运行
      if( strncmp( arg[0], "./", 2 ) == 0 )
    {
        arg[0] = arg[0] +2;//暂时不知道在什么情况下用
    }

    while(path[i]!= NULL)
    {
       if( ( dir = opendir(path[i])  ) <0 )
        {
            printf("没有这个参数 /bin\n");
        }
        while( (ptr = readdir(dir) ) != NULL )
        {
           // printf("%d   %s=====\n",i,ptr->d_name);
            if( strcmp( ptr->d_name , arg[0]) == 0 )
            {
             //  printf("%s-----+++==\n",ptr->d_name);
                closedir( dir);
                return 1;
            }
        }
        closedir(dir);
        i++;
    }
    return 0;
}






