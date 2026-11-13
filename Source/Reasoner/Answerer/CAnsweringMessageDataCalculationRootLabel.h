/*
 *		Copyright (C) 2013-2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONROOTLABEL_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONROOTLABEL_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageDataCalculationAdapter.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {

			/*!
			 *
			 *		\class		CRootLabelEntry
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		One concept of the root node of a completion graph: with its polarity and
			 *					whether it was derived without a choice.
			 *
			 */
			class CRootLabelEntry {
				public:
					CRootLabelEntry() : mConcept(nullptr), mNegated(false), mDeterministic(false) {}
					CRootLabelEntry(CConcept* concept, bool negated, bool deterministic) : mConcept(concept), mNegated(negated), mDeterministic(deterministic) {}
					CConcept* mConcept;
					bool mNegated;
					bool mDeterministic;
			};


			/*!
			 *
			 *		\class		CAnsweringMessageDataCalculationRootLabel
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		The label of the root node of a satisfiable completion graph of a concept,
			 *					with which the saturation decider completes a saturation that is not
			 *					reliable, see CSaturationSubsumptionDecider.
			 *
			 */
			class CAnsweringMessageDataCalculationRootLabel : public CAnsweringMessageDataCalculationAdapter {
				// public methods
				public:
					//! Constructor
					CAnsweringMessageDataCalculationRootLabel();

					CAnsweringMessageDataCalculationRootLabel* initRootLabelMessageData(CConcept* testedConcept, bool testedConceptNegation, CCLASSSUBSUMPTIONMESSAGELIST<CRootLabelEntry>* entryList);

					CCLASSSUBSUMPTIONMESSAGELIST<CRootLabelEntry>* getEntryList();
					CConcept* getTestedConcept();
					bool getTestedConceptNegation();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mTestedConcept;
					bool mTestedConceptNegation;
					CCLASSSUBSUMPTIONMESSAGELIST<CRootLabelEntry>* mEntryList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONROOTLABEL_H
