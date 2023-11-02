#pragma once

#include <assert.h>

#include <cinttypes>
#include <iostream>
#include <memory>
#include <vector>

namespace numsys_hw {

using TFDATA = uint32_t;
using TFDATAL = uint64_t;
using TFBITS = int32_t;
using TFMASK = uint32_t;
using TFEXP = int32_t;
using TFCVT = float;
static constexpr TFBITS NBIT_TFDATA = 32;

/**
 * @brief TypeFloatBase is a c++ class describing Floating-Point (FP) operations.
 * It follows IEEE 754 Standard, and provides additional configurational feature
 * to modify exponent-bias, bit-depth of mantissa and exponent.
 */
class TypeFloatBase {
protected:
    // Default configuration: IEEE 754 Single-precision (FP32)
    static constexpr bool DEFAULT_IS_SIGNED = true;
    static constexpr TFBITS DEFAULT_NBIT_MANT = 23U;  // The number of Mantissa except one bit of the leading one
    static constexpr TFBITS DEFAULT_NBIT_EXP = 8U;
    static constexpr TFEXP DEFULAT_BIAS_EXP = 127U;
    static constexpr TFMASK maskBit = 1U;

public:
    /**
     * @brief Construct a new TypeFloatBase object
     * This is Default construction, which configures it as IEEE 754 Single-precision (FP32)
     */
    TypeFloatBase()
        : isSigned(DEFAULT_IS_SIGNED),
          nbMant(DEFAULT_NBIT_MANT),
          nbExp(DEFAULT_NBIT_EXP),
          biasExp(DEFULAT_BIAS_EXP),
          data(0x0){};
    /**
     * @brief Construct a new TypeFloatBase object
     * @param _nbMant : The number of Mantissa except one bit of the leading one
     * @param _nbExp : The number of Exponent
     * @param _biasExp : The bias of Exponent
     */
    TypeFloatBase(int _nbMant, int _nbExp, int _biasExp)
        : isSigned(DEFAULT_IS_SIGNED), nbMant(_nbMant), nbExp(_nbExp), biasExp(_biasExp), data(0x0) {};
    /**
     * @brief Construct a new TypeFloatBase object
     * @param _isSigned : Boolean value whether it has a sign or not
     * @param _nbMant : The number of Mantissa except one bit of the leading one
     * @param _nbExp : The number of Exponent
     * @param _biasExp : The bias of Exponent
     */
    TypeFloatBase(bool _isSigned, int _nbMant, int _nbExp, int _biasExp)
        : isSigned(_isSigned), nbMant(_nbMant), nbExp(_nbExp), biasExp(_biasExp), data(0x0) {};
    /**
     * @brief Construct a new TypeFloatBase object
     * @param v : A initial value in various datatypes
     */
    TypeFloatBase(bool v)
        : isSigned(true), nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(char v)
        : isSigned(true), nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(unsigned char v)
        : nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(int v)
        : isSigned(true), nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(unsigned int v)
        : isSigned(true), nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(double v)
        : isSigned(true), nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(float v)
        : isSigned(true), nbMant(DEFAULT_NBIT_MANT), nbExp(DEFAULT_NBIT_EXP), biasExp(DEFULAT_BIAS_EXP), data(0x0) {
        operator=(v);
    }
    TypeFloatBase(const TypeFloatBase& v)
        : isSigned(v.isSigned), nbMant(v.nbMant), nbExp(v.nbExp), biasExp(v.biasExp), data(v.data) {}

    // Assignments
    // TODO: Check there is a method to merge operator=
    const TypeFloatBase& operator=(bool v);
    const TypeFloatBase& operator=(char v);
    const TypeFloatBase& operator=(unsigned char v);
    const TypeFloatBase& operator=(int v);
    const TypeFloatBase& operator=(unsigned int v);
    const TypeFloatBase& operator=(double v);
    const TypeFloatBase& operator=(float v);
    const TypeFloatBase& operator=(const TypeFloatBase& v);

