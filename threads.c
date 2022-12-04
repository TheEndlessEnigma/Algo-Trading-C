#include "threads.h"

// thread function
pthread_mutex_t locket;
void *curl_thread(void *ptr)
{
    struct package *p = (struct package*)ptr;
    struct list *l = p->l;
    struct shared *m = p->m;
    usleep(l->time_offset_ms * 1000);
    getList(l);
    int i=0;
    int error=0;

    for(i=0; i<l->ss && l->pvs>LOOKBACK_PERIOD+1; ++i)
    {
        //printf("ERROR: %s, volume: %ld, high: %f, low: %f, close: %f\n", l->s[i].ticker, l->s[i].volume[l->pvs-1], l->s[i].high[l->pvs-1], l->s[i].low[l->pvs-1], l->s[i].close[l->pvs-1]);
        if(error || (l->s[i].close[l->pvs-1] < 0.1 * l->s[i].close[l->pvs-2]) || (l->s[i].close[l->pvs-1] > 5.0 * l->s[i].close[l->pvs-2]))
        {
            // printf("ERROR: %s, volume: %ld, high: %f, low: %f, close: %f\n", l->s[i].ticker, l->s[i].volume[l->pvs-1], l->s[i].high[l->pvs-1], l->s[i].low[l->pvs-1], l->s[i].close[l->pvs-1]);
            l->s[i].close[l->pvs-1] = l->s[i].close[l->pvs-2];
            l->s[i].open[l->pvs-1] = l->s[i].open[l->pvs-2];
            l->s[i].high[l->pvs-1] = l->s[i].high[l->pvs-2];
            l->s[i].low[l->pvs-1] = l->s[i].low[l->pvs-2];
            l->s[i].volume[l->pvs-1] = l->s[i].volume[l->pvs-2];
            l->s[i].time[l->pvs-1] = l->s[i].time[l->pvs-2];
            error = 1;
            m->error = 1;
            continue;
        }
        else
        {
            l->s[i].volume_rate = (l->s[i].volume[l->pvs-1] - l->s[i].volume[l->pvs-2]) / l->s[i].volume[l->pvs-2];
        }
    }

    if(error)
        printf("..%d hiccup..", l->threadIndex);
    else
        printf("%d", l->threadIndex);
    p->is_running = 0;
    pthread_exit(NULL);
}


void *trade_thread(void *ptr)
{
	struct package *p = (struct package*)ptr;
	struct list *l = p->l;
	struct shared *m = p->m;

    if(m->error)
    {
        p->is_running = 0;
        pthread_exit(NULL);
    }

    int i;
	for(i=0; i<l->ss && l->pvs>LOOKBACK_PERIOD+1; ++i)
	{
        if(!(l->s[i].owned) && buyVolumePrice(l->s[i].open, l->s[i].high, l->s[i].low, l->s[i].close, l->s[i].volume, l->s[i].time, l->pvs, LOOKBACK_PERIOD))
		{
			//buy
            float current_cash_tmp;
            pthread_mutex_lock(&locket);
            current_cash_tmp = m->current_cash;
            pthread_mutex_unlock(&locket);
            if(current_cash_tmp > l->s[i].close[l->pvs-1])
            {
			    l->s[i].owned = 1;
                printf("\nBOUGHT: %s, bp: %f\n", l->s[i].ticker, l->s[i].close[l->pvs-1]);
			    l->s[i].buyPrice = l->s[i].close[l->pvs-1];
                l->s[i].buyIndex = l->pvs-1;
                pthread_mutex_lock(&locket);
			    m->current_cash -= (l->s[i].owned * l->s[i].buyPrice);
                pthread_mutex_unlock(&locket);
            }
		}
        else if(l->s[i].owned && sellVolumePrice(l->s[i].close, l->s[i].buyPrice, l->s[i].buyIndex, l->pvs))
		{
			//sell
            pthread_mutex_lock(&locket);
			m->current_cash += (l->s[i].close[l->pvs-1] * l->s[i].owned);
            pthread_mutex_unlock(&locket);
            printf("\nSOLD: %s, bp: %f, sp: %f\n", l->s[i].ticker, l->s[i].buyPrice, l->s[i].close[l->pvs-1]);
			l->s[i].owned = 0;
		}
	}
	p->is_running = 0;
	pthread_exit(NULL);
}


// return 1 as long as a thread is running - threads are detached and not joined
int threadsRunning(struct package *p, int num_threads)
{
	int i=0;
	while(i < num_threads){
		if(p[i++].is_running == 1)
			return 1;}
	return 0;
}

// create and run a new detached thread
int newThread(struct list *l, struct shared *m, struct package *p, int type)
{
	int i = l->threadIndex;
	p[i].is_running = 1;
	p[i].l = l;
	p[i].m = m;
	//p[i].mutex_lock = mutex;

	pthread_t thread1;
    int create_ret;
    if(type==0)
	    create_ret = pthread_create(&thread1, NULL, curl_thread, &p[i]);
    if(type==1)
        create_ret = pthread_create(&thread1, NULL, trade_thread, &p[i]);
	int detach_ret = pthread_detach(thread1);
	return 1;
}


// create and run threads for each list of stocks and wait for them to finish
int executeThreads(struct list **listArr, struct shared *m, int num_threads)
{
	int i;
	//pthread_mutex_t mutex;
	struct package p[MAX_THREADS];
	for(i=0; i<num_threads; ++i)
		newThread(&((*listArr)[i]), m, p, 0);
	while(threadsRunning(p, num_threads));
    rateIncrease(listArr, m);
    for(i=0; i<num_threads; ++i)
        newThread(&((*listArr)[i]), m, p, 1);
    while(threadsRunning(p, num_threads));
    m->error = 0;
	return 1;
}
