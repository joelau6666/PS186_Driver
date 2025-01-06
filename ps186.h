#ifndef __PS186_H__
#define __PS186_H__

#include <stdint.h>

#define PS186_NAME_MAX      16



#ifndef PIN_LOW
#define PIN_LOW             0
#endif

#ifndef PIN_HIGH
#define PIN_HIGH            1
#endif


#define PS186_CMD_GET_DP_LINK_STATUS    0x0000

struct gpio_param{
    uint32_t gpio_group;
    uint32_t gpio_pin;
    uint32_t mode;
    uint8_t initvalue;
    uint8_t runvalue;
};

struct i2c_param{
    uint32_t i2c_periph;
    uint16_t i2c_addr;
};


struct ps186_dev_info{
    struct gpio_param pinrst;
    struct gpio_param pinlanemode;
    struct i2c_param i2c;
};

struct ps186_dev_ops{
    int8_t (*PinMode)(struct gpio_param *gpio);
    int8_t (*PinWrite)(struct gpio_param *gpio, uint8_t value);
    int8_t (*I2CWrite)(uint32_t i2c_periph, uint16_t salve_addr, uint8_t *pbuffer, uint16_t len);
    int8_t (*I2CRead)(uint32_t i2c_periph, uint16_t salve_addr, uint8_t *pbuffer, uint16_t len);
};

struct ps186_dev{
    char name[PS186_NAME_MAX];
    struct ps186_dev_info *info;
    struct ps186_dev_ops *ops;
    struct ps186_dev *next;
    uint8_t isopen;
};






#endif
