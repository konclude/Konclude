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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDISJUNCTIONBRANCHINGSTATISTICS_H
#define KONCLUDE_REASONER_ONTOLOGY_CDISJUNCTIONBRANCHINGSTATISTICS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CBranchingStatistics.h"
#include "CDisjunctBranchingStatistics.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDisjunctionBranchingStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDisjunctionBranchingStatistics : public CBranchingStatistics {
				// public methods
				public:
					//! Constructor
					CDisjunctionBranchingStatistics();

					cint64 getExpandedCount();
					cint64 getClashFullyInvolvedCount();
					cint64 getClashPartiallyInvolvedCount();
					cint64 getSatisfiableOccurrenceCount();
 
					CDisjunctionBranchingStatistics* initDisjunctionBranchingStatistics();


					CDisjunctionBranchingStatistics* setExpandedCount(cint64 count);
					CDisjunctionBranchingStatistics* setClashFullyInvolvedCount(cint64 count);
					CDisjunctionBranchingStatistics* setClashPartiallyInvolvedCount(cint64 count);
					CDisjunctionBranchingStatistics* setSatisfiableOccurrenceCount(cint64 count);


					CDisjunctionBranchingStatistics* incExpandedCount(cint64 incCount = 1);
					CDisjunctionBranchingStatistics* incClashFullyInvolvedCount(cint64 incCount = 1);
					CDisjunctionBranchingStatistics* incClashPartiallyInvolvedCount(cint64 incCount = 1);
					CDisjunctionBranchingStatistics* incSatisfiableOccurrenceCount(cint64 incCount = 1);


					CDisjunctBranchingStatistics* getDisjunctBranchingStatistics();
					CDisjunctionBranchingStatistics* addDisjunctBranchingStatistics(CDisjunctBranchingStatistics* disjunctBranchingStatistics);
					CDisjunctionBranchingStatistics* setDisjunctBranchingStatistics(CDisjunctBranchingStatistics* disjunctBranchingStatistics);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

					CDisjunctBranchingStatistics* mDisjunctBranchingStatistics;

					cint64 mExpandedCount;
					cint64 mClashesFullyInvolvedCount;
					cint64 mClashesPartiallyInvolvedCount;
					cint64 mSatisfiableOccurrenceCount;

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDISJUNCTIONBRANCHINGSTATISTICS_H
