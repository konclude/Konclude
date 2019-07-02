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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDCONCEPTEXPANSION_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDCONCEPTEXPANSION_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CSaturationNodeAssociatedExpansionCacheContext.h"
#include "CSaturationNodeAssociatedConceptLinker.h"
#include "CSaturationNodeAssociatedDependentNominalSet.h"
#include "CCacheValueHasher.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CSaturationNodeAssociatedConceptExpansion
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeAssociatedConceptExpansion {
					// public methods
					public:
						//! Constructor
						CSaturationNodeAssociatedConceptExpansion(CSaturationNodeAssociatedExpansionCacheContext* context);

						CSaturationNodeAssociatedConceptExpansion* initConceptExpansion();

						CCACHINGHASH<CCacheValueHasher,CSaturationNodeAssociatedConceptLinker*>* getConceptExpansionHash();

						CSaturationNodeAssociatedConceptExpansion* addConceptExpansionLinker(CSaturationNodeAssociatedConceptLinker* conceptLinker);
						bool hasConceptExpansionLinker(CCacheValue* cacheValue);
						CSaturationNodeAssociatedConceptLinker* getConceptExpansionLinker(CCacheValue* cacheValue);
						CSaturationNodeAssociatedConceptLinker* getConceptExpansionLinker();
						cint64 getConceptExpansionCount();
						CSaturationNodeAssociatedConceptExpansion* setConceptExpansionCount(cint64 count);


						CSaturationNodeAssociatedDependentNominalSet* getDependentNominalSet(bool create = true);

						bool getHasTightAtMostRestriction();
						CSaturationNodeAssociatedConceptExpansion* setHasTightCardinalityRestriction(bool tightAtMostRestrictions);

						cint64 getConceptSetSignature();
						CSaturationNodeAssociatedConceptExpansion* setConceptSetSignature(cint64 signature);

						cint64 getTotalConceptCount();
						CSaturationNodeAssociatedConceptExpansion* setTotalConceptCount(cint64 totalConceptCount);

					// protected methods
					protected:

					// protected variables
					protected:

						CCACHINGHASH<CCacheValueHasher,CSaturationNodeAssociatedConceptLinker*> mConceptExpansionHash;
						CSaturationNodeAssociatedConceptLinker* mConceptExpansionLinker;
						cint64 mConceptExpansionCount;

						CSaturationNodeAssociatedDependentNominalSet* mDependentNominalSet;
						bool mHasTightCardinalityRestriction;
						cint64 mConceptSetSignature;
						cint64 mTotalConceptCount;

						CSaturationNodeAssociatedExpansionCacheContext* mContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDCONCEPTEXPANSION_H
