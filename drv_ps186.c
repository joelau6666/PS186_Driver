#include "dev_ps186.h"
#include <stdio.h>
#include <string.h>

#define I2C_WRITE_FLAG   0x0000
#define I2C_READ_FLAG    0x0001


#define REG_DP_LINK_RATE_INDEX    		0x20
#define REG_DP_LANE_NUM_INDEX     		0x21
#define REG_DP_HPD_INDEX          		0xA1
#define REG_DP_VERSION_INDEX      		0x00
#define REG_DP_FW_SET_RUNNING_INDEX     0x02
#define REG_DP_DSC_CONTROL_INDEX        0xFE

#define REG_DP_FW_DISABLE               0x05
#define REG_DP_FW_ENABLE                0x02

#define REG_DP_DSC_DISABLE              0x00
#define REG_DP_DSC_ENABLE               0x01





#define DP_HPD_LOW(VALUE)            (VALUE & ~(1 << 7))
#define DP_HPD_HIGH(VALUE)           (VALUE | (1 << 7))

enum PS186_PAGE{
    PS186_PAGE0 = 0x08,
    PS186_PAGE1 = 0x09,
    PS186_PAGE2 = 0x0A,
    PS186_PAGE3 = 0x0B,
    PS186_PAGE4 = 0x0C,
    PS186_PAGE5 = 0x0D,
    PS186_PAGE6 = 0x0E,
    PS186_PAGE7 = 0x0F,
};






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


static int8_t ps186_get_dp_linkrate(struct ps186_dev *i_pdev, float *o_pLinkRate){
    if(!i_pdev || !i_pdev->info || !i_pdev->ops || !o_pLinkRate){
        return -1;
    }

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    const uint8_t u8LinkRateIndex = REG_DP_LINK_RATE_INDEX;
    uint8_t u8ReadBuf = 0;

    struct i2c_msg tmsg[2] = {0};
    tmsg[0].device_addr = PS186_PAGE1;
    tmsg[0].flags = I2C_WRITE_FLAG;
    tmsg[0].len = 1;
    tmsg[0].buf = (uint8_t *)&u8LinkRateIndex;

    tmsg[1].device_addr = PS186_PAGE1;
    tmsg[1].flags = I2C_READ_FLAG;
    tmsg[1].len = 1;
    tmsg[1].buf = &u8ReadBuf;

    ps186_i2c_xfer(i_pdev, tmsg, sizeof(tmsg)/sizeof(struct i2c_msg));

    *o_pLinkRate = u8ReadBuf * 0.27;

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif


    return 0;
}

static int8_t ps186_get_dp_linenum(struct ps186_dev *i_pdev, uint8_t *o_u8LaneNums){
    if(!i_pdev || !i_pdev->info || !i_pdev->ops || !o_u8LaneNums){
        return -1;
    }

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    const uint8_t u8LaneNumIndex = REG_DP_LANE_NUM_INDEX;
    uint8_t u8ReadBuf = 0;

    struct i2c_msg tmsg[2] = {0};
    tmsg[0].device_addr = PS186_PAGE1;
    tmsg[0].flags = I2C_WRITE_FLAG;
    tmsg[0].len = 1;
    tmsg[0].buf = (uint8_t *)&u8LaneNumIndex;

    tmsg[1].device_addr = PS186_PAGE1;
    tmsg[1].flags = I2C_READ_FLAG;
    tmsg[1].len = 1;
    tmsg[1].buf = &u8ReadBuf;

    ps186_i2c_xfer(i_pdev, tmsg, sizeof(tmsg)/sizeof(struct i2c_msg));
    *o_u8LaneNums = u8ReadBuf & 0x0F;

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif
    
    
    return 0;
}

static int8_t ps186_set_dphpd(struct ps186_dev *i_pdev, uint8_t *i_u8Hpd){
    if(!i_pdev || !i_pdev->info || !i_pdev->ops || !i_u8Hpd){
        return -1;
    }

#if IS_WITH_OS
     i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    uint8_t u8WriteBuf = REG_DP_HPD_INDEX;
    uint8_t u8ReadBuf = 0;

    struct i2c_msg treadmsg[2] = {0};
    treadmsg[0].device_addr = PS186_PAGE2;
    treadmsg[0].flags = I2C_WRITE_FLAG;
    treadmsg[0].len = 1;
    treadmsg[0].buf = (uint8_t *)&u8WriteBuf;

    treadmsg[1].device_addr = PS186_PAGE2;
    treadmsg[1].flags = I2C_READ_FLAG;
    treadmsg[1].len = 1;
    treadmsg[1].buf = &u8ReadBuf;

    ps186_i2c_xfer(i_pdev, treadmsg, sizeof(treadmsg)/sizeof(struct i2c_msg));

    uint8_t u8WriteHpdBuf[2] = {REG_DP_HPD_INDEX, 0};

    if(0 == (*i_u8Hpd)){
        u8WriteHpdBuf[1] = DP_HPD_LOW(u8ReadBuf);
    }else{
        u8WriteHpdBuf[1] = DP_HPD_HIGH(u8ReadBuf);
    }

    struct i2c_msg twritemsg = {0};
    twritemsg.device_addr = PS186_PAGE2;
    twritemsg.flags = I2C_WRITE_FLAG;
    twritemsg.len = 2;
    twritemsg.buf = u8WriteHpdBuf;

    ps186_i2c_xfer(i_pdev, &twritemsg, sizeof(twritemsg)/sizeof(struct i2c_msg));

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif

    return 0;
}

static int8_t ps186_get_version(struct ps186_dev *i_pdev, uint32_t *o_u32Version){
    if(!i_pdev || !i_pdev->info || !i_pdev->ops || !o_u32Version){
        return -1;
    }

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    struct i2c_msg tmsg[4] = {0};
    const uint8_t write_buffer_0[2] = {0x8e, 0x50};
    const uint8_t write_buffer_1[2] = {0x8f, 0x01};
    uint8_t writereg0 = REG_DP_VERSION_INDEX;
    uint8_t read_buffer[3] = {0};

    tmsg[0].device_addr = PS186_PAGE2;
    tmsg[0].flags = I2C_WRITE_FLAG;
    tmsg[0].len = 2;
    tmsg[0].buf = (uint8_t *)write_buffer_0;

    tmsg[1].device_addr = PS186_PAGE2;
    tmsg[1].flags = I2C_WRITE_FLAG;
    tmsg[1].len = 2;
    tmsg[1].buf = (uint8_t *)write_buffer_1;

    tmsg[2].device_addr = PS186_PAGE7;
    tmsg[2].flags = I2C_WRITE_FLAG;
    tmsg[2].len = 1;
    tmsg[2].buf = (uint8_t *)&writereg0;

    tmsg[3].device_addr = PS186_PAGE7;
    tmsg[3].flags = I2C_READ_FLAG;
    tmsg[3].len = 3;
    tmsg[3].buf = read_buffer;

    ps186_i2c_xfer(i_pdev, tmsg, sizeof(tmsg)/sizeof(struct i2c_msg));

    *o_u32Version = (read_buffer[0] << 24) | (read_buffer[1] << 16) | (read_buffer[2] << 8);

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif

    return 0;
}

static int8_t ps186_set_dpdsc(struct ps186_dev *i_pdev, uint8_t *i_u8IsEnable){
    if(!i_pdev || !i_pdev->ops || !i_u8IsEnable){
        return -1;
    }

#if IS_WITH_OS
    if(!i_pdev->mutex || !i_pdev->mutex_ops){
        return -1;
    }
    i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    int8_t ret = -1;
    struct i2c_msg tWriteMsg = {0};

    const uint8_t aStopFw[2] = {REG_DP_FW_SET_RUNNING_INDEX, REG_DP_FW_DISABLE};
    tWriteMsg.device_addr = PS186_PAGE5;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = (uint8_t *)aStopFw;

    ret = ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg));
    if(-1 == ret){
        goto ERR_RET;
    }

    if(i_pdev->ops->Delay_ms){
        i_pdev->ops->Delay_ms(200);
    }

    const uint8_t aWriteBuf[2] = {0xFD, 0x70};
    tWriteMsg.device_addr = PS186_PAGE1;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = (uint8_t *)aWriteBuf;
    
    ret = ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg));
    if(-1 == ret){
        goto ERR_RET;
    }

    if(i_pdev->ops->Delay_ms){
        i_pdev->ops->Delay_ms(200);
    }


    uint8_t aDSCControl[2] = {REG_DP_DSC_CONTROL_INDEX, REG_DP_DSC_DISABLE};
    if(0 == (*i_u8IsEnable)){
        aDSCControl[1] = REG_DP_DSC_DISABLE;
    }else{
        aDSCControl[1] = REG_DP_DSC_ENABLE;
    }

    tWriteMsg.device_addr = PS186_PAGE1;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = aDSCControl;

    ret = ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg));
    if(-1 == ret){
        goto ERR_RET;
    }

    if(i_pdev->ops->Delay_ms){
        i_pdev->ops->Delay_ms(200);
    }

    const uint8_t aRunFw[2] = {REG_DP_FW_SET_RUNNING_INDEX, REG_DP_FW_ENABLE};
    tWriteMsg.device_addr = PS186_PAGE5;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = (uint8_t *)aRunFw;

    ret = ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg));
    if(-1 == ret){
        goto ERR_RET;
    }

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif

    return 0;

ERR_RET:
#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif
    return -1; 
}

