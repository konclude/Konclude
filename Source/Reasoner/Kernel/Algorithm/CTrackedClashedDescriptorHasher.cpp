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

#include "CTrackedClashedDescriptorHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CTrackedClashedDescriptorHasher::CTrackedClashedDescriptorHasher(CTrackedClashedDescriptor* trackedClashedDescriptor) {
					mTrackedClashedDes = trackedClashedDescriptor;
					mHashValue = calculateDescriptorHashValue(mTrackedClashedDes);
					mIndividualID = mTrackedClashedDes->getAppropriatedIndividualID();
					CConceptDescriptor* conDes = mTrackedClashedDes->getConceptDescriptor();
					mConcept = conDes ? conDes->getConcept() : nullptr;
					mNegation = conDes ? conDes->getNegation() : false;
					mDepTrackPoint = mTrackedClashedDes->getDependencyTrackPoint();
					mVarBindPath = mTrackedClashedDes->getVariableBindingPath();
				}


				CTrackedClashedDescriptorHasher::CTrackedClashedDescriptorHasher(const CTrackedClashedDescriptorHasher& hasher) {
					mHashValue = hasher.mHashValue;
					mTrackedClashedDes = hasher.mTrackedClashedDes;
					mIndividualID = hasher.mIndividualID;
					mConcept = hasher.mConcept;
					mNegation = hasher.mNegation;
					mDepTrackPoint = hasher.mDepTrackPoint;
					mVarBindPath = hasher.mVarBindPath;
				}



				cint64 CTrackedClashedDescriptorHasher::getDescriptorHashValue() const {
					return mHashValue;
				}

				cint64 CTrackedClashedDescriptorHasher::calculateDescriptorHashValue(CTrackedClashedDescriptor* des) {
					cint64 hashValue = 0;
					hashValue += des->getAppropriatedIndividualID();
					if (des->getConceptDescriptor()) {
						hashValue += (cint64)des->getConceptDescriptor()->getConcept();
						if (des->getConceptDescriptor()->getNegation()) {
							hashValue = (hashValue << 1)+13;
						}
					}
					hashValue += (cint64)des->getDependencyTrackPoint();
					hashValue += (cint64)des->getVariableBindingPath();
					return hashValue;
				}


				bool CTrackedClashedDescriptorHasher::operator==(const CTrackedClashedDescriptorHasher& clashedDesHasher) const {
					if (mHashValue != clashedDesHasher.mHashValue) {
						return false;
					}
					if (mIndividualID != clashedDesHasher.mIndividualID) {
						return false;
					}
					if (mConcept != clashedDesHasher.mConcept || mNegation != clashedDesHasher.mNegation) {
						return false;
					}
					if (mDepTrackPoint != clashedDesHasher.mDepTrackPoint) {
						return false;
					}
					if (mVarBindPath != clashedDesHasher.mVarBindPath) {
						return false;
					}
					return true;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
