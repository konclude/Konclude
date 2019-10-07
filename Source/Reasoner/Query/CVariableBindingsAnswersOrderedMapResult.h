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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSORDEREDMAPRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSORDEREDMAPRESULT_H

// Libraries includes
#include "CVariableBindingsAnswersResult.h"
#include "CVariableBindingsAnswerResultMapOrderer.h"

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
			 *		\class		CVariableBindingsAnswersOrderedMapResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingsAnswersOrderedMapResult : public CVariableBindingsAnswersResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingsAnswersOrderedMapResult(const QStringList& varNames, CVariableBindingsAnswerResultMapOrdering* mapOrdering);

					//! Destructor
					virtual ~CVariableBindingsAnswersOrderedMapResult();



					virtual CVariableBindingsAnswersOrderedMapResult* addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality = 1);
					virtual CVariableBindingsAnswersOrderedMapResult* addResultVariableBindings(CVariableBindingsListAnswerResult* varBindings, cint64 cardinality = 1);
					virtual CVariableBindingsAnswersOrderedMapResult* setResultVariableBindingsList(const QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>& indiBindingSet);
					virtual QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>* getResult();

					virtual CVariableBindingsAnswersResultIterator* getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt = nullptr);


					virtual const QStringList& getVariableNames();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CVariableBindingsAnswersOrderedMapResult *otherQueryResult);
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);


					virtual cint64 getResultCount();
				// protected methods
				protected:

				// protected variables
				protected:
					CVariableBindingsAnswerResultMapOrdering* mMapOrdering;
					QList<CVariableBindingsAnswerResult*> mBindingsContainer;
					QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*> mOrderedBindingsMap;
					QStringList mVariableNames;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSORDEREDMAPRESULT_H
