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

#include "CAnsweringMessageDataCalculationClassSubsumers.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringMessageDataCalculationClassSubsumers::CAnsweringMessageDataCalculationClassSubsumers() : CAnsweringMessageDataCalculationAdapter(CAnsweringMessageDataCalculationAdapter::CLASSSUBSUMERS) {
			}


			CAnsweringMessageDataCalculationClassSubsumers* CAnsweringMessageDataCalculationClassSubsumers::initAnsweringSubsumptionMessageData(CConcept* subsumedConcept, bool subsumedConceptNegation, CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList) {
				mSubsumedConcept = subsumedConcept;
				mSubsumerList = subsumerList;
				mSubsumedConceptNegation = subsumedConceptNegation;
				return this;
			}

			CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* CAnsweringMessageDataCalculationClassSubsumers::getClassSubsumerList() {
				return mSubsumerList;
			}


			CConcept* CAnsweringMessageDataCalculationClassSubsumers::getSubsumedConcept() {
				return mSubsumedConcept;
			}

			bool CAnsweringMessageDataCalculationClassSubsumers::getSubsumedConceptNegation() {
				return mSubsumedConceptNegation;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
