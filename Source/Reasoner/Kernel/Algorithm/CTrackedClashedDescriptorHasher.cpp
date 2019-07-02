/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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
				}


				CTrackedClashedDescriptorHasher::CTrackedClashedDescriptorHasher(const CTrackedClashedDescriptorHasher& hasher) {
					mHashValue = hasher.mHashValue;
					mTrackedClashedDes = hasher.mTrackedClashedDes;
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
					if (mTrackedClashedDes->getAppropriatedIndividualID() != clashedDesHasher.mTrackedClashedDes->getAppropriatedIndividualID()) {
						return false;
					}
					if (mTrackedClashedDes->getConceptDescriptor() && clashedDesHasher.mTrackedClashedDes->getConceptDescriptor()) {
						if (mTrackedClashedDes->getConceptDescriptor()->getConcept() != clashedDesHasher.mTrackedClashedDes->getConceptDescriptor()->getConcept()) {
							return false;
						}
						if (mTrackedClashedDes->getConceptDescriptor()->getNegation() != clashedDesHasher.mTrackedClashedDes->getConceptDescriptor()->getNegation()) {
							return false;
						}
					} else {
						if (mTrackedClashedDes->getConceptDescriptor() || clashedDesHasher.mTrackedClashedDes->getConceptDescriptor()) {
							return false;
						}
					}
					if (mTrackedClashedDes->getDependencyTrackPoint() != clashedDesHasher.mTrackedClashedDes->getDependencyTrackPoint()) {
						return false;
					}
					if (mTrackedClashedDes->getVariableBindingPath() != clashedDesHasher.mTrackedClashedDes->getVariableBindingPath()) {
						return false;
					}
					return true;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
