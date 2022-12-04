#include "structs.h"
#include "data_handling.h"
#include "shared.h"
#include "data_curl.h"
#include "calc_decision.h"
#include "threads.h"
#include "file_op.h"
#include <stdio.h>


int main()
{
	struct list *listArr;
	listArr = malloc(MAX_THREADS * sizeof(struct list));
	int listSize = 0;
	loadTickers(&listArr, &listSize, "sp500.txt", MAX_STOCKS_PER_THREAD);
	struct shared m;
	newShared(&m, 10000, 10000, 0, 10000, 0);
    m.num_threads = listSize;
	int i,j,run=0;
	for(;;)
	{
        unsigned long start_t = getEpoch();
        printf("run%d:", run++);
		executeThreads(&listArr, &m, listSize);
        /*
		for(i=0; i<listSize; ++i)
		{
			for(j=0; j<listArr[i].ss; ++j)
			{
				printf("%s: ", listArr[i].s[j].ticker);
				printf("%f\t", (listArr[i]).s[j].close[listArr[i].pvs-1]);
				printf("%ld\t", (listArr[i]).s[j].volume[listArr[i].pvs-1]);
				printf("position: %d\n", (listArr[i]).s[j].owned);
			}
		}
        */
		updateShared(&m, listArr, listSize);
		unsigned long end_t = getEpoch();
		printf("\ncurrent_stocks: $%f  ", m.current_stocks);
		printf("current_cash: $%f  ", m.current_cash);
		printf("num_positions: %d  ", m.num_positions);
		printf("current_value: $%f\n", m.current_value);
		start_t += 5;
		while(getEpoch() < start_t);
	}

	return 1;
}
