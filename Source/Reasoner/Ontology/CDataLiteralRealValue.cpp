/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CDataLiteralRealValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralRealValue::CDataLiteralRealValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralRealValue::getDataValueType() {
				return CDataLiteralValue::DLVT_REAL;
			}



			QString CDataLiteralRealValue::getValueString() {
				QString valueString;
				if (mNegative) {
					valueString += "-";
				}
				valueString += QString::number(mInteger);
				if (mNumerator != 0) {
					valueString += " ";
					valueString += QString::number(mNumerator);
					valueString += "/";
					valueString += QString::number(mDenominator);
				}
				return valueString;
			}


			CDataLiteralCompareValue* CDataLiteralRealValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralRealValue* realValue = dynamic_cast<CDataLiteralRealValue*>(value);
				if (realValue) {					
					mInfinite = realValue->mInfinite;
					mNegative = realValue->mNegative;
					mInteger = realValue->mInteger;
					mNumerator = realValue->mNumerator;
					mDenominator = realValue->mDenominator;
					mFlags = realValue->mFlags;
				}
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromInfinite(bool negative) {
				clearValue();
				mNegative = negative;
				mInfinite = true;
				normalize();
				mFlags |= DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromDecimal(cint64 integerValue, cint64 decimalValue, cint64 precedingZerors) {
				initRationalValueFromInteger(integerValue);
				clearFlags();
				if (decimalValue < 0) {
					mNegative = !mNegative;
					decimalValue = -decimalValue;
				}
				mNumerator = decimalValue;
				mDenominator = 1;
				while (mDenominator < decimalValue) {
					mDenominator *= 10;
				}
				while (precedingZerors > 0) {
					--precedingZerors;
					mDenominator *= 10;
				}
				normalize();
				mFlags |= DLRV_DECIMAL_FLAG | DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromDecimal(cuint64 integerValue, cuint64 decimalValue, bool negative, cint64 precedingZerors) {
				initRationalValueFromInteger(integerValue,negative);
				clearFlags();
				mNumerator = decimalValue;
				mDenominator = 1;
				while (mDenominator <= decimalValue) {
					mDenominator *= 10;
				}
				while (precedingZerors > 0) {
					--precedingZerors;
					mDenominator *= 10;
				}
				normalize();
				mFlags |= DLRV_DECIMAL_FLAG | DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromFraction(cint64 numeratorValue, cint64 denominatorValue) {
				clearValue();
				if (numeratorValue < 0 && denominatorValue < 0) {
					numeratorValue = -numeratorValue;
					denominatorValue = -denominatorValue;
				} else if (numeratorValue < 0) {
					numeratorValue = -numeratorValue;
					mNegative = true;
				} else if (denominatorValue < 0) {
					denominatorValue = -denominatorValue;
					mNegative = true;
				}
				mNumerator = numeratorValue;
				mDenominator = denominatorValue;
				normalize();
				mFlags |= DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromFraction(cuint64 numeratorValue, cuint64 denominatorValue, bool negative) {
				clearValue();
				mNumerator = numeratorValue;
				mDenominator = denominatorValue;
				mNegative = negative;
				normalize();
				mFlags |= DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromNextIntegerValue(CDataLiteralRealValue* value) {
				clearValue();
				mNegative = value->mNegative;
				mInteger = value->mInteger;
				if (mNegative) {
					if (mInteger > 0) {
						--mInteger;
					} else {
						mInteger = 0;
						mNegative = false;
					}
				} else {
					++mInteger;
				}
				normalize();
				mFlags |= DLRV_INTEGER_FLAG | DLRV_DECIMAL_FLAG | DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}



			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromInteger(cuint64 value, bool negation) {
				clearValue();
				mNegative = negation;
				mInteger = value;
				normalize();
				mFlags |= DLRV_INTEGER_FLAG | DLRV_DECIMAL_FLAG | DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::initRationalValueFromInteger(cint64 value) {
				clearValue();
				if (value < 0) {
					mNegative = true;
					mInteger = -value;
				} else {
					mNegative = false;
					mInteger = value;
				}
				normalize();
				mFlags |= DLRV_INTEGER_FLAG | DLRV_DECIMAL_FLAG | DLRV_RATIONAL_FLAG;
				identifyFlags();
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::initValue(CDataLiteralRealValue* value) {
				clearFlags();
				if (value) {
					mInfinite = value->mInfinite;
					mNegative = value->mNegative;
					mInteger = value->mInteger;
					mNumerator = value->mNumerator;
					mDenominator = value->mDenominator;
					mFlags = value->mFlags;
				}
				return this;
			}


			CDataLiteralRealValue* CDataLiteralRealValue::clearValue() {
				mInfinite = false;
				mNegative = false;
				mInteger = 0;
				mNumerator = 0;
				mDenominator = 1;
				clearFlags();
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::clearFlags() {
				mFlags = 0;
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::identifyFlags() {
				mFlags |=  DLRV_RATIONAL_FLAG;
				if (mNumerator == 0) {
					mFlags |= DLRV_INTEGER_FLAG | DLRV_DECIMAL_FLAG;
				} else {
					if (!hasFlag(DLRV_DECIMAL_FLAG)) {
						cuint64 denominatorFac = mDenominator;
						bool reduced = false;
						do {
							reduced = false;
							while (denominatorFac % 2 == 0) {
								denominatorFac /= 2;
								reduced = true;
							}
							if (denominatorFac % 5 == 0) {
								denominatorFac /= 5;
								reduced = true;
							}
						} while (reduced);
						if (denominatorFac == 2 || denominatorFac == 5 || denominatorFac == 1) {
							mFlags |= DLRV_DECIMAL_FLAG;
						}
					}
				}
				return this;
			}


			cint64 getGreatestCommonDivisor(cint64 u, cint64 v) {
				cint64 t;
				while (v) {
					t = u; 
					u = v; 
					v = t % v;
				}
				return u < 0 ? -u : u;
			}


			bool CDataLiteralRealValue::hasFlag(cint64 flag) {
				return (mFlags & flag) != 0;
			}


			bool CDataLiteralRealValue::normalize() {
				bool reduced = false;
				if (mInfinite) {
					mInteger = 0;
					mNumerator = 0;
					mDenominator = 1;
				}
				if (mNumerator >= mDenominator) {
					cint64 nonFraction = mNumerator/mDenominator;
					mNumerator = mNumerator-nonFraction*mDenominator;
					mInteger += nonFraction;
					reduced = true;
				}
				if (mNumerator == 0) {
					mDenominator = 1;
				} else {
					cint64 gcd = getGreatestCommonDivisor(mNumerator,mDenominator);
					if (gcd > 1) {
						mNumerator = mNumerator/gcd;
						mDenominator = mDenominator/gcd;
						reduced = true;
					}
				}
				if (mInteger == 0 && mNumerator == 0 && mNegative && !mInfinite) {
					mNegative = false;
				}
				return reduced;
			}


			bool CDataLiteralRealValue::isNegative() {
				return mNegative;
			}

			bool CDataLiteralRealValue::isInfinite() {
				return mInfinite;
			}


			bool CDataLiteralRealValue::isInfiniteBigNumber() {
				if (mInfinite) {
					return true;
				}
				if (mNegative && mInteger > Q_INT64_C(9223372036854775808)) {
					return true;
				} else if (!mNegative && mInteger > Q_INT64_C(9223372036854775807)) {
					return true;
				}
				return false;
			}

			cuint64 CDataLiteralRealValue::getInteger() {
				return mInteger;
			}

			cuint64 CDataLiteralRealValue::getNumerator() {
				return mNumerator;
			}

			cuint64 CDataLiteralRealValue::getDenominator() {
				return mDenominator;
			}


			cint64 CDataLiteralRealValue::getPosNegInteger() {
				if (!mNegative) {
					return mInteger;
				} else {
					return -mInteger;
				}
			}


			CDataLiteralRealValue* CDataLiteralRealValue::setNegative(bool negative) {
				mNegative = negative;
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::setInteger(cuint64 integer) {
				mInteger = integer;
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::setNumerator(cuint64 numerator) {
				mNumerator = numerator;
				return this;
			}

			CDataLiteralRealValue* CDataLiteralRealValue::setDenominator(cuint64 denominator) {
				mDenominator = denominator;
				return this;
			}




			bool CDataLiteralRealValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralRealValue* realValue = dynamic_cast<CDataLiteralRealValue*>(value);
				if (realValue) {
					return isEqualTo(realValue);
				}
				return false;
			}



			bool CDataLiteralRealValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralRealValue* realValue = dynamic_cast<CDataLiteralRealValue*>(value);
				if (realValue) {
					return isEqualTo(realValue) || isLessThan(realValue);
				}
				return false;
			}

			bool CDataLiteralRealValue::isLessEqualThan(CDataLiteralRealValue* realValue) {
				return isEqualTo(realValue) || isLessThan(realValue);
			}


			bool CDataLiteralRealValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralRealValue* realValue = dynamic_cast<CDataLiteralRealValue*>(value);
				if (realValue) {
					return isEqualTo(realValue);
				}
				return false;
			}

			bool CDataLiteralRealValue::isEqualTo(CDataLiteralRealValue* realValue) {
				if (mNegative == realValue->mNegative) {
					if (mInfinite && realValue->mInfinite) {
						return true;
					}
					if (mInteger == realValue->mInteger) {
						if (mNumerator == realValue->mNumerator) {
							if (mDenominator == realValue->mDenominator) {
								return true;
							}
						}
					}
				}
				return false;
			}

			bool CDataLiteralRealValue::isGreaterEqualThan(CDataLiteralRealValue* realValue) {
				return isEqualTo(realValue) || isGreaterThan(realValue);
			}

			bool CDataLiteralRealValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralRealValue* realValue = dynamic_cast<CDataLiteralRealValue*>(value);
				if (realValue) {
					return isEqualTo(realValue) || isGreaterThan(realValue);
				}
				return false;
			}

			bool CDataLiteralRealValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralRealValue::isGreaterThan(CDataLiteralRealValue* realValue) {
				return realValue->isLessThan(this);
			}


			bool CDataLiteralRealValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralRealValue* realValue = dynamic_cast<CDataLiteralRealValue*>(value);
				if (realValue) {
					return isLessThan(realValue);
				}
				return false;
			}





			bool CDataLiteralRealValue::isLessThan(CDataLiteralRealValue* realValue) {
				if (mNegative == realValue->mNegative && mInfinite && realValue->mInfinite) {
					return false;
				} else if (mNegative && mInfinite) {
					return true;
				} else if (!mNegative && mInfinite) {
					return false;
				} else if (!realValue->mNegative && realValue->mInfinite) {
					return true;
				} else if (realValue->mNegative && realValue->mInfinite) {
					return false;
				} else if (mNegative && !realValue->mNegative) {
					return true;
				} else if (!mNegative && realValue->mNegative) {
					return false;
				}
				if (mNegative) {
					if (mInteger > realValue->mInteger) {
						return true;
					} else if (mInteger < realValue->mInteger) {
						return false;
					} else {
						if (mDenominator == realValue->mDenominator) {
							if (mNumerator > realValue->mNumerator) {
								return true;
							}
						} else {
							cint64 ad = mNumerator*realValue->mDenominator;
							cint64 bc = realValue->mNumerator*mDenominator;
							if (ad > bc) {
								return true;
							}
						}
					}
				} else {
					if (mInteger < realValue->mInteger) {
						return true;
					} else if (mInteger > realValue->mInteger) {
						return false;
					} else {
						if (mDenominator == realValue->mDenominator) {
							if (mNumerator < realValue->mNumerator) {
								return true;
							}
						} else {
							cint64 ad = mNumerator*realValue->mDenominator;
							cint64 bc = realValue->mNumerator*mDenominator;
							if (ad < bc) {
								return true;
							}
						}
					}
				}
				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
