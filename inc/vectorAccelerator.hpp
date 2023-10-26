#ifndef VECTORACCELERATOR_H_
#define VECTORACCELERATOR_H_

#include <type_traits>
#include <cstdint>
#include <memory>

namespace metisx
{
        //int 삭제?
        //dynamic range는 줄이고 차원을 높이는게 정확도 높음 3072 dim -> 30000 dim까지는 
        // fp16에서 8000차원정도?

    // appropriate class name needed!
    // support "user defined datatype" fp16, fp32, ... template needed
    template <typename dataType, int64_t N>
    class dataChunk
    {        
        static_assert
        (
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t> || 
            std::is_same_v<dataType, float>, 
            "dataType must be one of int32_t, int64_t or float"
        );

        using returnType = typename std::conditional_t<
        std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, 
        std::conditional_t<std::is_same_v<dataType, float>, float, 
        void>>;       

    public:

        dataChunk() = default;
        ~dataChunk() = default;

        inline bool IsAllocated() const noexcept
        {
            //return rawPtr != nullptr;
            return true;

        }

        constexpr inline dataChunk& operator+() 
        { 
        }
        
        constexpr inline dataChunk& operator-() 
        { 
        }



    protected:

        std::unique_ptr<dataType> rawPtr;


       





    

    };



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

#endif // VECTORACCELERATOR_H_