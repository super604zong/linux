#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

int main(int argc,char **argv)
{
       char buf[100] = {0};
       int fd2, fd1, td;
        if(argc<3)//参数少于3，给出提示并退出
        {
                printf("请按如下格式运行！\n");
                printf("./程序 源文件 目标文件\n");
                return 0;
        }

      //只读方式打开源文件
        if(-1 == ( fd1 = open(argv[1],O_RDONLY)))
        {
                perror("打开文件失败");

                return 0;
        }

        fd2 = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0666);//读写方式打开文件，有则清空，没有则创建
        

        while(td = read(fd1,buf,sizeof(buf)))//从源文件读出
        {
                write(fd2,buf,td);//写入到目标文件
        }
        close(fd1);//关闭文件
        close(fd2);
        return 0;
}
