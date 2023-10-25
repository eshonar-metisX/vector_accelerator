#include <iostream>
#include <cstdint>
#include <type_traits>
#include <gtest/gtest.h>

#include "../inc/vector.hpp"

class VectorTest : public ::testing::Test 
{

public:

enum class TestDataType
{
    INT32,
    INT64,
    FLOAT32,
};

enum class TestCase
{
    ALL = 0,
    ADD,
    ADD_OVERFLOW,
    SUB,   
    SUB_UNDERFLOW,
    MUL,
    MUL_OVERFLOW,
    XOR,
};

protected:

    VectorTest()
    {
    }

    virtual ~VectorTest()
    {
    }
 
    virtual void SetUp()
    {

    };  
    virtual void TearDown()
    {
        
    }; 

    template <typename src, 
    typename = std::enable_if_t<std::is_same_v<src, int32_t> || std::is_same_v<src, float>>>
    bool CompArr(src*, src*);

    constexpr bool IsEqVector(const metisx::vector&, const metisx::vector&);
    
};

TEST_F(VectorTest, TestCase_ADD)
{
    EXPECT_TRUE(IsEqVector(3, metisx::vector(1) + metisx::vector(2)));
}

TEST_F(VectorTest, TestCase_ADD_OVERFLOW)
{
    EXPECT_TRUE(IsEqVector(static_cast<int64_t>(INT32_MAX) + 8, metisx::vector(INT32_MAX) + metisx::vector(8)));
}

TEST_F(VectorTest, TestCase_SUB)
{
    EXPECT_TRUE(IsEqVector(-1, metisx::vector(1) - metisx::vector(2)));
}

TEST_F(VectorTest, TestCase_SUB)
{
    EXPECT_TRUE(IsEqVector(-1, metisx::vector(1) - metisx::vector(2)));
}

TEST_F(VectorTest, TestCase_MUL)
{
    EXPECT_TRUE(IsEqVector(8, metisx::vector(4) * metisx::vector(2)));
}

TEST_F(VectorTest, TestCase_MUL_OVERFLOW)
{
    EXPECT_TRUE(IsEqVector(static_cast<int64_t>(INT32_MAX)  << 3, metisx::vector(INT32_MAX) * metisx::vector(8)));
}

TEST_F(VectorTest, TestCase_XOR)
{
    EXPECT_TRUE(IsEqVector(3 ^ 4, metisx::vector(3).xor(metisx::vector(4))));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
