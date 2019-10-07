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

#include "CConcreteOntologyRebuildingPreparationBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyRebuildingPreparationBuilder::CConcreteOntologyRebuildingPreparationBuilder(CConcreteOntology* updateConcreteOntology) : CConcreteOntologyRebuildingBuilder(updateConcreteOntology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyRebuildingPreparationBuilder",logTr("ConcreteOntologyRebuildingPreparationBuilder initialized."),this);
			}

			CConcreteOntologyRebuildingPreparationBuilder::~CConcreteOntologyRebuildingPreparationBuilder() {
			}




			CConcreteOntologyRebuildingPreparationBuilder* CConcreteOntologyRebuildingPreparationBuilder::prepareRebuildingUpdates() {
				mOntoData = mOnto->getDataBoxes();
				mOntoBuild = mOnto->getBuildData();

				mClassTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
				mConceptClassTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();
				mDataRangeTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getDataRangeTermConceptMappingHash();
				mConceptDataRangeTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptDataRangeTermMappingHash();
				mExpressionBuildHash = mOntoBuild->getStructuralExpressionBuildHash();
				mLocExpressionBuildHash = mOntoData->getExpressionDataBoxMapping()->getStructuralExpressionBuildHash();
				mLocExpressionBuildContainerList = mOntoData->getExpressionDataBoxMapping()->getExpressionBuildListContainer();
				mLocBuildConceptSet = mOntoData->getExpressionDataBoxMapping()->getBuildedConceptSet();


				mClassTermConceptHash->detach();
				mConceptClassTermHash->detach();
				mDataRangeTermConceptHash->detach();
				mConceptDataRangeTermHash->detach();
				mExpressionBuildHash->detach();
				mLocExpressionBuildHash->detach();
				mLocExpressionBuildContainerList->detach();
				mLocBuildConceptSet->detach();
				return this;
			}




		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude