#include <gtest/gtest.h>
#include "../VPE.hpp"

#include "../inc/user_defined_float_types.hpp"



template <class T>
class testParamPack : public testing::Test {};

static const uint64_t gTestSize = 100;

using testDataTypes = typename testing::Types<
std::tuple<float, float>, 
std::tuple<float, double>>;


TYPED_TEST_SUITE(testParamPack, testDataTypes);

TYPED_TEST(testParamPack, TEST_TEST)
{
   
   using A = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using B = typename std::tuple_element<1, decltype(TypeParam())>::type;
   //using C = std::tuple_element<2, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<A> lhs;
   metisx::sim::hwip::VPE<B> rhs;
   EXPECT_TRUE(lhs.IsAllocated());

}

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

//TEST_F(VectorAcceleratorTester, TestCase_ADD_INT32)
//{    
//    ConfigureTest(TestFunc::ADD, TestDataType::INT32, gTestSize);
//    SetUpArr(100, 200);
//    //metisx::vectorAccelerator::Add(intLhs, intRhs, intRes, gTestSize);
//    metisx::dataChunk<float, gTestSize> lhs;
//    pass::vectorAccelerator::Add(intLhs, intRhs, intRes, gTestSize);
//    EXPECT_TRUE(ValidateRes(300));
//}

