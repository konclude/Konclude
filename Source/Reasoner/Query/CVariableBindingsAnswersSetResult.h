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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSSETRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSSETRESULT_H

// Libraries includes
#include "CVariableBindingsAnswersResult.h"
#include "CVariableBindingsAnswerResultHasher.h"

// Namespace includes

// Other includes
#include <QList>

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingsAnswersSetResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingsAnswersSetResult : public CVariableBindingsAnswersResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingsAnswersSetResult(const QStringList& varNames);

					//! Destructor
					virtual ~CVariableBindingsAnswersSetResult();



					virtual CVariableBindingsAnswersSetResult* addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality = 1);
					virtual bool hasResultVariableBindings(CVariableBindingsAnswerResult* varBindings);
					virtual CVariableBindingsAnswersSetResult* setResultVariableBindingsList(const QSet<CVariableBindingsAnswerResultHasher>& indiBindingSet);
					virtual QSet<CVariableBindingsAnswerResultHasher>* getResult();

					virtual CVariableBindingsAnswersResultIterator* getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt = nullptr);


					virtual const QStringList& getVariableNames();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CVariableBindingsAnswersSetResult *otherQueryResult);
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);


					virtual cint64 getResultCount();
				// protected methods
				protected:

				// protected variables
				protected:
					QSet<CVariableBindingsAnswerResultHasher> mBindingsSet;
					QList<CVariableBindingsAnswerResult*> mBindingsContainer;
					QStringList mVariableNames;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSSETRESULT_H
