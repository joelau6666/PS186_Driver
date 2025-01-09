#ifndef __DRV_PS186_H__
#define __DRV_PS186_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "ps186.h"

int8_t ps186_register(struct ps186_dev *ptPS186Dev, char *i_pName);
struct ps186_dev *ps186_find(char *i_pName);
int8_t ps186_open(struct ps186_dev *i_pdev);
int8_t ps186_close(struct ps186_dev *i_pdev);
int8_t ps186_control(struct ps186_dev *i_pdev, uint16_t cmd, void *arg);


#ifdef __cplusplus
}
#endif

#endif

