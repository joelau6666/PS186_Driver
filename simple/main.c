#include "drv_ps186.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv){
    struct ps186_dev *p_tPS186Dev = calloc(1, sizeof(struct ps186_dev));
    if(!p_tPS186Dev){
        printf("%s calloc failed\n", __func__);
        return -1;
    }
    
    if(-1 == ps186_register(p_tPS186Dev, "ps186_dev")){
        printf("%s ps186_register failed\n", __func__);
        return -1;
    }

    printf("ps186_register success\n");

    while(1){

    }

    return 0;
}