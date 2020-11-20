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
void display_para(mode_t st)
{
    char parameters[11];
    if (st && S_ISREG(st))
        parameters[0] = '-';
    else if (st && S_ISDIR(st))
        parameters[0] = 'd';
    else if (st && S_ISFIFO(st))
        parameters[0] = '|';
    else if (st && S_ISSOCK(st))
        parameters[0] = 's';
    else if (st && S_ISCHR(st))
        parameters[0] = 'c';
    else if (st && S_ISBLK(st))
        parameters[0] = 'b';
    else
        parameters[0] = 'l'; // S_ISLNK
    parameters[1] = (st && S_IRUSR) ? 'r' : '-';
    parameters[2] = (st && S_IWUSR) ? 'w' : '-';
    parameters[3] = (st && S_IXUSR) ? 'x' : '-';
    parameters[4] = (st && S_IRGRP) ? 'r' : '-';
    parameters[5] = (st && S_IWGRP) ? 'w' : '-';
    parameters[6] = (st && S_IXGRP) ? 'x' : '-';
    parameters[7] = (st && S_IROTH) ? 'r' : '-';
    parameters[8] = (st && S_IWOTH) ? 'w' : '-';
    parameters[9] = (st && S_IXOTH) ? 'x' : '-';
    parameters[10] = '\0';
    printf("%s", parameters);
}
void validRequests_2(string s)
{
    DIR *dir;
    dirent *dp;
    struct stat buff;
    char buf[128];
    struct passwd pass, *passP;
    struct group grp, *grpt;
    char datestring[256];
    struct tm time;
    string path=s;
    dir = opendir(path.c_str());
    while (dp = readdir(dir))
    {
        if(!strcmp(dp->d_name,".."))
        {
            exit(0);
        }
        stat(dp->d_name, &buff);
        display_para(buff.st_mode);
        printf(" %d", (int)buff.st_nlink);
        if (!getpwuid_r(buff.st_uid, &pass, buf, sizeof(buf), &passP))
            printf(" %s", pass.pw_name);
        else
            printf(" %d", buff.st_uid);

        if (!getgrgid_r(buff.st_gid, &grp, buf, sizeof(buf), &grpt))
            printf(" %s", grp.gr_name);
        else
            printf(" %d", buff.st_gid);
        printf(" %5d", (int)buff.st_size);

        localtime_r(&buff.st_mtime, &time);
        /* Get localized date string. */
        strftime(datestring, sizeof(datestring), "%F %T", &time);

        printf(" %s %s\n", datestring, dp->d_name);
    }
}


struct dirent *dp;

void invalidRequests()
{
    cout << "Invalid Request" << endl;
    exit(0);
}

int validRequests(int argc, char *argv[], bool current)
{

    struct stat buf;
    struct stat buf2;
    string path;
    if (current)
    {
        path = "./";
    }
    else
    {
        path = argc == 3 ? argv[2] : argv[1];
    }

    DIR *dirp = opendir(path.c_str());
    dp = readdir(dirp);
    struct stat s;

    while (dp != NULL)
    {
        string temp = dp->d_name;
        // printf("%s\n", temp.c_str());
        dp = readdir(dirp);
        string s = path;
        if (!current)
        {
            s += "/";
        }
        if (!strcmp(temp.c_str(), ".."))
        {
            exit(0);
        }
        s += temp;
        if (stat(s.c_str(), &buf) == -1)
        {
            //perror("stat");
            return errno;
        }
        if (lstat(s.c_str(), &buf2) == -1)
        {
            //perror("stat");
            return errno;
        }

        if (S_ISDIR(buf.st_mode))
        {
            temp += "/";
        }
        cout << temp << " ";
        if (argc == 3)
        {
            if (S_ISLNK(buf2.st_mode))

            {
                char symLink[PATH_MAX];
                ssize_t r = readlink(s.c_str(), symLink, PATH_MAX);
                if (r != -1)
                {
                    symLink[r] = '\0';
                    printf(" -> %s\n", symLink);
                }
            }
            else
            {
                cout << endl;
            }
        }
        else
        {
            cout << endl;
        }
        //   cout<<dp->d_type;
    }
    closedir(dirp);
}
int main(int argc, char *argv[])
{

    if (argc > 3 )
    {

        invalidRequests();
    }

    if (argc == 1)
    {

        validRequests(argc, argv, true);
    }
    if (argc == 2)
    {
        if (!strcmp(argv[1], "-L"))
        {
            validRequests(argc, argv, true);
        }
        if (!strcmp(argv[1], "-l"))
        {
            string curr="./";
            validRequests_2(curr.c_str());
        }
    }   

    if (!strcmp(argv[1], "-L"))
        validRequests(argc, argv, false);
    
    if (!strcmp(argv[1], "-l"))
    {
        validRequests_2(argv[2]);
    }

    return 0;
}
