#ifndef METISX_SIM_HWIP_VPE_H_
#define METISX_SIM_HWIP_VPE_H_

#include <type_traits>
#include <cstdint>
#include <memory>
#include <utility>

namespace metisx
{        
namespace sim
{
namespace hwip
{

 template <typename dataType>
    class VPE
    {
        using returnType = float;

    public:

        VPE() = default;
        ~VPE() = default;

        VPE(VPE& src)
        {
            rawPtr = std::move(src.rawPtr);
        };

        bool AllocVector(const uint64_t& N)
        {
            size = N;
            rawPtr = std::make_unique<dataType[]>(size);
            return true;
        }

        inline bool IsAllocated() const noexcept
        {
            return rawPtr != nullptr;
            //return true;
        }

        constexpr VPE& AddElemW(const VPE& src)
        {
            for (uint64_t i = 0; i < size; i++)
            {
                rawPtr.get()[i] += src.rawPtr.get()[i];
            }

            return *this;
        };

         constexpr VPE& operator=(const VPE& src) 
        {                               
            if (rawPtr != nullptr && rawPtr.get() != src.rawPtr.get())
            {                
                rawPtr.reset();
            }
           
            AllocVector(src.size);
            
            for (uint64_t i = 0; i < size; i++)
            {
                rawPtr.get()[i] = src.rawPtr.get()[i];
            }
            //memcpy(rawPtr.get(), src.rawPtr.get(), src.size * sizeof(dataType));

            return *this;
        };


        constexpr VPE<returnType>& operator+() 
        {
            return VPE();
        };

        constexpr VPE<returnType>& SubElemW()
         {
            return VPE();
        };
        constexpr VPE<returnType>& operator-(){
            return VPE();
        };

        constexpr VPE<returnType>& MulElemW() 
        {
            return VPE();
        };
        constexpr VPE<returnType>& operator*() 
        {
            return VPE();
        };
        constexpr VPE<returnType>& XorElemW() 
        {
            return VPE();
        };
        constexpr returnType Dot()
        {
            return returnType(0);
        };
        constexpr returnType DistanceSquare()
        {
            return returnType(0);
        };

        constexpr returnType ReduceSum()
        {
            returnType sum = 0;
            for (uint64_t i = 0; i < size; i++)
            {
                sum += rawPtr.get()[i];
            }
            return sum;
        };  

        dataType& operator[](const uint64_t& idx)
        {
            return rawPtr.get()[idx];
        }


    protected:

        uint64_t size = 0;
        std::unique_ptr<dataType[]> rawPtr = nullptr;
    
    private:




    };



   

} // namespace hwip

} // namespace sim

} // namespace metisx   

#endif // METISX_SIM_HWIP_VPE_H_