#include "TypeFloat.h"

#include <algorithm>
#include <bitset>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <limits>

namespace numsys_hw {

// TypeFloatBase Assignments
const TypeFloatBase& TypeFloatBase::operator=(bool v) {
    TFCVT t = static_cast<TFCVT>(v);
    operator=(t);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(char v) {
    TFCVT t = static_cast<TFCVT>(v);
    operator=(t);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(unsigned char v) {
    TFCVT t = static_cast<TFCVT>(v);
    operator=(t);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(int v) {
    TFCVT t = static_cast<TFCVT>(v);
    operator=(t);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(unsigned int v) {
    TFCVT t = static_cast<TFCVT>(v);
    operator=(t);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(double v) {
    TFCVT t = static_cast<TFCVT>(v);
    operator=(t);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(float v) {
    TypeFloatBase v_tf;
    v_tf.data = numsys_hw::getFloatData(v);
    data = buildData(v_tf, false);
    return *this;
}

const TypeFloatBase& TypeFloatBase::operator=(const TypeFloatBase& v) {
    data = buildData(v, false);
    return *this;
}

TFDATA TypeFloatBase::buildData(const TypeFloatBase& v, bool rounding) const {
    TFDATA ret;
    if (hasSameConfig(v)) {
        ret = v.data;
    } else {
        // 1. Build masks for this
        const TFMASK maskThisExp = this->getMaskExp();

        // 2. Get {Sign, Mant, ExpBiased}, and masks of v
        const TFMASK maskVExp = v.getMaskExp();
        const TFDATA vSign = v.getSign();
        const TFDATA vMant = v.getMant();
        const TFEXP vExpBiased = v.getExpBiased();

        // 3. Receive the raw-data
        // 3.1. v is 0
        if (vExpBiased == 0 && vMant == 0) {
            ret = getData(vSign, 0U, 0U);
        } else if (vExpBiased == static_cast<TFEXP>(maskVExp)) {
            TFDATA thisMant;
            // 3.2. v is Inf
            if (vMant == 0) {
                thisMant = 0U;
            }
            // 3.3. v is NaN
            else {
                const bool msb = (vMant >> (v.nbMant - 1)) & maskBit;
                const bool sticky = (vMant & ((1U << (v.nbMant - 1)) - 1U)) != 0;
                /*qNaN*/ if (msb && !sticky) {
                    thisMant = this->getMantQuietNan();
                }
                /*sNaN*/ else if (!msb && sticky) {
                    thisMant = this->getMantSignalingNan();
                }
                /*IndNaN*/ else {
                    thisMant = this->getMantIndefiniteNan();
                }
            }
            ret = getData(vSign, static_cast<TFEXP>(maskThisExp), thisMant);
        }
        // 3.4. Others
        else {
            TFEXP lshMant = vExpBiased == 0 ? getZeroLengthOfMant(vMant, v.nbMant) : 0;
            TFDATA thisMant = vMant << lshMant;
            TFDATA rshMant = 0;
            TFEXP thisExpBiased = (vExpBiased - v.biasExp) - lshMant + biasExp;

            // 4. Apply nbMant modification
            if (this->nbMant < v.nbMant) {
                rshMant = v.nbMant - this->nbMant;
            } else if (this->nbMant > v.nbMant) {
                thisMant = thisMant << (this->nbMant - v.nbMant);
            }

            // 5. Manage Overflow/Underflow and Mantissa reduction
            // 5.1 Because Underflow occurs, Denormal expression is required
            if (thisExpBiased <= 0) {
                if (vExpBiased != 0) {
                    rshMant += (1 - thisExpBiased);
                    thisMant |= v.getOne();
                } else {
                    rshMant += (-thisExpBiased);
                }
                thisExpBiased = 0;
            }
            // 5.2 Because Overflow occurs, Inf expression is required
            else if (thisExpBiased >= static_cast<TFEXP>(maskThisExp)) {
                thisMant = 0U;
                thisExpBiased = static_cast<TFEXP>(maskThisExp);
            }
            thisMant = rounding ? roundHalfToEven(thisMant, rshMant) : thisMant >> rshMant;
            ret = getData(vSign, thisExpBiased, thisMant);
        }
    }
    return ret;
}

// TypeFloatBase Accessors
TFCVT TypeFloatBase::getValue() const {
    const TFMASK maskExp = getMaskExp();
    const TFDATA thisSign = getSign();
    const TFDATA thisMant = getMant();
    const TFEXP thisExpBiased = getExpBiased();
    // 1. this is 0
    if (thisMant == 0 && thisExpBiased == 0) {
        return thisSign ? -0.f : 0.f;
    } else if (thisExpBiased == static_cast<TFEXP>(maskExp)) {
        // 2. this is Inf
        if (thisMant == 0) {
            return thisSign ? -std::numeric_limits<TFCVT>::infinity() : std::numeric_limits<TFCVT>::infinity();
        }
        // 3. this is NaN
        else if (thisMant == getMantQuietNan()) {
            return thisSign ? -std::numeric_limits<TFCVT>::quiet_NaN() : std::numeric_limits<TFCVT>::quiet_NaN();
        } else {
            return thisSign ? -std::numeric_limits<TFCVT>::signaling_NaN()
                            : std::numeric_limits<TFCVT>::signaling_NaN();
        }
    }
    // 4. this is finite value
    else {
        return (thisSign ? -1.f : 1.f) *
               ((thisExpBiased ? 1.f : 0.f) +
                static_cast<TFCVT>(thisMant) / std::pow(2.f, static_cast<TFCVT>(nbMant))) *
               std::pow(2.f, static_cast<TFCVT>((thisExpBiased ? thisExpBiased : 1) - biasExp));
    }
}

bool TypeFloatBase::hasSameConfig(const TypeFloatBase& v) const {
    return (this->isSigned == v.isSigned) && (this->nbMant == v.nbMant) && (this->nbExp == v.nbExp) &&
           (this->biasExp == v.biasExp);
}

TFEXP TypeFloatBase::getZeroLengthOfMant(const TFDATA& m, const TFBITS& nbM) const {
    for (TFEXP it = 0; it < nbM; it++) {
        if ((m >> (nbM - 1 - it)) & maskBit) {
            return it;
        }
    }
    return static_cast<TFEXP>(nbM);
}

TFEXP TypeFloatBase::getZeroLengthOfMant(const TFDATAL& m, const TFBITS& nbM) const {
    for (TFEXP it = 0; it < nbM; it++) {
        if ((m >> (nbM - 1 - it)) & maskBit) {
            return it;
        }
    }
    return static_cast<TFEXP>(nbM);
}

// TypeFloatBase Rounding rules
TFDATA TypeFloatBase::roundHalfToEven(const TFDATA& m, const TFDATA& rsh) const {
    if (rsh == 0) {
        return m;
    }
    TFDATA lsbInte = (m >> rsh) & 1U;
    TFDATA msbFrac = (m >> (rsh - 1)) & 1U;
    TFDATA sticky = m & ((1U << (rsh - 1)) - 1U);
    return (lsbInte || sticky) && msbFrac ? roundHalfToNearest(m, rsh) : (m >> rsh);
}

TFDATAL TypeFloatBase::roundHalfToEven(const TFDATAL& m, const TFDATA& rsh) const {
    if (rsh == 0) {
        return m;
    }
    TFDATAL lsbInte = (m >> rsh) & 1U;
    TFDATAL msbFrac = (m >> (rsh - 1)) & 1U;
    TFDATAL sticky = m & ((1U << (rsh - 1)) - 1U);
    return (lsbInte || sticky) && msbFrac ? roundHalfToNearest(m, rsh) : (m >> rsh);
}

// TypeFloat Assignments
const TypeFloatBase& TypeFloat::operator=(TypeFloatBasePtr v) {
    TypeFloatBase::operator=(*v);
    v.reset();
    return *this;
}

const TypeFloatBase& TypeFloat::assignRounded(TypeFloatBasePtr v) {
    data = TypeFloatBase::buildData(*v, true);
    v.reset();
    return *this;
}

// TypeFloat Arithmetic operations
TypeFloatBasePtr TypeFloat::operator+(const TypeFloat& v) {
    // 1. Decide bit-precision of result
    const TFBITS retNbMant = this->nbMant > v.nbMant ? this->nbMant : v.nbMant;
    const TFBITS retNbExp = this->nbExp > v.nbExp ? this->nbExp : v.nbExp;
    const TFMASK retMaskExp = (1 << retNbExp) - 1;
    // TODO: Need to check retNbExp & retBiasExp covering the range of this & v
    const TFEXP retBiasExp = (1 << (retNbExp - 1)) - 1;
    TypeFloatBasePtr ret = std::make_shared<TypeFloatBase>(retNbMant, retNbExp, retBiasExp);

    // 2. Check special cases (Zero, Inf, and Nan)
    // 2.1 Either this or v is zero
    TFDATA thisSign = this->getSign();
    TFDATA vSign = v.getSign();
    if (this->isZero()) {
        if (v.isSignalingNan()) {
            ret->setIndefiniteNan(vSign);
        } else {
            *ret = v;
        }
        return ret;
    } else if (v.isZero()) {
        if (this->isSignalingNan()) {
            ret->setIndefiniteNan(thisSign);
        } else {
            *ret = *this;
        }
        return ret;
    }
    // 2.2 Either this or v is inf
    else if (this->isInf()) {
        if (v.isInf()) {
            if (thisSign == vSign) {
                ret->setInf(thisSign);
            } else {
                ret->setNegQuietNan();
            }
            return ret;
        } else {
            ret->setInf(thisSign);
            return ret;
        }
    } else if (v.isInf()) {
        ret->setInf(vSign);
        return ret;
    }
    // 2.3 Either this or v is NaN
    else if (this->isNan() || v.isNan()) {
        if (this->isSignalingNan() || v.isSignalingNan()) {
            ret->setPosSignalingNan();
        } else {
            ret->setPosQuietNan();
        }
        return ret;
    }

    // 3. Compute ADD (Normal and Denormal)
    const TFBITS nbTiny = 3;
    const TFDATA retOne = 1U << (retNbMant + nbTiny);
    TFEXP thisExp = this->getExp();
    TFEXP vExp = v.getExp();
    TFDATA thisMant = this->getMant() << (retNbMant - this->nbMant + nbTiny);
    TFDATA vMant = v.getMant() << (retNbMant - v.nbMant + nbTiny);
    // 3.1. Consider Denormal expression
    if (this->getExpBiased() == 0U) {
        thisExp += 1;
    } else {
        thisMant += retOne;
    }
    if (v.getExpBiased() == 0U) {
        vExp += 1;
    } else {
        vMant += retOne;
    }
    // 3.2. Find bigger
    bool thisIsBig;
    if (thisExp > vExp) {
        TFEXP diffExp = thisExp - vExp;
        TFDATA sticky = (vMant & ((1U << (diffExp + 1)) - 1U)) != 0U;
        thisIsBig = true;
        vMant = (retNbMant + nbTiny + 1) >= diffExp ? (((vMant >> diffExp) & (~1U)) | sticky) : 0U;
    } else if (vExp > thisExp) {
        TFEXP diffExp = vExp - thisExp;
        TFDATA sticky = (thisMant & ((1U << (diffExp + 1)) - 1U)) != 0U;
        thisIsBig = false;
        thisMant = (retNbMant + nbTiny + 1) >= diffExp ? (((thisMant >> diffExp) & (~1U)) | sticky) : 0U;
    } else if (thisMant >= vMant) {
        thisIsBig = true;
    } else {
        thisIsBig = false;
    }
    // 3.3. Compute ADD
    bool xorSign = thisSign ^ vSign;
    TFDATA retSign = thisIsBig ? thisSign : vSign;
    TFDATA retMant = xorSign ? (thisIsBig ? (thisMant - vMant) : (vMant - thisMant)) : (thisMant + vMant);
    TFEXP retExp = thisIsBig ? thisExp : vExp;
    // 4. Manage Round and Overflow/Underflow
    // 4.1. Get Zero-Length
    TFEXP lshMant = getZeroLengthOfMant(retMant, retNbMant + nbTiny + 2 /*carry-bit + one-bit*/) - 1 /*carry-bit*/;
    TFEXP retExpBiased = retExp - lshMant + retBiasExp;
    // 4.2. Manage Overflow/Underflow
    // 4.3.1 Because underflow occurs, Denormal expression is required
    if (retExpBiased <= 0) {
        lshMant = 0;
        retExpBiased = 0;
    }
    // 4.3.2. Because overflow occurs, Inf expression is required
    else if (retExpBiased >= static_cast<TFEXP>(retMaskExp)) {
        retMant = 0U;
        lshMant = 0;
        retExpBiased = static_cast<TFEXP>(retMaskExp);
    }
    // 4. Round
    lshMant -= nbTiny;
    retMant = lshMant < 0 ? roundHalfToEven(retMant, -lshMant) : retMant << lshMant;
    retExpBiased = retMant == 0 ? 0 : retExpBiased;
    // 5. Return the result
    ret->setData(ret->getData(retSign, retExpBiased, retMant));
    return ret;
}

TypeFloatBasePtr TypeFloat::operator-(const TypeFloat& v) {
    TypeFloat _v(v.nbMant, v.nbExp, v.biasExp);
    _v.data = v.data ^ (1U << (v.nbMant + v.nbExp));
    return operator+(_v);
}

TypeFloatBasePtr TypeFloat::operator*(const TypeFloat& v) {
    // 1. Decide bit-precision of result
    const TFBITS newNbMant = this->nbMant > v.nbMant ? this->nbMant : v.nbMant;
    const TFBITS newNbExp = this->nbExp > v.nbExp ? this->nbExp : v.nbExp;
    const TFMASK newMaskMant = (1 << newNbMant) - 1;
    const TFMASK newMaskExp = (1 << newNbExp) - 1;
    const TFEXP newBiasExp = (1 << (newNbExp - 1)) - 1;
    TypeFloatBasePtr ret = std::make_shared<TypeFloatBase>(newNbMant, newNbExp, newBiasExp);

    // 1.1. Get this value
    TFDATA vThisMant = this->data & ((1 << this->nbMant) - 1);
    const TFDATA vThisOne = 1 << this->nbMant;
    TFEXP vThisExp = ((this->data >> this->nbMant) & ((1 << this->nbExp) - 1)) - this->biasExp;
    const TFDATA sThis = (this->data >> (this->nbMant + this->nbExp)) & 0x1;

    // 1.1.1. Check whether this is special value or not
    bool thisIsZero = this->isZero();
    bool thisIsInf = this->isInf();
    bool thisIsQNaN = this->isQuietNan();
    bool thisIsSNaN = this->isSignalingNan();
    bool thisIsDenor = this->isDenormal();

    // 1.2. Get v value
    TFDATA vVMant = v.data & ((1 << v.nbMant) - 1);
    const TFDATA vVOne = 1 << v.nbMant;
    TFEXP vVExp = ((v.data >> v.nbMant) & ((1 << v.nbExp) - 1)) - v.biasExp;
    const TFDATA sV = (v.data >> (v.nbMant + v.nbExp)) & 0x1;

    // 1.2.1. Check whether v is special value or not
    bool vIsZero = v.isZero();
    bool vIsInf = v.isInf();
    bool vIsQNaN = v.isQuietNan();
    bool vIsSNaN = v.isSignalingNan();
    bool vIsDenor = v.isDenormal();

    // 2. Decide sign_bit of result
    TFDATA retSign = sThis ^ sV ? 1U : 0U;

    // 3. Spacial Case (NaN, infinity, zero, Denormal Number)
    // 3.1. if either data or v is NaN, NaN is returned
    if (thisIsQNaN || thisIsSNaN) {
        if (vIsQNaN || vIsSNaN) {
            ret->setQuietNan(sV);
            return ret;
        } else {
            ret->setQuietNan(sThis);
            return ret;
        }

        // 3.2. When either data or v is infinity, if value that is not infinity is zero, NaN is returned
    } else if (thisIsInf || vIsInf) {
        if (vIsZero || thisIsZero) {
            ret->setNegQuietNan();
            return ret;
        } else {
            ret->setInf(retSign);
            return ret;
        }

        // 3.3. if either data or v is zero, zero is returned
    } else if (thisIsZero || vIsZero) {
        ret->setZero(retSign);
        return ret;
    }

    // 3.4. if either data or v is denormalised Number, it should be normalized. And these value are used for operation.
    TFEXP lshMantThis, lshMantV;
    if (thisIsDenor) {
        lshMantThis = getZeroLengthOfMant(vThisMant, newNbMant) + 1;
        vThisMant = vThisMant << lshMantThis;
        vThisExp += 1;
        vThisExp -= lshMantThis;
    } else if (vIsDenor) {
        lshMantV = getZeroLengthOfMant(vVMant, newNbMant) + 1;
        vVMant = vVMant << lshMantV;
        vVExp += 1;
        vVExp -= lshMantV;
    }

    // 4. Compute Multiply (between Normalize Values)
    TFDATAL retMant = static_cast<TFDATAL>(vThisMant | vThisOne) *
                      static_cast<TFDATAL>(vVMant | vVOne);  // this->nbMant + v.nbMant + 2 bits
    TFEXP retExp = vThisExp + vVExp + 1;

    // 5. Manage Round and Overflow/Underflow
    // 5.1. Get Zero-Length(=lshMant), it should be 0 or 1
    TFEXP lshMantret = getZeroLengthOfMant(retMant, newNbMant + newNbMant + 2);
    TFEXP retExpBiased = retExp - lshMantret + newBiasExp;

    // 5.2. Manage Overflow/Underflow
    // 5.2.1 Because Overflow occurs, Inf expression is required
    bool isOverflow, isUnderflow;
    isOverflow = isUnderflow = false;
    if(retExpBiased >= static_cast<TFEXP>(newMaskExp)) {
        isOverflow = true;
        ret->setInf(retSign);
        return ret;
        // 5.2.2 Because Underflow occurs, Denormal expression is required
    } else if (retExpBiased <= 0) {
        lshMantret -= retExpBiased;
        retExpBiased = 0;
        isUnderflow = true;
    }
    // 6. Round
    retMant = (lshMantret >= 0 && (isUnderflow == false)) ? roundHalfToEven(retMant, newNbMant + 1 - lshMantret) : roundHalfToEven(retMant, newNbMant + lshMantret);

    // 7. Return the result
    ret->setData(ret->getData(retSign, retExpBiased, retMant));
    return ret;
}

double TypeFloat::operator/(TypeFloat& v) {
    const int newNbMant = this->nbMant > v.nbMant ? this->nbMant : v.nbMant;
    const int newNbExp = this->nbExp > v.nbExp ? this->nbExp : v.nbExp;
    const int newMaskMant = (1 << newNbMant) - 1;
    const int newMaskExp = (1 << newNbExp) - 1;
    const int newBiasExp = 1 << (newNbExp - 1);
    const int vNewOne = 1 << newNbMant;
    // get this value
    int vThisMant = this->data & ((1 << this->nbMant) - 1);
    const int vThisOne = 1 << this->nbMant;
    const int vThisExp = ((this->data >> this->nbMant) & ((1 << this->nbExp) - 1)) - this->biasExp;
    const int sThis = (this->data >> (this->nbMant + this->nbExp)) & 0x1;
    const int thisIsZero = vThisMant == 0 && vThisExp == 0;
    if (thisIsZero) return 0.0;
    // get v value
    int vVMant = v.data & ((1 << v.nbMant) - 1);
    const int vVOne = 1 << v.nbMant;
    const int vVExp = ((v.data >> v.nbMant) & ((1 << v.nbExp) - 1)) - v.biasExp;
    const int sV = (v.data >> (v.nbMant + v.nbExp)) & 0x1;
    // const int vIsZero = vVMant == 0 && vVExp == 0;

    // Compute (Div.)
    double sign = sThis ^ sV ? -1.0 : 1.0;
    int vMant, vExp;
    if (this->nbMant > v.nbMant) {
        int d = (int)((((long long)vThisMant + vThisOne) << (newNbMant + 1)) /
                      (((long long)vVMant + vVOne) << (this->nbMant - v.nbMant)));
        bool needShift = d < (vNewOne << 1);
        vMant = needShift ? d - vNewOne : (d >> 1) + (d & 0x1) - vNewOne;
        vExp = vThisExp - vVExp - needShift;
    } else {
        int d = (int)((((long long)vThisMant + vThisOne) << (v.nbMant - this->nbMant + newNbMant + 1)) /
                      (((long long)vVMant + vVOne)));
        bool needShift = d < (vNewOne << 1);
        vMant = needShift ? d - vNewOne : (d >> 1) + (d & 0x1) - vNewOne;
        vExp = vThisExp - vVExp - needShift;
    }

    vMant = vMant & newMaskMant;
    vExp = vExp + newBiasExp;
    vExp = (vExp > newMaskExp ? newMaskExp : (vExp < 0 ? 0 : vExp)) - newBiasExp;
    return sign * (1.0 + (double)vMant / std::pow(2.0, newNbMant)) * std::pow(2.0, (double)vExp);
}

// TypeFloat Comparing operations
void TypeFloat::operator>>(uint32_t rsh) {
    const bool isZero = !(data & ((1 << (nbMant + nbExp)) - 1));
    if (isZero) return;
    const int maskMant = (1 << nbMant) - 1;
    const int maskExp = (1 << nbExp) - 1;
    const int vMant = data & maskMant;
    int vExp = (data >> nbMant) & maskExp;
    const int vSign = (data >> (nbMant + nbExp)) & 0x1;

    // opeartor
    vExp = vExp - rsh;
    // Assign
    data = (vSign << (nbMant + nbExp)) | ((vExp & maskExp) << nbMant) | vMant;
}

bool TypeFloat::operator==(const TypeFloat& v) {
    // get this value
    int vThisMant = this->data & ((1 << this->nbMant) - 1);
    const int vThisExp = ((this->data >> this->nbMant) & ((1 << this->nbExp) - 1)) - this->biasExp;
    // const int sThis = (this->data >> (this->nbMant + this->nbExp)) & 0x1;
    const int thisIsZero = vThisMant == 0 && vThisExp == 0;

    // get v value
    int vVMant = v.data & ((1 << v.nbMant) - 1);
    const int vVExp = ((v.data >> v.nbMant) & ((1 << v.nbExp) - 1)) - v.biasExp;
    // const int sV = (v.data >> (v.nbMant + v.nbExp)) & 0x1;
    const int vIsZero = vVMant == 0 && vVExp == 0;

    // deicision
    if (thisIsZero && vIsZero)
        return true;
    else if (thisIsZero ^ vIsZero)
        return false;
    else if (vThisExp == vVExp) {
        if (this->nbMant > v.nbMant)
            vVMant = vVMant << (this->nbMant - v.nbMant);
        else
            vThisMant = vThisMant << (v.nbMant - this->nbMant);
        if (vThisMant == vVMant)
            return true;
        else
            return false;
    } else
        return false;
}

bool TypeFloat::operator!=(const TypeFloat& v) { return !operator==(v); }

bool TypeFloat::operator>(const TypeFloat& v) {
    // get this value
    int vThisMant = this->data & ((1 << this->nbMant) - 1);
    const int vThisExp = ((this->data >> this->nbMant) & ((1 << this->nbExp) - 1)) - this->biasExp;
    const int vThisSign = (this->data >> (this->nbMant + this->nbExp)) & 0x1;
    const int thisIsZero = vThisMant == 0 && vThisExp == 0;
    const int vThisOne = 1 << this->nbMant;

    // get v value
    int vVMant = v.data & ((1 << v.nbMant) - 1);
    const int vVExp = ((v.data >> v.nbMant) & ((1 << v.nbExp) - 1)) - v.biasExp;
    const int vVSign = (v.data >> (v.nbMant + v.nbExp)) & 0x1;
    const int vIsZero = vVMant == 0 && vVExp == 0;
    const int vVOne = 1 << v.nbMant;

    // deicision
    if (vVSign && (!vThisSign || thisIsZero))
        return true;
    else if (vThisSign) /*vThisSIgn && !vVSign*/
        return false;
    else if (thisIsZero && (!vVSign || vIsZero))
        return false;
    else if (vThisExp != vVExp)
        return vThisExp > vVExp;
    else {
        vVMant = (vVMant + vVOne) << (this->nbMant > v.nbMant ? this->nbMant - v.nbMant : 0);
        vThisMant = (vThisMant + vThisOne) << (v.nbMant > this->nbMant ? v.nbMant - this->nbMant : 0);
        if (vThisMant > vVMant)
            return true;
        else
            return false;
    }
}

bool TypeFloat::operator<(const TypeFloat& v) {
    // get this value
    int vThisMant = this->data & ((1 << this->nbMant) - 1);
    const int vThisExp = ((this->data >> this->nbMant) & ((1 << this->nbExp) - 1)) - this->biasExp;
    const int vThisSign = (this->data >> (this->nbMant + this->nbExp)) & 0x1;
    const int thisIsZero = vThisMant == 0 && vThisExp == 0;
    const int vThisOne = 1 << this->nbMant;

    // get v value
    int vVMant = v.data & ((1 << v.nbMant) - 1);
    const int vVExp = ((v.data >> v.nbMant) & ((1 << v.nbExp) - 1)) - v.biasExp;
    const int vVSign = (v.data >> (v.nbMant + v.nbExp)) & 0x1;
    const int vIsZero = vVMant == 0 && vVExp == 0;
    const int vVOne = 1 << v.nbMant;

    // deicision
    if (vThisSign && (!vVSign || vIsZero))
        return true;
    else if (vVSign) /*vThisSIgn && !vVSign*/
        return false;
    else if (vIsZero && (!vThisSign || thisIsZero))
        return false;
    else if (vThisExp != vVExp)
        return vThisExp < vVExp;
    else {
        vVMant = (vVMant + vVOne) << (this->nbMant > v.nbMant ? this->nbMant - v.nbMant : 0);
        vThisMant = (vThisMant + vThisOne) << (v.nbMant > this->nbMant ? v.nbMant - this->nbMant : 0);
        if (vThisMant < vVMant)
            return true;
        else
            return false;
    }
}

bool TypeFloat::operator>=(const TypeFloat& v) { return !operator<(v); }

bool TypeFloat::operator<=(const TypeFloat& v) { return !operator>(v); }

// BlockTypeFloatBase Assignments
const BlockTypeFloatBase& BlockTypeFloatBase::operator=(BlockTypeFloatBase& vs) {
    if (this->arrMants.size() == vs.arrMants.size()) {
        this->isBlocked = vs.isBlocked;
        this->hasMultCarry = vs.hasMultCarry;
        if (hasSameConfig(vs)) {
            this->arrMants = vs.arrMants;
            this->arrExps = vs.arrExps;
            this->data = vs.data;
        } else {
            const int szBlock = static_cast<int>(arrMants.size());
            const TFMASK maskVsMant = vs.getMaskMant();
            const TFMASK maskVsExp = vs.getMaskExp();
            const TFMASK maskThisExp = this->getMaskExp();
            const int diffNbMant = this->nbMant - vs.nbMant;
            for (int it = 0; it < szBlock; it++) {
                TFDATA mant = vs.arrMants[it] & maskVsMant;
                TFDATA sign = (vs.arrMants[it] >> (NBIT_TFDATA - 1)) & maskBit;
                this->arrMants[it] = (diffNbMant >= 0 ? (mant << diffNbMant) : mant >> (-diffNbMant)) | (sign << (NBIT_TFDATA - 1));
            }
            if(isBlocked) {
                TFEXP exp = vs.data - vs.biasExp + this->biasExp;
                this->data = exp < 0 ? 0 : (exp > maskThisExp ? maskThisExp : exp);
            }
            else {
                this->arrExps = vs.arrExps;
            }
        }
    }
    return *this;
}

bool BlockTypeFloatBase::set(const BTFIDX& idx, const TypeFloatBase& v) {
    if (idx >= static_cast<BTFIDX>(arrMants.size())) {
        return false;
    }
    // 1. Check special values
    if(v.isInf()) {
        this->setBlockInf();
        this->arrMants[idx] |= v.getSign() << (NBIT_TFDATA - 1);
        return true;
    }
    else if(v.isNan()) {
        this->setBlockNan();
        return true;
    }
    else if (this->isBlocked) {
        return false;
    }

    // 2. Assignment
    TFBITS vNbMant = v.getNbMant();
    TFDATA vMant = v.getMant();
    TFDATA vSign = v.getSign();
    TFEXP vExpBiased = v.getExpBiased();
    int diff = this->nbMant - (vNbMant + /*one*/1);
    if(v.isDenormal()) {
        TFEXP lenLeadZeros = this->getZeroLengthOfMant(vMant, vNbMant + /*one*/1);
        vExpBiased -= (lenLeadZeros - 1);
        diff += lenLeadZeros;
    }
    else if(vExpBiased != 0 || vMant != 0) {
        vMant |= (1U << vNbMant);
    }
    vMant = diff >= 0 ? vMant << diff : vMant >> (-diff);
    TFEXP thisExpBiased = vExpBiased - v.getBiasExp() + this->biasExp;
    if(thisExpBiased >= static_cast<TFEXP>(this->getMaskExp())) {
        this->setBlockInf();
    }
    else {
        this->arrMants[idx] = vMant | (vSign << (NBIT_TFDATA - 1));
        this->arrExps[idx] = (vMant == 0 ? 0 : thisExpBiased) - this->biasExp;
    }
    return true;
}

// BlockTypeFloatBase Accessors
TFCVT BlockTypeFloatBase::getValue(const BTFIDX& idx) {
    if (!isBlocked) {
        blocking();
    }
    const TFMASK thisMaskMant = this->getMaskMant();
    const TFDATA thisSign = (arrMants[idx] >> (NBIT_TFDATA-1)) & maskBit;
    const TFEXP thisExpBiased = data;
    if (thisExpBiased == this->getMaskExp()) {
        const TFDATA thisMant0 = arrMants[0] & thisMaskMant;
        if (thisMant0 == getMantBlockInf()) {
            return thisSign ? -std::numeric_limits<TFCVT>::infinity() : std::numeric_limits<TFCVT>::infinity();
        } else {
            return thisSign ? -std::numeric_limits<TFCVT>::quiet_NaN() : std::numeric_limits<TFCVT>::quiet_NaN();
        }
    } else {
        const TFDATA thisMant = arrMants[idx] & thisMaskMant;
        return (thisSign ? -1.f : 1.f) *
               (static_cast<TFCVT>(thisMant) / std::pow(2.f, static_cast<TFCVT>(this->nbMant - 1))) *
               std::pow(2.f, static_cast<TFCVT>((thisExpBiased ? thisExpBiased : 1) - this->biasExp));
    }
}

TypeFloatBasePtr BlockTypeFloatBase::getTypeFloat(const BTFIDX& idx) {
    if (!isBlocked) {
        blocking();
    }
    const TFMASK thisMaskMant = this->getMaskMant();
    TFDATA thisSign = (arrMants[idx] >> (NBIT_TFDATA-1)) & maskBit;
    TFEXP thisExpBiased = data;
    TypeFloatBasePtr ret = std::make_shared<TypeFloatBase>(isSigned, nbMant, nbExp, biasExp);
    if (thisExpBiased == this->getMaskExp()) {
        const TFDATA thisMant0 = arrMants[0] & thisMaskMant;
        if (thisMant0 == getMantBlockInf()) {
            ret->setInf(thisSign);
        } else {
            ret->setQuietNan(thisSign);
        }
        return ret;
    } else {
        TFDATA thisMant = arrMants[idx] & thisMaskMant;
        if (thisMant == 0U) {
            thisExpBiased = 0;
        } else {
            TFEXP lshMant = getZeroLengthOfMant(thisMant, this->nbMant + 1 /*one-bit*/);
            thisExpBiased -= lshMant - 1 /*one-bit*/;
            if (thisExpBiased <= 0) {
                lshMant += thisExpBiased - 1 /*one-bit*/;
                thisExpBiased = 0;
            }
            thisMant = (thisMant << lshMant) & thisMaskMant;
        }
        ret->setData(getData(thisSign, thisExpBiased, thisMant));
        return ret;
    }
}

void BlockTypeFloatBase::blocking() {
    if(isBlocked) {
        return;
    }
    // 1. Find maximum exponent
    TFEXP maxExp = *(std::max_element(arrExps.begin(), arrExps.end()));
    TFEXP maxExpBiased = maxExp + biasExp;

    // 2. Check Overflow
    if (maxExpBiased >= getMaskExp()) {
        setBlockInf();
        return;
    }
    // 3. Alignment
    else {
        const TFMASK maskMant = getMaskMant();
        for (int it = 0; it < static_cast<int>(arrMants.size()); it++) {
            TFDATA mant = arrMants[it] & maskMant;
            TFDATA sign = (arrMants[it] >> (NBIT_TFDATA - 1)) & maskBit;
            TFEXP diffExp = maxExp - arrExps[it];
            arrMants[it] = ((diffExp >= nbMant ? 0U : mant >> diffExp) & maskMant) | (sign << (NBIT_TFDATA - 1));
        }
        data = maxExpBiased < 0 ? 0 : maxExpBiased;
    }
    isBlocked = true;
}

// BlockTypeFloat Arithmetic operators
BlockTypeFloatBasePtr BlockTypeFloat::blockedElementWiseMult(BlockTypeFloat& vs, bool doCarryBitShift) {
    if (this->arrMants.size() != vs.arrMants.size()) {
        return nullptr;
    }
    if (!this->isBlocked) {
        this->blocking();
    }
    if (!vs.isBlocked) {
        vs.blocking();
    }
    // 1. Decide bit-precision of result
    const TFBITS retNbMant = this->nbMant > vs.nbMant ? this->nbMant : vs.nbMant;
    const TFBITS retNbExp = this->nbExp > vs.nbExp ? this->nbExp : vs.nbExp;
    const TFMASK retMaskExp = (1 << retNbExp) - 1;
    // TODO: Need to check retNbExp & retBiasExp covering the range of this & v
    const TFEXP retBiasExp = (1 << (retNbExp - 1)) - 1;
    BlockTypeFloatBasePtr ret = std::make_shared<BlockTypeFloatBase>(static_cast<uint32_t>(this->arrMants.size()),
                                                                     this->isSigned, retNbMant, retNbExp, retBiasExp);
    // 2. Check special values (Inf, NaN)
    if (this->data == static_cast<TFDATA>(this->getMaskExp())) {
        // 2.1. Has Inf
        if (this->getArrMants(0) == 0U) {
            ret->setBlockInf();
        }
        // 2.2 Has Nan
        else {
            ret->setBlockNan();
        }
        ret->setIsBlocked(true);
        return ret;
    } else if (vs.data == static_cast<TFDATA>(vs.getMaskExp())) {
        if (vs.getArrMants(0) == 0U) {
            ret->setBlockInf();
        } else {
            ret->setBlockNan();
        }
        ret->setIsBlocked(true);
        return ret;
    }
    // 3. Element-wise mantissa multiplication
    const TFBITS nbTiny = 2;
    const TFMASK thisMaskMant = this->getMaskMant();
    const TFMASK vMaskMant = vs.getMaskMant();
    const TFBITS rshIntMantMsb = this->nbMant + vs.nbMant - retNbMant - 1 /*carry-bit*/;
    const TFBITS rshIntMant = rshIntMantMsb - nbTiny;
    const TFDATAL retMaskSticky = (1UL << rshIntMant) - 1UL;
    TFEXP retExpBiased = this->getExp(0) + vs.getExp(0) + retBiasExp;
    TFEXP rshMant = nbTiny;
    bool hasCarryBit = false;
    for (int it = 0; it < static_cast<int>(this->arrMants.size()); it++) {
        TFDATAL intMant = static_cast<TFDATAL>(this->arrMants[it] & thisMaskMant) *
                          static_cast<TFDATAL>(vs.getArrMants(it) & vMaskMant);
        TFDATA intSign = (this->arrMants[it] >> this->nbMant) ^ (vs.getArrMants(it) >> vs.nbMant);
        TFDATA sticky = (intMant & retMaskSticky) != 0UL;
        TFDATA retMant = static_cast<TFDATA>(intMant >> rshIntMant);
        ret->setArrMants(it, (intSign << (NBIT_TFDATA - 1)) | (retMant & (~maskBit)) | sticky);
        if (doCarryBitShift && !hasCarryBit && (retMant >> rshIntMantMsb)) {
            rshMant++;
            retExpBiased++;
            hasCarryBit = true;
        }
    }
    // 4. Add exponents & check overflow/underflow
    // 4.1. Because underflow occurs, adjusting mantissa is necessary
    if (retExpBiased <= 0) {
        rshMant -= retExpBiased;
        retExpBiased = 0;
    }
    // 4.2. Because overflow occurs, Inf expression is required
    else if (retExpBiased >= retMaskExp) {
        retExpBiased = retMaskExp;
        ret->setBlockInf();
    }
    // 5. Round
    for (int it = 0; it < static_cast<int>(this->arrMants.size()); it++) {
        TFDATA retData = ret->getArrMants(it);
        TFDATA retSign = retData >> (NBIT_TFDATA - 1);
        ret->setArrMants(it, (retSign << retNbMant) | roundHalfToEven(retData, rshMant));
    }
    ret->setData(static_cast<TFDATA>(retExpBiased));
    ret->setIsBlocked(true);
    return ret;
}

BlockTypeFloatBasePtr BlockTypeFloat::nonBlockedElementWiseMult(BlockTypeFloat& vs, bool doCarryBitShift) {
    if (this->arrMants.size() != vs.arrMants.size()) {
        return nullptr;
    }
    bool thisIsInf = this->isInf();
    bool thisIsNan = this->isNan();
    bool vsIsInf = vs.isInf();
    bool vsIsNan = vs.isNan();
    if ((this->isBlocked && !thisIsInf && !thisIsNan) || (vs.isBlocked && !vsIsInf && !vsIsNan)) {
        return nullptr;
    }
    // 1. Decide bit-precision of result
    const TFBITS retNbMant = this->nbMant > vs.nbMant ? this->nbMant : vs.nbMant;
    const TFBITS retNbExp = this->nbExp > vs.nbExp ? this->nbExp : vs.nbExp;
    const TFMASK retMaskExp = (1 << retNbExp) - 1;
    const TFEXP retBiasExp = (1 << (retNbExp - 1)) - 1;
    const int szBlock = static_cast<int>(this->arrMants.size());
    BlockTypeFloatBasePtr ret = std::make_shared<BlockTypeFloatBase>(szBlock, this->isSigned, retNbMant, retNbExp, retBiasExp);

    // 2. Check special values (Inf, Nan)
    if(thisIsInf || vsIsInf) {
        ret->setBlockInf();
        return ret;
    }
    else if(thisIsNan || vsIsNan) {
        ret->setBlockNan();
        return ret;
    }
    
    // 3. Element-wise multiplication
    const TFMASK thisMaskMant = this->getMaskMant();
    const TFMASK thisMaskExp = this->getMaskExp();
    const TFMASK vsMaskMant = vs.getMaskMant();
    const TFMASK vsMaskExp = vs.getMaskExp();
    const TFBITS rshIntMant = this->nbMant + vs.nbMant - retNbMant - /*carry-bit*/1;
    const TFDATA thisOne = this->getOne();
    const TFDATA vsOne = vs.getOne();
    bool hasCarryBit = false;
    TFEXP rshMant = 0;
    for(int it = 0; it < szBlock; it++) {
        // 3.1. Arrange data
        const TFDATA thisData = this->arrMants[it];
        TFDATA thisMant = this->arrMants[it] & thisMaskMant;
        TFDATA thisSign = thisData >> (NBIT_TFDATA - 1U);
        const TFDATA vsData = vs.arrMants[it];
        TFDATA vsMant = vs.arrMants[it] & vsMaskMant;
        TFDATA vsSign = vsData >> (NBIT_TFDATA - 1U);
        // 3.2. Element-wise multiplication
        TFDATAL intMant = static_cast<TFDATAL>(thisMant) * static_cast<TFDATAL>(vsMant);
        TFDATA intSign = thisSign ^ vsSign;
        TFDATA retMant = static_cast<TFDATA>(intMant >> rshIntMant);
        TFEXP retExp = retMant == 0 ? -retBiasExp : this->arrExps[it] + vs.arrExps[it];
        ret->setArrMants(it, (intSign << (NBIT_TFDATA - 1)) | retMant);
        ret->setArrExps(it, retExp);
        // 3.4. Do CarryBitShift if it is necessary
        if (!hasCarryBit && (retMant >> retNbMant)) {
            hasCarryBit = true;
        }
    }

    // 4. Add exponents & check overflow/underflow
    // 4.1. Underflow is ok
    // 4.2. Because overflow occurs, Inf expression is required
    //TODO: Need to check the overflow cases

    // 5. Considering CarryBit generated by multiplication
    if(doCarryBitShift) {
        for(int it=0; it<szBlock; it++) {
            ret->setArrMants(it, roundHalfToEven(ret->getArrMants(it), 1));
            ret->setArrExps(it, ret->getArrExps(it) + static_cast<TFEXP>(hasCarryBit));
        }
    }
    else if (hasCarryBit) {
        ret->itHasMultCarry();
    }
    return ret;
}

TypeFloatBasePtr BlockTypeFloat::reduceSum() {
    if (!isBlocked) {
        blocking();
    }
    TypeFloatBasePtr ret = std::make_shared<TypeFloatBase>();
    // 1. Check special values (Inf, NaN)
    if (data == getMaskExp()) {
        // 1.1. has Inf
        if (arrMants[0] == 0U) {
            ret->setPosInf();
        }
        // 1.2. has Nan
        else {
            ret->setPosQuietNan();
        }
        return ret;
    }
    // 2. Add all mantissas
    const TFMASK maskMant = this->getMaskMant();
    const TFMASK maskExp = this->getMaskExp();
    int n = static_cast<int>(this->arrMants.size());
    // 2.1. The first stage of adder-tree,
    std::vector<int64_t> adder_tree((n + 1) / 2);
    TFEXP nStages = 1;
    for (int it = 0; it < n; it += 2) {
        TFDATA data0 = this->arrMants[it];
        TFDATA mant0 = data0 & maskMant;
        TFDATA sign0 = data0 >> (NBIT_TFDATA - 1);
        TFDATA data1 = this->arrMants[it + 1];
        TFDATA mant1 = data1 & maskMant;
        TFDATA sign1 = data1 >> (NBIT_TFDATA - 1);
        int64_t a = sign0 ? -static_cast<int64_t>(mant0) : static_cast<int64_t>(mant0);
        int64_t b = sign1 ? -static_cast<int64_t>(mant1) : static_cast<int64_t>(mant1);
        adder_tree[it / 2] = a + ((it + 2 <= n) ? b : 0);
    }
    n = (n + 1) / 2;
    // 2.2 The other stages
    while (n > 1) {
        for (int it = 0; it < n; it += 2) {
            adder_tree[it / 2] = adder_tree[it] + ((it + 2 <= n) ? adder_tree[it + 1] : 0);
        }
        nStages++;
        n = (n + 1) / 2;
    }
    // 3. Manage Round and Overflow/Underflow
    // 3.1 Convert it to a term of sign & magnitude
    const TFMASK retMaskExp = (1U << ret->getNbExp()) - 1U;
    TFDATA retSign = static_cast<TFDATA>(adder_tree[0] < 0);
    TFDATAL retMantL = retSign ? static_cast<TFDATAL>(-adder_tree[0]) : static_cast<TFDATAL>(adder_tree[0]);
    // 3.2 Get Zero-length
    // nStages = doCarryBitShift ? nStages : nStages + 1;
    nStages = hasMultCarry ? nStages + 1 : nStages;
    TFEXP lshMant = getZeroLengthOfMant(retMantL, this->nbMant + nStages /*carry-bit*/) - nStages /*carry-bit*/;
    TFEXP retExpBiased = (static_cast<TFEXP>(data) - this->biasExp) + ret->getBiasExp() - lshMant;
    lshMant += 1;  // one-bit encoding
    lshMant += ret->getNbMant() - this->nbMant;
    // 3.3 Manage Overflow/Underflow
    // 3.3.1 Because underflow occurs, denormal expression is required
    if (retExpBiased <= 0) {
        lshMant = 0;
        retExpBiased = 0;
    }
    // 3.3.2 Because overflow occurs, Inf expression is required
    else if (retExpBiased >= static_cast<TFEXP>(retMaskExp)) {
        retMantL = 0;
        lshMant = 0;
        retExpBiased = static_cast<TFEXP>(retMaskExp);
    }
    // 3.4. Round
    TFDATA retMant = lshMant < 0 ? roundHalfToEven(retMantL, -lshMant) : retMantL << lshMant;
    retExpBiased = retMant == 0 ? 0 : retExpBiased;
    // 4. Dump
    if(doDump) {
        arrDump[0] = retMantL;
    }
    // 5. Return the result
    ret->setData(ret->getData(retSign, retExpBiased, retMant));
    return ret;
}

TypeFloatBasePtr BlockTypeFloat::macTree(BlockTypeFloat& vs) {
    const bool doCarryBitShift = false;
    BlockTypeFloat muls = this->nonBlockedElementWiseMult(vs, doCarryBitShift);
    return muls.reduceSum();
}

TFBITS getBitWidth(const TFBITS nb) {
    if(nb > 1) {
        TFBITS ret = 0;
        for(int it = nb - 1; it>0; it=it>>1) { ret++; }
        return ret;
    } else { return 1U;}
}

}  // namespace numsys_hw