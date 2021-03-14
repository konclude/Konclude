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

#include "CBackendRepresentativeMemoryCacheLabelAssociationWriteData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheLabelAssociationWriteData::CBackendRepresentativeMemoryCacheLabelAssociationWriteData() {
				}

				CBackendRepresentativeMemoryCacheLabelAssociationWriteData* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::initWriteData(cint64 ontologyIdentifier, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker, CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheTemporaryInvolvedIndividualDataLinker* involvedIndiDataLinker, CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* propCutDataLinker, cint64 repCompId) {
					mTempAssUseDataLinker = tempAssUseDataLinker;
					mTempAssWriteDataLinker = tempAssWriteDataLinker;
					mTempLabelWriteDataLinker = tempLabelWriteDataLinker;
					mTempCardWriteDataLinker = tempCardWriteDataLinker;
					mTempNomIndirectConnDataLinker = tempNomIndirectConnDataLinker;
					mInvolvedIndiDataLinker = involvedIndiDataLinker;
					mType = BACKENDASSOCIATIONWRITEDATATYPE;
					mOntologyIdentifier = ontologyIdentifier;
					mPropCutDataLinker = propCutDataLinker;
					mRecompuationId = repCompId;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getRecompuationId() {
					return mRecompuationId;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryAssociationWriteDataLinker() {
					return mTempAssWriteDataLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryLabelWriteDataLinker() {
					return mTempLabelWriteDataLinker;
				}


				CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryCardinaltyWriteDataLinker() {
					return mTempCardWriteDataLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryAssociationUseDataLinker() {
					return mTempAssUseDataLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryNominalIndirectConnectionDataLinker() {
					return mTempNomIndirectConnDataLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryInvolvedIndividualDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryInvolvedIndividualIdDataLinker() {
					return mInvolvedIndiDataLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* CBackendRepresentativeMemoryCacheLabelAssociationWriteData::getTemporaryPropagationCutDataLinker() {
					return mPropCutDataLinker;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
