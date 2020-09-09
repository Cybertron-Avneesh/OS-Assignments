#include <bits/stdc++.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace std;
#define ll long long int
const ll mod = 1e9 + 7;


set<int> generateFieldNumber(string fieldString,int maxLength)
{
    set<int> fieldNumbers;

    char comma = ',';

    size_t pos = 0;
    string token;

    int minField = 1e9;

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
            else if (i.back() == '-')
            {
                i.pop_back();
                int temp = atoi(i.c_str());
                minField=min(minField,temp);
               
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


    for (int i = minField; i <maxLength; i++)
    {
          fieldNumbers.insert(i);   
    }
    

    for (auto a : fieldNumbers)
    {
        if (a == 0)
        {
            validField = false;
            break;
        }
    }
    if(validField == false)
    {
        cout << "mycut: invalid field value found." << endl;
        exit(0);
    }
    return fieldNumbers;
}

int main(int argc, char **argv)
{

    bool DELEMITER_SPECIFIED = false;
    bool FIELD_NUMBER_SPECIFIED = false;
    int c;
    int digit_optind = 0;
    char delimiter;
    string str_fieldNumber;

    while (1)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"fields", required_argument, NULL, 'f'},
            {"delimiter", required_argument, NULL, 'd'},
            {NULL, 0, NULL, 0}};

        c = getopt_long(argc, argv, "d:f:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'd':
            // printf("option d with value '%s'\n", optarg);
            DELEMITER_SPECIFIED = true;
            delimiter = optarg[0];
            break;

        case 'f':
            // printf("option f with value '%s'\n", optarg);
            FIELD_NUMBER_SPECIFIED = true;
            str_fieldNumber = optarg;
            break;

        default:
            break;
        }
    }

    if (optind < argc)
    {   
        bool INVALID_OPT_FOUND = false;
        while (optind < argc){
            string _argv = argv[optind++];
            if(_argv.length() > 1)
                continue;
            else{
                cout<<_argv<<" ";
                INVALID_OPT_FOUND = true;
            }            
        }
        if (INVALID_OPT_FOUND)
        {
            printf(": option(s) not available.\nTry using [d] and [f] options");
            exit(0);    
        }
        
    }





/*********************2nd Part***********************/
    if (FIELD_NUMBER_SPECIFIED)
    {   
        char buffer;
        int i = 0;
        string line;
        char delim = (DELEMITER_SPECIFIED == true) ? delimiter : '\t';
       // set<int> fieldNumbers = generateFieldNumber(str_fieldNumber);
        vector<string> splitWords;
        int fileIndex = (DELEMITER_SPECIFIED)?5:((!DELEMITER_SPECIFIED)?3:argc-1);
        if(argv[fileIndex] == NULL){
            cout<<"mycut: file pathname not provided."<<endl;
            exit(0);
        }
        int fd = open(argv[fileIndex], O_RDONLY);
        
        // if (fd == -1) // file DNE
        // {
        //     perror(argv[fileIndex]);
        // }
        // else if (fd >= 0)
        // {
        //     string str;
        //     // cout << argv[fileIndex]<< ": File opened.\n";

        //     while (read(fd, &buffer, 1) == 1)
        //     {   
        //         if (buffer == '\n')
        //         {    
        //             size_t pos = 0;
        //             string token;
        //             while ((pos = line.find(delim)) != string::npos)
        //             {   
        //                 token = line.substr(0, pos);
        //                 splitWords.push_back(token);
        //                 // cout << token << endl;
        //                 line.erase(0, pos + sizeof(delim));
        //             }
        //             splitWords.push_back(line);
        //             //  cout << "outside loop" << endl;
        //             for (auto i : fieldNumbers)
        //             {
        //                 if (i - 1 >= splitWords.size())
        //                 {
        //                     cout << " ";
        //                 }
        //                 else
        //                 {
        //                     //cout << "possible error :- " <<i<< " "<<splitWords.size()<<endl;
        //                     if (i == (splitWords.size() - 1) || i == *fieldNumbers.rbegin())
        //                         cout << splitWords[i - 1];
        //                     else
        //                     {
        //                         cout << splitWords[i - 1] << delim;
        //                     }
        //                 }
        //             }
        //             cout << endl;
        //             line.clear();
        //             splitWords.clear();
        //         }
        //         else
        //         {
        //             line.push_back(buffer);
        //         }
        //     }
        // }

        if (fd == -1) // file DNE
    {
        // print program detail "Success or failure"
        perror("abc.txt");
    }
    else if (fd >= 0)
    {
        string str;
        // cout << "abc.txt"
        //      << ": File opened.\n";

        while (read(fd, &buffer, 1) == 1)
        {

            if (buffer == '\n')
            {
                size_t pos = 0;
                if ((pos = line.find(delim)) == string::npos)
                {
                    cout << line << endl;
                    line.clear();
                    splitWords.clear();
                }
                else
                {
                    pos = 0;
                    string token;
                    vector<string> printWord;
                    while ((pos = line.find(delim)) != string::npos)
                    {
                        token = line.substr(0, pos);
                        splitWords.push_back(token);
                        // cout << token << endl;
                        line.erase(0, pos + sizeof(delim));
                    }
                    splitWords.push_back(line);
                    set<int> fieldNumbers = generateFieldNumber(str_fieldNumber,splitWords.size());
                    //  cout << "outside loop" << endl;
                    for (auto i : fieldNumbers)
                    {
                        if (i - 1 < splitWords.size())
                        {
                            printWord.push_back(splitWords[i - 1]);
                        }
                    }
                    for (int i = 0; i < printWord.size(); i++)
                    {
                        if (i == (printWord.size() - 1))
                            cout << printWord[i];
                        else
                        {
                            cout << printWord[i] << delim;
                        }
                    }
                    cout << endl;
                    line.clear();
                    splitWords.clear();
                    printWord.clear();
                }
            }
            else
            {
                line.push_back(buffer);
            }
        }
    }
    }
    else
    {
        cout << "mycut: Atleast [-f] option must be used with argument.\n";
    }

    return 0;
}