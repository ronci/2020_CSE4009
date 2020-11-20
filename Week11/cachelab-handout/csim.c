//2019015414

#include "cachelab.h"
#include <math.h>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int flag;
int setbit, blockbit, line;
int cntSet, blockSize, tagSize;
char *input, *address, *inputByte;
int *head, *tail; //head old, tail recent
int hits, misses, evictions;

struct typedef cache_t {
    bool v;
    char *tag;
    char *block;
    int next;
    int prev;
} **cache;


//return 2 : hits, return 1 : just miss, return 0 : miss eviction
int findCache(cache_t **cache, char *address, int *sindex, int *lindex) {
    int addressDex = (int)strtol(address, NULL, 16);
    int addressTag =  addressDex / (pow(2, setbit + blockbit));
    int setIndex = addressDex % (pow(2, setbit + blockbit)) / pow(2, blockbit);
    int now = head[setIndex];
    while(1) {
        if(cache[setIndex][now]->v == 0) {
            *sindex = setIndex;
            *lindex = now;
            return 1;
        }
        if(atoi(cache[setIndex][now]->tag) == addressDex) {
            *sindex = setIndex;
            *lindex = now;
            return 2;
        }
        if(now == tail[setIndex]) {
            break;
        }
        now = cache[setIndex][now]->next;
    }
    return 0;
}

void storeCache(cache_t **cache, char *address, int sindex, int lindex, chat *inputbyte, int result) {
    int addressDex = (int)strtol(address, NULL, 16);
    int addressTag =  addressDex / (pow(2, setbit + blockbit));
    int addressBlock = addressDex / pow(2, blockbit);
    char *addressBlockHex = (char)malloc(sizeof(char*) * blockbit);
    int storeByte = atoi(inputbyte) * 4;
    sprintf(addressBlockHex, "%x", addressBlock);
    if(result != 0) {
        cache[sindex][lindex]->v = 1;
        strncpy(cache[sindex][lindex]->block + addressBlock, addressBlockHex, storeByte);
    }
    else {
        int now = head[sindex];
        head[sindex] = cache[sindex][now]->next;
        int recent = tail[sindex];
        tail[sindex] = now;
        cache[sindex][recent]->next = now;
        cache[sindex][now]->prev = recent;
        char addressTagHex[tagSize];
        sprintf(addressTagHex, "%x", addressTag);
        strncpy(cache[sindex][now]->tag, addressTagHex, tagSize);
        memset(cache[sindex][lindex]->block, 0, blockbit);
        strncpy(cache[sindex][lindex]->block + addressBlock, addressBlockHex, storeByte);
    }
    free(addressBlockHex);
}

int main(int argc, char )
{
    FILE *fin;
    int option;
    char cmd;
    int i, j;
    while((option = getopt(argc, argv, "vs:E:b:t:"))) {
        switch (option) {
        case 'v':
            flag = 1;
            break;
        case 's':
            setbit = atoi(optarg);
            break;
        case 'E':
            line = atoi(optarg);
            break;
        case 'b':
            blockbit = atoi(optarg);
            break;
        case 't':
            fin = fopen(optarg, "r");
            break;
        default:
            break;
        }
    }

    cntSet = pow(2, setbit);
    blockSize = pow(2, blockbit);
    tagSize = blockSize * 4 - setbit - blockbit;
    cache = (cache_t**)malloc(sizeof(cache_t*)*cntSet);
    head = (int*)malloc(sizeof(int)*cntSet);
    tail = (int*)malloc(sizeof(int)*cntSet);
    for(i = 0; i < cntSet; i++) {
        cache[i] = (cache_t*)malloc(sizeof(cache_t)*line);
        head[i] = 0;
        tail[i] = line - 1;
        for(j = 0; j < line; j++) {
            cache[i][j]->v = 0;
            cache[i][j]->tag = (char*)malloc(sizeof(char)*tagSize);
            cache[i][j]->block = (char*)malloc(sizeof(char)*blockbit);
            cache[i][j]->next = j + 1;
            cache[i][j]->prev = j - 1;
        }
    }    

    int sindex, lindex, result;
    char *ptr;
    input = (char*)malloc(sizeof(char)*(blockSize * 4));
    address = (char*)malloc(sizeof(char)*(blockSize * 4));
    inputByte = (char*)malloc(sizeof(char)*(blockSize * 4));
    while(fscanf(fin, "%c", &cmd) != 1) {
        if(cmd == "I") {
            fscanf(fin, " %s\n", input);
        }
        else {
            fscanf(fin, "%c", &cmd);
            fscanf(fin, " %s\n", input);
            switch (cmd)
            {
            case 'L':
                if(flag == 1) printf("%c %s ", cmd, input);
                ptr = strtok(intput, ",");
                strcpy(address, ptr);
                ptr = strtok(NULL, ",");
                strcpy(inputByte, ptr);
                
                result = findCache(cache, address, &sindex, &lindex);
                if(result == 0) {
                    if(flag == 1) printf("miss eviction\n");
                    evictions++;
                    misses++;
                }
                else if(result == 1) {
                    if(flag == 1) printf("miss\n");
                    misses++;
                }
                else {
                    if(flag == 1) printf("hit\n");
                    hits++;
                }
                break;
            
            case 'S':
                if(flag == 1) printf("%c %s ", cmd, input); 
                ptr = strtok(intput, ",");
                strcpy(address, ptr);
                ptr = strtok(NULL, ",");
                strcpy(inputByte, ptr);
                
                result = findCache(cache, address, &sindex, &lindex);
                storeCache(cache, address, sindex, lindex, inputByte, result);
                if(flag == 1) printf("hit\n");
                hits++;
                break;
            
            case 'M':
                if(flag == 1) printf("%c %s ", cmd, input);
                ptr = strtok(intput, ",");
                strcpy(address, ptr);
                ptr = strtok(NULL, ",");
                strcpy(inputByte, ptr);
                
                result = findCache(cache, address, &sindex, &lindex);
                if(result == 0) {
                    if(flag == 1) printf("miss eviction ");
                    evictions++;
                    misses++;
                }
                else if(result == 1) {
                    if(flag == 1) printf("miss ");
                    misses++;
                }
                else {
                    if(flag == 1) printf("hit ");
                    hits++;
                }

                storeCache(cache, address, sindex, lindex, inputByte, result);
                if(flag == 1) printf("hit\n");
                hits++;

                break;
            
            default:
                break;
            }
        }
    }
    
    printSummary(hits, misses, evictions);

    free(inputByte);
    free(address);
    free(input);
    free(head);
    free(tail);
    for(i = 0; i < cntSet; i++) {
        for(j = 0; j < line; j++) {
            free(cache[i][j]->tag);
            free(cache[i][j]->block);
        }
        free(cache[i]);
    }    
    free(cache);
    fclose(fin);
    return 0;
}
