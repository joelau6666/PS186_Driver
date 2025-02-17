#ifndef __DEV_PS186_H__
#define __DEV_PS186_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "ps186.h"

struct ps186_dev *dev_ps186_get(char *i_name);
int8_t dev_ps186_add(struct ps186_dev *ptPS186Dev);


#ifdef __cplusplus
}
#endif

#endif
