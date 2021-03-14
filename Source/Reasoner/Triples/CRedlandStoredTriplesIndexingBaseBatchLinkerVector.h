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

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKERVECTOR_H
#define KONCLUDE_REASONER_ANSWERER_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKERVECTOR_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"

#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CObjectAllocator.h"

#include "Utilities/UtilitiesSettings.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Context;
	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Ontology;

		namespace Triples {


			/*! 
				*
				*		\class		CRedlandStoredTriplesIndexingBaseBatchLinkerVector
				*		\author		Andreas Steigmiller
				*		\version	0.1
				*		\brief		TODO
				*
				*/
			template<class T>
			class CRedlandStoredTriplesIndexingBaseBatchLinkerVector : public QVector<CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>*>, public CMemoryPoolNewAllocationIncreasingContext {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndexingBaseBatchLinkerVector(cint64 size);
					CRedlandStoredTriplesIndexingBaseBatchLinkerVector();


					CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>* getVectorLinkerData(cint64 index, bool createIfNotExists = true);


					CRedlandStoredTriplesIndexingBaseBatchLinkerVector* addBatchLinker(cint64 index, T* linker);


					T* createBatchLinker();

					cint64 getNextIndex(bool moveNext = true);


					CXLinker<CIndividual*>* getIndvidualCreationLinker() const;
					CRedlandStoredTriplesIndexingBaseBatchLinkerVector* addCreationIndvidual(CIndividual* indi);


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mNextIndex;
					cint64 mSize;

					CXLinker<CIndividual*>* mIndiCreationLinker;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CRedlandStoredTriplesIndexingBaseBatchLinkerVector.cpp"

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKERVECTOR_H
