#include "shared.h"

// setup a new shared struct
int newShared(struct shared *m, float starting_balance, float current_cash, float current_stocks, float current_value, int num_positions)
{
	m->starting_balance = starting_balance;
	m->current_cash = current_cash;
	m->current_stocks = current_stocks;
	m->current_value = current_value;
	m->num_positions = num_positions;
    m->error = 0;
    m->rate_sd = 0;
    m->rate_avg = 0;
    m->rate_count = 0;
	return 1;
}

// update shared struct position info
int updateShared(struct shared *m, struct list *listArr, int numLists)
{
	int i;
	int j;
	double current_stocks_sum = 0.0;
	int positions = 0;
	for(i=0; i<numLists; ++i)
	{
		for(j=0; j<listArr[i].ss; ++j)
		{
			if(listArr[i].s[j].owned)
			{
				//printf("%f\n", current_stocks_sum);
				current_stocks_sum += (listArr[i].s[j].owned * listArr[i].s[j].close[ listArr[i].pvs-1 ]);
				++positions;
			}
		}
	}
	m->current_stocks = current_stocks_sum;
	m->current_value = current_stocks_sum + m->current_cash;
	m->num_positions = positions;
}
