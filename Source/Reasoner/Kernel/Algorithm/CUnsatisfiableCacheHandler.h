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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CUNSATISFIABLECACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CUNSATISFIABLECACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CClashDescriptorFactory.h"
#include "CTrackedClashedDescriptor.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData.h"

#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheReader.h"
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheWriter.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Cache;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CUnsatisfiableCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CUnsatisfiableCacheHandler {
					// public methods
					public:
						//! Constructor
						CUnsatisfiableCacheHandler(COccurrenceUnsatisfiableCacheReader* occurUnsatCacheReader, COccurrenceUnsatisfiableCacheWriter* occurUnsatCacheWriter);

						//! Destructor
						virtual ~CUnsatisfiableCacheHandler();


						bool isIndividualNodeUnsatisfiableCached(CIndividualProcessNode* individualNode, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContext* calcAlgContext);

						bool areClashedDescriptorsAlreadyCached(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContext* calcAlgContext);
						bool writeUnsatisfiableClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContext* calcAlgContext);
						bool areClashedDescriptorsLastCached(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:

					// protected variables
					protected:
						COccurrenceUnsatisfiableCacheReader* mOccurUnsatCacheReader;
						COccurrenceUnsatisfiableCacheWriter* mOccurUnsatCacheWriter;

						bool mConfConceptDataUnsatisfiablePrecheck;

						QList<CCacheValue> mUnsatItemList;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CUNSATISFIABLECACHEHANDLER_H
