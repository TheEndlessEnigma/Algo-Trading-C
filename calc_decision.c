#include "calc_decision.h"

// get average
float avg(float *data, int size, int n, int offset)
{
	int i;
	float sum = 0.0;
	for(i=0; i<n; ++i)
		sum += data[size-offset-1-i];
	return sum/n;
}

// Calculate standard deviation of the last n values of a float array
float sd(float *data, int size, int n, int offset)
{
	if(n>size)
		return -1;
	float s = 0.0;
	float sd = 0.0;
	int i;
	for(i=0; i<n; ++i)
		s += data[size-offset-1-i];
	s /= n;
	for(i=0; i<n; ++i)
		sd += ((data[size-offset-1-i] - s)*(data[size-offset-1-i] - s));
	return sqrt(sd/(n-1));
}

float avgLong(unsigned long *data, int size, int n, int offset)
{
    int i;
    float sum = 0.0;
    for(i=0; i<n; ++i)
        sum += data[size-offset-1-i];
    return sum/n;
}

// Calculate standard deviation of the last n values of a float array
float sdLong(unsigned long *data, int size, int n, int offset)
{
    if(n>size)
        return -1;
    float s = 0.0;
    float sd = 0.0;
    int i;
    for(i=0; i<n; ++i)
        s += data[size-offset-1-i];
    s /= n;
    for(i=0; i<n; ++i)
        sd += ((data[size-offset-1-i] - s)*(data[size-offset-1-i] - s));
    return sqrt(sd/(n-1));
}

// buy decision
int highestHigh(float *open, float *high, float *low, float *close, unsigned long *volume, unsigned long *time, int size, int count)
{
	if(close[size-1] < close[0])
		return 0;
	int i;
	for(i=0; i<count; ++i) {
		if(close[size-1-i] < close[size-2-i])
			return 0;
		float x = (high[size-1-i] - low[size-1-i])/2.0 + low[size-1-i];
		if(x>close[size-1-i])
			return 0;
	}
	return 1;
}


int buyVolumePrice(float *open, float *high, float *low, float *close, unsigned long *volume, unsigned long *time, int size, int count)
{
    if(size < count+1)
        return 0;
    if(close[size-1] < (1.005 * close[size-1-count]))
        return 0;
    float mean = avgLong(volume, size, count, 1);
    float stddev = sdLong(volume, size, count, 1);
    if(volume[size-1] > mean + stddev)
        return 1;
    return 0;
}

int buyVolumePrice2(struct list *l, struct shared *m, int stock_index, int count)
{
    if(l->pvs < count+1)
        return 0;
    if(l->s[stock_index].close[l->pvs-1] < (1.005 * l->s[stock_index].close[l->pvs-1-count]))
        return 0;

    if(l->s[stock_index].volume_rate > (m->rate_avg + 2*m->rate_sd))
        return 1;
    return 0;
}

int sellVolumePrice(float *close, float buyPrice, int buyIndex, int size)
{
    if(close[size-1] < 0.99 * buyPrice)
        return 1;
    float highest;
    int highestIndex;
    getHighestInRange(&highest, &highestIndex, close, buyIndex, size-1);
    if(size-(highestIndex+1) > 60 && close[size-1] > buyPrice)
        return 1;
    if(size-(highestIndex+1) > 360 && close[size-1] <= buyPrice)
        return 1;
    if((close[size-1] < buyPrice + (highest-buyPrice)/2.0) && close[size-1] > 1.01*buyPrice)
        return 1;
    return 0;
}

void getHighestInRange(float *highest, int *highestIndex, float *data, int startIndex, int endIndex)
{
    int i;
    *highest = data[startIndex];
    *highestIndex = startIndex;
    for(i=startIndex+1; i<=endIndex; ++i)
    {
        if(data[i] >= *highest)
        {
            *highest = data[i];
            *highestIndex = i;
        }
    }
}


int rateIncrease(struct list **listArr, struct shared *m)
{
    if((*listArr)[0].pvs < 2 || m->error)
        return 0;
    int i, j;
    float sum = 0.0;
    m->rate_count = 0;
    for(i=0; i<m->num_threads; ++i)
    {
        for(j=0; j<(*listArr)[i].ss; ++j)
        {
            int p = (*listArr)[i].pvs;
            float tmp = (*listArr)[i].s[j].volume_rate;
            if(tmp>0)
            {
                sum += tmp;
                ++(m->rate_count);
            }
        }
    }
    m->rate_avg = (float)(sum / m->rate_count);
    m->rate_sd = 0.0;
    for(i=0; i<m->num_threads; ++i)
    {
        for(j=0; j<(*listArr)[i].ss; ++j)
        {
            int p = (*listArr)[i].pvs;
            float tmp = (*listArr)[i].s[j].volume_rate;
            if(tmp>0)
                m->rate_sd += (tmp-m->rate_avg)*(tmp-m->rate_avg);
        }
    }
    m->rate_sd /= m->rate_count;
    return 1;
}
