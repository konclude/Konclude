/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CDatatypeValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeValueSpaceData::CDatatypeValueSpaceData(CProcessContext* processContext) : mClashDepTrackPointCollection(processContext) {
					mProcessContext = processContext;
				}


				CDatatypeValueSpaceData* CDatatypeValueSpaceData::initValueSpaceData(CDatatypeValueSpaceData* spaceData) {
					if (spaceData) {
						mValueSpaceClashed = spaceData->mValueSpaceClashed;
						mValueSpaceTriggeringCompleted = spaceData->mValueSpaceTriggeringCompleted;
						mValueSpaceTriggeringRequired = spaceData->mValueSpaceTriggeringRequired;
						mValueSpaceInitialisationDepTrackPoint = spaceData->mValueSpaceInitialisationDepTrackPoint;
						mValueSpaceTriggeringStarted = spaceData->mValueSpaceTriggeringStarted;
						mValueSpaceCounted = spaceData->mValueSpaceCounted;
						mValueSpaceClashCheckingRequired = spaceData->mValueSpaceClashCheckingRequired;
						mValuesCounter.initValueCounter(&spaceData->mValuesCounter);
						mClashDepTrackPointCollection.initDependencyTrackPointCollection(&spaceData->mClashDepTrackPointCollection);
					} else {
						mValueSpaceClashed = false;
						mValueSpaceTriggeringCompleted = false;
						mValueSpaceTriggeringRequired = false;
						mValueSpaceTriggeringStarted = false;
						mValueSpaceCounted = false;
						mValueSpaceClashCheckingRequired = false;
						mValueSpaceInitialisationDepTrackPoint = nullptr;
						mValuesCounter.initValueCounter();
						mClashDepTrackPointCollection.initDependencyTrackPointCollection(nullptr);
					}
					return this;
				}


				CDatatypeDependencyTrackPointCollection* CDatatypeValueSpaceData::getClashDependencyTrackPointCollection() {
					return &mClashDepTrackPointCollection;
				}

				CClashedDependencyDescriptor* CDatatypeValueSpaceData::getClashDependencyDescriptorLinker() {
					return mClashDepTrackPointCollection.getClashedDependencyDescriptors();
				}


				bool CDatatypeValueSpaceData::isValueSpaceClashed() {
					return mValueSpaceClashed;
				}

				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceClashed(bool clashed) {
					mValueSpaceClashed = clashed;
					return this;
				}


				bool CDatatypeValueSpaceData::isValueSpaceTriggeringCompleted() {
					return mValueSpaceTriggeringCompleted;
				}

				bool CDatatypeValueSpaceData::isValueSpaceTriggeringRequired() {
					return mValueSpaceTriggeringRequired;
				}

				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceTriggeringCompleted(bool triggeringCompleted) {
					mValueSpaceTriggeringCompleted = triggeringCompleted;
					return this;
				}

				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceTriggeringRequired(bool triggeringRequired) {
					mValueSpaceTriggeringRequired = triggeringRequired;
					return this;
				}


				bool CDatatypeValueSpaceData::isValueSpaceTriggeringStarted() {
					return mValueSpaceTriggeringStarted;
				}

				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceTriggeringStarted(bool triggeringStarted) {
					mValueSpaceTriggeringStarted = triggeringStarted;
					return this;
				}


				bool CDatatypeValueSpaceData::isValueSpaceCountingRequired() {
					return mValueSpaceCountingRequired;
				}


				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceCountingRequired(bool triggeringStarted) {
					mValueSpaceCountingRequired = triggeringStarted;
					return this;
				}


				bool CDatatypeValueSpaceData::isValueSpaceClashCheckingRequired() {
					return mValueSpaceClashCheckingRequired;
				}


				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceClashCheckingRequired(bool clashCheckingRequired) {
					mValueSpaceClashCheckingRequired = clashCheckingRequired;
					return this;
				}


				bool CDatatypeValueSpaceData::isValueSpaceCounted() {
					return mValueSpaceCounted;
				}

				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceCounted(bool counted) {
					mValueSpaceCounted = counted;
					return this;
				}


				CDependencyTrackPoint* CDatatypeValueSpaceData::getValueSpaceInitialisationDependencyTrackPoint() {
					return mValueSpaceInitialisationDepTrackPoint;
				}


				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceInitialisationDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint) {
					mValueSpaceInitialisationDepTrackPoint = depTrackPoint;
					return this;
				}


				bool CDatatypeValueSpaceData::setValueSpaceInitialisationDependencyTrackPointCandidate(CDependencyTrackPoint* depTrackPoint) {
					if (!mValueSpaceInitialisationDepTrackPoint) {
						mValueSpaceInitialisationDepTrackPoint = depTrackPoint;
						return true;
					}
					return false;
				}


				CDatatypeValueSpaceData* CDatatypeValueSpaceData::setValueSpaceModification() {
					if (mValueSpaceTriggeringStarted && !mValueSpaceTriggeringCompleted) {
						mValueSpaceTriggeringRequired = true;
					}
					mValueSpaceCounted = false;
					mValueSpaceCountingRequired = true;
					if (!mValueSpaceClashed) {
						mValueSpaceClashCheckingRequired = true;
					}
					return this;
				}

				CDatatypeValueSpaceValuesCounter* CDatatypeValueSpaceData::getValuesCounter() {
					return &mValuesCounter;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
