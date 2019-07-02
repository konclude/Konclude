/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_PREPROCESS_CBRANCHTRIGGERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CBRANCHTRIGGERPREPROCESS_H

// Libraries includes

// Namespace includes
#include "CConcreteOntologyPreProcess.h"
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
			class CBranchTriggerPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CBranchTriggerPreProcess();

					//! Destructor
					virtual ~CBranchTriggerPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:
					CConceptRoleBranchingTrigger *createBranchingTriggers(CConcept* concept, bool negated);

					bool addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated);
					CConcept* getRoleDomainTriggerConcept(CRole* role);
					CConcept* createTriggerConcept();

				// protected variables
				protected:
					CMemoryAllocationManager* mMemMan;
					cint64 mStatConceptBranchTriggers;
					cint64 mStatRoleBranchTriggers;

					cint64 mStatBranchTriggers;

					CConcreteOntology* mOnto;
					CConceptVector* mConceptVec;
					CTBox* mTBox;
					QHash<CRole*,CConcept*>* mRoleDomainTriggerConceptHash;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CBRANCHTRIGGERPREPROCESS_H
