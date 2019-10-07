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

#ifndef KONCLUDE_REASONER_PREPROCESS_CBRANCHTRIGGERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CBRANCHTRIGGERPREPROCESS_H

// Libraries includes

// Namespace includes
#include "CConcreteOntologyContinuablePreProcess.h"
#include "CConceptRoleIndividualLocator.h"


// Other includes
#include "Reasoner/Ontology/CConceptRoleBranchingTrigger.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CBranchTriggerPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CBranchTriggerPreProcess : public CConcreteOntologyContinuablePreProcess {
				// public methods
				public:
					//! Constructor
					CBranchTriggerPreProcess();

					//! Destructor
					virtual ~CBranchTriggerPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);
					virtual CConcreteOntology* continuePreprocessing();

				// protected methods
				protected:
					CBranchTriggerPreProcess* createBranchingTriggers();


					CConceptRoleBranchingTrigger *createBranchingTriggers(CConcept* concept, bool negated);

					bool addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated);
					CConcept* getRoleDomainTriggerConcept(CRole* role, bool createIfNotExists = false);
					CConcept* createTriggerConcept();

				// protected variables
				protected:
					CMemoryAllocationManager* mMemMan;
					cint64 mStatConceptBranchTriggers;
					cint64 mStatRoleBranchTriggers;

					cint64 mStatBranchTriggers;

					CConcreteOntology* mOnto;
					CConceptVector* mConceptVec;
					CRoleVector* mRoleVector;
					CTBox* mTBox;
					CRBox* mRBox;
					QHash<CRole*,CConcept*>* mRoleDomainTriggerConceptHash;

					cint64 mLastConceptId;

					bool mConfLocalizeRolesToGetTrigger;


				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CBRANCHTRIGGERPREPROCESS_H
