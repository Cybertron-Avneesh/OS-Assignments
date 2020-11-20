#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;
struct dirent *dp;

void invalidRequests()
{
    cout << "Invalid Request" << endl;
    exit(0);
}

int validRequests(int argc, char *argv[])
{

    struct stat buf;
    struct stat buf2;
    string path=argc==3?argv[2]:argv[1];
    DIR *dirp = opendir(path.c_str());
    dp = readdir(dirp);
    struct stat s;
   
    while (dp != NULL)
    {
        string temp = dp->d_name;
        // printf("%s\n", temp.c_str());
        dp = readdir(dirp);
        string s = path;
        s += "/";
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
                cout<<endl;
            }
            
        }
        else
        {
            cout<<endl;
        }
        
       
        

        //   cout<<dp->d_type;
    }
    closedir(dirp);
   
}
int main(int argc, char *argv[])
{

    if (argc > 3 || (argc == 3 && strcmp(argv[1], "-L")))
    {
    
        invalidRequests();
    }

    validRequests(argc, argv);

    return 0;
}
