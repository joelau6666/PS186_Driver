#ifndef __DEV_PS186_H__
#define __DEV_PS186_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "ps186_config.h"

struct gpio_param{
    uint32_t gpio_group;
    uint32_t gpio_pin;
    uint8_t initvalue;
    uint8_t runvalue;
};

struct i2c_param{
    uint32_t i2c_periph;
};


struct ps186_dev_info{
    struct gpio_param pinrst;
    struct gpio_param pinlanemode;
    struct gpio_param pindpalt;
    struct i2c_param i2c;
};

struct ps186_dev_ops{
    int8_t (*PinMode)(struct gpio_param *i_ptGpio, uint8_t u8Mode);
    int8_t (*PinWrite)(struct gpio_param *i_ptGpio, uint8_t u8WriteValue);
    int8_t (*PinRead)(struct gpio_param *i_ptGpio);
    int8_t (*I2CWrite)(uint32_t i2c_periph, uint16_t salve_addr, uint8_t *pbuffer, uint16_t len);
    int8_t (*I2CRead)(uint32_t i2c_periph, uint16_t salve_addr, uint8_t *pbuffer, uint16_t len);
    void (*Delay_ms)(uint32_t ms);
};

#if IS_WITH_OS
struct ps186_mutex_ops{
    void *(*mutex_create)(void);
    int8_t (*mutex_lock)(void *mutex);
    int8_t (*mutex_unlock)(void *mutex);
};
#endif

struct ps186_dev{
    char name[PS186_NAME_MAX];
    struct ps186_dev_info *info;
    struct ps186_dev_ops *ops;
#if IS_WITH_OS
    void *mutex;
    struct ps186_mutex_ops *mutex_ops;
#endif
    struct ps186_dev *next;
    uint8_t isopen;
};


struct ps186_dev *dev_ps186_get(char *i_name);
int8_t dev_ps186_add(struct ps186_dev *ptPS186Dev);


#ifdef __cplusplus
}
#endif

#endif
