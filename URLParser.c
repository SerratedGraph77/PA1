/*URL Parser (Assignment # 1) For CS 3377.0W1*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isProtocolValid(char*);
int isDomainValid(char*);
int findChar(char*, char, int);
int isPortValid(int);
int isFilePathValid(char*);
int isParameterValid(char*, int);

int main()
{
	char url[] = "https://cs.utdallas.edu:80/directory/faculty/people.html?name=Robert"; //for testing purposes
	int invalidURL = 0; // 0 if valid, -1 if invalid

	fgets(url, (strlen(url) + 1), stdin); //get the URL from the user input	

	char tempProtocol[7], protocol[7];
	memcpy(tempProtocol, &url[0], 6); //copies the first 6 characters of the url
	tempProtocol[strlen(tempProtocol)] = '\0'; //set the null terminator (just in case)

	int numChar = isProtocolValid(tempProtocol);

	//Error message for an invalid protocol
	if (numChar == -1){
		invalidURL = -1; //Sets the URL to be invalid (for future print formatting)
		printf("Invalid URL with following erroneous components:\n");
		printf("%-12s", "Protocol: ");
		printf("%s is not a valid protocol.\n", tempProtocol);
	}
	else { //ftp, ftps, http, and https protocols
		memcpy(protocol, &url[0], (6 - numChar)); //subtract the non-alphabetic characters
		protocol[strlen(protocol)] = '\0'; //set the null terminator (just in case)
	}

	int endIndex, port=0; //port is default 0 to denote a valid port
	if (findChar(url, ':', 2) != -1){ //if we found a port
		endIndex = findChar(url, ':', 2); //copy just before the port
	}
	else{
		port = -1; //this means the URL does NOT have a port
		endIndex = findChar(url, '/', 3); //copies just before the file path
	}		


	int startIndex = findChar(url, '/', 2); //finds the index of the 2nd occurence of "/"
	startIndex++; //move to the character after

	char domain[50];
	memcpy(domain, &url[startIndex], (endIndex-startIndex)); //this is the domain
	domain[strlen(domain)] = '\0'; //set the null terminator (just in case)

	if (isDomainValid(domain) != 0){ //if the domain was invalid
		if (invalidURL != -1){ //if this is the first incorrect URL part
			printf("Invalid URL with following erroneous components:\n"); //print "invalid URL" message
			invalidURL = -1; //set the flag to show the URL is invalid
		}
		printf("%-12s", "Domain: ");
		printf("%s is not a valid domain.\n", domain);
	}


	//We are now finding the port

	if (port != -1){ //if there is a port (we checked earlier)
		startIndex = findChar(url, ':', 2); //location of the port (index)
		startIndex++; //character after the colon is the first port number
		endIndex = findChar(url, '/', 3); //copies just before the file path

		char portStr[10];
		memcpy(portStr, &url[startIndex], (endIndex-startIndex));
		portStr[strlen(portStr)] = '\0'; //set the null terminator (just in case)
		port = atoi(portStr); //converts a "string" to an integer

		if (isPortValid(port) != 0){ //if the port was invalid
			if (invalidURL != -1){ //if this is the first incorrect URL part
				printf("Invalid URL with following erroneous components:\n");
				invalidURL = -1; //set the flag to show the URL is invalid
			}
			printf("%-12s", "Port: ");
			printf("port number must be between 1 and 65535\n");
		}
	}


	//Getting and Verifying the File Path
	
	startIndex = findChar(url, '/', 3); //start at the file path
	
	if (findChar(url, '?', 1) == -1){ //Could not find parameters
		endIndex = (findChar(url, '.', 3) + 4); //ending at "htm"
		if (url[endIndex+1] == 'l'){ //if it's "html", not "htm"
			endIndex++; //Increment for "html"
		}
	}
	else{
		endIndex = findChar(url, '?', 1); //end just before the parameter(s)
	}

	char filePath[50];

	memcpy(filePath, &url[startIndex], (endIndex - startIndex)); //if no parameters, memcpy may copy extra bits, causing a memory leak
	filePath[strlen(filePath)] = '\0';
	int n = strlen(filePath); //the Following is to solve a memory leak due to repeated use of memcpy.
	if (n > (endIndex-startIndex)){ //if the string took more characters than expected, it's memory from somewhere else
		for (int i=0; i< (n - (endIndex-startIndex)); i++){
			filePath[n-i-1] = '\0'; //manually set the end of the string for each extra character
		}
	}


	//Invalid File Path - Printing the Error
	if (isFilePathValid(filePath) == -1){
		if (invalidURL != -1){ //if this is the first incorrect URL part
			printf("Invalid URL with following erroneous components:\n");
			invalidURL = -1; //set the flag to show the URL is invalid
		}
		printf("%-12s", "File Path: ");
		printf("%s is not a valid file path\n", filePath);
	}

	//We will Print everything except Parameters (they are continuous and optional, we will do them later)

	if (invalidURL != -1) //if the URL was NOT invalid
	{
		printf("%-12s", "Protocol: ");
		printf("%s\n", protocol);
		printf("%-12s", "Domain: ");
		printf("%s\n", domain);

		if (port != -1){ //if there is a valid port (ports are optional)
			printf("%-12s", "Port: ");
			printf("%d\n", port);
		}

		printf("%-12s", "File path: ");
		printf("%s\n", filePath);
	}
	else{
		return 0; //end the program if the URL was invalid
	}


	//Now checking the Parameters (starts with "?")

	startIndex = findChar(url, '?', 1);
	endIndex = findChar(url, '&', 1); //if no "&", a -1 is returned

	int numParameters = 1; //counter for the number of parameters

	while (startIndex != -1){ //while statement because it's optional and can have multiple
		char parameter[20]; //holds the string representation of the parameters
		if (numParameters == 1){ //if this is the first parameter
			printf("%-12s", "Parameters: ");
		}
		startIndex++; //start after the "?"

		int endOfURLFlag;
		if (endIndex == -1){ //if we reached the end of the string (no "&" was found)
			endOfURLFlag = endIndex;
			endIndex = strlen(url); //set the last index to avoid memory issues
		}
		memcpy(parameter, &url[startIndex], (endIndex - startIndex)); //copy the next parameter
		parameter[strlen(parameter)] = '\0'; //set the null terminator (just in case)

		//if this is valid, it will automatically print the parameters, if not, it doens't print
		if (isParameterValid(parameter, numParameters) == -1){ //if the parameter was invalid
			printf("%s is not a valid parameter\n", parameter);
			return 0; //end the program
		}

		if (endOfURLFlag == -1){ //if there are no more parameters to get
			startIndex = -1; //this will end the while loop
		}
		else{
			startIndex = endIndex;
		}
		endIndex = findChar(url, '&', ++numParameters); //increment counter for parameters
	}
	printf("\n"); //newline at the end of all the output
}

int isParameterValid(char *parameter, int numCount){
	
	char* str1 = strtok(parameter, "=");

	//Format should match xxx=yyy
	if (str1 != NULL){ //if there was an equal sign
		char* str2 = strtok(NULL, "="); //copies from the equal sign to the end of the string
		if (numCount > 1){ //if this is NOT the first parameter
			printf(", "); //place a comma to separate them
		}
		printf("%s=%s", str1, str2); //print the valid parameter
		return 0; //return 0 if everything was valid
	}
	else{
		return -1; //The parameters were invalid
	}
}

int isFilePathValid(char *path){
	int start = findChar(path, '.', 1);
	start++; //starting after the dot
	int end = strlen(path);

	char pathEnding[5];
	memcpy(pathEnding, &path[start], (end - start)); //copy everything after the dot (htm or html)

	//check if it ends with .htm or .html
	if ((strcmp(pathEnding, "htm") == 0) || (strcmp(pathEnding, "html") == 0)){
		return 0; //valid path
	}
	else{
		return -1; //invalid path
	}
}	

int isProtocolValid(char *protocol){

	if ((strcmp(protocol, "http:/") != 0) && (strcmp(protocol, "https:") != 0) && (strcmp(protocol, "ftps:/") != 0) && (strcmp(protocol, "ftp://") != 0)){
		return -1; //Invalid protocol
	}
	

	//This is used to determine what should be printed as the Protocol
	//It returns the number of non-alphabetic characters to help with formatting later
	if (strcmp(protocol, "ftp://") == 0){
		return 3;
	}
	else if (strcmp(protocol, "http:/") == 0){
		return 2;
	}
	else if (strcmp(protocol, "https:") == 0){
		return 1;
	}
	else if (strcmp(protocol, "ftps:/") == 0){
		return 2;
	}
}

int isPortValid(int num){

	//Checks the valid range of the port
	if ((num >= 1) && (num <= 65535)){
		return 0;
	}
	else{
		return -1;
	}
}

int isDomainValid(char *domain){
	//Checking the first part of the domain
	int start = 0;
	int end = findChar(domain, '.', 1); //find the first period

	if (end == -1){ //if there was no period
		return -1; //the domain is invalid
	}

	//Checking the second part of the domain

	start = end;
	end = findChar(domain, '.', 2); //find location of the second period

	if (end == -1){ //if we didn't find a second period
		return -1; //the domain is invalid
	}

	//Checking the third part of the domain
	
	start = ++end; //One character after the period
	end = strlen(domain); //Length of the string = its last index

	char name[3];
	memcpy(name, &domain[start], (end-start)); //copies everything after the second period

	//Checking if the host is valid
	if ((strcmp(name, "com") != 0) && (strcmp(name, "net") != 0) && (strcmp(name, "edu") != 0) && (strcmp(name, "biz") != 0) && (strcmp(name, "gov") != 0)){
		return -1; //if invalid
	}
	
	return 0; //No issues were found with the domain name
}

int findChar(char *str, char ch, int n){ //Finds nth occurence of a character and returns it's index in it's array
	int index=-1;
	for (int i=0; i<n; i++){ //repeat "n" times
		index++; //increment so that it doesn't find the same character every time
		for (int j=index; str[j]!='\0'; j++){ //search the entire string
			if (str[j] == ch){ //if we found the character at index j
				index = j; //set the index
				break;
			}
			index = j;
		}
	}
	if (str[index+1] == '\0'){ //if we didn't find the character
		return -1; //-1 means our character was not found
	}
	return index;
}

