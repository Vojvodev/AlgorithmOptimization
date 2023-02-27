#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif					// Uklanjanje nekih upozorenja na Visual Studio

#include<stdio.h>
#include<stdlib.h>

#define CREATED_INPUT_FILE "input_0-50000.bin"

int main(int args, char* argv[])
{

	int numOfRanges = 100;
	int numOfElements = 2 * numOfRanges;
	int* rangesArray = (int*)calloc(numOfElements, sizeof(int));
	FILE* fp = fopen(CREATED_INPUT_FILE, "wb");

	if (fp == NULL) { return(printf("ERROR WHILE CREATING A FILE"), -1); }

	fwrite(&numOfRanges, sizeof(int), 1, fp);

	int i = 0;
	int first = 0, last = 50000;
	while (i < numOfRanges)
	{

		rangesArray[i * 2] = first;
		rangesArray[i * 2 + 1] = last;
		
		first += 40; last += 130;

		printf("%d First: %d   Last: %d \n", i, rangesArray[i * 2], rangesArray[i * 2 + 1]);
		
		i++;
	}

	fwrite(rangesArray, sizeof(int), numOfElements, fp);
	fclose(fp);

	return 0;
}