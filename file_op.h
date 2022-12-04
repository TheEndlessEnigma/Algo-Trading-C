#ifndef __GUARD_FILE_OP__
#define __GUARD_FILE_OP__

#define TICKER_ARR_SIZE 4000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "data_handling.h"


// Function Declarations
int loadTickers(struct list **listArr, int *listSize, const char *fname, int groupSize);

#endif