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

#ifndef KONCLUDE_REASONER_PREPROCESS_CCOREONTOLOGYCYCLESEXTRACTORPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CCOREONTOLOGYCYCLESEXTRACTORPREPROCESS_H

// Libraries includes


// Namespace includes
#include "CConcreteOntologyPreProcess.h"


// Other includes
#include "Reasoner/Ontology/OntologySettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {


			/*! 
			 *
			 *		\class		CCoreOntologyCyclesExtractorPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCoreOntologyCyclesExtractorPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CCoreOntologyCyclesExtractorPreProcess();

					//! Destructor
					virtual ~CCoreOntologyCyclesExtractorPreProcess();

					virtual CConcreteOntology* preprocess(CConcreteOntology* ontology, CPreProcessContext* context);

				// protected methods
				protected:
					bool hasConceptCycle(CConcept* concept, bool negated, QSet<TConceptNegPair>* visitedConceptSet, QSet<TConceptNegPair>* cylceConceptSet);

					bool addConceptsToList(CSortedNegLinker<CConcept*>* opLinker, bool negated, QList<TConceptNegPair>& nextTestingConceptList);

					bool getConceptCycle(QSet<TConceptNegPair>* visitedConceptSet, QSet<TConceptNegPair>* cylceConceptSet, bool* cycleNominalConnection);

				// protected variables
				protected:
					class CTestingItem {
						public:
							CTestingItem(CConcept* concept = nullptr, bool negated = false, cint64 searchingDepth = 0, CTestingItem* prevItem = nullptr) : mConNegPair(concept,negated),mSearchingDepth(searchingDepth),mPrevItem(prevItem) {
							}
							CTestingItem(const TConceptNegPair& conNegPair, cint64 searchingDepth = 0, CTestingItem* prevItem = nullptr) : mConNegPair(conNegPair),mSearchingDepth(searchingDepth),mPrevItem(prevItem) {
							}
							TConceptNegPair mConNegPair;
							cint64 mSearchingDepth;
							CTestingItem* mPrevItem;
					};

					cint64 mStatExtractedCyclesCount;
					cint64 mStatCyclicConceptsCount;

					QSet<TConceptNegPair> mCycleConceptSet;


				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CCOREONTOLOGYCYCLESEXTRACTORPREPROCESS_H
