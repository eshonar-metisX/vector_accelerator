#include <gtest/gtest.h>
#include "../VPE.hpp"
#include "../VectorType.hpp"

#include "vector_test_helper_func.cpp"
#include "../inc/user_defined_float_types.hpp"
#include <vector>

template <class T>
class testParamPack : public testing::Test {};

template <class T>
class testClassToType : public T {};


static const int gTestSize = 100;
static const int gTestVerifSize = gTestSize;


//using testDataTypesTuple = typename testing::Types<
//std::tuple<float, float>, 
//std::tuple<half_float::half, float>,
//std::tuple<float, half_float::half>,
//std::tuple<float, HalfFloat>,
//std::tuple<HalfFloat, float>>;
//TYPED_TEST(testParamPack, VPE_ALLOC_TEST_TUPLE)
//{
//   
//   using dTypeA = typename std::tuple_element<0, decltype(TypeParam())>::type;
//   using dTypeB = typename std::tuple_element<1, decltype(TypeParam())>::type;
//
//   metisx::sim::hwip::VPE<dTypeA> lhs;
//   metisx::sim::hwip::VPE<dTypeB> rhs;
//
//   lhs.AllocVector(gTestSize);
//   rhs.AllocVector(gTestSize);
//
//   EXPECT_TRUE(lhs.IsAllocated());
//   EXPECT_TRUE(rhs.IsAllocated());
//
//}

// HalfFloat always fails: excluding for now
using testDataTypes = typename testing::Types<
float, half_float::half, double
>;

testing::AssertionResult EqComparator([[maybe_unused]] const char* m_expr, [[maybe_unused]] const char* n_expr, const float& m, const float& n) 
{
   if (m == n) return testing::AssertionSuccess();

   return testing::AssertionFailure()
         << "Expected equality of these values:\n"
         << "  " << m_expr << "\n    Which is: " << m << "\n"
         << "  " << n_expr << "\n    Which is: " << n << "\n";
}

TYPED_TEST_SUITE(testParamPack, testDataTypes);

TYPED_TEST(testParamPack, VPE_ALLOC_TEST)
{
   VectorType<TypeParam> arr;
   arr.Resize(gTestSize);
   EXPECT_EQ(arr.Size(), gTestSize);
   arr.Free();
}

TYPED_TEST(testParamPack, VPE_DEALLOC_TEST)
{   
   VectorType<TypeParam> arr;
   arr.Resize(gTestSize);
   EXPECT_EQ(arr.Size(), gTestSize);
   arr.Free();
   EXPECT_EQ(arr.Size(), 0);
}

TYPED_TEST(testParamPack, VPE_REALLOC_TEST)
{   
   VectorType<TypeParam> arr;
   arr.Resize(gTestSize);
   EXPECT_EQ(arr.Size(), gTestSize);
   arr.Free();
   EXPECT_EQ(arr.Size(), 0);
   arr.Resize(gTestSize * 2);
   EXPECT_EQ(arr.Size(), gTestSize * 2);
   arr.Free();
}

TYPED_TEST(testParamPack, VPE_VALUE_ASSIGN_TEST)
{   
   VectorType<TypeParam> arr;
   arr.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
         arr[i] = 0.;
   }

   arr[3] = 1.1851;   

   EXPECT_TRUE(EXPECT_EQ_CUSTOM_FLT(arr[3], static_cast<TypeParam>(1.1851)));
   EXPECT_TRUE(EXPECT_EQ_CUSTOM_FLT(arr[25], static_cast<TypeParam>(0.)));

}

TYPED_TEST(testParamPack, VPE_EQUAL_ASSIGN_TEST)
{      
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;

   lhs.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
         lhs[i] = static_cast<float>(i);        
   }

   rhs = lhs;

   for (int i = 0; i < gTestVerifSize; i++)
   {
      EXPECT_PRED_FORMAT2(EqComparator, lhs[i], rhs[i]);
   }   
}

TYPED_TEST(testParamPack, VPE_ADD_ELEMWISE_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<float>(i);
      rhs[i] = static_cast<float>(i * 2);
      res[i] = 0.;
   }

   VectorProcessingEngine<TypeParam>::AddElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i + i * 2);
   }
}

