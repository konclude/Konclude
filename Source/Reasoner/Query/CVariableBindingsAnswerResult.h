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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERRESULT_H

// Libraries includes
#include "CQueryResult.h"
#include "CVariableBindingResult.h"
#include "CVariableBindingsResultIterator.h"

// Namespace includes

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingsAnswerResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingsAnswerResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingsAnswerResult();


					virtual CVariableBindingsAnswerResult* addResultVariableBinding(CVariableBindingResult* varBinding) = 0;

					virtual CVariableBindingsResultIterator* getVariableBindingsIterator(CVariableBindingsResultIterator* reuseIt = nullptr) = 0;

					virtual cint64 getHashCode() = 0;

					virtual CVariableBindingsAnswerResult* getCopy() = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERRESULT_H