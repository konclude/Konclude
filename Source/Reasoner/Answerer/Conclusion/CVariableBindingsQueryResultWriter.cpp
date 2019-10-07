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

#include "CVariableBindingsQueryResultWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Conclusion {

				CVariableBindingsQueryResultWriter::CVariableBindingsQueryResultWriter() {
				}


				CVariableBindingsQueryResultWriter::~CVariableBindingsQueryResultWriter() {
				}



				CVariableBindingResult* CVariableBindingsQueryResultWriter::createVariableBindingResult(CComplexAssertionsIndividualVariablesAnsweringQuery* query, bool dataliteral) {
					if (query->getResultOrderingLinker() || dataliteral) {
						return new CVariableBindingStringDataResult();
					} else {
						return new CVariableBindingStringResult();
					}
				}


				CVariableBindingResult* CVariableBindingsQueryResultWriter::createVariableBindingResult(CDataLiteral* dataLiteral, CComplexAssertionsIndividualVariablesAnsweringQuery* query) {
					return createVariableBindingResult(query, true)->initVariableBinding(dataLiteral);
				}



				CVariableBindingResult* CVariableBindingsQueryResultWriter::createVariableBindingResult(CIndividual* individual, CComplexAssertionsIndividualVariablesAnsweringQuery* query) {
					return createVariableBindingResult(query, true)->initVariableBinding(individual);
				}




				CVariableBindingResult* CVariableBindingsQueryResultWriter::createVariableBindingResult(const CIndividualReference& indiRef, CComplexAssertionsIndividualVariablesAnsweringQuery* query, CIndividualNameResolver* indiNameResolver) {
					return createVariableBindingResult(query, false)->initVariableBinding(indiRef, indiNameResolver);
				}


				CVariableBindingResult* CVariableBindingsQueryResultWriter::createVariableBindingResult(const CIndividualReference& indiRef, CComplexAssertionsIndividualVariablesAnsweringQuery* query, const QString& resolvedIndiName) {
					return createVariableBindingResult(query, false)->initVariableBinding(indiRef, resolvedIndiName);
				}


				CVariableBindingsAnswersResult* CVariableBindingsQueryResultWriter::addVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, cint64 cardinality) {
					bool filtered = true;
					for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker && filtered; filteringLinker = filteringLinker->getNext()) {
						if (!filteringLinker->isFiltered(filteringAnsweringMapping, bindAns)) {
							filtered = false;
						}
					}
					if (filtered) {
						return bindsAnswersResult->addResultVariableBindings(bindAns, cardinality);
					} else {
						delete bindAns;
						return bindsAnswersResult;
					}
				}



				CVariableBindingsAnswersResult* CVariableBindingsQueryResultWriter::addReusedVariableBindingAnswerToResult(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, cint64 cardinality) {
					if (cardinality > 0) {
						bool filtered = true;
						for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker && filtered; filteringLinker = filteringLinker->getNext()) {
							if (!filteringLinker->isFiltered(filteringAnsweringMapping, bindAns)) {
								filtered = false;
							}
						}
						if (filtered) {
							return bindsAnswersResult->addReusedResultVariableBindings(bindAns, cardinality);
						} else {
							return bindsAnswersResult;
						}
					}
					return bindsAnswersResult;
				}





				CVariableBindingsAnswersResult* CVariableBindingsQueryResultWriter::addReusedVariableBindingAnswerToResultConsideringOffsetLimit(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingsListAnswerResult* bindAns, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, CComplexQueryProcessingData* queryProcessingData, cint64 cardinality) {
					cint64 writingCardinality = cardinality;

					if (compAssIndVarQuery->getResultOffset() > 0) {
						cint64 offsetSkippedMappingCount = queryProcessingData->getOffsetSkippedMappingCount();

						cint64 remainingSkipMappingCard = compAssIndVarQuery->getResultOffset() - offsetSkippedMappingCount;

						queryProcessingData->incOffsetSkippedMappingCount(qMin(writingCardinality, remainingSkipMappingCard));

						writingCardinality -= remainingSkipMappingCard;
					}

					if (writingCardinality > 0) {
						if (compAssIndVarQuery->getResultLimitIncludingOffset() != -1) {
							writingCardinality = qMin(writingCardinality, compAssIndVarQuery->getResultLimitIncludingOffset() - bindsAnswersResult->getResultCount());
						}
						if (writingCardinality > 0) {
							return addReusedVariableBindingAnswerToResult(bindsAnswersResult, bindAns, filteringAnsweringMapping, compAssIndVarQuery, writingCardinality);
						}
					}
					return bindsAnswersResult;
				}

			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
