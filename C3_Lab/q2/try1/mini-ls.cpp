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
int main(int argc, char *argv[])
{

    struct stat buf;

    DIR *dirp = opendir(argv[1]);
    dp = readdir(dirp);
    struct stat s;
    while (dp != NULL)
    {
        string temp=dp->d_name;
       // printf("%s\n", temp.c_str());
        dp = readdir(dirp);
        string s=argv[1];
        s+="/";
        s+=temp;
        if (stat(s.c_str(), &buf) == -1)
        {
            //perror("stat");
            return errno;
        }
        if(S_ISDIR(buf.st_mode))
        {
            s+="/";
            cout<<s<<endl;
        }
 
        //   cout<<dp->d_type;
    }
    closedir(dirp);
  

    return 0;
}