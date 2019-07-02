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

					CRole* getTopRole();
					CRBox* setTopRole(CRole* role);

					CRole* getBottomRole();
					CRBox* setBottomRole(CRole* role);


				// protected methods
				protected:

				// protected variables
				protected:
					CRoleVector* roles;
					CRoleChainVector* mRoleChainVec;

					CBoxContext *mBoxContext;
					CBoxContext *mDeleteBoxContext;
					CMemoryAllocationManager* mMemMan;

					cint64 mTopRoleTag;
					cint64 mBottomRoleTag;

					bool mTopRoleReferenced;
					bool mBottomRoleReferenced;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CRBOX_H
