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

int main()
{
    string fieldString;
    cout<<"enter a field string ";
    cin >> fieldString;
    set<int> fieldNumbers;

    char comma = ',';

    size_t pos = 0;
    string token;

    vector<string> fieldSubString;

    while ((pos = fieldString.find(comma)) != string::npos)
    {
        token = fieldString.substr(0, pos);

        fieldSubString.push_back(token);

        fieldString.erase(0, pos + sizeof(comma));
    }
    fieldSubString.push_back(fieldString);
    bool validField = true;

    // to check for non numeric character excluding hiphen

    for (auto i : fieldSubString)
    {
        if (count(i.begin(), i.end(), '-') > 1)
        {
            validField = false;
            break;
        }
        for (auto a : i)
        {
            if ((a < 48 || a > 57) && a != '-')
            {
                validField = false;
                break;
            }
        }
    }
   

    for (auto i : fieldSubString)
    {
        if (count(i.begin(), i.end(), '-') == 0)
        {
            int temp = atoi(i.c_str());
            fieldNumbers.insert(temp);
        }
        else
        { // when '-' is at start
            if (*i.begin() == '-')
            {
                i.erase(i.begin());
                int temp = atoi(i.c_str());
                // cout<<temp<<endl;
                for (int i = 1; i <= temp; i++)
                {
                    fieldNumbers.insert(i);
                }
            }
            // when '-' is at end
            else if (*i.end() == '-')
            {
                i.erase(i.end());
                int temp = atoi(i.c_str());
                for (int i = temp; i <= 100; i++)
                {
                    fieldNumbers.insert(i);
                }
            }
            // when '-' is somewhere in the middle
            else
            {
                string firstString;
                int a = 0;
                while (i[a] != '-')
                {
                    firstString.push_back(i[a]);
                    a++;
                }

                int firstNumber = atoi(firstString.c_str());

                string secondString;

                while (a < i.size())
                {
                    secondString.push_back(i[a]);
                    a++;
                }
                secondString.erase(secondString.begin());

                int secondNumber = atoi(secondString.c_str());
                //      cout<<firstNumber<<" "<<secondNumber<<endl;
                for (int x = firstNumber; x <= secondNumber; x++)
                {
                    fieldNumbers.insert(x);
                }
            }
        }
    }
    for (auto a : fieldNumbers)
        {
           if(a==0)
           {
               validField=false;
               break;
           }
        }
    if (validField == true)
    {
        for (auto a : fieldNumbers)
        {
            cout << a << " ";
        }
        cout<<endl;
    }
    else
    {
        cout<<"invalid field number"<<endl;
    }
    
}
