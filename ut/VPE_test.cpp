#include <gtest/gtest.h>
#include "../VPE.hpp"

#include "vector_test_helper_func.cpp"
#include "../inc/user_defined_float_types.hpp"
#include <vector>

template <class T>
class testParamPack : public testing::Test {};

template <class T>
class testClassToType : public T {};


static const uint64_t gTestSize = 100;

using testDataTypes = typename testing::Types<
std::tuple<float, float>, 
std::tuple<float, double>,
std::tuple<half_float::half, float>,
std::tuple<HalfFloat, half_float::half>>;

TYPED_TEST_SUITE(testParamPack, testDataTypes);

TYPED_TEST(testParamPack, VPE_ALLOC_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

   lhs.AllocVector(gTestSize);
   rhs.AllocVector(gTestSize);

   EXPECT_TRUE(lhs.IsAllocated());
   EXPECT_TRUE(rhs.IsAllocated());

}

TYPED_TEST(testParamPack, VPE_DEALLOC_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

   lhs.AllocVector(gTestSize);
   rhs.AllocVector(gTestSize);

   EXPECT_TRUE(lhs.IsAllocated());
   EXPECT_TRUE(rhs.IsAllocated());

}

TYPED_TEST(testParamPack, VPE_VALUE_ASSIGN_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeB> rhs;

   lhs.AllocVector(gTestSize);
   rhs.AllocVector(gTestSize);

   lhs[3] = 1.1851;

   EXPECT_TRUE(EXPECT_EQ_CUSTOM_FLT(lhs[3], static_cast<dTypeA>(1.1851)));
   EXPECT_TRUE(EXPECT_EQ_CUSTOM_FLT(rhs[25], static_cast<dTypeB>(0)));


   //EXPECT_EQ(lhs[3], static_cast<dTypeA>(1));
   //EXPECT_EQ(rhs[25], static_cast<dTypeB>(0));

}

TYPED_TEST(testParamPack, VPE_EQUAL_TEST)
{   
   {
      using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;   

      metisx::sim::hwip::VPE<dTypeA> lhs;
      metisx::sim::hwip::VPE<dTypeA> rhs;

      lhs.AllocVector(gTestSize);
      rhs.AllocVector(gTestSize);

      for (uint64_t i = 0; i < gTestSize; i++)
      {
            lhs[i] = static_cast<float>(i);
            rhs[i] = static_cast<float>(i);
      } 

      for (uint64_t i = 0; i < gTestSize; i++)
      {
            EXPECT_EQ(lhs[i], rhs[i]);
      }
   }
   {
      using dTypeA = typename std::tuple_element<1, decltype(TypeParam())>::type;   

      metisx::sim::hwip::VPE<dTypeA> lhs;
      metisx::sim::hwip::VPE<dTypeA> rhs;

      lhs.AllocVector(gTestSize);
      rhs.AllocVector(gTestSize);

      for (uint64_t i = 0; i < gTestSize; i++)
      {
            lhs[i] = static_cast<float>(i + 1);
            rhs[i] = static_cast<float>(i + 1.0000000000000000000001);
      }

      for (uint64_t i = 0; i < gTestSize; i++)
      {
            EXPECT_EQ(lhs[i], rhs[i]);
      }
   }


}

TYPED_TEST(testParamPack, VPE_COPY_TEST)
{
   {
      using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;   

      metisx::sim::hwip::VPE<dTypeA> lhs;
      metisx::sim::hwip::VPE<dTypeA> rhs;

      lhs.AllocVector(gTestSize);
      rhs.AllocVector(gTestSize);

      for (uint64_t i = 0; i < gTestSize; i++)
      {
         lhs[i] = static_cast<float>(i);
      }

      rhs = lhs;

      for (uint64_t i = 0; i < gTestSize; i++)
      {
            EXPECT_EQ(lhs[i], rhs[i]);
      }
   }
   {
      using dTypeA = typename std::tuple_element<1, decltype(TypeParam())>::type;   

      metisx::sim::hwip::VPE<dTypeA> lhs;
      metisx::sim::hwip::VPE<dTypeA> rhs;

      lhs.AllocVector(gTestSize);
      rhs.AllocVector(gTestSize);

      for (uint64_t i = 0; i < gTestSize; i++)
      {
         lhs[i] = static_cast<float>(i);
      }

      rhs = lhs;

      for (uint64_t i = 0; i < gTestSize; i++)
      {
            EXPECT_EQ(lhs[i], rhs[i]);
      }
   }

}


TYPED_TEST(testParamPack, VPE_ADD_ELEMWISE_TEST)
{
   
   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
   //using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;

   metisx::sim::hwip::VPE<dTypeA> lhs;
   metisx::sim::hwip::VPE<dTypeA> rhs;

   lhs.AllocVector(gTestSize);
   rhs.AllocVector(gTestSize);

   for (uint64_t i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<float>(i);
      rhs[i] = static_cast<float>(i);
   }

   lhs = lhs.AddElemW(rhs);

   //for (uint64_t i = 0; i < gTestSize; i++)
   //{
   //    EXPECT_EQ(lhs[i], static_cast<dTypeA>(2 * i));
   //}


   //metisx::sim::hwip::VPE<float> ctrlGroup;
   //std::vector<float> stdCtrlGroup;



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

TYPED_TEST(testParamPack, VPE_REDUCE_SUM_TEST)
{
   
   //using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
//
   //metisx::sim::hwip::VPE<dTypeA> vec;
//
   //vec.AllocVector(gTestSize);
//
   //for (uint64_t i = 0; i < gTestSize; i++)
   //{
   //    vec[i] = static_cast<dTypeA>(i) + static_cast<dTypeA>(0.1);
   //}
//
   //float sum = vec.ReduceSum();
//
   //EXPECT_TRUE(EXPECT_EQ_CUSTOM_FLT(sum, static_cast<float>(4950 + 0.1 * gTestSize)));
   

}

