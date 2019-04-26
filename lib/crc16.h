#pragma once


#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
用法说明：

直接计算(数据量较少的情况):
uint16_t crc = crc16(0, data, len);

分步计算(数据量较多的情况):
uint16_t crc = crc16(0, data1, len1);
crc = crc16(crc, data2, len2);
...
crc = crc16(crc, dataN, lenN);

校验(按常量)(假设数据体后2字节就是crc)：
uint16_t crc = crc16(0, data, len + 2);
if(crc == CRC16_GOOD){
    //OK
}else{
    //ERROR
}

*/


//校验时计算所得最终常数
#define CRC16_GOOD 0x0F47

//计算crc16
uint16_t crc16(uint16_t crc, const uint8_t* data, size_t len);

#ifdef __cplusplus
}
#endif


