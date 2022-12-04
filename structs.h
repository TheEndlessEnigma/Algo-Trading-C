#ifndef __GUARD_STRUCTS__
#define __GUARD_STRUCTS__

#include <stddef.h>
#include <stdlib.h>


// string struct used for curl
struct string
{
    char *ptr;
    size_t len;
};

// struct for individual stock
struct stock
{
    char ticker[8];
    float *close; // current size is pvs in list
    float *open;
    float *high;
    float *low;
    float volume_rate;
    unsigned long *volume; // current size is pvs in list
    unsigned long *time;
    int owned;
    float buyPrice;
    int buyIndex;
};

// struct for a list of stocks
struct list
{
    struct stock *s;
    char name[16];
    int threadIndex;
    int ss; // number of stocks in list
    int pvs; // number of prices and volumes each stock has
    int time_offset_ms;
};

// struct for holding balance and position info
struct shared
{
    // money stuff
    float starting_balance;
    float current_cash;
    float current_stocks;
    float current_value;
    float rate_sd;
    float rate_avg;
    int rate_count;
    int num_positions;
    int error;
    int num_threads;
};

// package struct passed to thread
struct package
{
    struct list *l;
    struct shared *m;
    //pthread_mutex_t *mutex_lock;
    int is_running;
};


#endif