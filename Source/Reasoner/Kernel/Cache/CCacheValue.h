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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUE_H

// Libraries includes
#include <QHash>

// Namespace includes


// Other includes
#include "Utilities/CTrible.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CCacheValue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCacheValue : public CTrible<qint64> {
					// public methods
					public:
						enum CACHEVALUEIDENTIFIER { CACHEVALCONCEPTONTOLOGYTAG, CACHEVALNEGATEDCONCEPTONTOLOGYTAG, 
							CACHEVALROLEONTOLOGYTAG, CACHEVALUETERMINATOR, CACHEVALUENOTHING, CACHEVALTAGANDCONCEPT, CACHEVALTAGANDNEGATEDCONCEPT, CACHEVALTAGAND_NONDETERMINISTIC_CONCEPT, CACHEVALTAGAND_NONDETERMINISTIC_NEGATED_CONCEPT,
							CACHEVALTAGANDROLE, CACHEVALTAGANDINVERSEDROLE, CACHEVALTAGANDASSERTEDROLE, CACHEVALTAGANDINVERSEDASSERTEDROLE, CACHEVALTAGAND_NOMINAL_CONNECTED_ROLE, CACHEVALTAGANDINVERSED_NOMINAL_CONNECTED_ROLE,
							CACHEVALTAGAND_NONDETERMINISTIC_ROLE, CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE, CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE, CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE, CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE, CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE,
							CACHE_VALUE_INDIVIDUAL_ID, CACHE_VALUE_NEGATED_INDIVIDUAL_ID,
							CACHE_VALUE_TAG_AND_ENTRY, CACHE_VALUE_TAG_AND_TEMPORARY_ENTRY
						};

						//! Constructor
						CCacheValue();
						CCacheValue(const qint64 &first, const qint64 &second, const qint64 &third);
						CCacheValue(const qint64 &tag, const qint64 &identification, const CACHEVALUEIDENTIFIER &cacheValueIdentifier = CACHEVALCONCEPTONTOLOGYTAG);

						CCacheValue* initCacheValue(const qint64 &tag, const qint64 &identification, const CACHEVALUEIDENTIFIER &cacheValueIdentifier = CACHEVALCONCEPTONTOLOGYTAG);

						bool isCachedConcept();
						bool isCachedRole();
						bool isCachedTerminator();
						bool isCachedNothing();

						
						qint64 getTag() const;
						qint64 getIdentification() const;
						CACHEVALUEIDENTIFIER getCacheValueIdentifier() const;


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUE_H
