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

#include "CCriticalIndividualNodeConceptTestSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CCriticalIndividualNodeConceptTestSet::CCriticalIndividualNodeConceptTestSet(CProcessContext* processContext) 
						: mProcessContext(processContext),mIndiConceptTestedSet(processContext) {

				}

				CCriticalIndividualNodeConceptTestSet* CCriticalIndividualNodeConceptTestSet::initIndividualNodeConceptTestSet(CCriticalIndividualNodeConceptTestSet* processingQueue) {
					if (processingQueue) {
						mIndiConceptTestedSet = processingQueue->mIndiConceptTestedSet;
					} else {
						mIndiConceptTestedSet.clear();
					}
					return this;
				}

				bool CCriticalIndividualNodeConceptTestSet::isConceptTestedForIndividual(CConcept* concept, CIndividualSaturationProcessNode* individual) {
					return mIndiConceptTestedSet.contains( QPair<CIndividualSaturationProcessNode*,CConcept*>(individual,concept) );
				}

				bool CCriticalIndividualNodeConceptTestSet::isConceptTestedForIndividual(CConceptSaturationDescriptor* criticalConDes, CIndividualSaturationProcessNode* individual) {
					return mIndiConceptTestedSet.contains( QPair<CIndividualSaturationProcessNode*,CConcept*>(individual,criticalConDes->getConcept()) );
				}

				CCriticalIndividualNodeConceptTestSet* CCriticalIndividualNodeConceptTestSet::insertConceptTestedForIndividual(CConcept* concept, CIndividualSaturationProcessNode* individual) {
					mIndiConceptTestedSet.insert( QPair<CIndividualSaturationProcessNode*,CConcept*>(individual,concept) );
					return this;
				}

				CCriticalIndividualNodeConceptTestSet* CCriticalIndividualNodeConceptTestSet::insertConceptTestedForIndividual(CConceptSaturationDescriptor* criticalConDes, CIndividualSaturationProcessNode* individual) {
					mIndiConceptTestedSet.insert( QPair<CIndividualSaturationProcessNode*,CConcept*>(individual,criticalConDes->getConcept()) );
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
