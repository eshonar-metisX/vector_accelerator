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

TYPED_TEST(testParamPack, VPE_ALLOC_TEST)
{
   
   using A = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using B = typename std::tuple_element<1, decltype(TypeParam())>::type;
   //using C = std::tuple_element<2, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<A> lhs;
   metisx::sim::hwip::VPE<B> rhs;

   lhs.AllocVector(100);
   rhs.AllocVector(100);

   EXPECT_TRUE(lhs.IsAllocated());
   EXPECT_TRUE(rhs.IsAllocated());

}

TYPED_TEST(testParamPack, VPE_VALUE_ASSIGN_TEST)
{
   
   using A = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using B = typename std::tuple_element<1, decltype(TypeParam())>::type;
   //using C = std::tuple_element<2, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<A> lhs;
   metisx::sim::hwip::VPE<B> rhs;

   lhs.AllocVector(100);
   rhs.AllocVector(100);

   lhs[3] = 1;

   EXPECT_EQ(lhs[3], 1);
   EXPECT_EQ(rhs[25], 0);

}

