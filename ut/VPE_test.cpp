#include <gtest/gtest.h>
#include "../VPE/mu_VPE.hpp"
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

template <int Mant = 23, int Exp = 8, int BiasExp = 127>
class MetisX_Float : public numsys_hw::TypeFloat
{

public:
   MetisX_Float() : numsys_hw::TypeFloat(Mant, Exp, BiasExp) {}

   template <typename inputType>
   MetisX_Float(const inputType& v) : numsys_hw::TypeFloat(v) {}

};

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

// array initializer function for dynamic range test?

// HalfFloat always fails: excluding for now
// numsys_hw::TypeFloat operator 부족함, 추가해야
using testDataTypes = typename testing::Types<
float, half_float::half
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

   arr.Free();

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

   lhs.Free();
   rhs.Free();
}

TYPED_TEST(testParamPack, VPE_ADD_ELEMWISE_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<TypeParam> res;  

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<float>(i);
      rhs[i] = static_cast<float>(i * 2);
      res[i] = 0.;
   }

   VectorProcessingEngine::AddElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestVerifSize; i++)
   {
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i + i * 2);
   }

   lhs.Free();
   rhs.Free();
   res.Free();
}

// TYPED_TEST(testParamPack, VPE_ADD_ELEMWISE_OPERATOR_TEST)
// {   
//    VectorType<TypeParam> lhs;
//    VectorType<TypeParam> rhs;
//    VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

//    lhs.Resize(gTestSize);
//    rhs.Resize(gTestSize);

//    for (int i = 0; i < gTestSize; i++)
//    {
//       lhs[i] = static_cast<float>(i);
//       rhs[i] = static_cast<float>(i * 2);
//    }

//    //res = lhs + rhs; // lhs + lhs +
//    res = lhs + lhs + lhs + rhs; //

//    for (int i = 0; i < gTestVerifSize; i++)
//    {      
//       EXPECT_PRED_FORMAT2(EqComparator, res[i], i + i + i + i * 2);
//    }

//    lhs.Free();
//    rhs.Free();
//    res.Free();
// }

TYPED_TEST(testParamPack, VPE_SUB_ELEMWISE_TEST)
{
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<TypeParam> res;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i);
      rhs[i] = static_cast<TypeParam>(i * 2);
      res[i] = 0.;
   }

   VectorProcessingEngine::SubElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], i - i * 2);
   }
   lhs.Free();
   rhs.Free();
   res.Free();
}

// TYPED_TEST(testParamPack, VPE_SUB_ELEMWISE_OPERATOR_TEST)
// {   
//    VectorType<TypeParam> lhs;
//    VectorType<TypeParam> rhs;
//    VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

//    lhs.Resize(gTestSize);
//    rhs.Resize(gTestSize);

//    for (int i = 0; i < gTestSize; i++)
//    {
//       lhs[i] = static_cast<float>(i);
//       rhs[i] = static_cast<float>(i * 2);
//    }

//    res = lhs - lhs - lhs - rhs;

//    for (int i = 0; i < gTestVerifSize; i++)
//    {      
//       EXPECT_PRED_FORMAT2(EqComparator, res[i], i - i - i - i * 2);
//    }
//    lhs.Free();
//    rhs.Free();
//    res.Free();
// }

TYPED_TEST(testParamPack, VPE_MUL_ELEMWISE_TEST)
{   
   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;
   VectorType<float> res;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);
   res.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
      res[i] = 0.;
   }

   VectorProcessingEngine::MulElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestVerifSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], static_cast<TypeParam>(i + 0.1) * static_cast<TypeParam>(i * 2 + 0.01));
   }
   lhs.Free();
   rhs.Free();
   res.Free();
}

// TYPED_TEST(testParamPack, VPE_MUL_ELEMWISE_OPERATOR_TEST)
// {   
//    VectorType<TypeParam> lhs;
//    VectorType<TypeParam> rhs;
//    VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

//    lhs.Resize(gTestSize);
//    rhs.Resize(gTestSize);

//    for (int i = 0; i < gTestSize; i++)
//    {
//       lhs[i] = static_cast<TypeParam>(i);
//       rhs[i] = static_cast<TypeParam>(i * 2);
//    }

//    res = lhs * rhs;

