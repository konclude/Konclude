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

#include "CConceptSetSignature.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptSetSignature::CConceptSetSignature() {
					reset();
				}


				CConceptSetSignature::~CConceptSetSignature() {
				}


				CConceptSetSignature* CConceptSetSignature::reset() {
					mSigValue = 0;
					mValue1 = 0;
					mValue2 = 1;
					mValue3 = 0;
					return this;
				}


				cint64 CConceptSetSignature::getSignatureValue() {
					return mSigValue;
				}

				CConceptSetSignature* CConceptSetSignature::addConceptSignature(CConcept* concept, bool negation) {
					cint64 conTag = concept->getConceptTag();
					cint64 conSig = conTag;
					if (negation) {
						conSig = std::numeric_limits<cint64>::max() - conTag;
					}
					mValue1 += conSig;
					mValue2 *= conSig;
					mValue3 += (cint64)concept;
					mSigValue = mValue1 ^ mValue2 ^ mValue3;
					return this;
				}

				CConceptSetSignature* CConceptSetSignature::addConceptSignature(CConceptDescriptor* conceptDescriptor) {
					CConcept* concept = conceptDescriptor->getConcept();
					bool negation = conceptDescriptor->isNegated();
					addConceptSignature(concept,negation);
					return this;
				}


				bool CConceptSetSignature::operator==(const CConceptSetSignature& conSig) {
					return mSigValue == conSig.mSigValue;
				}

				bool CConceptSetSignature::isSignatureEquivalent(const CConceptSetSignature& conSig) {
					return mSigValue == conSig.mSigValue;
				}

				bool CConceptSetSignature::isSignatureEquivalent(CConceptSetSignature* conSig) {
					return conSig && mSigValue == conSig->mSigValue;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
