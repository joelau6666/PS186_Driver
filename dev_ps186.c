#include "dev_ps186.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct ps186_dev g_tPS186DevHead = {
    .name = "ps186_head",
    .next = NULL,
};


struct ps186_dev *dev_ps186_get(char *i_name){
    if(!i_name){
        return NULL;
    }

    struct ps186_dev *ptPS186Dev = g_tPS186DevHead.next;
    
    while(ptPS186Dev){
        if(0 == strncmp(ptPS186Dev->name, i_name, PS186_NAME_MAX)){
            return ptPS186Dev;
        }
        ptPS186Dev = ptPS186Dev->next;
    }

    return NULL;
}


int8_t dev_ps186_add(struct ps186_dev *ptPS186Dev){
    if(!ptPS186Dev){
        return -1;
    }

    if(dev_ps186_get(ptPS186Dev->name)){
        return -1;
    }

    ptPS186Dev->next = g_tPS186DevHead.next;
    g_tPS186DevHead.next = ptPS186Dev;

    return 0;
}





