#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<iostream>
#include<signal.h>


extern int errno; 
using namespace std;
void invalidArgumentsError(){
	printf("Incorrect Arguments passed.");
	printf("\nWrite like: \n ./mycut [-d] 'delimiter' [-f] FieldNumber file1 file2 file3 ...\n");
	exit(0);
}

int main(int argc, char **argv){
	string str[10];
	string delimiterValue;
	string fieldNumber;

	string *listFiles;
	if(argc < 6){
		invalidArgumentsError();
	}

	printf("%s\n%s\n%s\n%s\n%s\n", argv[1],argv[2], argv[3], argv[4], argv[5]);

	for (int i = 0; ; ++i)
	{
		if(argv[i] == NULL){
			break;
		}
		str[i] = argv[i];
	}

	if(str[1] == "-d" && str[3] == "-f"){
		delimiterValue = str[2];
		fieldNumber = str[4];
	}
	else if(str[1] == "-f" && str[3] == "-d"){
		delimiterValue = str[4];
		fieldNumber = str[2];
	}
	else{
		invalidArgumentsError();
	}
	cout<<"___________________________________________"<<endl;
	cout<<"delimeter value      "<<delimiterValue<<endl;
	cout<<"fieldNumber          "<<fieldNumber<<endl;
	cout<<"___________________________________________"<<endl;

	for (int i = 5; ; ++i)
	{	
		if(argv[i] == NULL){
			break;
		}
		int fd = open(argv[i], O_RDONLY);  
      
    	if (fd ==-1) // file DNE
    	{ 
        	// print program detail "Success or failure" 
        	perror(argv[i]);                  
    	} 
    	else if(fd >= 0){
    		cout<<argv[i]<<": File opened.\n";
    	}
	}
}
