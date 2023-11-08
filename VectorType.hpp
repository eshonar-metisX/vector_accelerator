#pragma once

#include <cstdint>
#include <type_traits>
#include "VPE/mu_VPE.hpp"

template <typename dataType>
class VectorType
{
    // forced to float32 for now
    using returnType = float;

public:

    VectorType() = default;

//TODO
    explicit VectorType(dataType* srcPtr)
    {
        arr_ = srcPtr;
    }

    VectorType(VectorType& rhs)
    {
        arr_ = rhs.GetRawPtr();
        rhs.arr_ = nullptr;
    }

    ~VectorType() noexcept
    {
    
    }

    inline dataType* GetRawPtr()
    {
        return arr_;
    };

    inline dataType* GetRawPtr() const
    {
        return arr_;
    };

    void Alloc(const uint64_t& size)
    {
        if (arr_ != nullptr)
        {
            Free();
        }
        arr_ = new dataType[size];
    }

    void Resize(const uint64_t& size)
    {
        // TODO "resize"
        if (arr_ != nullptr)
        {
            Free();
        }
        arr_ = new dataType[size];
        size_ = size;
    }

    void Free()
    {
        if (arr_ != nullptr)
        {
            delete[] arr_;
            arr_ = nullptr;
        }        
        size_ = 0;
    }

    inline uint64_t Size() const
    {
        return size_;
    }

    dataType& operator[](const uint64_t& idx)
    {
        return arr_[idx];
    }

    dataType operator[](const uint64_t& idx) const
    {
        return arr_[idx];
    }

    //copy assignment
    VectorType& operator=(const VectorType& rhs)
    {
        //std::cout << "copy! " << std::endl;

        if (this != &rhs)
        {            
            //std::cout << "deep copy! " << std::endl;

            if (arr_ != nullptr)
            {
                delete[] arr_;
            }
            Resize(rhs.Size());
            for (uint64_t i = 0; i < size_; i++)
            {
                arr_[i] = rhs.arr_[i];
            }
        }
        return *this;
    }

     VectorType& operator=(dataType* rhs)
    {
        //std::cout << "copy! " << std::endl;

        if (arr_ != rhs)
        {            
            //std::cout << "shallow copy! " << std::endl;

            if (arr_ != nullptr)
            {
                delete[] arr_;
            }
            arr_ = rhs;
        }
        return *this;
    }

    // VectorType& operator=(VectorType& rhs)
    // {
    //     std::cout << "copy! " << std::endl;

    //     if (this != &rhs)
    //     {            
    //         std::cout << "deep copy! " << std::endl;

    //         if (arr_ != nullptr)
    //         {
    //             delete[] arr_;
    //         }
    //         Resize(rhs.Size());
    //         for (uint64_t i = 0; i < size_; i++)
    //         {
    //             arr_[i] = rhs.arr_[i];
    //         }
    //     }
    //     return *this;
    // }

private:

    uint64_t size_ = 0;
    dataType* arr_ = nullptr;

};

template <typename inputTypeA, typename inputTypeB, typename returnType = inputTypeA>
returnType* operator+(const VectorType<inputTypeA>& lhs, const VectorType<inputTypeB>& rhs)
{
    VectorType<returnType> res;
    res.Resize(lhs.Size());

    VectorProcessingEngine::AddElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), lhs.Size());

    return res.GetRawPtr();
}

template <typename inputTypeA, typename inputTypeB, typename returnType = inputTypeA>
returnType* operator+(const VectorType<inputTypeA>& lhs, inputTypeB* rhs)
{
    VectorProcessingEngine::AddElemwise(rhs, lhs.GetRawPtr(), rhs, lhs.Size());

    return rhs;
}

template <typename inputTypeA, typename inputTypeB, typename returnType = inputTypeA>
returnType* operator+(inputTypeA* lhs, const VectorType<inputTypeB>& rhs)
{
    VectorProcessingEngine::AddElemwise(lhs, lhs, rhs.GetRawPtr(), rhs.Size());

    return lhs;
}

template <typename inputTypeA, typename inputTypeB, typename returnType = inputTypeA>
const VectorType<returnType> operator-(const VectorType<inputTypeA>& lhs, const VectorType<inputTypeB>& rhs)
{
    VectorType<returnType> res;
    res.Resize(lhs.Size());

    VectorProcessingEngine::SubElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), lhs.Size());

    return res;
}

template <typename inputTypeA, typename inputTypeB, typename returnType = inputTypeA>
const VectorType<returnType> operator*(const VectorType<inputTypeA>& lhs, const VectorType<inputTypeB>& rhs)
{
    VectorType<returnType> res;
    res.Resize(lhs.Size());

    VectorProcessingEngine::MulElemwise(res.GetRawPtr(), lhs.GetRawPtr(), rhs.GetRawPtr(), lhs.Size());

    return res;
}