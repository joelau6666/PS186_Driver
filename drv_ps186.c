#include "dev_ps186.h"
#include <stdio.h>
#include <string.h>

#define I2C_WRITE_FLAG   0x0000
#define I2C_READ_FLAG    0x0001


struct i2c_msg
{
    uint16_t device_addr;   
    uint16_t flags;
    uint16_t len;            
    uint8_t  *buf;
};

static int8_t ps186_i2c_xfer(struct ps186_dev *i_pdev, struct i2c_msg msgs[], uint32_t num){
    struct i2c_param *i2c = NULL;
    struct i2c_msg *msg = NULL;
    uint32_t i = 0;
    int8_t ret = -1;

    if(!i_pdev || !i_pdev->ops || !i_pdev->info){
        return -1;
    }

    i2c = &i_pdev->info->i2c;

    for (i = 0; i < num; i++){
        msg = &msgs[i];

        // if (msg->flags & RT_I2C_ADDR_10BIT)
        // {
        //     i2c_mode_addr_config(gd32_i2c->i2c_periph,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_10BITS,0);
        // }
        // else
        // {
        //     i2c_mode_addr_config(gd32_i2c->i2c_periph,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,0);
        // }

        if (msg->flags & I2C_READ_FLAG){
            if(i_pdev->ops->I2CRead){
                if(0 != i_pdev->ops->I2CRead(i2c->i2c_periph, msg->device_addr, msg->buf, msg->len)){
                    goto out;
                }
            }
        }else{
            if(i_pdev->ops->I2CWrite){
                if(0 != i_pdev->ops->I2CWrite(i2c->i2c_periph, msg->device_addr, msg->buf, msg->len)){
                    goto out;
                }
            }
        }
    }

    ret = i;

out:
//    LOG_E("send stop condition\n");

    return ret;
}




static int8_t ps186_init(struct ps186_dev *i_pdev){
    if(!i_pdev || !i_pdev->info || !i_pdev->ops){
        return -1;
    }

    /* PinMode Output */
    if(i_pdev->ops->PinModeOut){
        i_pdev->ops->PinModeOut(&i_pdev->info->pinrst);
        i_pdev->ops->PinModeOut(&i_pdev->info->pinlanemode);
    }

    /* Init PinValue */
    if(i_pdev->ops->PinWrite){
        i_pdev->ops->PinWrite(&i_pdev->info->pinrst, i_pdev->info->pinrst.initvalue);
        i_pdev->ops->PinWrite(&i_pdev->info->pinlanemode, i_pdev->info->pinlanemode.initvalue);
    }

    return 0;
}

int8_t ps186_register(struct ps186_dev *i_pDev, char *i_pName){
    if(!i_pDev || !i_pName){
        return -1;
    }

    strncpy(i_pDev->name, i_pName, PS186_NAME_MAX);

    if(-1 == ps186_init(i_pDev)){
        return -1;
    }

    if(-1 == dev_ps186_add(i_pDev)){
        return -1;
    }

    return 0;
}


struct ps186_dev *ps186_find(char *i_pName){
    if(!i_pName){
        return NULL;
    }

    return dev_ps186_get(i_pName);
}

int8_t ps186_open(struct ps186_dev *i_pdev){
    if(!i_pdev || !i_pdev->ops || !i_pdev->info){
        return -1;
    }

    if(i_pdev->isopen){
        return -1;
    }

    if(i_pdev->ops->PinWrite){
        i_pdev->ops->PinWrite(&i_pdev->info->pinrst, i_pdev->info->pinrst.runvalue);
        i_pdev->ops->PinWrite(&i_pdev->info->pinlanemode, i_pdev->info->pinlanemode.runvalue);
    }

    i_pdev->isopen = 1;

    return 0;
}


int8_t ps186_close(struct ps186_dev *i_pdev){
    if(!i_pdev || !i_pdev->ops || !i_pdev->info){
        return -1;
    }

    if(!i_pdev->isopen){
        return -1;
    }

    if(i_pdev->ops->PinWrite){
        i_pdev->ops->PinWrite(&i_pdev->info->pinrst, i_pdev->info->pinrst.initvalue);
        i_pdev->ops->PinWrite(&i_pdev->info->pinlanemode, i_pdev->info->pinlanemode.initvalue);
    }

    i_pdev->isopen = 0;

    return 0;
}


int8_t ps186_control(struct ps186_dev *i_pdev, uint16_t cmd, void *arg){
    if(!i_pdev || !i_pdev->ops){
        return -1;
    }


    switch(cmd){
        case PS186_CMD_GET_DP_LINK_STATUS:{
            
        }

        default:{
            break;
        }
    }

    return 0;
}