    // Accessors
    TFCVT getValue() const;
    bool getIsSigned() const { return isSigned; }
    TFBITS getNbMant() const { return nbMant; }
    TFBITS getNbExp() const { return nbExp; }
    TFEXP getBiasExp() const { return biasExp; }
    TFDATA getData() const { return data; }
    TFDATA getSign() const { return isSigned ? static_cast<TFDATA>((data >> (nbMant + nbExp)) & maskBit) : 0U; }
    TFDATA getMant() const { return data & getMaskMant(); }
    TFEXP getExpBiased() const { return static_cast<TFEXP>(((data >> nbMant) & getMaskExp())); }
    TFEXP getExp() const { return getExpBiased() - biasExp; }
    bool isZero() const { return (data & getMaskExpMant()) == 0U; }
    bool isPosZero() const { return data == 0U; };
    bool isNegZero() const { return data == (1U << (nbMant + nbExp)); }
    bool isDenormal() const { return (getExpBiased() == 0U) && (getMant() != 0U); }
    bool isInf() const { return (getExpBiased() == static_cast<TFEXP>(getMaskExp())) && (getMant() == 0U); }
    bool isPosInf() const { return isInf() && (getSign() == 0U); }
    bool isNegInf() const { return isInf() && (getSign() == 1U); }
    bool isNan() const { return (getExpBiased() == static_cast<TFEXP>(getMaskExp())) && (getMant() != 0U); }
    bool isQuietNan() const {
        return (getExpBiased() == static_cast<TFEXP>(getMaskExp())) && (getMant() == getMantQuietNan());
    }
    bool isPosQuietNan() const { return isQuietNan() && (getSign() == 0U); }
    bool isNegQuietNan() const { return isQuietNan() && (getSign() == 1U); }
    bool isSignalingNan() const {
        return (getExpBiased() == static_cast<TFEXP>(getMaskExp())) && (getMant() == getMantSignalingNan());
    }
    bool isPosSignalingNan() const { return isSignalingNan() && (getSign() == 0U); }
    bool isNegSignalingNan() const { return isSignalingNan() && (getSign() == 1U); }
    void setData(const TFDATA& d) { data = d; }
    void setZero(const TFDATA& s) { s == 0 ? setPosZero() : setNegZero(); }
    void setPosZero() { data = 0U; };
    void setNegZero() { data = (1U << (nbMant + nbExp)); }
    void setInf(const TFDATA& s) { s == 0 ? setPosInf() : setNegInf(); }
    void setPosInf() { data = getData(0U, getMaskExp(), 0U); }
    void setNegInf() { data = getData(1U, getMaskExp(), 0U); }
    void setQuietNan(const TFDATA& s) { s == 0 ? setPosQuietNan() : setNegQuietNan(); }
    void setPosQuietNan() { data = getData(0U, getMaskExp(), getMantQuietNan()); }
    void setNegQuietNan() { data = getData(1U, getMaskExp(), getMantQuietNan()); }
    void setSignalingNan(const TFDATA& s) { s == 0 ? setPosSignalingNan() : setNegSignalingNan(); }
    void setPosSignalingNan() { data = getData(0U, getMaskExp(), getMantSignalingNan()); }
    void setNegSignalingNan() { data = getData(1U, getMaskExp(), getMantSignalingNan()); }
    void setIndefiniteNan(const TFDATA& s) { s == 0 ? setPosIndefiniteNan() : setNegIndefiniteNan(); }
    void setPosIndefiniteNan() { data = getData(0U, getMaskExp(), getMantIndefiniteNan()); }
    void setNegIndefiniteNan() { data = getData(1U, getMaskExp(), getMantIndefiniteNan()); }
    TFDATA getData(const TFDATA& s, const TFEXP& eb, const TFDATA& m) const {
        return ((s & maskBit) << (nbMant + nbExp)) | ((static_cast<TFDATA>(eb) & getMaskExp()) << nbMant) |
               (m & getMaskMant());
    }

protected:
    TFDATA buildData(const TypeFloatBase& v, bool rounding) const;
    bool hasSameConfig(const TypeFloatBase& v) const;
    TFEXP getZeroLengthOfMant(const TFDATA& m, const TFBITS& nbM) const;
    TFEXP getZeroLengthOfMant(const TFDATAL& m, const TFBITS& nbM) const;
    inline TFDATA getOne() const { return 1U << nbMant; }
    inline TFMASK getMaskMant() const { return (1U << nbMant) - 1U; }
    inline TFMASK getMaskExp() const { return (1U << nbExp) - 1U; }
    inline TFMASK getMaskExpMant() const { return (1U << (nbExp + nbMant)) - 1U; }
    inline TFDATA getMantQuietNan() const { return (1U << (nbMant - 1)); }
    inline TFDATA getMantSignalingNan() const { return 1U << (nbMant - 2); }
    inline TFDATA getMantIndefiniteNan() const { return 11U << (nbMant - 2); }

