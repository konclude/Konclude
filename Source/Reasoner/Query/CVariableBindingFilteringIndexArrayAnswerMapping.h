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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERINGINDEXARRAYANSWERMAPPING_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERINGINDEXARRAYANSWERMAPPING_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CVariableBindingFilteringAnswerMapping.h"


// Other includes
#include "Utilities/Container/CLinker.h"
#include "Utilities/UtilitiesSettings.h"



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingFilteringIndexArrayAnswerMapping
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingFilteringIndexArrayAnswerMapping : public CVariableBindingFilteringAnswerMapping {
				// public methods
				public:
					//! Constructor
					CVariableBindingFilteringIndexArrayAnswerMapping(cint64 varIndexCount);

					virtual ~CVariableBindingFilteringIndexArrayAnswerMapping();

					cint64* getVariableIndexArray();
					cint64 getVariableIndexCount();


				// protected methods
				protected:

				// protected variables
				protected:
					cint64* mVarIndexArray;
					cint64 mVarIndexCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERINGINDEXARRAYANSWERMAPPING_H
