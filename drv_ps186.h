#ifndef __DRV_PS186_H__
#define __DRV_PS186_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "dev_ps186.h"

// #ifndef PIN_LOW
// #define PIN_LOW             0
// #endif

// #ifndef PIN_HIGH
// #define PIN_HIGH            1
// #endif

#ifndef DP_PIN_MODE_OUTPUT
#define DP_PIN_MODE_OUTPUT         0
#endif

#ifndef DP_PIN_MODE_INPUT
#define DP_PIN_MODE_INPUT          1
#endif

#define PS186_CMD_RST                       0x0000
#define PS186_CMD_GET_DP_LINK_STATUS        0x0001
#define PS186_CMD_GET_DP_VIDEO_INFO         0x0002
#define PS186_CMD_SET_DP_HPD                0x0003
#define PS186_CMD_SET_DP_DSC                0x0004
#define PS186_CMD_GET_DP_DSC                0x0005
#define PS186_CMD_SET_DP_HDCP               0x0006
#define PS186_CMD_GET_VERSION               0x0007
#define PS186_CMD_GET_REG                   0x0008

int8_t ps186_register(struct ps186_dev *ptPS186Dev, char *i_pName);
struct ps186_dev *ps186_find(char *i_pName);
int8_t ps186_open(struct ps186_dev *i_pdev);
int8_t ps186_close(struct ps186_dev *i_pdev);
int8_t ps186_control(struct ps186_dev *i_pdev, uint16_t cmd, void *arg);


#ifdef __cplusplus
}
#endif

#endif

