#ifdef USE_GTEST

#include "crc16.h"

#include "gtest/gtest.h"


TEST(crc16,normal){
    //单步计算crc
    uint16_t crc = crc16(0, (uint8_t*)"123456", 6);
    ASSERT_EQ(0xE672, crc);
}

TEST(crc16,multi){
    //分步计算crc
    uint16_t crc = crc16(0, (uint8_t*)"12", 2);
    crc = crc16(crc, (uint8_t*)"345", 3);
    crc = crc16(crc, (uint8_t*)"6", 1);
    ASSERT_EQ(0xE672, crc);
}

TEST(crc16,check){
    //校验crc

    //生成crc并追加到数据结尾 （数据6字节，crc2字节）
    char buf[8] = "123456";
    uint16_t crc = crc16(0, (uint8_t*)buf, 6);
    *(uint16_t*)(&buf[6]) = crc;

    //校验整个数据 （8字节）
    uint16_t check = crc16(0, (uint8_t*)buf, 8);
    ASSERT_EQ(CRC16_GOOD, check);
}


TEST(crc16,check2){
    //校验crc
    uint8_t buf[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x59,0x48,0xA8,0x84}; //1 2 3 4 5 6 Y H CRC16
    uint16_t check = crc16(0, (uint8_t*)buf, 10);
    ASSERT_EQ(CRC16_GOOD, check);
}

TEST(crc16,check3){
    //校验crc （数据已改变）
    uint8_t buf[] = {0x31,0x32,0x33,0x33,0x35,0x36,0x59,0x48,0xA8,0x84}; //1 2 3 4 5 6 Y H CRC16
    uint16_t check = crc16(0, (uint8_t*)buf, 10);
    ASSERT_NE(CRC16_GOOD, check);
}



#endif

