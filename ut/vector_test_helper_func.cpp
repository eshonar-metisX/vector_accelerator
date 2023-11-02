#include <vector>
#include <stdint.h>

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


