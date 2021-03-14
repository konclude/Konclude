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

#ifndef KONCLUDE_REASONER_ANSWERER_CONCLUSION_CVARIABLEBINDINGSQUERYRESULTWRITER_H
#define KONCLUDE_REASONER_ANSWERER_CONCLUSION_CVARIABLEBINDINGSQUERYRESULTWRITER_H

// Libraries includes


// Namespace includes
#include "ConclusionSettings.h"


// Other includes
#include "Reasoner/Query/CVariableBindingResult.h"
#include "Reasoner/Query/CComplexVariablesAnsweringQuery.h"
#include "Reasoner/Query/CVariableBindingStringDataResult.h"
#include "Reasoner/Query/CVariableBindingStringResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CVariableBindingsListAnswerResult.h"

#include "Reasoner/Answerer/CComplexQueryExpressionProcessingData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {

			namespace Conclusion {

				/*! 
				 *
				 *		\class		CVariableBindingsQueryResultWriter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CVariableBindingsQueryResultWriter {
					// public methods
					public:
						//! Constructor
						CVariableBindingsQueryResultWriter();

						virtual ~CVariableBindingsQueryResultWriter();

						CVariableBindingResult* createVariableBindingResult(CComplexVariablesAnsweringQuery* query, bool dataliteral);
						CVariableBindingResult* createVariableBindingResult(CDataLiteral* dataLiteral, CComplexVariablesAnsweringQuery* query);
						CVariableBindingResult* createVariableBindingResult(CIndividual* individual, CComplexVariablesAnsweringQuery* query);
						CVariableBindingResult* createVariableBindingResult(CConcept* concept, CComplexVariablesAnsweringQuery* query);
						CVariableBindingResult* createVariableBindingResult(CRole* role, CComplexVariablesAnsweringQuery* query);

						CVariableBindingResult* createVariableBindingResult(const CIndividualReference& indiRef, CComplexVariablesAnsweringQuery* query, CIndividualNameResolver* indiNameResolver);
						CVariableBindingResult* createVariableBindingResult(const CIndividualReference& indiRef, CComplexVariablesAnsweringQuery* query, const QString& resolvedIndiName);

						CVariableBindingsAnswersResult* addVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexVariablesAnsweringQuery* compAssIndVarQuery, cint64 cardinality = 1);
						CVariableBindingsAnswersResult* addReusedVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexVariablesAnsweringQuery* compAssIndVarQuery, cint64 cardinality = 1);
						CVariableBindingsAnswersResult* addReusedVariableBindingAnswerToResultConsideringOffsetLimit(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexVariablesAnsweringQuery* compAssIndVarQuery, CComplexQueryExpressionProcessingData* queryProcessingData, cint64 cardinality);


						CVariableBindingsAnswersResult* addVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, cint64 cardinality = 1);
						CVariableBindingsAnswersResult* addReusedVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, cint64 cardinality = 1);

					// protected methods
					protected:

					// protected variables
					protected:


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CONCLUSION_CVARIABLEBINDINGSQUERYRESULTWRITER_H
