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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CRBOX_H
#define KONCLUDE_REASONER_ONTOLOGY_CRBOX_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CRole.h"
#include "CRoleChain.h"
#include "CBoxContext.h"
#include "CConcreteOntologyContextBase.h"
#include "CRoleVector.h"
#include "CDataBox.h"
#include "CRoleChainVector.h"


// Other includes
#include "Utilities/CSortedNegLinker.hpp"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CRBox
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRBox : public CDataBox {
				// public methods
				public:
					//! Constructor
					CRBox(CBoxContext* boxContext = nullptr);

					//! Destructor
					virtual ~CRBox();


					CRBox *setRoleVector(CRoleVector* roleVec);
					CRoleVector* getRoleVector(bool create = true);

					CRBox *setRoleChainVector(CRoleChainVector* roleChainVec);
					CRoleChainVector* getRoleChainVector(bool create = true);

					qint64 getRoleCount();
					qint64 getRoleChainCount();

					virtual CRBox *referenceRBox(CRBox *rBox);
					virtual CRBox *getRBoxReferenceCopy(CBoxContext *boxContext);

					virtual CBoxContext *getBoxContext(bool create = true);


					CRole* getTopObjectRole();
					CRBox* setTopObjectRole(CRole* role);

					CRole* getBottomObjectRole();
					CRBox* setBottomObjectRole(CRole* role);

					cint64 getTopObjectRoleIndex();
					cint64 getBottomObjectRoleIndex();



					CRole* getTopDataRole();
					CRBox* setTopDataRole(CRole* role);

					CRole* getBottomDataRole();
					CRBox* setBottomDataRole(CRole* role);

					cint64 getTopDataRoleIndex();
					cint64 getBottomDataRoleIndex();

					CBOXSET<CRole*>* getActivePropertyRoleSet(bool create = true);
					CRBox* setActivePropertyRoleSet(CBOXSET<CRole*>* takeActiveRoleSet);


				// protected methods
				protected:

				// protected variables
				protected:
					CRoleVector* roles;
					CRoleChainVector* mRoleChainVec;

					CBoxContext* mBoxContext;
					CBoxContext* mDeleteBoxContext;
					CMemoryAllocationManager* mMemMan;

					cint64 mTopObjectRoleTag;
					cint64 mBottomObjectRoleTag;

					cint64 mTopDataRoleTag;
					cint64 mBottomDataRoleTag;

					CBOXSET<CRole*>* mActiveRoleSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CRBOX_H
