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

#ifndef KONCLUDE_REASONER_PREPROCESS_CCONCEPTTRIGGERLINKER_H
#define KONCLUDE_REASONER_PREPROCESS_CCONCEPTTRIGGERLINKER_H

// Libraries includes


// Namespace includes
#include "Reasoner/Ontology/CConcept.h"

// Other includes
#include "Utilities/Container/CSortedLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CConceptTriggerLinker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptTriggerLinker : public CSortedLinkerBase<CConceptTriggerLinker*,CConceptTriggerLinker> {
				// public methods
				public:
					//! Constructor
					CConceptTriggerLinker();

					CConceptTriggerLinker* initConceptTriggerLinker(CConcept* triggerConcept, cint64 triggerComplexity);

					CConcept* getTriggerConcept();
					cint64 getTriggerComplexity();
					CConceptTriggerLinker* setTriggerComplexity(cint64 complexity);

					bool operator<=(const CConceptTriggerLinker& conceptTriggerLinker);
					/*!
					 *	In how many conjunctions the trigger's conjunct occurs (for a merged pair, the rarer of the two).
					 *	With frequency ordering the chain of a definition starts with its rarest conjunct, so that a
					 *	node is unfolded into the implications of the few definitions sharing its rare conjuncts
					 *	instead of into one implication per definition sharing a common one. Unknown for triggers
					 *	that do not stem from a counted conjunct; those are ordered after the counted ones.
					 */
					cint64 getTriggerFrequency();
					CConceptTriggerLinker* setTriggerFrequency(cint64 frequency);
					static const cint64 UNKNOWN_TRIGGER_FREQUENCY = (cint64)1 << 60;
					//! set by the absorber from Konclude.Calculation.Preprocessing.FrequencyOrderedTriggerChains
					static bool sFrequencyOrdered;

				// protected methods
				protected:

				// private methods
				private:
					CConcept* mTriggerConcept;
					cint64 mTriggerComplexity;
					cint64 mTriggerFrequency;

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CCONCEPTTRIGGERLINKER_H