static int8_t ps186_get_dpdsc(struct ps186_dev *i_pdev, uint8_t *o_u8IsEnable){
    if(!i_pdev || !o_u8IsEnable){
        return -1;
    }

#if IS_WITH_OS
    if(!i_pdev->mutex || !i_pdev->mutex_ops){
        return -1;
    }
    if(i_pdev->mutex_ops->mutex_lock){
        i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
    }
#endif
    
    struct i2c_msg tWriteMsg = {0}; 
    const uint8_t aStopFw[2] = {REG_DP_FW_SET_RUNNING_INDEX, REG_DP_FW_DISABLE};
    tWriteMsg.device_addr = PS186_PAGE5;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = (uint8_t *)aStopFw;

    if(-1 == ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg))){
        goto ERR_RET;
    }

    if(i_pdev->ops->Delay_ms){
        i_pdev->ops->Delay_ms(200);
    }

    const uint8_t aWriteBuf[2] = {0xFD, 0x70};
    tWriteMsg.device_addr = PS186_PAGE1;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = (uint8_t *)aWriteBuf;

    if(-1 == ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg))){
        goto ERR_RET;
    }

    if(i_pdev->ops->Delay_ms){
        i_pdev->ops->Delay_ms(200);
    }

    struct i2c_msg treadmsg[2] = {0};

    const uint8_t u8DscIndex = REG_DP_DSC_CONTROL_INDEX;
    treadmsg[0].device_addr = PS186_PAGE1;
    treadmsg[0].flags = I2C_WRITE_FLAG;
    treadmsg[0].len = 1;
    treadmsg[0].buf = (uint8_t *)&u8DscIndex;

    treadmsg[1].device_addr = PS186_PAGE1;
    treadmsg[1].flags = I2C_READ_FLAG;
    treadmsg[1].len = 1;
    treadmsg[1].buf = o_u8IsEnable;

    if(-1 == ps186_i2c_xfer(i_pdev, treadmsg, sizeof(treadmsg)/sizeof(struct i2c_msg))){
        goto ERR_RET;
    }

    const uint8_t aRunFw[2] = {REG_DP_FW_SET_RUNNING_INDEX, REG_DP_FW_ENABLE};
    tWriteMsg.device_addr = PS186_PAGE5;
    tWriteMsg.flags = I2C_WRITE_FLAG;
    tWriteMsg.len = 2;
    tWriteMsg.buf = (uint8_t *)aRunFw;

    if(-1 == ps186_i2c_xfer(i_pdev, &tWriteMsg, sizeof(tWriteMsg)/sizeof(struct i2c_msg))){
        goto ERR_RET;
    }

#if IS_WITH_OS
    if(i_pdev->mutex_ops->mutex_unlock){
        i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
    }
#endif

    return 0;

ERR_RET:
#if IS_WITH_OS
    if(i_pdev->mutex_ops->mutex_unlock){
        i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
    }
#endif
    return -1;
}
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

#if IS_WITH_OS
    i_pDev->mutex = i_pDev->mutex_ops->mutex_create();
    if(!i_pDev->mutex){
        return -1;
    }
#endif

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

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    if(i_pdev->isopen){
#if IS_WITH_OS
        i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif
        return -1;
    }

    if(i_pdev->ops->PinWrite){
        i_pdev->ops->PinWrite(&i_pdev->info->pinrst, i_pdev->info->pinrst.runvalue);
        i_pdev->ops->PinWrite(&i_pdev->info->pinlanemode, i_pdev->info->pinlanemode.runvalue);
    }

    i_pdev->isopen = 1;

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif

    return 0;
}


int8_t ps186_close(struct ps186_dev *i_pdev){
    if(!i_pdev || !i_pdev->ops || !i_pdev->info){
        return -1;
    }

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_lock(i_pdev->mutex);
#endif

    if(!i_pdev->isopen){
#if IS_WITH_OS
        i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif
        return -1;
    }

    if(i_pdev->ops->PinWrite){
        i_pdev->ops->PinWrite(&i_pdev->info->pinrst, i_pdev->info->pinrst.initvalue);
        i_pdev->ops->PinWrite(&i_pdev->info->pinlanemode, i_pdev->info->pinlanemode.initvalue);
    }

    i_pdev->isopen = 0;

#if IS_WITH_OS
    i_pdev->mutex_ops->mutex_unlock(i_pdev->mutex);
#endif

    return 0;
}


int8_t ps186_control(struct ps186_dev *i_pdev, uint16_t cmd, void *arg){
    if(!i_pdev || !i_pdev->ops){
        return -1;
    }


    switch(cmd){
        case PS186_CMD_GET_DP_LINK_STATUS:{
            ps186_get_dp_linkrate(i_pdev, arg);
            break;
        }

        case PS186_CMD_GET_DP_LINE_NUMS:{
            ps186_get_dp_linenum(i_pdev, arg);
            break;
        }

        case PS186_CMD_SET_DP_HPD:{
            ps186_set_dphpd(i_pdev, arg);
            break;
        }

        case PS186_CMD_SET_DP_DSC:{
        	ps186_set_dpdsc(i_pdev, arg);
            break;
        }

        case PS186_CMD_GET_DP_DSC:{
            ps186_get_dpdsc(i_pdev, arg);
            break;
        }
        case PS186_CMD_GET_VERSION:{
            ps186_get_version(i_pdev, arg);
            break;
        }
		

        default:{
            break;
        }
    }

    return 0;
}


