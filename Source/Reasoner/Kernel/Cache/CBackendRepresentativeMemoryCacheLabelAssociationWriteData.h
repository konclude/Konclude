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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHELABELASSOCIATIONWRITEDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHELABELASSOCIATIONWRITEDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheWriteData.h"
#include "CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheLabelAssociationWriteData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheLabelAssociationWriteData : public CBackendRepresentativeMemoryCacheWriteData {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData();

						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* initWriteData(cint64 ontologyIdentifier, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker, CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker);


						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* getTemporaryAssociationWriteDataLinker();
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* getTemporaryLabelWriteDataLinker();
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* getTemporaryCardinaltyWriteDataLinker();
						CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* getTemporaryAssociationUseDataLinker();
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* getTemporaryNominalIndirectConnectionDataLinker();


					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* mTempAssWriteDataLinker;
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* mTempLabelWriteDataLinker;
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* mTempCardWriteDataLinker;
						CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* mTempAssUseDataLinker;
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* mTempNomIndirectConnDataLinker;

					// private methods
					private:

					// private variables
					private:


				};


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHELABELASSOCIATIONWRITEDATA_H
