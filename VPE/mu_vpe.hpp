#pragma once

#include <cstdint>
#include <type_traits>
// #include "metisx_type.hpp"
// LTGC, LTO? inline function?
namespace mu
{
namespace VectorProcessingEngine
{
template <typename dataType, typename returnType = dataType>
void addElemwise(returnType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

template <typename dataType, typename returnType = dataType>
void subElemwise(returnType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

template <typename dataType, typename returnType = float>
void mulElemwise(returnType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

template <typename dataType>
void xorElemwise(dataType* res, const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

template <typename dataType, typename returnType = float>
returnType reduceSum(const dataType* const src, const uint64_t& size);

template <typename dataType, typename returnType = float>
returnType dotProduct(const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

template <typename dataType, typename returnType = float>
returnType distanceSqaure(const dataType* const lhs, const dataType* const rhs, const uint64_t& size);

};  // namespace VectorProcessingEngine
}  // namespace mu