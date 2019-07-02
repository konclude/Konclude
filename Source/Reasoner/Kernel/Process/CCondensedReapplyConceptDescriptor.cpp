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

#include "CCondensedReapplyConceptDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CCondensedReapplyConceptDescriptor::CCondensedReapplyConceptDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes) : CReapplyConceptDescriptor() {
					setData(conceptDescriptor);
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mProcessingRestriction = nullptr;
					mPositive = isPositiveDes;
					mExtended = false;
				}

				CCondensedReapplyConceptDescriptor::CCondensedReapplyConceptDescriptor() : CReapplyConceptDescriptor() {
					mExtended = false;
					mPositive = true;
				}

				CCondensedReapplyConceptDescriptor *CCondensedReapplyConceptDescriptor::initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes) {
					setData(conceptDescriptor);
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mPositive = isPositiveDes;
					mExtended = false;
					mProcessingRestriction = nullptr;
					return this;
				}


				CCondensedReapplyConceptDescriptor* CCondensedReapplyConceptDescriptor::getNext() {
					return (CCondensedReapplyConceptDescriptor*)CReapplyConceptDescriptor::getNext();
				}


				CCondensedReapplyConceptDescriptor *CCondensedReapplyConceptDescriptor::initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes, CProcessingRestrictionSpecification* procRest) {
					setData(conceptDescriptor);
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mPositive = isPositiveDes;
					mProcessingRestriction = procRest;
					return this;
				}


				bool CCondensedReapplyConceptDescriptor::isPositiveDescriptor() {
					return mPositive;
				}


				bool CCondensedReapplyConceptDescriptor::isExtended() {
					return mExtended;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