    /**
     * @brief It calculates (m / 2^rsh) and replaces it with an approximate value that has a shorter representation.
     * @param m : Mantissa value, not rounded yet
     * @param rsh : Right-shift value for dividing
     * @return TFDATA : Right-shifted mantissa vluae with the round half to even rule.
     */
    TFDATA roundHalfToEven(const TFDATA& m, const TFDATA& rsh) const;
    TFDATAL roundHalfToEven(const TFDATAL& m, const TFDATA& rsh) const;
    inline TFDATA roundHalfToNearest(const TFDATA& m, const TFDATA& rsh) const {
        return rsh == 0 ? m : (((m >> (rsh - 1U)) + 1U) >> 1U);
    }
    inline TFDATAL roundHalfToNearest(const TFDATAL& m, const TFDATA& rsh) const {
        return rsh == 0 ? m : (((m >> (rsh - 1U)) + 1U) >> 1U);
    }

    /// @brief Boolean value whether it has a sign-bit or not
    bool isSigned;
    /// @brief The number of bits of Mantissa except one bit of the leading one
    TFBITS nbMant;
    /// @brief The number of bits of Exponent
    TFBITS nbExp;
    /// @brief Bias value of the exponent
    TFEXP biasExp;
    ///@brief Encoded binary string 32-bit container
    TFDATA data;
};

using TypeFloatBasePtr = std::shared_ptr<TypeFloatBase>;

/**
 * @brief TypeFloat inheriting TypeFloatBase provides arithmetic, shift, and comparing operators.
 */
class TypeFloat : public TypeFloatBase {
public:
    TypeFloat() : TypeFloatBase() {}
    TypeFloat(int _nbMant, int _nbExp, int _biasExp) : TypeFloatBase(_nbMant, _nbExp, _biasExp) { checkAsserts(); }
    TypeFloat(bool _isSigned, int _nbMant, int _nbExp, int _biasExp)
        : TypeFloatBase(_isSigned, _nbMant, _nbExp, _biasExp) { checkAsserts(); }
    TypeFloat(bool v) : TypeFloatBase(v) {}
    TypeFloat(char v) : TypeFloatBase(v) {}
    TypeFloat(unsigned char v) : TypeFloatBase(v) {}
    TypeFloat(int v) : TypeFloatBase(v) {}
    TypeFloat(unsigned int v) : TypeFloatBase(v) {}
    TypeFloat(double v) : TypeFloatBase(v) {}
    TypeFloat(float v) : TypeFloatBase(v) {}
    TypeFloat(const TypeFloatBase& v) : TypeFloatBase(v) {}
    TypeFloat(TypeFloatBasePtr v) : TypeFloatBase(*v) { operator=(v); }

    // Assignment operator
    const TypeFloatBase& operator=(TypeFloatBasePtr v);
    const TypeFloatBase& assignRounded(TypeFloatBasePtr v);

    // Arithmetic operators
    // TODO: Modify /operator
    double operator/(TypeFloat& v);
    TypeFloatBasePtr operator+(const TypeFloat& v);
    TypeFloatBasePtr operator-(const TypeFloat& v);
    TypeFloatBasePtr operator*(const TypeFloat& v);
    // TypeFloatBasePtr operator/(TypeFloat& v);

    // Shift operators
    void operator>>(uint32_t rsh);

