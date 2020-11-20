#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;
struct dirent *directory;
string permissions(mode_t st){
    char perms[11];
    if(S_ISREG(st))
        perms[0] = '-';
    else if(S_ISDIR(st))
        perms[0] = 'd';
    else if(S_ISFIFO(st))
        perms[0] = '|';
    else if(S_ISSOCK(st))
        perms[0] = 's';
    else if(S_ISCHR(st))
        perms[0] = 'c';
    else if(S_ISBLK(st))
        perms[0] = 'b';
    else if(S_ISLNK(st))
        perms[0] = 'l';
    perms[1]=(st&S_IRUSR)?'r':'-';
    perms[2]=(st&S_IWUSR)?'w':'-';
    perms[3]=(st&S_IXUSR)?'x':'-';
    perms[4]=(st&S_IRGRP)?'r':'-';
    perms[5]=(st&S_IWGRP)?'w':'-';
    perms[6]=(st&S_IXGRP)?'x':'-';
    perms[7]=(st&S_IROTH)?'r':'-';
    perms[8]=(st&S_IWOTH)?'w':'-';
    perms[9]=(st&S_IXOTH)?'x':'-';
    perms[10]='\0';
    string d=perms;
    return d;
}
int validRequests(int argc,string str){
    struct stat buffer;
    struct stat details;
    string path=str;
    DIR *open_directory=opendir(path.c_str());
    directory=readdir(open_directory);
    int flag=0;
    while(directory!= NULL){
        string temp = directory->d_name;
        string s = path;
        s+="/";
        s+=temp;
        if(stat(s.c_str(),&buffer)==-1){
            perror("stat");
            return errno;
        }
        if(lstat(s.c_str(),&details)==-1){
            perror("stat");
            return errno;
        }
        if(S_ISDIR(buffer.st_mode)){
            temp+="/";
        }
        if(argc!=3)
            cout<<temp<<" ";
        if(argc==3){
            if(flag==0){
                cout<<"permission    no. of hardlinks   owner of file   user group   size   date & time modified   date & time created   file or directory name\n\n";
                flag=1;
            }
            string permission=permissions(details.st_mode);
            cout<<permission<<"\t    ";
            printf("%d\t\t     ", (int)buffer.st_nlink);
            struct passwd pwent,*pwentp;
            struct group grp,*grpt;
            char datestring[256];
            struct tm time;
            char storage[128];
            if(!getpwuid_r(buffer.st_uid, &pwent, storage, sizeof(storage), &pwentp)){
                printf("%s\t   ",pwent.pw_name);
            }                
            else{
                printf("%d\t   ",buffer.st_uid);
            }
            if(!getgrgid_r(buffer.st_gid, &grp, storage, sizeof(storage), &grpt)){
                printf("%s\t     ",grp.gr_name);
            }
            else{
                printf("%d\t     ",buffer.st_gid);
            }
            printf("%5d   ",(int)buffer.st_size);
            localtime_r(&buffer.st_mtime, &time);
            strftime(datestring,sizeof(datestring),"%F %T",&time);
            printf(" %s   ",datestring);
            localtime_r(&buffer.st_ctime, &time);
            strftime(datestring,sizeof(datestring),"%F %T",&time);
            printf(" %s   ",datestring);
            if(S_ISLNK(details.st_mode)){
                char link_to[PATH_MAX];
                ssize_t r=readlink(s.c_str(),link_to,PATH_MAX);
                if(r!=-1){
                    link_to[r]='\0';
                    cout<<temp;
                    printf(" -> %s\n",link_to);
                }
            }
            else{
                cout<<temp <<" ";
                cout<<endl;
            }
        }
        else{
            cout<<endl;
        }
        directory=readdir(open_directory);
    }
    closedir(open_directory);
}
int main(int argc, char *argv[]){
    if (argc > 3 || (argc == 3 && strcmp(argv[1], "-L"))){
        perror("Invalid Request");
        exit(1);
    }
    if(argc==1){
        argc= 2;
        validRequests(argc,".");
    }
    else if(argc==2){
        if(!strcmp(argv[1],"-L")){
            argc=3;
            validRequests(argc,".");
        }
        else{
            validRequests(argc,argv[1]);
        }
    }
    else{
        validRequests(argc,argv[2]);
    }
    return 0;
}