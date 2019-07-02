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

#ifndef KONCLUDE_REASONER_CMBOX_H
#define KONCLUDE_REASONER_CMBOX_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CDataBox.h"
#include "CConceptDependence.h"
#include "CRoleDependence.h"
#include "CIndividualDependence.h"
#include "CConceptDependenceVector.h"
#include "CRoleDependenceVector.h"
#include "CIndividualDependenceVector.h"
#include "CConcreteOntologyContextBase.h"
#include "CConceptDataVector.h"
#include "CBranchingTriggerVector.h"
#include "CImplicationReplacementVector.h"
#include "CNominalSchemaTemplateVector.h"
#include "CDatatypeValueSpacesTriggers.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CMBox
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMBox : public CDataBox {
				// public methods
				public:
					//! Constructor
					CMBox(CBoxContext* boxContext = nullptr);

					//! Destructor
					virtual ~CMBox();

					CConceptDependenceVector *getConceptDependenceVector(bool create = true);
					CMBox *setConceptDependenceVector(CConceptDependenceVector *conceptDependenceVector);

					virtual CMBox *referenceMBox(CMBox *mBox);
					virtual CMBox *getMBoxReferenceCopy(CBoxContext *boxContext);

					CRoleDependenceVector *getRoleDependenceVector(bool create = true);
					CMBox *setRoleDependenceVector(CRoleDependenceVector *roleDependenceVector);

					CIndividualDependenceVector *getIndividualDependenceVector(bool create = true);
					CMBox *setIndividualDependenceVector(CIndividualDependenceVector *indiDependenceVector);


					CConceptDataVector* getConceptDataVector(bool create = true);
					CMBox *setConceptDataVector(CConceptDataVector* conceptDataVector);

					CBranchingTriggerVector* getBranchingTriggerVector(bool create = true);
					CMBox *setBranchingTriggerVector(CBranchingTriggerVector* branchTriggVector);

					CImplicationReplacementVector* getImplicationReplacementVector(bool create = true);
					CMBox *setImplicationReplacementVector(CImplicationReplacementVector* impRepVector);

					CNominalSchemaTemplateVector* getNominalSchemaTemplateVector(bool create = true);
					CMBox *setNominalSchemaTemplateVector(CNominalSchemaTemplateVector* nomSchTemplVector);


					CDatatypeValueSpacesTriggers* getValueSpacesTriggers(bool create = true);

					virtual CBoxContext *getBoxContext(bool create = true);

				// protected methods
				protected:

				// protected variables
				protected:
					CConceptDependenceVector *conceptDep;
					CRoleDependenceVector *roleDep;
					CIndividualDependenceVector *indiDep;

					CConceptDataVector* mConDataVector;
					CBranchingTriggerVector* mBranchTriggVector;
					CImplicationReplacementVector* mImpRepVector;
					CNominalSchemaTemplateVector* mNomSchemaTemplVector;

					CDatatypeValueSpacesTriggers* mValueSpacesTriggers;

					CBoxContext *mBoxContext;
					CBoxContext *mDeleteBoxContext;
					CMemoryAllocationManager* mMemMan;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CMBOX_H
