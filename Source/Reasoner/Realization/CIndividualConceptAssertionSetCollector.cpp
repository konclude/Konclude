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

#include "CIndividualConceptAssertionSetCollector.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {


			CIndividualConceptAssertionSetCollector::CIndividualConceptAssertionSetCollector(CIndividual* indi, QSet< QPair<CIndividual*,CConcept*> >* assSet) {
				mIndi = indi;
				mAssSet = assSet;
			}

			bool CIndividualConceptAssertionSetCollector::visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization) {
				return conRealization->visitConcepts(item,this);
			}

			bool CIndividualConceptAssertionSetCollector::visitConcept(CConcept* concept, CConceptRealization* conRealization) {
				mAssSet->insert(QPair<CIndividual*,CConcept*>(mIndi,concept));
				return true;
			}

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude
