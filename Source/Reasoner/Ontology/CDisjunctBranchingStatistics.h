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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDISJUNCTBRANCHINGSTATISTICS_H
#define KONCLUDE_REASONER_ONTOLOGY_CDISJUNCTBRANCHINGSTATISTICS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CBranchingStatistics.h"


// Other includes
#include "Utilities/Container/CLinker.h"
#include "Utilities/CSortedNegLinker.hpp"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDisjunctBranchingStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDisjunctBranchingStatistics : public CBranchingStatistics, public CLinkerBase< CSortedNegLinker<CConcept*>*,CDisjunctBranchingStatistics > {
				// public methods
				public:
					//! Constructor
					CDisjunctBranchingStatistics();


					CDisjunctBranchingStatistics* initDisjunctBranchingStatistics(CSortedNegLinker<CConcept*>* disjunctLinker);

					CSortedNegLinker<CConcept*>* getDisjunctLinker();

					cint64 getExpandedCount();
					cint64 getClashInvolvedCount();
					cint64 getSatisfiableOccurrenceCount();


					CDisjunctBranchingStatistics* setExpandedCount(cint64 count);
					CDisjunctBranchingStatistics* setClashInvolvedCount(cint64 count);
					CDisjunctBranchingStatistics* setSatisfiableOccurrenceCount(cint64 count);


					CDisjunctBranchingStatistics* incExpandedCount(cint64 incCount = 1);
					CDisjunctBranchingStatistics* incClashInvolvedCount(cint64 incCount = 1);
					CDisjunctBranchingStatistics* incSatisfiableOccurrenceCount(cint64 incCount = 1);


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

					cint64 mExpandedCount;
					cint64 mClashesInvolvedCount;
					cint64 mSatisfiableOccurrenceCount;


			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDISJUNCTBRANCHINGSTATISTICS_H
