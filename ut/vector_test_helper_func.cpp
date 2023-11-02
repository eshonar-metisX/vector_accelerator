#include <vector>
#include <stdint.h>
#include "gtest/gtest.h"

enum class errorType : uint32_t
{
    VEC_ELEMENTS,
    MEAN_SQUARED,
    SUM,    
};


double GetMeanSquaredError(float* exp, float* ctrl, uint64_t size)
{
    double sum = 0.0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += (exp[i] - ctrl[i]) * (exp[i] - ctrl[i]);
    }
    return sum / size;
}

double GetSumError(float* exp, float* ctrl, uint64_t size)
{
    double sum = 0.0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += (exp[i] - ctrl[i]);
    }
    return sum;
}       

template <typename T>
testing::AssertionResult EXPECT_EQ_CUSTOM_FLT(const T& m, const T& n) {
  if (m == n) return testing::AssertionSuccess();

  return testing::AssertionFailure() << "because " << static_cast<double>(m) << " != " << static_cast<double>(n);
}


//TEST(FloatComparison, UsePred) {
//  float expectedResult = 0.07263389974832534800;
//  float actualResult = 0.07263390719890594500;
//
//  EXPECT_TRUE(MyFloatEq(expectedResult, actualResult));
//  std::cout << "----------------------------------------------------"
//            << std::endl;
//}