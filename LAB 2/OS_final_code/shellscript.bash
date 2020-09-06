#!/bin/bash
read -p "Please Enter the absolute pathname of the file: " pathname
if [ ! -f "$pathname" ];
then
	echo "No such file exists!!!!"
	flag=0
	while [ $flag -eq 0 ]
	do
		read -p "Please Enter the correct absolute pathname of the file: " pathname
		if [ -f "$pathname" ];
		then
			flag=`expr $flag+1`
			echo "file found!"
			break
		else
			echo "No such file exists!!!!"
			continue
		fi
	done
fi
read -p "Please Enter the absolute destination pathname: " destination_name
if [ ! -d "$destination_name" ];
then
	echo "No such directory exits!!!!"
	flag=0
	while [ $flag -eq 0 ]
	do
		read -p "Please Enter the correct absolute destination name: " destination_name
		if [ -d "$destination_name" ];
		then
			flag=`expr $flag+1`
			echo "directory found!"
			break
		else
			echo "No such directory exits!!!!"
			continue
		fi
	done
fi
IFS= read -r -p "Please enter the Delimiter: " delimiter
unset IFS
read -p "Please enter the fieldnumber: " fieldnumber
out="/output"
destination_file=$destination_name$out
`touch $destination_file`
`g++ mycut.cpp -o mycut`
`./mycut -d "$delimiter" -f $fieldnumber $pathname > $destination_file`
exit
