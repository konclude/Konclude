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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELEXISTENTIALDATAROLEITEMITERATOR_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELEXISTENTIALDATAROLEITEMITERATOR_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedRepresentativeKPSetCacheLabelItemIterator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Cache;
		using namespace Realization;

		namespace Realizer {
			
			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator : public COptimizedRepresentativeKPSetCacheLabelItemIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true);

				// protected methods
				protected:
					
					CRealizationIndividualInstanceItemReferenceIterator* getCopy();
					CRealizationIndividualInstanceItemReferenceIterator* initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator);

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELEXISTENTIALDATAROLEITEMITERATOR_H
