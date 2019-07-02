/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CConceptUnambiguousHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {

			CConceptUnambiguousHasher::CConceptUnambiguousHasher(CConcept* concept, bool tagRequired) : mConcept(concept),mTagRequired(tagRequired) {
				mHashValue = calculateHashValue();
			}


			cint64 CConceptUnambiguousHasher::calculateHashValue() {
				cint64 hashValue = mConcept->getOperatorCode() << 50;
				if (mTagRequired) {
					hashValue += mConcept->getConceptTag();
				} else {
					cint64 hashValueExt = ((cint64)mConcept->getRole() << 3)*7;
					hashValueExt += ((cint64)mConcept->getNominalIndividual())*13;
					hashValueExt += mConcept->getParameter()*23;
					hashValue += hashValueExt;
					cint64 hashValueOps1 = 0;
					cint64 hashValueOps2 = 1;
					CSortedNegLinker<CConcept*>* opLinkerIt = mConcept->getOperandList();
					while (opLinkerIt) {
						hashValueOps1 += ((cint64)opLinkerIt->getData()) << (opLinkerIt->isNegated()+3);
						hashValueOps2 *= ((cint64)opLinkerIt->getData()) << (opLinkerIt->isNegated()+3);
						opLinkerIt = opLinkerIt->getNext();
					}
					hashValue += hashValueOps1+hashValueOps2;
				}
				return hashValue;
			}


			cint64 CConceptUnambiguousHasher::getConceptHashValue() const {
				return mHashValue;
			}

			bool CConceptUnambiguousHasher::operator==(const CConceptUnambiguousHasher& expressionHasher) const {
				if (mHashValue != expressionHasher.getConceptHashValue()) {
					return false;
				} else {
					if (mTagRequired != expressionHasher.mTagRequired) {
						return false;
					}
					if (mConcept->getOperatorCode() != expressionHasher.mConcept->getOperatorCode()) {
						return false;
					}
					if (mTagRequired) {
						if (mConcept->getConceptTag() != expressionHasher.mConcept->getConceptTag()) {
							return false;
						}
					} else {
						if (mConcept->getRole() != expressionHasher.mConcept->getRole()) {
							return false;
						}
						if (mConcept->getNominalIndividual() != expressionHasher.mConcept->getNominalIndividual()) {
							return false;
						}
						if (mConcept->getParameter() != expressionHasher.mConcept->getParameter()) {
							return false;
						}
						CSortedNegLinker<CConcept*>* opLinker1It = mConcept->getOperandList();
						CSortedNegLinker<CConcept*>* opLinker2 = expressionHasher.mConcept->getOperandList();
						while (opLinker1It) {
							bool foundConcept = false;
							CSortedNegLinker<CConcept*>* opLinker2It = opLinker2;
							while (opLinker2It && !foundConcept) {
								if (opLinker1It->getData() == opLinker2It->getData() && opLinker1It->isNegated() == opLinker2It->isNegated()) {
									foundConcept = true;
								} else {
									opLinker2It = opLinker2It->getNext();
								}
							}
							if (foundConcept) {
								if (opLinker2 == opLinker2It) {
									opLinker2 = opLinker2->getNext();
								}
							} else {
								return false;
							}
							opLinker1It = opLinker1It->getNext();
						}
					}
				}
				return true;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude