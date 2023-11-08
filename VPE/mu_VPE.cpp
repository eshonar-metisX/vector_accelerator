#include "mu_VPE.hpp"

#include <cstdint>
#include <type_traits>

#include "../inc/user_defined_float_types.hpp"
using float16Type = half_float::half;

template<>
void VectorProcessingEngine::AddElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] + rhs[i];
    }
};

template<>
void VectorProcessingEngine::AddElemwise(float16Type* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] + rhs[i];
    }
};

template <>
void VectorProcessingEngine::SubElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] - rhs[i];
    }

};

template <>
void VectorProcessingEngine::SubElemwise(float16Type* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] - rhs[i];
    }

};

template <>
void VectorProcessingEngine::MulElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] * rhs[i];
    }
            
};

template <>
void VectorProcessingEngine::MulElemwise(float* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] * rhs[i];
    }
            
};

template <>
void VectorProcessingEngine::XorElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = static_cast<float>(static_cast<uint32_t>(lhs[i]) ^ static_cast<uint32_t>(rhs[i]));
    }

};

template <>
void VectorProcessingEngine::XorElemwise(float16Type* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = static_cast<float16Type>(static_cast<uint16_t>(lhs[i]) ^ static_cast<uint16_t>(rhs[i]));
    }

};

template <>
float VectorProcessingEngine::ReduceSum(const float* const src, const uint64_t& size)
{
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum = sum + src[i];
    }
    return sum;
};

template <>
float VectorProcessingEngine::ReduceSum(const float16Type* const src, const uint64_t& size)
{
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum = sum + src[i];
    }
    return sum;
};

template <>
float VectorProcessingEngine::DotProduct(const float* const lhs, const float* const rhs, const uint64_t& size)
{      
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += (lhs[i] * rhs[i]);
    }

    return sum;
};

template <>
float VectorProcessingEngine::DotProduct(const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{      
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += (lhs[i] * rhs[i]);
    }

    return sum;
};

template <>
float VectorProcessingEngine::DistanceSqaure(const float* const lhs, const float* const rhs, const uint64_t& size)
{      
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += ((lhs[i] - rhs[i]) * (lhs[i] - rhs[i]));
    }

    return sum;
};

template <>
float VectorProcessingEngine::DistanceSqaure(const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{     
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += ((lhs[i] - rhs[i]) * (lhs[i] - rhs[i]));
    }

    return sum;
};
