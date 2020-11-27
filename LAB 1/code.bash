#!/bin/bash

dir=" "                                                     # The absolute path of the directory will be contained in variable named dir
echo -n Enter the absolute path of the directory :
read dir 
cd "$dir"                                                   # Command to go to the directory $dir

filesDeleted=0                                              # variable to store number of files deleted

extension=" "                                               # The extension of the file will be contained in variable named extension
echo -n Enter the extension you want to delete : 
read extension


IFS=$'\n'                                                   # To set the default delimeter to a newline character
for ITEM in `find ./| grep "\\.$extension$"`
# To filter the files and folder of specified directory
do 
    abspath=`realpath "$ITEM"`                              # Used to find the exact path of the file/folder of given extension
    itemname=`basename "$abspath"`                          # Command used to get the filename/foldername
    if [ -d "$abspath" ];                                   # Command to detect a folder (will run if $abspath is a directory)
    then
        continue                                            # This command is used to continue FOR loop to skip process this time(for folder) and to continue for next file or folder
    fi
    
    input=' '                                               # Variable defination
    echo  -n Do you want to delete $itemname?[Y/N]          # Prompt user to ask to delete or not
    read input                                              # Value that user entered above is stored in input variable
    if [[ $input == 'y' || $input == 'Y' ]];                # This if statement will only executed if user wants to delete the file
    then
        rm $abspath                                         #rm command will the remove the file
        echo Name of the file delete  :   $itemname         #Command to show the filename that has been deleted
        echo Absolute path of the file : $abspath           #Command to print the path of the file deleted
        echo [ Deletion Succesful ]                         #Command to print that file has been successfully deleted
        filesDeleted="$[filesDeleted+1]"                    #Arithmetic command to increment total files deleted by 1
        echo                                                #Command to skip a line
    elif [[ $input == 'n' || $input == 'N' ]];              #This elif statement will only be executed if user does not wants to delete the file
    then
        echo [ FILE NOT DELETED ]                           #Command to display message that file is not deleted
        echo
    else                                                    #This else statement eill execute if user has entered a valid input
        echo [INVALID INPUT, FILE NOT DELETED]              #Command to display message invalid input
        echo
    fi                                                      #Compulsory statement for closing if-else statements
done                                                        #Compulsory statement to close the for loop        
unset IFS                                                   #This command is used to set IFS to its default value 

echo -n "Number of files deleted : " $filesDeleted          #Command to print the total deleted files. $filesDeleted  will return total files deleted
echo
exit 0                                                      #Basic command to exit
