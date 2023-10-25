#include <iostream>
#include <cstdint>
#include <type_traits>
#include <gtest/gtest.h>

class VectorAcceleratorTester : public ::testing::Test
{

public:

enum TestDataType
{
    INT32 = 1 << 0,
    INT64 = 1 << 1,
    UINT32 = 1 << 2,
    UINT64 = 1 << 3,
    FLOAT32 = 1 << 4,    
    ALL = INT32 | INT64 | UINT32 | UINT64 | FLOAT32,
};

enum class TestFunc
{    
    ADD,
    SUB, 
    DOT,
    DIST_SQUARE,
    ELEM_MUL,
    XOR,
};

public:

    int32_t* intLhs = nullptr;
    int32_t* intRhs = nullptr;

    int64_t* int64Lhs = nullptr;
    int64_t* int64Rhs = nullptr;

    //unsigned signed discrimination?

    int64_t* intRes = nullptr;

    float* flLhs = nullptr;
    float* flRhs = nullptr;
    float* flRes = nullptr;

    uint64_t testSize = 1;
    uint64_t resultSize = 1;
    TestDataType dtFlag = TestDataType::INT32;
    TestFunc caseFlag = TestFunc::ADD;

protected:

    VectorAcceleratorTester()
    {
    }   

    ~VectorAcceleratorTester() override
    {
        if (intLhs != nullptr && intRhs != nullptr)
        {
            delete[] intLhs;
            delete[] intRhs;

            intLhs = nullptr;
            intRhs = nullptr;
        }
       
        if (int64Lhs != nullptr && int64Rhs != nullptr)
        {
            delete[] int64Lhs;
            delete[] int64Rhs;

            int64Lhs = nullptr;
            int64Rhs = nullptr;
        }

        if (intRes != nullptr)
        {
            delete[] intRes;
            intRes = nullptr;
        }

        if (flLhs != nullptr && flRhs != nullptr)
        {
            delete[] flLhs;
            delete[] flRhs;

            flLhs = nullptr;
            flRhs = nullptr;
        }

        if (flRes != nullptr)
        {
            delete[] flRes;
            flRes = nullptr;
        }
        
    }

    inline bool IfTestFuncReturnsScalar() const
    {
        return (caseFlag == TestFunc::DOT || caseFlag == TestFunc::DIST_SQUARE);
    }

    void ConfigureTest(const TestFunc& caseFlagParam = TestFunc::ADD, const TestDataType& dtFlagParam = TestDataType::ALL, const uint64_t size = 1)
    {
        caseFlag = caseFlagParam;
        dtFlag = dtFlagParam;
        testSize = size;

        if (IfTestFuncReturnsScalar())
        {
            resultSize = 1;
        }

        else
        {
            resultSize = testSize;
        }

    }

    template <typename dataType>
    void SetUpArr(const dataType& lhs, const dataType& rhs)
    {
        
        if (dtFlag | TestDataType::INT32)
        {
            intLhs = new int32_t[testSize];
            intRhs = new int32_t[testSize];

            memset(intLhs, static_cast<int32_t>(lhs), testSize * sizeof(int32_t));
            memset(intRhs, static_cast<int32_t>(rhs), testSize * sizeof(int32_t));
        }

        if (dtFlag | TestDataType::INT64)
        {
            int64Lhs = new int64_t[testSize];
            int64Rhs = new int64_t[testSize];

            memset(intLhs, static_cast<int64_t>(lhs), testSize * sizeof(int64_t));
            memset(intRhs, static_cast<int64_t>(rhs), testSize * sizeof(int64_t));
        }

        // if (dtFlag | TestDataType::UINT32)
        // {
        //     intLhs = new int32_t[testSize];
        //     intRhs = new int32_t[testSize];
        //     intRes = new int64_t[testSize];

        //     memset(intLhs, static_cast<int32_t>(lhs), testSize * sizeof(int32_t));
        //     memset(intRhs, static_cast<int32_t>(rhs), testSize * sizeof(int32_t));
        //     memset(intRes, 0, testSize * sizeof(int64_t));
        // }

        // if (dtFlag | TestDataType::UINT64)
        // {
        //     int64Lhs = new int64_t[testSize];
        //     int64Rhs = new int64_t[testSize];
        //     intRes = new int64_t[testSize];

        //     memset(intLhs, static_cast<int64_t>(lhs), testSize * sizeof(int64_t));
        //     memset(intRhs, static_cast<int64_t>(rhs), testSize * sizeof(int64_t));
        //     memset(intRes, 0, testSize * sizeof(int64_t));
        // }       

        if (dtFlag | (TestDataType::INT32 | TestDataType::INT64))
        {
            intRes = new int64_t[resultSize];
            memset(intRes, 0, resultSize * sizeof(int64_t));
        }

        if (dtFlag | TestDataType::FLOAT32)
        {
            flLhs = new float[testSize];
            flRhs = new float[testSize];
            flRes = new float[resultSize];

            memset(flLhs, static_cast<float>(lhs), testSize * sizeof(float));
            memset(flRhs, static_cast<float>(rhs), testSize * sizeof(float));
            memset(flRes, 0, resultSize * sizeof(float));
        }
      
    }   

    //for single value answer
    template <typename src>
    inline bool ValidateRes(const src& val) const
    {             
        try
        {          
            for (uint64_t i = 0; i < resultSize; i++)
            {               
                if (intRes[i] != val)
                {
                    return false;
                }
            }
            return true;            
        }

        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }

    };
    
    template <typename src>
    bool ValidateRes(const src* valArr) const
    {
        return true; 
    };


};
