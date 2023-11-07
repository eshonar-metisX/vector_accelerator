#pragma once

#include <cstdint>
#include <type_traits>

template <typename dataType>
class VectorProcessingEngine
{
public:

    // forced to float32 for now
    using returnType = std::conditional_t<std::is_same_v<dataType, double>, double, float>;
    using bitwiseType = std::conditional_t<sizeof(dataType) == 4, uint32_t, uint64_t>;

    VPE() = delete;
    
    template <typename inputTypeA, typename inputTypeB>
    static inline void AddElemwise(returnType* res, const inputTypeA* const lhs, const inputTypeB* const rhs, const uint64_t& size)
    {         
        for (uint64_t i = 0; i < size; i++)
        {
            res[i] = lhs[i] + rhs[i];
        }

    };

    template <typename inputTypeA, typename inputTypeB>
    static inline void SubElemwise(returnType* res, const inputTypeA* const lhs, const inputTypeB* const rhs, const uint64_t& size)
    {         
        for (uint64_t i = 0; i < size; i++)
        {
            res[i] = lhs[i] - rhs[i];
        }
    
    };

    template <typename inputTypeA, typename inputTypeB>
    static inline void MulElemwise(returnType* res, const inputTypeA* const lhs, const inputTypeB* const rhs, const uint64_t& size)
    {         
        for (uint64_t i = 0; i < size; i++)
        {
            res[i] = lhs[i] * rhs[i];
        }
                
    };

    template <typename inputTypeA, typename inputTypeB>
    static inline void XorElemwise(inputTypeA* res, const inputTypeA* const lhs, const inputTypeB* const rhs, const uint64_t& size)
    {         
        for (uint64_t i = 0; i < size; i++)
        {
            res[i] = static_cast<inputTypeA>(static_cast<bitwiseType>(lhs[i]) ^ static_cast<bitwiseType>(rhs[i]));
        }
    
    };

    template <typename inputType>
    static inline returnType ReduceSum(const inputType* const src, const uint64_t& size)
    {
        returnType sum = 0;
        for (uint64_t i = 0; i < size; i++)
        {
            sum = sum + src[i];
        }
        return sum;
    };

    static returnType DotProduct(const dataType* const lhs, const dataType* const rhs, const uint64_t& size)
    {      
        returnType* res = new returnType[size];

        MulElemwise(res, lhs, rhs, size);

        returnType sum = ReduceSum(res, size);

        delete[] res;

        return sum;

    };

    static returnType DistanceSqaure(const dataType* const lhs, const dataType* const rhs, const uint64_t& size)
    {      
        returnType* res = new returnType[size];

        SubElemwise(res, lhs, rhs, size);
        MulElemwise(res, res, res, size);

        returnType sum = ReduceSum(res, size);

        delete[] res;

        return sum;

    };

};