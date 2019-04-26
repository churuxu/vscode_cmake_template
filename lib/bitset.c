#include "bitset.h"





int bitset_get(const void* data, size_t datalen, size_t index){
    uint8_t* pdata;
    size_t bytei = index / 8;
    size_t biti = index % 8;
    if(bytei >= datalen)return 0;
    pdata = (uint8_t*)data + bytei;
    return (((*pdata) >> biti) & 1);
}


int bitset_set(void* data, size_t datalen, size_t index, int val){
    uint8_t* pdata;
    size_t bytei = index / 8;
    size_t biti = index % 8;
    if(bytei >= datalen)return -1;    
    pdata = (uint8_t*)data + bytei;
    if(val){
        (*pdata) |= (uint8_t)(1 << biti);
    }else{
        (*pdata) &= ~((uint8_t)(1 << biti));
    }   
    return 0;
}






