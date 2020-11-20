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
void print_perms(mode_t st)
{
    char perms[11];
    if (st && S_ISREG(st))
        perms[0] = '-';
    else if (st && S_ISDIR(st))
        perms[0] = 'd';
    else if (st && S_ISFIFO(st))
        perms[0] = '|';
    else if (st && S_ISSOCK(st))
        perms[0] = 's';
    else if (st && S_ISCHR(st))
        perms[0] = 'c';
    else if (st && S_ISBLK(st))
        perms[0] = 'b';
    else
        perms[0] = 'l'; // S_ISLNK
    perms[1] = (st && S_IRUSR) ? 'r' : '-';
    perms[2] = (st && S_IWUSR) ? 'w' : '-';
    perms[3] = (st && S_IXUSR) ? 'x' : '-';
    perms[4] = (st && S_IRGRP) ? 'r' : '-';
    perms[5] = (st && S_IWGRP) ? 'w' : '-';
    perms[6] = (st && S_IXGRP) ? 'x' : '-';
    perms[7] = (st && S_IROTH) ? 'r' : '-';
    perms[8] = (st && S_IWOTH) ? 'w' : '-';
    perms[9] = (st && S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';
    printf("%s", perms);
}
void validRequests_2(char path[])
{
    DIR *dir;
    dirent *file;
    struct stat sbuf;
    char buf[128];
    struct passwd pwent, *pwentp;
    struct group grp, *grpt;
    char datestring[256];
    struct tm time;
    dir = opendir(path);
    while (file = readdir(dir))
    {
        if(!strcmp(file->d_name,".."))
        {
            exit(0);
        }
        stat(file->d_name, &sbuf);
        print_perms(sbuf.st_mode);
        printf(" %d", (int)sbuf.st_nlink);
        if (!getpwuid_r(sbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
            printf(" %s", pwent.pw_name);
        else
            printf(" %d", sbuf.st_uid);

        if (!getgrgid_r(sbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
            printf(" %s", grp.gr_name);
        else
            printf(" %d", sbuf.st_gid);
        printf(" %5d", (int)sbuf.st_size);

        localtime_r(&sbuf.st_mtime, &time);
        /* Get localized date string. */
        strftime(datestring, sizeof(datestring), "%F %T", &time);

        printf(" %s %s\n", datestring, file->d_name);
    }
}
void ls_r(char path[])
{
    DIR *dir;
    dirent *file;
    struct stat sbuf;
    char tmp[128];
    dir = opendir(path);
    while (file = readdir(dir))
    {
        if (file->d_name[0] == '.')
            continue;
        strcpy(tmp, path);
        strcat(tmp, "/");
        strcat(tmp, file->d_name);
        //printf("%s\n", tmp);
        stat(tmp, &sbuf);
        if (sbuf.st_mode && S_ISDIR(sbuf.st_mode))
        {
            printf("\n%s/\n", tmp);
            ls_r(tmp);
        }
        else
            printf("%s  ", file->d_name);
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
            validRequests_2("./");
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

