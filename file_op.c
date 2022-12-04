#include "file_op.h"

// read tickers from files and load them into array of lists
int loadTickers(struct list **listArr, int *listSize, const char *fname, int groupSize)
{
	char **tickerArr = malloc(TICKER_ARR_SIZE*sizeof(char*));
	int i;
	for(i=0; i<TICKER_ARR_SIZE; ++i)
		tickerArr[i] = malloc(10*sizeof(char));
	FILE* filePointer;
	int bufferLength = 10;
	char buffer[bufferLength]; /* not ISO 90 compatible */
	filePointer = fopen(fname, "r");
	i=0;
	while(fgets(buffer, bufferLength, filePointer))
	{
		buffer[strcspn(buffer, "\n")] = 0;
		strcpy(tickerArr[i++], buffer);
	}
	fclose(filePointer);

	int numLists = ceil(((float)i) / ((float)groupSize));
	*listArr = malloc(numLists * sizeof(struct list));

	int x, y=0, z;
	for(x=0; x<numLists; ++x)
	{
		(*listArr)[x] = newList("default", x);
		for(z=0; z<groupSize; ++z)
		{
			struct stock temp = newStock(tickerArr[y++]);
			addList(&(*listArr)[x], temp);
			if(y==i)
				break;
		}
	}
	*listSize = numLists;
	for(i=0; i<TICKER_ARR_SIZE; ++i)
		free(tickerArr[i]);
	free(tickerArr);
	return 1;
}
