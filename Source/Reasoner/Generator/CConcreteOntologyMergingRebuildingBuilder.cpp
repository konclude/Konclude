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

#include "CConcreteOntologyMergingRebuildingBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyMergingRebuildingBuilder::CConcreteOntologyMergingRebuildingBuilder(CConcreteOntology* baseConcreteOntology) : CConcreteOntologyRebuildingBuilder(baseConcreteOntology) {
			}

			CConcreteOntologyMergingRebuildingBuilder::~CConcreteOntologyMergingRebuildingBuilder() {
			}




			bool CConcreteOntologyMergingRebuildingBuilder::mergeOntology(CConcreteOntology* mergingConcreteOntology, CConcreteOntology* prevConcreteOntology) {



				cint64 nextNewPrefix = prevConcreteOntology->getStringMapping()->getPrefixList()->count();
				CMAPPINGLIST<CNamePrefix*>* mergePrefixList = mergingConcreteOntology->getStringMapping()->getPrefixList();
				while (nextNewPrefix < mergePrefixList->count()) {
					CNamePrefix* namePrefix = (*mergePrefixList)[nextNewPrefix++];
					mAbbreviatedNamePrefixMapHash->insert(namePrefix->getAbbreviatedPrefixString(), namePrefix);
					mNamePrefixMapHash->insert(namePrefix->getPrefixString(), namePrefix);
				}


				cint64 nextNewAxiom = prevConcreteOntology->getBuildData()->getChangeAxiomList()->count();
				CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*, bool> >* changeUpdatedAxiomList = mergingConcreteOntology->getBuildData()->getChangeAxiomList();
				while (nextNewAxiom < changeUpdatedAxiomList->count()) {
					QPair<CAxiomExpression*, bool> changedAxiomPair((*changeUpdatedAxiomList)[nextNewAxiom++]);
					CAxiomExpression* prevAxiom = changedAxiomPair.first;
					CAxiomExpression* rebuildAxiom = (CAxiomExpression*)rebuildExpression(prevAxiom);
					bool addedAxiom = changedAxiomPair.second;
					if (addedAxiom) {
						tellOntologyAxiom(rebuildAxiom);
					} else {
						retractOntologyAxiom(rebuildAxiom);
					}
				}

				return true;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude