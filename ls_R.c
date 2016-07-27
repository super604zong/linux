# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <limits.h>
# include <dirent.h>

void display_dir(char *path);
void my_err(const char * err_string, int line)
{
    fprintf(stderr, "line : %d", line);
    perror(err_string);
    exit(-1);
}

int main(int argc, char ** argv)
{
   // printf("xiao\n");
    char path[PATH_MAX+1];
    if( argc == 1 )
    {
  
       // strcpy(path, "./");
       if( getcwd(path, 512) == -1)
        {
            my_err("getcwd", __LINE__);
        }
        strcat( path, "/" );
      //  printf(" %s\n",path);
       // path[9] = '\0';
      //  printf("sfaaa\n");
        display_dir(path);
    }

    return 0;
}
void display_dir( char * path )
{
    int i, j,count=0;
    DIR * dir;
    struct stat buf;
    struct dirent * ptr;
    char filenames[256][PATH_MAX+1];
    char mulunames[256][PATH_MAX+1];
    char str[PATH_MAX+1];
 //  strcpy( mulunames[], path );


    if( ( dir  = opendir( path )) == -1 )
    {
        my_err("opendir", __LINE__);
    }
    i = 0;
      printf("%s\n",path);
    while( (ptr = readdir(dir)) != NULL)
    {
      //  printf("xingbuxing\n");

     //    strcpy( filenames[count], ptr->d_name );
       // filenames[count][strlen( ptr->d_name )] = '\0';
       // if( (strcmp( ptr->d_name, '.' ) != 0) &&  (strcmp( ptr->d_name , "..")!= 0 ))
       // {
      char filename[200];
        strcpy(filename, path);
        strcat(filename, ptr->d_name);
       // printf("-=-%s--=%s+++\n", ptr->d_name, filename);
            if( stat(filename, &buf) ==-1)
         // )
        {
            my_err("stat", __LINE__);
        }
       // printf("sfd\n");
        if( S_ISDIR( buf.st_mode ) && (strcmp( ptr->d_name, "." ) !=0) && ( strcmp( ptr->d_name , "..")!=0) )
        {

            mulunames[count][strlen(filename)] = '\0';
            
           // strcat( mulunames[count],'/' );
            if( filename[strlen( filename ) -1] != '/' )
            {
               // puts(filename);
                strcat(filename, "/");
              //  printf("!!!! %s\n",ptr->d_name)a
               // puts(filename);
            }
            else
            {
                filename[ strlen( filename ) ] = '\0';
            }

                strcat( mulunames[count],filename );
           // strcat( mulunames[count], ptr->d_name );
            mulunames[count][strlen( mulunames[count])] = '\0';
            //  display_dir( mulunames[count] );     
         //   printf(" $$$ %s\n", mulunames[count]);
            count++;
          //  printf("\n");

        }
        else{

            strcpy( filenames[i], ptr->d_name );
            filenames[i][strlen( ptr->d_name )] = '\0';
           // i++;
            printf("    %s",filenames[i]);
            i++;
            }
        // }
     }
//    closedir(dir);

//   printf("  %s \n", mulunames[0]);


   //printf(" %s\n", path);
/*
   for( j = 0; j< i; j++ )
    {
        printf("  %s", filenames[j]);
    }
    */
    printf("\n");

    if( count != 0 )
    
    {
       // printf("sf\n");
       for( j = 0; j<count; ++j )
        {
          // printf("!!  %s!!\n", mulunames[j]);
               
        display_dir( mulunames[j] );    
        }
         


   }
   
}
