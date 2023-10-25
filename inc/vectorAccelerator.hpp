#include <type_traits>
#include <cstdint>

namespace metisx
{
    // class name too long?
    class vectorAccelerator
    {
        public:

        // asserting lhs, rhs, res are allocated        
        // signaling error code?
        // signaling NaN? overflow? underflow?
        // if C++14?        
        // making usage of memory offset and chunk size for partial array access?
        // what if scalar is added to vector? broadcasting behavior? 

        // if unsigned integer is needed, use std::is_integral_v & std::is_floating_point_v instead.

        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Add(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {   
                
            }

         template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Sub(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                
            }


        // dot product results in single value: should return type be pointer? value? reference? void type function?
        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Dot(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                
            }

        // dist square results in single value: should return type be pointer? value? reference? void type function?
        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void DistSquare(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                
            }

        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void ElemwiseMul(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                
            }

        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Xor(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                
            }


    };

}