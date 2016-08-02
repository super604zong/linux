/*************************************************************************
	> File Name: 6.c
	> Author: 
	> Mail: mZ	> Created Time: 2016年08月01日 星期一 14时22分34秒
 ************************************************************************/

#include<stdio.h>
# include <pthread.h>
# include <ctype.h>
# include <stdlib.h>

struct arg_set{
    char *fname;//文件的名字和文件的词数
    int count;
};

struct arg_set * mailbox;//容器
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;//锁
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;//小红旗

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    struct arg_set args1, args2;//属于两个线程各自的数据容器
    void *count_words(void *);
    int reports_in = 0;//记录线程的个数
    
    int total_words = 0;//记录总的词数
    if( argc!= 3){
        printf("usage: %s file1 file2\n", argv[0]);
        exit(1);
    }
    pthread_mutex_lock(&lock);//现将邮箱锁起来mZ
    args1.count = 0;
    args1.fname = argv[1];
    pthread_create(&t1, NULL, count_words, (void*)&args1);//结构体的地址

    args2.fname = argv[2];
    args2.count = 0;
    pthread_create(&t2, NULL,  count_words, (void *)&args2);//

    while( reports_in < 2 )
    {
        printf("MAIN: Waiting for flag to go up\n");
        pthread_cond_wait(&flag, &lock);//开始执行第一个线程
        printf("MAIN: WOw! flag was raised, i have the lock\n");
        printf(" %d : %s\n", mailbox->count, mailbox->fname);
        total_words +=mailbox->count;
        if( mailbox == &args1 )
           pthread_join(t1, NULL);//使得调用线程挂起直到由t1参数指定的线程终止
        if( mailbox == &args2)
        pthread_join(t2, NULL);
        mailbox = NULL;
        
        pthread_cond_signal(&flag);
        reports_in++;
 

    }
    printf("%7d: total_words\n",total_words);


    return 0;
}
void *count_words(void *a)
{
    struct arg_set * args = a;
    FILE * fp;
    int c, prevc = '\0';
    if( (fp = fopen(args->fname, "r")) != NULL )
    {
        while( (c =getc(fp)) != EOF){
            if( !isalnum(c) && isalnum(prevc) )
              args->count++;
            prevc = c;
        }
        fclose(fp);
    }
    else  
      perror(args->fname);
    printf("COUNT: Waiting to get lock\n");
    pthread_mutex_lock(&lock);
    printf("COUNT: have lock, storing data\n");
    if( mailbox  != NULL )
      pthread_cond_wait(&flag, &lock);
    mailbox = args;//结构体整体赋值，将第一个线程已经计算好的箱子，付给main中的箱子
    printf("COUNT: raitsing flag\n");
    pthread_cond_signal(&flag);//升起小红旗等待中心的人来将数据取走
    printf("COUNT: unlocking box\n");
    pthread_mutex_unlock(&lock);//解锁
    return NULL;



}
