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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDISJOINTSUCCESSORROLEHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDISJOINTSUCCESSORROLEHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CNegationDisjointEdge.h"
#include "CDisjointSuccessorRoleIterator.h"
#include "CProcessContext.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDisjointSuccessorRoleHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDisjointSuccessorRoleHash {
					// public methods
					public:
						//! Constructor
						CDisjointSuccessorRoleHash(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CDisjointSuccessorRoleHash();

						CDisjointSuccessorRoleHash* initDisjointSuccessorRoleHash(CDisjointSuccessorRoleHash* prevDisjRoleHash);

						CDisjointSuccessorRoleHash* insertDisjointSuccessorRoleLink(cint64 succIndi, CNegationDisjointEdge* link);
						CDisjointSuccessorRoleHash* removeDisjointSuccessorRoleLinks(cint64 succIndi);

						CDisjointSuccessorRoleIterator getDisjointRoleIterator(cint64 succIndi);


						bool hasDisjointSuccessorRoleLink(cint64 succIndi, CRole* role);
						CNegationDisjointEdge* getDisjointSuccessorRoleLink(cint64 succIndi, CRole* role);



					// protected methods
					protected:
						CPROCESSHASH<CRole*,CNegationDisjointEdge*>* getNegDisRoleHash(cint64 successor, bool located = true);

					// protected variables
					protected:
						class CDisjointSuccessorRoleData {							
						public:
							inline CDisjointSuccessorRoleData() {
								mUseNegDisSet = nullptr;
								mLocNegDisSet = nullptr;
							}
							inline CDisjointSuccessorRoleData(const CDisjointSuccessorRoleData& disjointSuccRoleData) {
								mUseNegDisSet = disjointSuccRoleData.mUseNegDisSet;
								mLocNegDisSet = nullptr;
							}

							CPROCESSHASH<CRole*,CNegationDisjointEdge*> *mLocNegDisSet;
							CPROCESSHASH<CRole*,CNegationDisjointEdge*> *mUseNegDisSet;
						};



						CPROCESSHASH<cint64,CDisjointSuccessorRoleData> mSuccNegDisEdgeHash;
						CProcessContext* mContext;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDISJOINTSUCCESSORROLEHASH_H
