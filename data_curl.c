#define TD_API_KEY INSERT_YOUR_KEY_HERE
#include "data_curl.h"

// get current unix time
unsigned long getEpoch()
{
    return (unsigned long)time(NULL);
}

// initialize string struct used for curl
void init_string(struct string *s)
{
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

// write function for string struct used for curl
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size*nmemb;
}

// get all data for a list of stocks for one point in time
int getList(struct list *l)
{
    CURL *curl;
    CURLcode curl_res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    struct string str;

    if(curl)
    {
        init_string(&str);
        char url[2048] = "https://api.tdameritrade.com/v1/marketdata/quotes?apikey=TD_API_KEY&symbol=";
        int i;
        int j;

        for(j=0; j<l->ss; ++j)
        {
            strcat(url,l->s[j].ticker);
            if(j != l->ss-1)
                strcat(url,"%2C");
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compactible; MSIE 5.01; Windows NT 5.0)");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);

        // Perform the request, curl_res will get the return code
        curl_res = curl_easy_perform(curl);

        // Check for errors
        if(curl_res != CURLE_OK)
            fprintf(stderr, "curl_eassy_perform() failed: %s\n", curl_easy_strerror(curl_res));
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    else
    {
        printf("cURL error\n");
        return 0;
    }

    char *pointer;
    char *end;
    pointer = str.ptr;
    int j;
    //printf("%s\n", str.ptr);
    int i=0;
    while(i < l->ss)
    {
        // Go to ticker
        if((pointer=strstr(pointer, "symbol"))==NULL)
            break;
        pointer+=8;
        if((pointer=strstr(pointer, l->s[i].ticker))==NULL)
            break;

        // Get open
        if((pointer=strstr(pointer, "openPrice"))==NULL)
            return 0;
        pointer+=11;
        sscanf(pointer, "%f", &l->s[i].open[l->pvs]);

        // Get high
        if((pointer=strstr(pointer, "highPrice"))==NULL)
            return 0;
        pointer+=11;
        sscanf(pointer, "%f", &l->s[i].high[l->pvs]);

        // Get low
        if((pointer=strstr(pointer, "lowPrice"))==NULL)
            return 0;
        pointer+=10;
        sscanf(pointer, "%f", &l->s[i].low[l->pvs]);

        // Get volume
        if((pointer=strstr(pointer, "totalVolume"))==NULL)
            return 0;
        pointer+=13;
        sscanf(pointer, "%ld", &l->s[i].volume[l->pvs]);

        // Get time
        if((pointer=strstr(pointer, "quoteTimeInLong"))==NULL)
            return 0;
        pointer+=17;
        sscanf(pointer, "%ld", &l->s[i].time[l->pvs]);

        // Get close
        if((pointer=strstr(pointer, "mark"))==NULL)
            break;
        pointer+=6;
        sscanf(pointer, "%f", &l->s[i].close[l->pvs]);

        // Increment for next stock
        ++i;
    }
    // Increment number of price points by 1
    l->pvs += 1;
    free(str.ptr);
    return 1;
}