//    for (int i = 0; i < gTestVerifSize; i++)
//    {      
//       TypeParam lhs_elem = static_cast<TypeParam>(i);
//       TypeParam rhs_elem = static_cast<TypeParam>(i * 2);
//       EXPECT_PRED_FORMAT2(EqComparator, res[i], lhs_elem * rhs_elem);
//    }
//    lhs.Free();
//    rhs.Free();
//    res.Free();
// }

// TYPED_TEST(testParamPack, VPE_COMP_ELEMWISE_OPERATOR_TEST)
// {   
//    VectorType<TypeParam> lhs;
//    VectorType<TypeParam> rhs;
//    VectorType<typename VectorProcessingEngine<TypeParam>::returnType> res;  

//    lhs.Resize(gTestSize);
//    rhs.Resize(gTestSize);

//    for (int i = 0; i < gTestSize; i++)
//    {
//       lhs[i] = static_cast<float>(i);
//       rhs[i] = static_cast<float>(i * 2);
//    }

//    res = lhs * lhs - lhs * (rhs + rhs);

//    for (int i = 0; i < gTestVerifSize; i++)
//    {      
//       TypeParam lhs_elem = static_cast<TypeParam>(i);
//       TypeParam rhs_elem = static_cast<TypeParam>(i * 2);
//       //EXPECT_PRED_FORMAT2(EqComparator, res[i], lhs_elem * lhs_elem - lhs_elem * (rhs_elem + rhs_elem));
//       EXPECT_PRED_FORMAT2(EqComparator, res[i], lhs_elem * lhs_elem - lhs_elem * (rhs_elem + rhs_elem));
//    }
//    lhs.Free();
//    rhs.Free();
//    res.Free();
// }

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

   VectorProcessingEngine::XorElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {      
      EXPECT_PRED_FORMAT2(EqComparator, res[i], static_cast<uint64_t>(i + 0.1) ^ static_cast<uint64_t>(i * 2 + 0.01));
   }
   lhs.Free();
   rhs.Free();
   res.Free();
}

TYPED_TEST(testParamPack, VPE_REDUCE_SUM_TEST)
{   
   float sum = 0;
   VectorType<TypeParam> src;

   src.Resize(gTestSize);

   for (int i = 0; i < gTestSize; i++)
   {
      src[i] = static_cast<TypeParam>(i + 0.1);
   }

   sum = VectorProcessingEngine::ReduceSum(src.GetRawPtr(), gTestSize);
   
   float test_sum = 0;
   for (int i = 0; i < gTestSize; i++)
   {
      test_sum += static_cast<TypeParam>(i + 0.1);
   }

   EXPECT_PRED_FORMAT2(EqComparator, sum, test_sum);
   src.Free();
}

TYPED_TEST(testParamPack, VPE_DOT_TEST)
{   
   float dot = 0;

   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);  

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
   }

   dot = VectorProcessingEngine::DotProduct(lhs.GetRawPtr(), rhs.GetRawPtr(),gTestSize);
   
   float test_dot = 0;
   for (int i = 0; i < gTestSize; i++)
   {
      test_dot += static_cast<TypeParam>(i + 0.1) * static_cast<TypeParam>(i * 2 + 0.01);
   }

   EXPECT_PRED_FORMAT2(EqComparator, dot, test_dot);   
   lhs.Free();
   rhs.Free();
}

TYPED_TEST(testParamPack, VPE_DIST_SQUARE_TEST)
{   
   float dot = 0;

   VectorType<TypeParam> lhs;
   VectorType<TypeParam> rhs;

   lhs.Resize(gTestSize);
   rhs.Resize(gTestSize);  

   for (int i = 0; i < gTestSize; i++)
   {
      lhs[i] = static_cast<TypeParam>(i + 0.1);
      rhs[i] = static_cast<TypeParam>(i * 2 + 0.01);
   }

   dot = VectorProcessingEngine::DistanceSqaure(lhs.GetRawPtr(), rhs.GetRawPtr(),gTestSize);
   
   float test_dot = 0;
   for (int i = 0; i < gTestSize; i++)
   {
      test_dot += (static_cast<TypeParam>(i + 0.1) - static_cast<TypeParam>(i * 2 + 0.01)) * (static_cast<TypeParam>(i + 0.1) - static_cast<TypeParam>(i * 2 + 0.01));
   }

   EXPECT_PRED_FORMAT2(EqComparator, dot, test_dot);
   lhs.Free();
   rhs.Free();
}