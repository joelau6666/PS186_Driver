#ifndef __PS186_INFO_H__
#define __PS186_INFO_H__


#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

struct ps186_link_status{
    uint8_t dsc;
    uint8_t lanecount;
    float linkrate;
};

struct ps186_video_info{
    uint16_t hactive;
    uint16_t vactive;
    uint8_t colorspace;
    uint8_t deepcolor;
};


#ifdef __cplusplus
}
#endif


#endif
