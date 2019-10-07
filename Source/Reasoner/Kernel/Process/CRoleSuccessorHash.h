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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CROLESUCCESSORHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CROLESUCCESSORHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualLinkEdge.h"
#include "CRoleSuccessorLinkIterator.h"
#include "CProcessContext.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CRoleSuccessorHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CRoleSuccessorHash {
					// public methods
					public:
						//! Constructor
						CRoleSuccessorHash(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CRoleSuccessorHash();

						CRoleSuccessorHash* initRoleSuccessorHash(CRoleSuccessorHash* prevRoleSuccHash);

						CRoleSuccessorHash* insertRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link);
						CRoleSuccessorHash* removeRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link);

						CRoleSuccessorLinkIterator getRoleSuccessorLinkIterator(CRole* role);


					// protected methods
					protected:
						cint64 getCoupledIndividualID(CIndividualLinkEdge* link);
						cint64 getCoupledIndividualID(cint64 indi1ID, cint64 indi2ID);

					// protected variables
					protected:
						class CRoleSuccessorData {							
							public:
								inline CRoleSuccessorData() {
									mLinkLinker = nullptr;
									mLocatedLinkSet = false;
									mLinkSet = nullptr;
								}
								inline CRoleSuccessorData(const CRoleSuccessorData& roleSuccData) {
									if (roleSuccData.mLinkSet) {
										mLinkLinker = nullptr;
										mLinkSet = roleSuccData.mLinkSet;
									} else {
										mLinkLinker = roleSuccData.mLinkLinker;
										mLinkSet = nullptr;
									}
									mLocatedLinkSet = false;
								}

								CPROCESSHASH<cint64,CIndividualLinkEdge*> *mLinkSet;
								CIndividualLinkEdge* mLinkLinker;
								bool mLocatedLinkSet;
						};


						CProcessContext* mContext;
						CPROCESSHASH<CRole*,CRoleSuccessorData> mRoleSuccessorDataHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CROLESUCCESSORHASH_H
