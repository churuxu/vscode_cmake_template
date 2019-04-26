#pragma once


#include <stdlib.h>
#include <stdint.h>

/**
bitset
*/

#ifdef __cplusplus
extern "C" {
#endif

/** bitset 获取index的bit
 * 返回1或0
*/
int bitset_get(const void* data, size_t datalen, size_t index);

/** bitset 设置index的bit, val=1或0
 * 返回0， 如果超出限制返回-1
*/
int bitset_set(void* data, size_t datalen, size_t index, int val);



#ifdef __cplusplus
}
#endif

