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
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

   lhs.AllocVector(100);
   rhs.AllocVector(100);

   EXPECT_TRUE(lhs.IsAllocated());
   EXPECT_TRUE(rhs.IsAllocated());

}

TYPED_TEST(testParamPack, VPE_DEALLOC_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

   lhs.AllocVector(100);
   rhs.AllocVector(100);

   EXPECT_TRUE(lhs.IsAllocated());
   EXPECT_TRUE(rhs.IsAllocated());

}

TYPED_TEST(testParamPack, VPE_VALUE_ASSIGN_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

   lhs.AllocVector(100);
   rhs.AllocVector(100);

   lhs[3] = 1;

   EXPECT_EQ(lhs[3], 1);
   EXPECT_EQ(rhs[25], 0);

}

TYPED_TEST(testParamPack, VPE_ADD_ELEMWISE_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;


}


TYPED_TEST(testParamPack, VPE_SUB_ELEMWISE_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;


}


TYPED_TEST(testParamPack, VPE_MUL_ELEMWISE_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;


}


TYPED_TEST(testParamPack, VPE_XOR_ELEMWISE_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

}


TYPED_TEST(testParamPack, VPE_DOT_PRODUCT_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;


}

TYPED_TEST(testParamPack, VPE_DIST_SQUARE_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

}

