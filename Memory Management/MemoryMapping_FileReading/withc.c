#include <stdio.h>
#include <stdlib.h>

int main()
{
    
    FILE *filePointer;
    char ch;
	int count=0;

    
    filePointer = fopen("loremipsum.txt", "r");

    
    if (filePointer == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        
        while ((ch = fgetc(filePointer)) != EOF)
        {
            if(ch=='a'){
				count++;
			}
        }
	   printf("The occurence of character 'a' is %d times. \n", count);
    }

    
    fclose(filePointer);

    return 0;
}