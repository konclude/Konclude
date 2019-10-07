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

#ifndef KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALSORTING_H
#define KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALSORTING_H

// Libraries includes

// Namespace includes
#include "RealizationSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CRealizationIndividualSorting
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizationIndividualSorting {
				// public methods
				public:
					enum SORTING_TYPE {
						SORT_TYPE_NOTHING,
						SORT_TYPE_INDIVIDUAL_ID,
					};

					enum SORTING_GROUPING {
						SORT_GROUP_NOTHING,
						SORT_GROUP_SAME_UNIQUE,
					};

					enum SORTING_ORDER {
						SORT_ORDER_NOTHING,
						SORT_ORDER_ASCENDING,
						SORT_ORDER_DESCENDING,
					};

					//! Constructor
					CRealizationIndividualSorting(SORTING_TYPE type = SORT_TYPE_NOTHING, SORTING_ORDER order = SORT_ORDER_NOTHING, SORTING_GROUPING grouping = SORT_GROUP_NOTHING);


					SORTING_TYPE getType() const;
					SORTING_ORDER getOrder() const;
					SORTING_GROUPING getGrouping() const;


					bool isNothing() const;

				// protected methods
				protected:

				// protected variables
				protected:
					SORTING_TYPE mType;
					SORTING_ORDER mOrder;
					SORTING_GROUPING mGrouping;

				// private methods
				private:

				// private variables
				private:

			};




		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALSORTING_H
