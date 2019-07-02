/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_CLASSIFIER_CPARTIALPRUNINGTAXONOMY_H
#define KONCLUDE_REASONER_CLASSIFIER_CPARTIALPRUNINGTAXONOMY_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CPartialPruningHierarchyNode.h"
#include "CClassifierStatistics.h"

// Other includes
#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Config/CConfigDataReader.h"
#include "Config/CConfigurationBase.h"

#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Taxonomy;

		namespace Classifier {

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define DEBUGTESTTAXONOMYCONSITENCE

#endif

			/*! 
			 *
			 *		\class		CPartialPruningTaxonomy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPartialPruningTaxonomy : public CTaxonomy {
				// public methods
				public:
					//! Constructor
					CPartialPruningTaxonomy(CConcept *topConcept, CConcept *bottomConcept);

					//! Destructor
					virtual ~CPartialPruningTaxonomy();

					virtual CPartialPruningTaxonomy *readConfig(CConfigurationBase *config);

					virtual CTaxonomy *setConceptSatisfiable(CConcept *concept, bool satisfiable);
					virtual CTaxonomy *setConceptSubsumption(CConcept *subsumerConcept, CConcept *subsumedConcept);
					virtual CTaxonomy *setConceptNotSubsumption(CConcept *notSubsumerConcept, CConcept *notSubsumedConcept);

					virtual CTaxonomy *setConceptDisjoints(CConcept *disjointConcept1, CConcept *disjointConcept2);


					virtual CPartialPruningTaxonomy *createStatistics(CClassifierStatistics *nextClassificationStatistics);
					void testSubsumptionStatistics(qint64 subsumCount);

					virtual CTaxonomy *completeUnknownAsNonSubsumptions();

				// protected methods
				protected:
					virtual CHierarchyNode *createHierarchyNode(CConcept *concept);
					void testTaxonomyConsistence();



				// protected variables
				protected:
					CClassifierStatistics *statistics;
					bool confDerivateRelations;
					bool confPruneRelations;

#ifdef DEBUGTESTTAXONOMYCONSITENCE
					QString taxString;
#endif

					CClassifierStatistics *testCompStatistics;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CPARTIALPRUNINGTAXONOMY_H
