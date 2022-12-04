#ifndef __GUARD_SHARED__
#define __GUARD_SHARED__

#include <stddef.h>
#include <stdlib.h>

#include "structs.h"
#include "data_handling.h"

// Function Declarations
int newShared(struct shared *m, float starting_balance, float current_cash, float current_stocks, float current_value, int num_positions);
int updateShared(struct shared *m, struct list *listArr, int numLists);


#endif