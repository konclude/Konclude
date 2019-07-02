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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEEXPANSIONWRITEDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEEXPANSIONWRITEDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CSaturationNodeAssociatedExpansionCacheWriteData.h"
#include "CSaturationNodeAssociatedConceptLinker.h"
#include "CSaturationNodeAssociatedDependentNominalSet.h"


// Other includes
#include "Utilities/Memory/CMemoryPool.h"
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Cache {


				/*! 
				 *
				 *		\class		CSaturationNodeAssociatedExpansionCacheExpansionWriteData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeAssociatedExpansionCacheExpansionWriteData : public CSaturationNodeAssociatedExpansionCacheWriteData {
					// public methods
					public:
						//! Constructor
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData();

						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* initExpansionWriteData(CIndividualSaturationProcessNode* saturationNode, CSaturationNodeAssociatedConceptLinker* conLinker);

						CIndividualSaturationProcessNode* getSaturationIndividualNode();

						CSaturationNodeAssociatedDependentNominalSet* getDependentNominalSet();
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* setDependentNominalSet(CSaturationNodeAssociatedDependentNominalSet* depNomSet);


						CSaturationNodeAssociatedConceptLinker* getExpansionConceptLinker();
						bool hasTightAtMostRestriction();
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* setTightAtMostRestriction(bool tightAtMostRestriction);


						bool isDeterministicExpansion();
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* setDeterministicExpansion(bool detExpansion);

						bool requiresNondeterministicExpansion();
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* setRequiresNondeterministicExpansion(bool requiresnondetExpansion);
						
						cint64 getConceptSetSignature();
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* setConceptSetSignature(cint64 signature);

						cint64 getTotalConceptCount();
						CSaturationNodeAssociatedExpansionCacheExpansionWriteData* setTotalConceptCount(cint64 conceptCount);

					// protected methods
					protected:

					// protected variables
					protected:
						CIndividualSaturationProcessNode* mSaturationNode;
						bool mTightAtMostRestriction;
						bool mDeterministicExpansion;

						bool mRequiresNondeterministicExpansion;
						cint64 mConceptSetSignature;
						cint64 mTotalConceptCount;

						CSaturationNodeAssociatedDependentNominalSet* mDependentNominalSet;
						CSaturationNodeAssociatedConceptLinker* mExpansionConceptLinker;



					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEEXPANSIONWRITEDATA_H
