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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSSTREAMINGRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSSTREAMINGRESULT_H

// Libraries includes
#include "CVariableBindingsAnswersResult.h"
#include "CVariableBindingsAnswersStreamingHandler.h"

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
			 *		\class		CVariableBindingsAnswersStreamingResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingsAnswersStreamingResult : public CVariableBindingsAnswersResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingsAnswersStreamingResult(CVariableBindingsAnswersStreamingHandler* handler);
					CVariableBindingsAnswersStreamingResult(CVariableBindingsAnswersConcurrentStreamingHandler* concurrentHandler);

					//! Destructor
					virtual ~CVariableBindingsAnswersStreamingResult();


					bool initResult(const QStringList& varNames);


					virtual CVariableBindingsAnswersStreamingResult* getConcurrentlyWriteableStreamingResults();
					virtual CVariableBindingsAnswersStreamingResult* releaseConcurrentlyWriteableStreamingResults(CVariableBindingsAnswersStreamingResult* result);


					virtual CVariableBindingsAnswersStreamingResult* addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality = 1);
					virtual CVariableBindingsAnswersResult* addReusedResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality = 1);

					virtual bool flushResults();
					

					virtual CVariableBindingsAnswersResultIterator* getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt = nullptr);


					virtual const QStringList& getVariableNames();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					virtual cint64 getResultCount();

				// protected methods
				protected:

				// protected variables
				protected:
					CVariableBindingsAnswersConcurrentStreamingHandler* mConcurrentHandler;
					CVariableBindingsAnswersStreamingHandler* mHandler;
					QStringList mVariableNames;
					cint64 mResultCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERSSTREAMINGRESULT_H
