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

// Function to generate field number from the given fieldstring
set<int> generateFieldNumber(string fieldString, int maxLength)
{
    set<int> fieldNumbers;

    char comma = ',';

    size_t pos = 0;
    string token;

    int minField = 1e9;

    vector<string> fieldSubString;

    // To split the field string with comma "," as delimeter
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
                minField = min(minField, temp);
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

                // Decreasing range is invalid
                if (secondNumber < firstNumber)
                {
                    validField = false;
                }
                for (int x = firstNumber; x <= secondNumber; x++)
                {
                    fieldNumbers.insert(x);
                }
            }
        }
    }

    for (int i = minField; i <= maxLength; i++)
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
    if (validField == false)
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

    // to fetch list of options passed
    while (1)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"fields", required_argument, NULL, 'f'},
            {"delimiter", required_argument, NULL, 'd'},
            {NULL, 0, NULL, 0}};

        // "d:f:" denotes the optstring, which shows that d and f are legitimate options and they both need a valid argument
        c = getopt_long(argc, argv, "d:f:", long_options, &option_index);
        // returns -1 when all characters of argv are scanned, exit condition
        if (c == -1)
            break;

        switch (c)
        {
        case 'd':
            DELEMITER_SPECIFIED = true;
            delimiter = optarg[0];
            break;

        case 'f':
            FIELD_NUMBER_SPECIFIED = true;
            str_fieldNumber = optarg;
            break;

        default:
            break;
        }
    }

    // to check if there are any invalid options used.
    if (optind < argc)
    {
        bool INVALID_OPT_FOUND = false;
        while (optind < argc)
        {
            string _argv = argv[optind++];
            if (_argv.length() > 1)
                continue;
            else
            {
                cout << _argv << " ";
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
        // set the delimeter either to the specififed value aur to the
        // default value that is tab '\t'
        char delim = (DELEMITER_SPECIFIED == true) ? delimiter : '\t';

        vector<string> splitWords;

        //If the delimiter is provided  by the user then filename is stored at
        //5th position else store it  in
        //stored at 3rd position in the argument list

        int fileIndex = (DELEMITER_SPECIFIED) ? 5 : ((!DELEMITER_SPECIFIED) ? 3 : argc - 1);
        if (argv[fileIndex] == NULL)
        {
            cout << "mycut: file pathname not provided." << endl;
            exit(0);
        }

        //syscall to ope the file in read-only mode
        int fd = open(argv[fileIndex], O_RDONLY);

        if (fd == -1) // file DNE
        {
            // print program detail "Success or failure"
            perror(argv[fileIndex]);
        }
        else if (fd >= 0)
        {
            string str;

            int endOfFile = 0;

            //Syscall to read file  character by character and store it in a buffer
            while ((endOfFile = read(fd, &buffer, 1)) >= 0)
            {

                //When the line does not contain the delimiter then print the whole line
                if (buffer == '\n' || endOfFile == 0)
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

                        //Split the line according to the delimiter and store the word in spliwords Array.
                        while ((pos = line.find(delim)) != string::npos)
                        {
                            token = line.substr(0, pos);
                            splitWords.push_back(token);

                            line.erase(0, pos + sizeof(delim));
                        }
                        splitWords.push_back(line);
                        set<int> fieldNumbers = generateFieldNumber(str_fieldNumber, splitWords.size());

                        //All the word to be printed are stored in printWord array
                        for (auto i : fieldNumbers)
                        {
                            if (i - 1 < splitWords.size())
                            {
                                printWord.push_back(splitWords[i - 1]);
                            }
                        }

                        //Printing the whole array printWord array
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
                //Terminating the loop when end of the file is reached.
                if (endOfFile == 0)
                {
                    break;
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