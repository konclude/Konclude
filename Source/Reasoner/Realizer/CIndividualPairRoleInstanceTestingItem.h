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

#ifndef KONCLUDE_REASONER_REALIZER_CINDIVIDUALPAIRROLEINSTANCETESTINGITEM_H
#define KONCLUDE_REASONER_REALIZER_CINDIVIDUALPAIRROLEINSTANCETESTINGITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizingTestingItem.h"
#include "COptimizedKPSetIndividualItemPair.h"
#include "COptimizedKPSetRoleInstancesItem.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			/*! 
			 *
			 *		\class		CIndividualInstanceTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualPairRoleInstanceTestingItem : public CRealizingTestingItem {
				// public methods
				public:
					//! Constructor
					CIndividualPairRoleInstanceTestingItem(COntologyRealizingItem* preproItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItemPair individualItemPair, COntologyRealizingDynamicRequirmentProcessingData* procData);

					virtual REALIZINGTESTINGTYPE getRealizingTestingType();

					COptimizedKPSetRoleInstancesItem* getInstancesItem();
					COptimizedKPSetIndividualItemPair getIndividualItemPair();

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyRealizingItem* mOntologyPreproItem;
					COptimizedKPSetRoleInstancesItem* mInstancesItem;
					COptimizedKPSetIndividualItemPair mIndividualItemPair;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CINDIVIDUALPAIRROLEINSTANCETESTINGITEM_H