    // Compare operators
    // TODO: When v is NaN, maybe all comparions should return false
    bool operator==(const TypeFloat& v);
    bool operator!=(const TypeFloat& v);
    bool operator>(const TypeFloat& v);
    bool operator<(const TypeFloat& v);
    bool operator>=(const TypeFloat& v);
    bool operator<=(const TypeFloat& v);

private:
    /// @brief Checks basic assertions for TypeFloat
    void checkAsserts() {
        assert((nbMant + nbExp + static_cast<TFBITS>(isSigned)) <= NBIT_TFDATA);
        assert(nbMant > 0);
        assert(nbExp > 0);
    }
};





// /**
//  * @brief BlockTypeFloatBase inheriting TypeFloatBase provides Block Floating-Point arithmetic operators
//  * This class stores multiple floating-point values using one expoenent stored in data member-variable of TypeFloatBase,
//  * and the corresponding mantissas are contained in an arrMants.
//  * Because of the sharing the exponent, the mantissas cannot be normalized and encoding the leading one is not adopted.
//  * When the exponent is the maximum value, the first element of arrMants representive of this BlockTypeFloat
//  * meaning Inf when 0, and Nan when the maximum value.
//  */
// using BTFIDX = uint32_t;

// class BlockTypeFloatBase : public TypeFloatBase {
// public:
//     BlockTypeFloatBase(uint32_t _n) : TypeFloatBase(), arrMants(_n), arrExps(_n), isBlocked(false), hasMultCarry(false) {}
//     BlockTypeFloatBase(uint32_t _n, int _nbMant, int _nbExp, int _biasExp)
//         : TypeFloatBase(_nbMant, _nbExp, _biasExp), arrMants(_n), arrExps(_n), isBlocked(false), hasMultCarry(false) {}
//     BlockTypeFloatBase(uint32_t _n, bool _isSigned, int _nbMant, int _nbExp, int _biasExp)
//         : TypeFloatBase(_isSigned, _nbMant, _nbExp, _biasExp), arrMants(_n), arrExps(_n), isBlocked(false), hasMultCarry(false) {}

//     // Assignments
//     const BlockTypeFloatBase& operator=(BlockTypeFloatBase& vs);
//     bool set(const BTFIDX& idx, const TypeFloatBase& v);
//     bool set(const BTFIDX& idx, const bool& v) { return set(idx, (TFCVT)v); }
//     bool set(const BTFIDX& idx, const char& v) { return set(idx, (TFCVT)v); }
//     bool set(const BTFIDX& idx, const unsigned char& v) { return set(idx, (TFCVT)v); }
//     bool set(const BTFIDX& idx, const int& v) { return set(idx, (TFCVT)v); }
//     bool set(const BTFIDX& idx, const unsigned int& v) { return set(idx, (TFCVT)v); }
//     bool set(const BTFIDX& idx, const double& v) { return set(idx, (TFCVT)v); }
//     bool set(const BTFIDX& idx, const TypeFloatBasePtr& v) { return set(idx, *v); }
//     bool set(const BTFIDX& idx, const TFCVT& v) { TypeFloatBase v_tf = v; return set(idx, v_tf); }

//     // Block management
//     void blocking();
//     void itHasMultCarry() { hasMultCarry = true; }

//     // Accessors
//     TFCVT getValue(const BTFIDX& idx);
//     TypeFloatBasePtr getTypeFloat(const BTFIDX& idx);
//     inline bool getSign(const BTFIDX& idx) { return (arrMants[idx] >> (NBIT_TFDATA - 1U)) & maskBit; }
//     inline TFDATA getMant(const BTFIDX& idx) { return arrMants[idx] & getMaskMant(); }
//     inline TFEXP getExpBiased(const BTFIDX& idx) { return isBlocked ? static_cast<TFEXP>(data) : arrExps[idx] + biasExp; }
//     inline TFEXP getExp(const BTFIDX& idx) { return isBlocked ? static_cast<TFEXP>(data) - biasExp : arrExps[idx]; }
//     TFDATA getArrMants(const BTFIDX& idx) { return arrMants[idx]; }
//     TFEXP getArrExps(const BTFIDX& idx) { return arrExps[idx]; }
//     void setBlockNan() { data = getMaskExp(); arrMants[0] = getMantBlockNan(); isBlocked = true; hasMultCarry = false; }
//     void setBlockInf() { data = getMaskExp(); arrMants[0] = getMantBlockInf(); isBlocked = true; hasMultCarry = false; }
//     bool isInf() { return data == getMaskExp() && arrMants[0] == getMantBlockInf(); }
//     bool isNan() { return data == getMaskExp() && arrMants[0] == getMantBlockNan(); }
//     void setArrMants(const BTFIDX& idx, const TFDATA& mant) { arrMants[idx] = mant; }
//     void setArrExps(const BTFIDX& idx, const TFEXP& exp) { arrExps[idx] = exp; }
//     void setIsBlocked(const bool b) { isBlocked = b; }

// protected:
//     inline TFMASK getMaskMant() const { return hasMultCarry ? (1U << (nbMant + 1U)) - 1U : (1U << (nbMant)) - 1U; }
//     inline TFDATA getMantBlockNan() const { return (1U << (nbMant - 1)); }
//     inline TFDATA getMantBlockInf() const { return 0U; }

//     /**
//      * @brief It means whether floating-point values in BlockTypeFloatBase is sharing an exponent value or not.
//      * If isBlocked = 0, pData contains raw data of TypeFloat values and data doesn't represent anything.
//      * In this case, configurations of the TypeFloat values should be same each other, and also same with
//      * BlockTypeFloatBase If isBlocked = 1, pData contains mantissa of TypeFloat values aligned with the maximum
//      * exponent, which is stored in data. Blocking is done automatically when arithmetic operator or assignment is
//      * executed.
//      */
//     bool isBlocked;
//     bool hasMultCarry;
//     /// @brief A mentissa array
//     std::vector<TFDATA> arrMants;
//     /// @brief A exponent array
//     std::vector<TFEXP> arrExps;
// };

// using BlockTypeFloatBasePtr = std::shared_ptr<BlockTypeFloatBase>;
// class BlockTypeFloat : public BlockTypeFloatBase {
// public:
//     BlockTypeFloat(uint32_t _n) : BlockTypeFloatBase(_n), doDump(false), arrDump() {}
//     BlockTypeFloat(uint32_t _n, int _nbMant, int _nbExp, int _biasExp)
//         : BlockTypeFloatBase(_n, _nbMant, _nbExp, _biasExp), doDump(false), arrDump() { checkAsserts(); }
//     BlockTypeFloat(uint32_t _n, bool _isSigned, int _nbMant, int _nbExp, int _biasExp)
//         : BlockTypeFloatBase(_n, _isSigned, _nbMant, _nbExp, _biasExp), doDump(false), arrDump() {}
//     BlockTypeFloat(BlockTypeFloatBasePtr vs) : BlockTypeFloatBase(*vs), doDump(false), arrDump() { operator=(vs); checkAsserts(); }
//     BlockTypeFloat(uint32_t _n, int _nbMant, int _nbExp, int _biasExp, bool _doDump)
//         : BlockTypeFloatBase(_n, _nbMant, _nbExp, _biasExp), doDump(_doDump), arrDump(_n) { checkAsserts(); }

//     /// @brief Assignment operator
//     const BlockTypeFloatBase& operator=(BlockTypeFloatBasePtr vs) { BlockTypeFloatBase::operator=(*vs); vs.reset(); return *this; }

//     /// @brief Arithmetic operators
//     // BlockTypeFloatBasePtr operator+(BlockTypeFloat& vs);
//     BlockTypeFloatBasePtr operator*(BlockTypeFloat& vs) { return blockedElementWiseMult(vs, true); }
//     BlockTypeFloatBasePtr blockedElementWiseMult(BlockTypeFloat& vs) { return blockedElementWiseMult(vs, false); }
//     BlockTypeFloatBasePtr nonBlockedElementWiseMult(BlockTypeFloat& vs) { return nonBlockedElementWiseMult(vs, false); }
//     TypeFloatBasePtr reduceSum();
//     TypeFloatBasePtr macTree(BlockTypeFloat& vs);

//     /// @brief Accessor for Dump feature
//     TFDATAL getArrDump(const BTFIDX& idx) { return doDump ? arrDump[idx] : 0LLU; }

// private:
//     BlockTypeFloatBasePtr blockedElementWiseMult(BlockTypeFloat& vs, bool doCarryBitShift);
//     BlockTypeFloatBasePtr nonBlockedElementWiseMult(BlockTypeFloat& vs, bool doCarryBitShift);

//     /// @brief Dump feature
//     bool doDump;
//     std::vector<TFDATAL> arrDump;

//     /// @brief Checks basic assertions for BlockTypeFloat
//     void checkAsserts() {
//         assert(static_cast<TFBITS>(arrMants.size()) > 0);
//         assert((nbMant + static_cast<TFBITS>(isSigned) + /*mult_carry_bit*/1) <= NBIT_TFDATA);
//         assert(nbExp <= NBIT_TFDATA);
//         assert(nbMant > 0);
//         assert(nbExp > 0);
//     }
// };

// /// @brief These functions acquire raw-data of float(v) and double(v)
// inline TFDATA getFloatData(const float& v) { return *((TFDATA*)&v); }
// inline bool getFloatSign(const float& v) { return (getFloatData(v) >> 31U) & 1U; }
// inline TFDATA getFloatMant(const float& v) { return getFloatData(v) & ((1U << 23U) - 1U); }
// inline TFEXP getFloatExpBiased(const float& v) { return static_cast<TFEXP>((getFloatData(v) >> 23) & ((1 << 8) - 1)); }
// inline TFEXP getFloatExp(const float& v) { return getFloatExpBiased(v) - 127; }
// inline TFDATAL getDoubleData(const double& v) { return *((TFDATAL*)&v); }
// inline bool getDoubleSign(const double& v) { return (getDoubleData(v) >> 63LLU) & 1LLU; }
// inline TFDATAL getDoubleMant(const double& v) { return getDoubleData(v) & ((1LLU << 52LLU) - 1LLU); }
// inline TFEXP getDoubleExpBiased(const double& v) { return static_cast<TFEXP>((getDoubleData(v) >> 52) & ((1 << 11) - 1)); }
// inline TFEXP getDoubleExp(const float& v) { return getDoubleExpBiased(v) - 1023; }
// inline TFDATA getData2sComp(const bool s, const TFDATA v) { return s ? ((~v) + 1U) : v; }
// TFBITS getBitWidth(const TFBITS nb);

}  // namespace numsys_hw