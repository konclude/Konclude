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

#include "CConceptProcessDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptProcessDescriptor::CConceptProcessDescriptor(CConceptDescriptor *conceptDescriptor, CConceptProcessPriority processPriority, bool conceptReapplied, CDependencyTrackPoint *dependencyTrackPoint) : CSortedLinkerBase<CConceptProcessDescriptor*,CConceptProcessDescriptor>(this) {
					conceptDes = conceptDescriptor;
					priority = processPriority;
					depTrackPoint = dependencyTrackPoint;
					mReapplied = conceptReapplied;
					mProcSpec = nullptr;
				}


				CConceptProcessDescriptor::CConceptProcessDescriptor() : CSortedLinkerBase<CConceptProcessDescriptor*,CConceptProcessDescriptor>(this) {
					conceptDes = nullptr;
					depTrackPoint = nullptr;
					mProcSpec = nullptr;
					mReapplied = false;
					priority = 0.;
				}


				CConceptProcessDescriptor *CConceptProcessDescriptor::init(CConceptDescriptor *conceptDescriptor, CConceptProcessPriority processPriority, bool conceptReapplied, CDependencyTrackPoint *dependencyTrackPoint, CProcessingRestrictionSpecification* procSpec) {
					conceptDes = conceptDescriptor;
					priority = processPriority;
					depTrackPoint = dependencyTrackPoint;
					mReapplied = conceptReapplied;
					mProcSpec = procSpec;
					return this;
				}

				CConceptProcessDescriptor* CConceptProcessDescriptor::initCopy(CConceptProcessDescriptor* conceptProcessDes) {
					conceptDes = conceptProcessDes->conceptDes;
					priority = conceptProcessDes->priority;
					depTrackPoint = conceptProcessDes->depTrackPoint;
					mReapplied = conceptProcessDes->mReapplied;
					mProcSpec = conceptProcessDes->mProcSpec;
					return this;
				}


				CConceptDescriptor *CConceptProcessDescriptor::getConceptDescriptor() {
					return conceptDes;
				}

				CConceptProcessPriority CConceptProcessDescriptor::getProcessPriority() {
					return priority;
				}

				CDependencyTrackPoint *CConceptProcessDescriptor::getDependencyTrackPoint() {
					return depTrackPoint;
				}

				bool CConceptProcessDescriptor::isConceptReapplied() {
					return mReapplied;
				}

				CProcessingRestrictionSpecification *CConceptProcessDescriptor::getProcessingRestrictionSpecification() {
					return mProcSpec;
				}

				bool CConceptProcessDescriptor::operator<=(const CConceptProcessDescriptor &descriptor) {
					return descriptor.priority <= priority;
				}


			}; // end namespace Kernel

		}; // end namespace Process

	}; // end namespace Reasoner

}; // end namespace Konclude
