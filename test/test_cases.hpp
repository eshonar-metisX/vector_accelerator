#include <gtest/gtest.h>
#include "VectorAcceleratorTester.hpp"
#include "../inc/vectorAccelerator.hpp"
#include "vectorAcceleratorPass.hpp"

template <class T>
class testParamPack : public testing::Test {};

static const uint64_t gTestSize = 100;

using testScenarios = typename testing::Types<
std::tuple<int32_t, std::integer_sequence<int64_t, gTestSize>>, 
std::tuple<int64_t, std::integer_sequence<int64_t, gTestSize>>,
std::tuple<float, std::integer_sequence<int64_t, gTestSize>>>;


TYPED_TEST_CASE(testParamPack, testScenarios);


//TYPED_TEST(testParamPack, TEST_TEST)
//{
//
//    using A = typename std::tuple_element<0, decltype(TypeParam())>::type;
//    using B = typename std::tuple_element<1, decltype(TypeParam())>::type;
//    //using C = std::tuple_element<2, decltype(TypeParam())>::type;
//
//    metisx::dataChunk<A, B> lhs;
//    EXPECT_TRUE(lhs.IsAllocated());
//
//}

//TEST_F(metisx::dataChunk, TestCase_FILL_N)
//{
//
//
//
//
//
//
//
//}

TEST_F(VectorAcceleratorTester, TestCase_ADD_INT32)
{    
    ConfigureTest(TestFunc::ADD, TestDataType::INT32, gTestSize);
    SetUpArr(100, 200);
    //metisx::vectorAccelerator::Add(intLhs, intRhs, intRes, gTestSize);
    metisx::dataChunk<float, gTestSize> lhs;
    pass::vectorAccelerator::Add(intLhs, intRhs, intRes, gTestSize);
    EXPECT_TRUE(ValidateRes(300));
}


//TEST_F(VectorAcceleratorTester, TestCase_ADD_INT32_OVERFLOW)
//{
//    ConfigureTest(TestFunc::ADD, TestDataType::INT32, gTestSize);
//    SetUpArr(INT32_MAX, 200);
//    //metisx::vectorAccelerator::Add(intLhs, intRhs, intRes, gTestSize);
//    pass::vectorAccelerator::Add(intLhs, intRhs, intRes, gTestSize);
//    EXPECT_TRUE(ValidateRes(300));
//}

// TEST_F(VectorAcceleratorTester, TestCase_ADD_INT64)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vectorAccelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, gTestSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_INT64_OVERFLOW)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vectorAccelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, gTestSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT32)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vectorAccelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, gTestSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT32_OVERFLOW)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vectorAccelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, gTestSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT64)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vectorAccelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, gTestSize), 3));
// }

// TEST_F(VectorAcceleratorTester, TestCase_ADD_UINT64_OVERFLOW)
// {
//    EXPECT_TRUE(IsEqArr(metisx::vectorAccelerator::add(int_arr_lhs, int_arr_rhs, int_arr_res, gTestSize), 3));
// }




//
//
//
//
//TEST_F(VectorAcceleratorTester, TestCase_SUB)
//{
//   EXPECT_TRUE(isEqArr(-1, metisx::vectorAccelerator(1) - metisx::vector(2)));
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
