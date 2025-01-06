#ifndef __DRV_PS186_H__
#define __DRV_PS186_H__

#include "ps186.h"

int8_t ps186_register(struct ps186_dev *ptPS186Dev, char *i_pName);
struct ps186_dev *ps186_find(char *i_pName);


#endif

