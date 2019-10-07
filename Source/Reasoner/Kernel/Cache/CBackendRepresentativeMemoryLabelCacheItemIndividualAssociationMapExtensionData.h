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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALASSOCIATIONMAPEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALASSOCIATIONMAPEXTENSIONDATA_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItemExtensionData.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator.h"


// Other includes
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {



				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData : public CBackendRepresentativeMemoryLabelCacheItemExtensionData {
					// public methods
					public:

						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData(CBackendRepresentativeMemoryCacheContext* context);

						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* addIndividualIdAssociation(cint64 indiId, bool sameIndividualMerged);
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* removeIndividualIdAssociation(cint64 indiId, bool sameIndividualMerged);
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* addIndividualIdAssociation(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData);
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* removeIndividualIdAssociation(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData);


						cint64 getIndividualIdAssociationCount();
						bool visitIndividualIdAssociationsAscending(function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool visitBaseIndividuals = true, bool visitSameMergedIndividuals = true);
						bool visitIndividualIdAssociationsDescending(function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool visitBaseIndividuals = true, bool visitSameMergedIndividuals = true);
						bool visitIndividualIdAssociations(function<bool(cint64 indiId, bool sameIndividualMerged)> visitFunc, bool ascending = true, bool visitBaseIndividuals = true, bool visitSameMergedIndividuals = true);


						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator getIterator(bool ascending = true, bool visitBaseIndividuals = true, bool visitSameMergedIndividuals = true);
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator getIterator(cint64 cursorId, bool moveOverCursor = true, bool ascending = true, bool visitBaseIndividuals = true, bool visitSameMergedIndividuals = true);

					// protected methods
					protected:

					// protected variables
					protected:
						QMap<cint64, DummyValue> mBaseIndiAssoMap;
						QMap<cint64, DummyValue> mSameIndiMergedAssoMap;
						


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALASSOCIATIONMAPEXTENSIONDATA_H
