#ifndef __GUARD_THREADS__
#define __GUARD_THREADS__

#define MAX_THREADS 32
#define MAX_STOCKS_PER_THREAD 130
#define LOOKBACK_PERIOD 12

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "structs.h"
#include "data_handling.h"
#include "data_curl.h"
#include "shared.h"
#include "calc_decision.h"

// GLOBAL VARS


// FUNCTIONS
void *curl_thread(void *ptr);
void *trade_thread(void *ptr);
int threadsRunning(struct package *p, int num_threads);
int newThread(struct list *l, struct shared *m, struct package *p, int type);
int executeThreads(struct list **listArr, struct shared *m, int num_threads);

#endif