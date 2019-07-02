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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCRITICALPREDECESSORROLECARDINALITYHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCRITICALPREDECESSORROLECARDINALITYHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CCriticalPredecessorRoleCardinalityData.h"


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
				 *		\class		CCriticalPredecessorRoleCardinalityHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCriticalPredecessorRoleCardinalityHash {
					// public methods
					public:
						//! Constructor
						CCriticalPredecessorRoleCardinalityHash(CProcessContext* context = nullptr);


						CCriticalPredecessorRoleCardinalityHash* initCriticalPredecessorRoleCardinalityHash();
						CCriticalPredecessorRoleCardinalityHash* copyCriticalPredecessorRoleCardinalityHash(CCriticalPredecessorRoleCardinalityHash* copyRoleSuccHash);


						CCriticalPredecessorRoleCardinalityHash* addCriticalPredecessorRoleCardinality(CRole* role, CConcept* unproblematicConcept, bool unproblematicNegation);
						
						CCriticalPredecessorRoleCardinalityData* getCriticalPredecessorRoleCardinalityData(CRole* role, bool forceCreation = false);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;
						CPROCESSHASH<CRole*,CCriticalPredecessorRoleCardinalityData*> mCriticalPredecessorRoleDataHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCRITICALPREDECESSORROLECARDINALITYHASH_H
