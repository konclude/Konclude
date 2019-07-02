/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CCondensedReapplyConceptDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CCondensedReapplyConceptDescriptor::CCondensedReapplyConceptDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes) : CReapplyConceptDescriptor() {
					mConceptDes = conceptDescriptor;
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mProcessingRestriction = nullptr;
					mPositive = isPositiveDes;
				}

				CCondensedReapplyConceptDescriptor::CCondensedReapplyConceptDescriptor() : CReapplyConceptDescriptor() {
					mPositive = true;
				}

				CCondensedReapplyConceptDescriptor *CCondensedReapplyConceptDescriptor::initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes) {
					mConceptDes = conceptDescriptor;
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mPositive = isPositiveDes;
					mProcessingRestriction = nullptr;
					return this;
				}


				CCondensedReapplyConceptDescriptor* CCondensedReapplyConceptDescriptor::getNext() {
					return (CCondensedReapplyConceptDescriptor*)CReapplyConceptDescriptor::getNext();
				}


				CCondensedReapplyConceptDescriptor *CCondensedReapplyConceptDescriptor::initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes, CProcessingRestrictionSpecification* procRest) {
					mConceptDes = conceptDescriptor;
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mPositive = isPositiveDes;
					mProcessingRestriction = procRest;
					return this;
				}


				bool CCondensedReapplyConceptDescriptor::isPositiveDescriptor() {
					return mPositive;
				}


				bool CCondensedReapplyConceptDescriptor::hasConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					return mConceptDes == conceptDescriptor;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
