#pragma once

#include <cstdint>
#include <type_traits>
// #include "metisx_type.hpp"
//LTGC, LTO? inline function?
namespace VectorProcessingEngine
{    
    template <typename dataType, typename returnType = dataType>
    void AddElemwise(returnType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

    template <typename dataType, typename returnType = dataType>
    void SubElemwise(returnType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

    template <typename dataType, typename returnType = float>
    void MulElemwise(returnType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

    template <typename dataType>
    void XorElemwise(dataType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

    template <typename dataType, typename returnType = float>
    returnType ReduceSum(const dataType* const src, const uint64_t& size);

    template <typename dataType, typename returnType = float>
    returnType DotProduct(const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

    template <typename dataType, typename returnType = float>
    returnType DistanceSqaure(const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

};