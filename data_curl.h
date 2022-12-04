#ifndef __GUARD_DATA_CURL__
#define __GUARD_DATA_CURL__

#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
//#include </usr/local/include/curl/curl.h> //openbsd
#include <string.h>
#include <time.h>

#include "structs.h"
#include "data_handling.h"

// Function Declarations
unsigned long getEpoch();
void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
int getList(struct list *l);
#endif

