/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTROLEBRANCHINGTRIGGER_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTROLEBRANCHINGTRIGGER_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CConceptRoleBranchingTrigger
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptRoleBranchingTrigger : public CLinkerBase<CConceptRoleBranchingTrigger*,CConceptRoleBranchingTrigger> {
				// public methods
				public:
					//! Constructor
					CConceptRoleBranchingTrigger();

					CConceptRoleBranchingTrigger* initConceptBranchingTrigger(CConcept* triggerConcept, bool triggerNegation);
					CConceptRoleBranchingTrigger* initRoleBranchingTrigger(CRole *triggerRole);

					CConcept* getTriggerConcept();
					CRole* getTriggerRole();
					bool getTriggerNegation();

					bool isConceptTrigger();
					bool isRoleTrigger();

					enum BRANCHINGTRIGGERTYPE { BTTCONCEPT, BTTROLE };

					BRANCHINGTRIGGERTYPE getBranchingTriggerType();

					CConceptRoleBranchingTrigger* getNextBranchingTrigger();


				// protected methods
				protected:

				// protected variables
				protected:
					CRole* mTriggerRole;
					CConcept* mTriggerConcept;
					bool mTriggerNegation;
					bool mUsedConceptTrigger;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTROLEBRANCHINGTRIGGER_H
