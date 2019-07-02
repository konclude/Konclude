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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CDatatypeValueTestingCollection_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CDatatypeValueTestingCollection_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Context/CContextBase.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CDatatypeValueTestingCollection
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeValueTestingCollection {
					// public methods
					public:
						//! Constructor
						CDatatypeValueTestingCollection(CCalculationAlgorithmContext* collectionCalcAlgContext);


						CDatatypeValueTestingCollection* addDataLiteralValueUse(CDataLiteralValue* dataLiteralValue, CDatatypeValueSpaceType* valueSpaceType);
						bool isDataLiteralValueUsed(CDataLiteralValue* dataLiteralValue, CDatatypeValueSpaceType* valueSpaceType);
						CDatatypeValueTestingCollection* releaseUsedDataLiteralValues();


						CDataLiteralValue* createDataLiteralValue(CDatatypeValueSpaceType* valueSpaceType);
						CDatatypeValueTestingCollection* releaseDataLiteralValue(CDataLiteralValue* dataLiteralValue, CDatatypeValueSpaceType* valueSpaceType);

					// protected methods
					protected:
						CXLinker<CDataLiteralValue*>* createFreeLinker();
						CDatatypeValueTestingCollection*  releaseFreeLinker(CXLinker<CDataLiteralValue*>* linker);


					// protected variables
					protected:
						CCalculationAlgorithmContext* mCollectionCalcAlgContext;


						CXLinker<CDataLiteralValue*>* mFreeLinker;
						CXLinker<CDataLiteralValue*>* mFreeDataLiteralValueVec[CDatatypeValueSpaceType::VALUESPACETYPECOUNT];


						CXLinker<CDataLiteralValue*>* mUseDataLiteralValueVec[CDatatypeValueSpaceType::VALUESPACETYPECOUNT];


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CDatatypeValueTestingCollection_H
