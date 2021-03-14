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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETINDIVIDUALINSTANTIATEDITEMMULTIHASH_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETINDIVIDUALINSTANTIATEDITEMMULTIHASH_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetIndividualItem.h"

// Other includes

#include "Utilities/UtilitiesSettings.h"

#include "Reasoner/Ontology/CIndividual.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetIndividualInstantiatedItemMultiHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetIndividualInstantiatedItemMultiHash {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetIndividualInstantiatedItemMultiHash(cint64 concurrentMultiSize);
					~COptimizedKPSetIndividualInstantiatedItemMultiHash();


					QList<COptimizedKPSetIndividualItem*>* getIndividualInstantiatedItemContainerList();
					COptimizedKPSetIndividualItem* getIndividualInstantiatedItem(cint64 indiId, bool directCreate, CIndividual* individual);
					COptimizedKPSetIndividualItem* getIndividualInstantiatedItem(cint64 indiId);
					COptimizedKPSetIndividualInstantiatedItemMultiHash* updateIndividualInstantiatedItem(cint64 indiId, COptimizedKPSetIndividualItem* item);

					bool hasIndividualInstantiatedItem(cint64 indiId);

				// protected methods
				protected:

				// protected variables
				protected:
					QHash<cint64, COptimizedKPSetIndividualItem*>** mIndividualInstantiatedItemMultiHash;
					QList<COptimizedKPSetIndividualItem*>** mContainerMultiList;
					QList<COptimizedKPSetIndividualItem*>* mContainerList;
					cint64 mConcurrentMultiSize;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETINDIVIDUALINSTANTIATEDITEMMULTIHASH_H
