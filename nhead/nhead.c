#include <stdio.h>
#include <string.h>
#include <stdlib.h>


unsigned int mode=2;

char arr[128];
unsigned int argvL; // length of parameters (-c=2,-c-=3)
char files[9][255]; // array for file names
unsigned int fCnt = 0; //files counter
unsigned int vOn = 0; // -v mode; 1 - on, 0 - off
int nHead(char* fName, int nCnt, int cCnt) // work with file
{
	unsigned int number_of_lines = 0; // number of lines in file
	unsigned int fSize = 0; // number of chars in file
	FILE *infile; // file
	if((infile=fopen(fName, "r"))==NULL) //oppening file
		return 1;
	int ch;

	while (EOF != (ch=getc(infile))) // reading a char from file
	{
		fSize++; // increasing char counter
		if ('\n' == ch) // increasing lines counter
			number_of_lines++;
	}
	rewind(infile); // go back to the begining of the file

	if(argvL == 3 && mode == 0) // if -c-
		cCnt = fSize - cCnt; // chars count to print
	if(argvL == 3 && mode == 1) // if -n-
		nCnt = number_of_lines - nCnt; //lines count to print
	if(vOn)
		printf ("==> %s <==\n", fName);
	while (EOF != (ch=getc(infile)))  // reading a char from file
	{
		switch(mode){
			case 0 : // c mode
			if(cCnt>0)
			{
				printf("%c", ch);
				cCnt--;
			}
			break;
			case 1 : // n mode
				if(nCnt>0)
					printf("%c", ch);
				if ('\n' == ch)
					nCnt--;
			break;
		}
	}
	fclose(infile); // close file
	return 0;
}

int main(int argc, char ** argv)
{
	unsigned int cCnt=0, nCnt=0; // number of chars, lines
	if(argc < 2) // launch without arguments --> read input
	{
		char input[1024];
		for(;;)
		{
			fgets(input, 1024, stdin); // get 1024 chars until 'enter'
			printf("%s", input);
		}
	}
	else
	{
		for(int i = 1; i < argc; i++) // handle each argument
		{
			if(argv[i][0] == '-' && argv[i][1] == 'c') // handel -c and -c-
			{
				mode = 0; // -c => mode 0
				if(argv[i][2] == '-') // if -c-
					argvL=3;
				else
					argvL=2;
				for(int j = argvL; j < strlen(argv[i]); j++) // getting chars num
					if((argv[i][j] >= '0') && (argv[i][j] <= '9'))
						arr[j-argvL] = argv[i][j];
				cCnt=atoi(arr); // get number (int from chars array)
			}	
			else if(argv[i][0] == '-' && argv[i][1] == 'n') // handle -n and -n-
			{
				mode = 1; // -n => mode 1
				if(argv[i][2] == '-') // if -n-
					argvL=3;
				else
					argvL=2;
				for(int j = argvL; j < strlen(argv[i]); j++) // getting lines num
					if((argv[i][j] >= '0') && (argv[i][j] <= '9'))
						arr[j-argvL] = argv[i][j];
				nCnt=atoi(arr);// get number (int from chars array)
			}	
			else if(strcmp(argv[i],"-v")==0) // handle -v
				vOn = 1;
			else // -handel file names
			{
				strcpy(files[fCnt],argv[i]); // writing names of files to an array
				fCnt++; // files counter
			}
		}
	}
	if(fCnt > 1) // if files count is more then 1 turn on -v
		vOn = 1;
	if(mode == 2) // if no -c or -n arguments then do default output
	{	
		mode = 1; // default -n
		nCnt = 10; // default 10 lines
	}
	for (int i = 0; i < fCnt; i++) // handling files
	{
		if(nHead(files[i], nCnt, cCnt))
		printf("nHead: cannot open \'%s\' for reading: No such file or directory\n", files[i]);
	}	
	return 0;
}
