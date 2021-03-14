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

#include "CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker() : CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker*, CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker>(this) {
				}



				CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::initPropagationCutData(cint64 individualID, cint64 neighbourArrayIndex, cint64 neighbourPropagationCutCursor, CCacheValue conceptPropagationValue, cint64 associationUpdateId, CXLinker<cint64>* expandedIndividualIdsLinker, CCacheValue propMarkRoleValue) {
					mAssociationUpdateId = associationUpdateId;
					mIndividualID = individualID;
					mExpandedIndividualIdsLinker = expandedIndividualIdsLinker;
					mArrayIndex = neighbourArrayIndex;
					mConceptPropagationValue = conceptPropagationValue;
					mNeighbourPropagationCutCursor = neighbourPropagationCutCursor;
					mPropMarkRoleValue = propMarkRoleValue;
					mMissingNondeterministicExpansionPropagation = false;
					return this;
				}



				cint64 CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getIndividualID() {
					return mIndividualID;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getAssociationUpdateId() {
					return mAssociationUpdateId;
				}

				CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::setAssociationUpdateId(cint64 id) {
					mAssociationUpdateId = id;
					return this;
				}

				CXLinker<cint64>* CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getExpandedIndividualIdsLinker() {
					return mExpandedIndividualIdsLinker;
				}

				cint64 CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getArrayIndex() {
					return mArrayIndex;
				}

				CCacheValue CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getConceptPropagationValue() {
					return mConceptPropagationValue;
				}

				cint64 CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getNeighbourPropagationCutCursor() {
					return mNeighbourPropagationCutCursor;
				}

				CCacheValue CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::getPropagationMarkingRoleValue() {
					return mPropMarkRoleValue;
				}

				bool CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::isMissingNondeterministicExpansionPropagation() {
					return mMissingNondeterministicExpansionPropagation;
				}

				CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker::setMissingNondeterministicExpansionPropagation(bool missingProp) {
					mMissingNondeterministicExpansionPropagation = missingProp;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
