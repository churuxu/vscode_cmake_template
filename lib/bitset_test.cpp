#ifdef USE_GTEST

#include "bitset.h"

#include "gtest/gtest.h"


TEST(bitset,get){
    uint8_t b10101111 = 0b10101111;
    
    EXPECT_EQ(1, bitset_get(&b10101111, 1, 0));
    EXPECT_EQ(1, bitset_get(&b10101111, 1, 1));
    EXPECT_EQ(0, bitset_get(&b10101111, 1, 6));
    EXPECT_EQ(1, bitset_get(&b10101111, 1, 7));
}

#endif


