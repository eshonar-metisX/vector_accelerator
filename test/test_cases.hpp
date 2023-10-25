#include <gtest/gtest.h>
#include "VectorAcceleratorTester.hpp"
#include "../inc/vector_accelerator.hpp"

static const uint64_t testSize = 100;

TEST_F(VectorAcceleratorTester, TestCase_ADD_INT32)
{    
    ConfigureTest(TestCase::ADD, TestDataType::INT32, testSize);
    SetUpArr(100, 200);
    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(intLhs, intRhs, intRes, testSize), 300, testSize));
}

TEST_F(VectorAcceleratorTester, TestCase_ADD_INT32_OVERFLOW)
{
    ConfigureTest(TestCase::ADD, TestDataType::INT32, testSize);
    SetUpArr(100, 200);
    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(intLhs, intRhs, intRes, testSize), 300, testSize));
}



// TEST_F(VectorAcceleratorTester, TestCase_ADD_INT64)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, testSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_INT64_OVERFLOW)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, testSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT32)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, testSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT32_OVERFLOW)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, testSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT64)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, testSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT64_OVERFLOW)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vector_accelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, testSize), 3));
// }




//
//
//
//
//TEST_F(VectorAcceleratorTester, TestCase_SUB)
//{
//   EXPECT_TRUE(isEqArr(-1, metisx::vector_accelerator(1) - metisx::vector(2)));
//}
//
//TEST_F(VectorAcceleratorTester, TestCase_SUB)
//{
//   EXPECT_TRUE(isEqArr(-1, metisx::vector(1) - metisx::vector(2)));
//}
//
//TEST_F(VectorAcceleratorTester, TestCase_MUL)
//{
//   EXPECT_TRUE(isEqArr(8, metisx::vector(4) * metisx::vector(2)));
//}
//
//TEST_F(VectorAcceleratorTester, TestCase_MUL_OVERFLOW)
//{
//   EXPECT_TRUE(isEqArr(static_cast<int64_t>(INT32_MAX)  << 3, metisx::vector(INT32_MAX) * metisx::vector(8)));
//}
//
//TEST_F(VectorAcceleratorTester, TestCase_XOR)
//{
//   EXPECT_TRUE(isEqArr(3 ^ 4, metisx::vector(3).xor(metisx::vector(4))));
//}
