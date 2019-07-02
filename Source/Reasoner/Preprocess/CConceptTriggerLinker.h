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

				// protected methods
				protected:

				// private methods
				private:
					CConcept* mTriggerConcept;
					cint64 mTriggerComplexity;

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CCONCEPTTRIGGERLINKER_H
