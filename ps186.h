#ifndef __PS186_H__
#define __PS186_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

#define PS186_NAME_MAX      16

#define IS_WITH_OS          1


#ifndef PIN_LOW
#define PIN_LOW             0
#endif

#ifndef PIN_HIGH
#define PIN_HIGH            1
#endif


#define PS186_CMD_GET_DP_LINK_STATUS        0x0000
#define PS186_CMD_GET_DP_LINE_NUMS          0x0001
#define PS186_CMD_SET_DP_HPD                0x0002
#define PS186_CMD_SET_DP_DSC                0x0003
#define PS186_CMD_GET_DP_DSC                0x0004
#define PS186_CMD_GET_VERSION               0x0005

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
    struct i2c_param i2c;
};

struct ps186_dev_ops{
    int8_t (*PinModeOut)(struct gpio_param *gpio);
    int8_t (*PinWrite)(struct gpio_param *gpio, uint8_t value);
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


#ifdef __cplusplus
}
#endif



#endif
