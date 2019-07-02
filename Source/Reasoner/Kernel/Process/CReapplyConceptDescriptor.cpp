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

#include "CReapplyConceptDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CReapplyConceptDescriptor::CReapplyConceptDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isStaticDes) : CLinkerBase<CConceptDescriptor*,CReapplyConceptDescriptor>(conceptDescriptor) {
					mTrackPoint = depTrackPoint;
					mStatic = isStaticDes;
					mProcessingRestriction = nullptr;
				}

				CReapplyConceptDescriptor::CReapplyConceptDescriptor() : CLinkerBase<CConceptDescriptor*,CReapplyConceptDescriptor>(nullptr) {
					mTrackPoint = 0;
					mProcessingRestriction = nullptr;
				}


				CReapplyConceptDescriptor *CReapplyConceptDescriptor::initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isStaticDes) {
					setData(conceptDescriptor);
					mTrackPoint = depTrackPoint;
					mStatic = isStaticDes;
					mProcessingRestriction = nullptr;
					return this;
				}


				CReapplyConceptDescriptor *CReapplyConceptDescriptor::initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, CProcessingRestrictionSpecification* procRest) {
					setData(conceptDescriptor);
					mTrackPoint = depTrackPoint;
					mStatic = false;
					mProcessingRestriction = procRest;
					return this;
				}


				CConceptDescriptor *CReapplyConceptDescriptor::getConceptDescriptor() {
					return getData();
				}

				CDependencyTrackPoint *CReapplyConceptDescriptor::getDependencyTrackPoint() {
					return mTrackPoint;
				}

				bool CReapplyConceptDescriptor::isStaticDescriptor() {
					return mStatic;
				}

				CProcessingRestrictionSpecification* CReapplyConceptDescriptor::getReapplyProcessingRestriction() {
					return mProcessingRestriction;
				}

				bool CReapplyConceptDescriptor::hasConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					return getData() == conceptDescriptor;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
