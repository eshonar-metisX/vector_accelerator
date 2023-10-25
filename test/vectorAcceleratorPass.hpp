#include <type_traits>
#include <cstdint>

namespace pass
{
    class vectorAccelerator
    {
        public:     

        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Add(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {   
                for (unit64_t i = 0; i < size; i++)
                {
                    res[i] = lhs[i] + rhs[i];
                }
            }

         template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Sub(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                 for (unit64_t i = 0; i < size; i++)
                {
                    res[i] = lhs[i] - rhs[i];
                }
            }


        // dot product results in single value: should return type be pointer? value? reference? void type function?
        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Dot(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                dataType sum = 0;
                 for (unit64_t i = 0; i < size; i++)
                {
                    sum += lhs[i] * rhs[i];
                }
                *res = sum;
            }

        // dist square results in single value: should return type be pointer? value? reference? void type function?
        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void DistSquare(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                 dataType sum = 0;
                 for (unit64_t i = 0; i < size; i++)
                {
                    sum += ((lhs[i] - rhs[i]) * (lhs[i] - rhs[i]));
                }
                *res = sum;
            }

        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void ElemwiseMul(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                for (unit64_t i = 0; i < size; i++)
                {
                    res[i] = lhs[i] * rhs[i];
                }
            }

        template <typename dataType,
            typename returnType = std::conditional_t<
            std::is_same_v<dataType, int32_t> || std::is_same_v<dataType, int64_t>, int64_t, std::conditional_t<
            std::is_same_v<dataType, float>, float, void>>>
            static void Xor(const dataType* const lhs, const dataType* const rhs, returnType* res, const uint64_t& size = 1)
            {
                for (unit64_t i = 0; i < size; i++)
                {
                    res[i] = lhs[i] ^ rhs[i];
                }
            }


    };

}