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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTPROCESSDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTPROCESSDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CConceptData.h"
#include "CUnsatisfiableCachingTags.h"
#include "CConceptRoleBranchingTrigger.h"
#include "CConceptReferenceLinking.h"
#include "CReplacementData.h"
#include "CBranchingStatistics.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CConceptProcessData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptProcessData : public CConceptData {
				// public methods
				public:
					//! Constructor
					CConceptProcessData();

					CConceptProcessData* initConceptProcessExtensionData(CConceptRoleBranchingTrigger* conceptRoleBranchTrigger, CReplacementData* replacementData);


					CConceptRoleBranchingTrigger* getConceptRoleBranchTrigger();
					bool hasConceptRoleBranchTrigger();
					CConceptProcessData* setConceptRoleBranchTrigger(CConceptRoleBranchingTrigger* conceptRoleBranchTrigger);


					CUnsatisfiableCachingTags* getUnsatisfiableCachingTags(bool conceptNegation);

					bool hasUnsatisfiableCachingTags(bool conceptNegation);


					CConceptProcessData* setUnsatisfiableCachingTags(bool conceptNegation, CUnsatisfiableCachingTags* tags);


					CReplacementData* getReplacementData();
					CConceptProcessData* setReplacementData(CReplacementData* replacementData);

					CConceptReferenceLinking* getConceptReferenceLinking();
					CConceptProcessData* setConceptReferenceLinking(CConceptReferenceLinking* refLinking);

					bool isInvalidatedReferenceLinking();
					CConceptProcessData* setInvalidatedReferenceLinking(bool invalidatedReferenceLinking);

					bool isCoreBlockingConcept(bool negated);
					CConceptProcessData* setCoreBlockingConcept(bool negated, bool coreBlockingConcept = true);

					bool hasPropagationIntoCreationDirection();
					CConceptProcessData* setPropagationIntoCreationDirection(bool propagationIntoCreationDirection);

					bool hasInferRelevantFlag();
					CConceptProcessData* setInferRelevantFlag(bool flag);

					CBranchingStatistics* getBranchingStatistics();
					CConceptProcessData* setBranchingStatistics(CBranchingStatistics* branchingStatistics);


				// protected methods
				protected:

				// protected variables
				protected:
					CConceptRoleBranchingTrigger* mConceptRoleBranchTrigger;
					CReplacementData* mReplacementData;
					CUnsatisfiableCachingTags* mUnsatCachingTags[2];
					CConceptReferenceLinking* mRefLinking;
					bool mInvalidatedRefLinking;
					bool mPropagationIntoCreationDirection;
					bool mInferRelevantFlag;
					bool mCoreConceptFlags[2];
					CBranchingStatistics* mBranchingStatistics;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTPROCESSDATA_H
