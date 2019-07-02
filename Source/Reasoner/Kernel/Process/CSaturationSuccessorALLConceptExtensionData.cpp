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

#include "CSaturationSuccessorALLConceptExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationSuccessorALLConceptExtensionData::CSaturationSuccessorALLConceptExtensionData(CProcessContext* processContext) : CLinkerBase<bool,CSaturationSuccessorALLConceptExtensionData>(false),mConceptExtensionMap(processContext) {
					mProcessContext = processContext;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationSuccessorALLConceptExtensionData::initSuccessorConceptExtensionData(CRole* role, CIndividualSaturationProcessNode* indiProcSatNode) {
					setData(false);
					setNext(nullptr);
					mRole = role;
					mIndiProcSatNode = indiProcSatNode;
					mLastResolvedIndiProcSatNode = nullptr;
					mRequiredSuccCount = 0;
					mLastConnectedSuccCount = 0;
					mSuccessorCardinalityUpdatedFlag = false;
					mConceptsUpdatedFlag = false;
					return this;
				}

				bool CSaturationSuccessorALLConceptExtensionData::isExtensionProcessingQueued() {
					return getData();
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationSuccessorALLConceptExtensionData::setExtensionProcessingQueued(bool queued) {
					setData(queued);
					return this;
				}


				CSaturationSuccessorConceptExtensionMap* CSaturationSuccessorALLConceptExtensionData::getSuccessorConceptExtensionMap() {
					return &mConceptExtensionMap;
				}

				bool CSaturationSuccessorALLConceptExtensionData::addExtensionConcept(CConcept* concept, bool negation) {
					mConceptsUpdatedFlag |= mConceptExtensionMap.addExtensionConcept(concept,negation);
					return mConceptsUpdatedFlag;
				}

				CRole* CSaturationSuccessorALLConceptExtensionData::getRole() {
					return mRole;
				}

				CIndividualSaturationProcessNode* CSaturationSuccessorALLConceptExtensionData::getIndividualNode() {
					return mIndiProcSatNode;
				}

				CIndividualSaturationProcessNode* CSaturationSuccessorALLConceptExtensionData::getLastResolvedIndividualNode() {
					return mLastResolvedIndiProcSatNode;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationSuccessorALLConceptExtensionData::setLastResolvedIndividualNode(CIndividualSaturationProcessNode* indiNode) {
					mLastResolvedIndiProcSatNode = indiNode;
					return this;
				}

				cint64 CSaturationSuccessorALLConceptExtensionData::getRequiredSuccessorCardinality() {
					return mRequiredSuccCount;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationSuccessorALLConceptExtensionData::setRequiredSuccessorCardinality(cint64 succCard) {
					mRequiredSuccCount = succCard;
					return this;
				}


				cint64 CSaturationSuccessorALLConceptExtensionData::getLastConnectedSuccessorCardinality() {
					return mLastConnectedSuccCount;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationSuccessorALLConceptExtensionData::setLastConnectedSuccessorCardinality(cint64 succCard) {
					mLastConnectedSuccCount = succCard;
					return this;
				}

				bool CSaturationSuccessorALLConceptExtensionData::addRequiredSuccessorCardinality(cint64 succCard) {
					if (mRequiredSuccCount < succCard) {
						mRequiredSuccCount = succCard;
						mSuccessorCardinalityUpdatedFlag = true;
					}
					return mSuccessorCardinalityUpdatedFlag;
				}


				bool CSaturationSuccessorALLConceptExtensionData::hasSuccessorCardinalityUpdatedFlag() {
					return mConceptsUpdatedFlag;
				}

				bool CSaturationSuccessorALLConceptExtensionData::hasConceptsUpdatedFlag() {
					return mSuccessorCardinalityUpdatedFlag;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationSuccessorALLConceptExtensionData::clearUpdatedFlags() {
					mConceptsUpdatedFlag = false;
					mSuccessorCardinalityUpdatedFlag = false;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
