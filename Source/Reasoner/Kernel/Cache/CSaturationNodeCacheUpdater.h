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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODECACHEUPDATER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODECACHEUPDATER_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CSaturationNodeAssociatedExpansionCacheContext.h"

// Other includes
#include "Concurrent/CThread.h"


// Logger includes
#include "Logger/CLogger.h"


//#define KONCLUCE_SATISFIBALE_EXPANDER_CACHE_DIRECT_WRITING


namespace Konclude {

	using namespace Concurrent;
	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				using namespace Events;

				/*! 
				 *
				 *		\class		CSaturationNodeCacheUpdater
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeCacheUpdater {
					// public methods
					public:
						//! Constructor
						CSaturationNodeCacheUpdater(CSaturationNodeAssociatedExpansionCacheContext* context);

						CSaturationNodeCacheUpdater* propagateUnsatisfibility(CIndividualSaturationProcessNode* node, CSaturationNodeAssociatedExpansionCacheContext* context);

					// protected methods
					protected:
						CIndividualSaturationProcessNodeStatusUpdateLinker* createIndividualSaturationUpdateLinker(CSaturationNodeAssociatedExpansionCacheContext* context);
						void releaseIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker, CSaturationNodeAssociatedExpansionCacheContext* context);
						bool requiresDirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context);
						bool requiresIndirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context);


						void updateDirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context);
						void updateIndirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context);


					// protected variables
					protected:
						CSaturationNodeAssociatedExpansionCacheContext* mContext;

						cint64 mIndirectUpdatedStatusIndiNodeCount;
						cint64 mDirectUpdatedStatusIndiNodeCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODECACHEUPDATER_H
