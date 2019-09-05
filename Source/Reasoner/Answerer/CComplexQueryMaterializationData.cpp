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

#include "CComplexQueryMaterializationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexQueryMaterializationData::CComplexQueryMaterializationData() {
				mTemporaryMaterializationOntology = nullptr;
			}


			CComplexQueryMaterializationData::~CComplexQueryMaterializationData() {
			}



			CIndividual* CComplexQueryMaterializationData::getVariableIndividual(CExpressionVariable* variable) {
				CIndividual*& indi = mVariableIndividualHash[variable];

				if (!indi) {
					CABox* tempMatAbox = mTemporaryMaterializationOntology->getABox();
					CIndividualVector* indiVec = tempMatAbox->getIndividualVector(true);

					CIndividual* tmpIndi = new CIndividual();
					tmpIndi->initIndividual(tempMatAbox->getNextIndividualId(true));
					indiVec->setData(tmpIndi->getIndividualID(), tmpIndi);
					tmpIndi->setTemporaryFakeIndividual(true);

					indi = tmpIndi;

					mIndividualVariableHash.insert(indi, variable);
				}

				return indi;
			}


			CExpressionVariable* CComplexQueryMaterializationData::getVariable(CIndividual* indi) {
				return mIndividualVariableHash.value(indi);
			}



			QList<CIndividual*> CComplexQueryMaterializationData::getVariableIndividualList() {
				return mVariableIndividualHash.values();
			}


			QHash<CExpressionVariable*, CConcept*>* CComplexQueryMaterializationData::getVariableMaterializedConceptsHash() {
				return &mVariableMaterializedConceptsHash;
			}


			CConcreteOntology* CComplexQueryMaterializationData::getTemporaryMaterializationOntology() {
				return mTemporaryMaterializationOntology;
			}

			CComplexQueryMaterializationData* CComplexQueryMaterializationData::setTemporaryMaterializationOntology(CConcreteOntology* tmpOnto) {
				mTemporaryMaterializationOntology = tmpOnto;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
