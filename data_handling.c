#include "data_handling.h"

// create a new stock struct
struct stock newStock(const char *t)
{
	struct stock s;
    s.close = (float*)calloc(PRICE_SIZE, sizeof(float));
    s.open = (float*)calloc(PRICE_SIZE, sizeof(float));
    s.high = (float*)calloc(PRICE_SIZE, sizeof(float));
    s.low = (float*)calloc(PRICE_SIZE, sizeof(float));
    s.volume = (unsigned long*)calloc(PRICE_SIZE, sizeof(unsigned long));
    s.time = (unsigned long*)calloc(PRICE_SIZE, sizeof(unsigned long));
	strcpy(s.ticker, t);
	s.owned = 0;
	return s;
}

// create a new list struct
struct list newList(const char *t, int threadIndex)
{
	struct list l;
	l.s = malloc(STOCK_SIZE*sizeof(struct stock));
	strcpy(l.name, t);
	l.ss = 0;
	l.pvs = 0;
	l.threadIndex = threadIndex;
	l.time_offset_ms = threadIndex * THREAD_TIME_OFFSET;
	return l;
}

// free a list struct
void freeList(struct list *l)
{
    int i, j;
    for(i=0; i<STOCK_SIZE; ++i)
    {
        for(j=0; j<PRICE_SIZE; ++j)
        {
            free(l->s[j].close);
            free(l->s[j].open);
            free(l->s[j].high);
            free(l->s[j].low);
            free(l->s[j].volume);
            free(l->s[j].time);
        }
    }
    free(l->s);
}

// Add stock to list
int addList(struct list *l, struct stock s)
{
	l->s[l->ss++] = s;
	return 1;
}

// Remove stock from list
int remList(struct list *l, int index)
{
    //freeStock(l.s[index]);
	int i;
	for(i=index; i<l->ss-1; ++i)
		l->s[i] = l->s[i+1];
	--(l->ss);
	return 1;
}

// Move stock from one list to another
int moveList(struct list *to, struct list *from, int index)
{
	addList(to, from->s[index]);
	remList(from, index);
	return 1;
}

// Insert stock into list at index
int insertList(struct list *l, struct stock s, int index)
{
	int i;
	for(i = l->ss; i>index; --i)
		l->s[i] = l->s[i-1];
	l->s[index] = s;
	++(l->ss);
	return 1;
}

// Insert stock into sorted list by ticker
int insertByTicker(struct list *l, struct stock s)
{
	if(l->ss==0)
		return addList(l, s);
	int le = 0;
	int r = l->ss-1;
	int i = l->ss/2;
	int strcmp_result;
	while((strcmp_result = strcmp(s.ticker, l->s[i].ticker))!=0) {
		if(strcmp_result < 0)
			r = i - 1;
		else
			le = i + 1;
		i = (r-le)/2 + le;
		if(le>r)
			return insertList(l, s, i);
	}
	return insertList(l, s, i);
}

// Sort list by stock ticker
int sortByTicker(struct list *l)
{
	struct list new;
	new.pvs = l->pvs;
	int i;
	for(i=0; i<l->ss; ++i)
		insertByTicker(&new, l->s[i]);
	for(i=0; i<l->ss; ++i)
		l->s[i] = new.s[i];
	return 1;
}

// Insert stock into sorted list by volume
int insertByVolume(struct list *l, struct stock s)
{
	if(l->ss==0)
		return addList(l, s);
	int le = 0;
	int r = l->ss-1;
	int i = l->ss/2;
	int result;
	while((result = (s.volume[l->pvs-1] - l->s[i].volume[l->pvs-1]))!=0) {
		if(result < 0)
			r = i - 1;
		else
			le = i + 1;
		i = (r-le)/2 + le;
		if(le>r)
			return insertList(l, s, i);
	}
	return insertList(l, s, i);
}

// Sort list by stock volume
int sortByVolume(struct list *l)
{
	struct list new;
	new.pvs = l->pvs;
	int i;
	for(i=0; i<l->ss; ++i)
		insertByVolume(&new, l->s[i]);
	for(i=0; i<l->ss; ++i)
		l->s[i] = new.s[i];
	return 1;
}

// Insert stock into sorted list by price
int insertByPrice(struct list *l, struct stock s)
{
	if(l->ss==0)
		return addList(l, s);
	int le = 0;
	int r = l->ss-1;
	int i = l->ss/2;
	int result;
	while((result = (s.close[l->pvs-1] - l->s[i].close[l->pvs-1]))!=0) {
		if(result < 0)
			r = i - 1;
		else
			le = i + 1;
		i = (r-le)/2 + le;
		if(le>r)
			return insertList(l, s, i);
	}
	return insertList(l, s, i);
}

// Sort list by stock price
int sortByPrice(struct list *l)
{
	struct list new;
	new.pvs = l->pvs;
	int i;
	for(i=0; i<l->ss; ++i)
		insertByPrice(&new, l->s[i]);
	for(i=0; i<l->ss; ++i)
		l->s[i] = new.s[i];
	return 1;
}
