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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSLISTANSWERRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSLISTANSWERRESULT_H

// Libraries includes
#include "CVariableBindingsAnswerResult.h"

// Namespace includes

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingsListAnswerResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingsListAnswerResult : public CVariableBindingsAnswerResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingsListAnswerResult();
					CVariableBindingsListAnswerResult(cint64 count);
					CVariableBindingsListAnswerResult(const CVariableBindingsListAnswerResult& varListResult);

					//! Destructor
					virtual ~CVariableBindingsListAnswerResult();

					CVariableBindingsListAnswerResult* clearResultVariableBindings();

					virtual CVariableBindingsListAnswerResult* addResultVariableBinding(CVariableBindingResult* varBinding);
					virtual CVariableBindingsListAnswerResult* setResultVariableBindings(const QList<CVariableBindingResult*>& bindingsList);
					virtual QList<CVariableBindingResult*>* getResult();
					CVariableBindingResult* getResult(cint64 index);
					CVariableBindingsListAnswerResult* setResult(cint64 index, CVariableBindingResult* varBinding);

					virtual CVariableBindingsResultIterator* getVariableBindingsIterator(CVariableBindingsResultIterator* reuseIt = nullptr);


					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CVariableBindingsListAnswerResult *otherQueryResult);
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					virtual cint64 getHashCode();

					virtual CVariableBindingsAnswerResult* getCopy();


				// protected methods
				protected:

				// protected variables
				protected:
					QList<CVariableBindingResult*> mBindingsList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSLISTANSWERRESULT_H