TYPED_TEST(testParamPack, VPE_ADD_ELEMWISE_OPERATOR_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<float>(i);
      rhs[i] = static_cast<float>(i * 2);
   }

   res = lhs + lhs + lhs + rhs;

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i + i + i + i * 2);
   }
}

TYPED_TEST(testParamPack, VPE_SUB_ELEMWISE_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i);
      rhs[i] = static_cast<TypeParam>(i * 2);
      res[i] = 0.;
   }

   VectorProcessingEngine<TypeParam>::SubElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i - i * 2);
   }
}


TYPED_TEST(testParamPack, VPE_SUB_ELEMWISE_OPERATOR_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<float>(i);
      rhs[i] = static_cast<float>(i * 2);
   }

   res = lhs - lhs - lhs - rhs;

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i - i - i - i * 2);
   }
}

TYPED_TEST(testParamPack, VPE_MUL_ELEMWISE_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
      res[i] = 0.;
   }

   VectorProcessingEngine<TypeParam>::MulElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], static_cast<TypeParam>(i + 0.1) * static_cast<TypeParam>(i * 2 + 0.01));
   }
}

TYPED_TEST(testParamPack, VPE_MUL_ELEMWISE_OPERATOR_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<float>(i);
      rhs[i] = static_cast<float>(i * 2);
   }

   res = lhs * lhs * lhs * rhs;

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i * i * i * i * 2);
   }
}

TYPED_TEST(testParamPack, VPE_XOR_ELEMWISE_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<TypeParam> res;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
      res[i] = 0.;
   }

   VectorProcessingEngine<TypeParam>::XorElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], static_cast<uint64_t>(i + 0.1) ^ static_cast<uint64_t>(i * 2 + 0.01));
   }
}

TYPED_TEST(testParamPack, VPE_REDUCE_SUM_TEST)
{   
   typename VectorProcessingEngine<TypeParam>::returnType sum = 0;
   VectorType<TypeParam> src;

   src.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      src[i] = static_cast<TypeParam>(i + 0.1);
   }

   sum = VectorProcessingEngine<TypeParam>::ReduceSum(src.GetRawPtr(), gTestSize);
   
   typename VectorProcessingEngine<TypeParam>::returnType test_sum = 0;
   for (int i = 0; i < gTestSize; i++)
   {
      test_sum += static_cast<TypeParam>(i + 0.1);
   }

   EXPECT_PRED_FORMAT2(EqComparator, sum, test_sum);
   
}

TYPED_TEST(testParamPack, VPE_DOT_TEST)
{   
   typename VectorProcessingEngine<TypeParam>::returnType dot = 0;

   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);  

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
   }

   dot = VectorProcessingEngine<TypeParam>::DotProduct(lhs.GetRawPtr(), rhs.GetRawPtr(),gTestSize);
   
   typename VectorProcessingEngine<TypeParam>::returnType test_dot = 0;
   for (int i = 0; i < gTestSize; i++)
   {
      test_dot += static_cast<TypeParam>(i + 0.1) * static_cast<TypeParam>(i * 2 + 0.01);
   }

   EXPECT_PRED_FORMAT2(EqComparator, dot, test_dot);   
}

TYPED_TEST(testParamPack, VPE_DIST_SQUARE_TEST)
{   
   typename VectorProcessingEngine<TypeParam>::returnType dot = 0;

   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);  

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
   }

   dot = VectorProcessingEngine<TypeParam>::DistanceSqaure(lhs.GetRawPtr(), rhs.GetRawPtr(),gTestSize);
   
   typename VectorProcessingEngine<TypeParam>::returnType test_dot = 0;
   for (int i = 0; i < gTestSize; i++)
   {
      test_dot += (static_cast<TypeParam>(i + 0.1) - static_cast<TypeParam>(i * 2 + 0.01)) * (static_cast<TypeParam>(i + 0.1) - static_cast<TypeParam>(i * 2 + 0.01));
   }

   EXPECT_PRED_FORMAT2(EqComparator, dot, test_dot);
   
}