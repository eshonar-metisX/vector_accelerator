#include "mu_vpe.hpp"

#include <cstdint>
#include <type_traits>

#include "../inc/user_defined_float_types.hpp"
using float16Type = half_float::half;

template<>
void mu::VectorProcessingEngine::addElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] + rhs[i];
    }
};

template<>
void mu::VectorProcessingEngine::addElemwise(float16Type* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] + rhs[i];
    }
};

template <>
void mu::VectorProcessingEngine::subElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] - rhs[i];
    }

};

template <>
void mu::VectorProcessingEngine::subElemwise(float16Type* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] - rhs[i];
    }

};

template <>
void mu::VectorProcessingEngine::mulElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] * rhs[i];
    }
            
};

template <>
void mu::VectorProcessingEngine::mulElemwise(float* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = lhs[i] * rhs[i];
    }
            
};

template <>
void mu::VectorProcessingEngine::xorElemwise(float* res, const float* const lhs, const float* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = static_cast<float>(static_cast<uint32_t>(lhs[i]) ^ static_cast<uint32_t>(rhs[i]));
    }

};

template <>
void mu::VectorProcessingEngine::xorElemwise(float16Type* res, const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{         
    for (uint64_t i = 0; i < size; i++)
    {
        res[i] = static_cast<float16Type>(static_cast<uint16_t>(lhs[i]) ^ static_cast<uint16_t>(rhs[i]));
    }

};

template <>
float mu::VectorProcessingEngine::reduceSum(const float* const src, const uint64_t& size)
{
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum = sum + src[i];
    }
    return sum;
};

template <>
float mu::VectorProcessingEngine::reduceSum(const float16Type* const src, const uint64_t& size)
{
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum = sum + src[i];
    }
    return sum;
};

template <>
float mu::VectorProcessingEngine::dotProduct(const float* const lhs, const float* const rhs, const uint64_t& size)
{      
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += (lhs[i] * rhs[i]);
    }

    return sum;
};

template <>
float mu::VectorProcessingEngine::dotProduct(const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{      
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += (lhs[i] * rhs[i]);
    }

    return sum;
};

template <>
float mu::VectorProcessingEngine::distanceSqaure(const float* const lhs, const float* const rhs, const uint64_t& size)
{      
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += ((lhs[i] - rhs[i]) * (lhs[i] - rhs[i]));
    }

    return sum;
};

template <>
float mu::VectorProcessingEngine::distanceSqaure(const float16Type* const lhs, const float16Type* const rhs, const uint64_t& size)
{     
    float sum = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        sum += ((lhs[i] - rhs[i]) * (lhs[i] - rhs[i]));
    }

    return sum;
};
