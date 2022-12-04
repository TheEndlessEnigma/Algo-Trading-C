#ifndef __GUARD_CALC_DECISION__
#define __GUARD_CALC_DECISION__

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "data_handling.h"
#include "shared.h"

// Function Declarations
float avg(float *data, int size, int n, int offset);
float sd(float *data, int size, int n, int offset);
float avgLong(unsigned long *data, int size, int n, int offset);
float sdLong(unsigned long *data, int size, int n, int offset);
int highestHigh(float *open, float *high, float *low, float *close, unsigned long *volume, unsigned long *time, int size, int count);
int buyVolumePrice(float *open, float *high, float *low, float *close, unsigned long *volume, unsigned long *time, int size, int count);
int buyVolumePrice2(struct list *l, struct shared *m, int stock_index, int count);
int sellVolumePrice(float *close, float buyPrice, int buyIndex, int size);
void getHighestInRange(float *highest, int *highestIndex, float *data, int startIndex, int endIndex);
int rateIncrease(struct list **listArr, struct shared *m);
#endif

