#ifndef __GUARD_DATA_HANDLING__
#define __GUARD_DATA_HANDLING__

#define STOCK_SIZE 150
#define PRICE_SIZE 5000
#define THREAD_TIME_OFFSET 700

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"

// Function Declarations
struct stock newStock(const char *t);
struct list newList(const char *t, int threadIndex);
void freeList(struct list *l);
int addList(struct list *l, struct stock s);
int remList(struct list *l, int index);
int moveList(struct list *to, struct list *from, int index);
int insertList(struct list *l, struct stock s, int index);
int insertByTicker(struct list *l, struct stock s);
int sortByTicker(struct list *l);
int insertByVolume(struct list *l, struct stock s);
int sortByVolume(struct list *l);
int insertByPrice(struct list *l, struct stock s);
int sortByPrice(struct list *l);

#endif
