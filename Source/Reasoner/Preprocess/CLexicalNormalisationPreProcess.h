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

#ifndef KONCLUDE_REASONER_PREPROCESS_CLEXICALNORMALISATIONPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CLEXICALNORMALISATIONPREPROCESS_H

// Libraries includes


// Namespace includes
#include "CConcreteOntologyContinuablePreProcess.h"
#include "CConceptUnambiguousHasher.h"


// Other includes
#include "Utilities/CHashableIntConvertedNTuble.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDynamicExpandingMemoryManager.hpp"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CLexicalNormalisationPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		Not thread safe
			 *
			 */
			class CLexicalNormalisationPreProcess : public CConcreteOntologyContinuablePreProcess {
				// public methods
				public:
					//! Constructor
					CLexicalNormalisationPreProcess();

					//! Destructor
					virtual ~CLexicalNormalisationPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);
					virtual CConcreteOntology* continuePreprocessing();


				// protected methods
				protected:
					void negateOperandConcepts(CConcept *concept, int firstNegationOperand = 0);
					void addNormalizeConcept(CConcept *concept);
					void normalizeListedConcepts();
					void normalizeConcept(CConcept *concept, bool isLocal);

					CConceptUnambiguousHasher getConceptHasher(CConcept *concept);
					CSortedNegLinker<CConcept*>* resortConceptLinkersByTagging(CSortedNegLinker<CConcept*>* conceptLinkers);

				// private methods
				private:

				// private variables
				private:
					CConcept *topConcept;
					QHash<CConceptUnambiguousHasher,CConcept*>* replacementHash;
					QHash< CConcept*,QPair<bool,CConcept*> >* conceptReplaceHash;
					QSet<CConcept*>* processedSet; 
					QList<CConcept*>* processingList;

					CMemoryAllocationManager* mBoxMemMan;

					CConceptVector *concepts;
					CRoleVector *roles;
					CIndividualVector *individuals;


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CLEXICALNORMALISATIONPREPROCESS_H
