#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <signal.h>

using namespace std;

#define ll long long int
const ll mod = 1e9 + 7;

int main()
{
    char buffer;
    int i = 0;
    string line;
    string delim = ";";
    set<int> fieldNumbers;
    fieldNumbers.insert(1);
    fieldNumbers.insert(2);
    fieldNumbers.insert(3);
    fieldNumbers.insert(9);
    vector<string> splitWords;

    int fd = open("abc.txt", O_RDONLY);
    if (fd == -1) // file DNE
    {
        // print program detail "Success or failure"
        perror("abc.txt");
    }
    else if (fd >= 0)
    {
        string str;
        cout << "abc.txt"
             << ": File opened.\n";

        while (read(fd, &buffer, 1) == 1)
        {

            if (buffer == '\n')
            {
                
                size_t pos = 0;
                string token;
                while ((pos = line.find(delim)) != string::npos)
                {
                    token = line.substr(0, pos);
                    splitWords.push_back(token);
                   // cout << token <<endl;
                    line.erase(0, pos + sizeof(delim));
                }
                for (auto i:fieldNumbers)
                {
                     if(i-1>splitWords.size())
                     cout<<" ";
                     else
                     {
                        cout<<splitWords[i-1]<<delim;
                     }
                               
                }
                
                cout<<endl;
                line.clear();
                splitWords.clear();
            }
            else
            {
                line.push_back(buffer);
            }
        }
    }
    return 0;
}
