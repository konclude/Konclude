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

#ifndef KONCLUDE_REASONER_ANSWERER_CONCLUSION_CABSTRACTSTREAMINGCOMPLEXQUERYFINISHINGHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_CONCLUSION_CABSTRACTSTREAMINGCOMPLEXQUERYFINISHINGHANDLER_H

// Libraries includes


// Namespace includes
#include "ConclusionSettings.h"
#include "CAbstractComplexQueryFinishingHandler.h"
#include "CVariableBindingsQueryResultWriter.h"


// Other includes
#include "Reasoner/Answerer/CComplexQueryExpressionProcessingData.h"
#include "Reasoner/Answerer/COptimizedComplexExpressionOntologyAnsweringItem.h"
#include "Reasoner/Answerer/COptimizedComplexVariableCompositionItemDependence.h"
#include "Reasoner/Answerer/COptimizedComplexVariableCompositionItem.h"
#include "Reasoner/Answerer/COptimizedComplexBuildingClassVariableCompositionsItem.h"

#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringQuery.h"
#include "Reasoner/Query/CVariableBindingsAnswersStreamingResult.h"
#include "Reasoner/Query/CVariableBindingsListAnswerResult.h"
#include "Reasoner/Query/CVariableBindingsAnswerResultMapOrdering.h"
#include "Reasoner/Query/CVariableBindingsAnswersSetResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersListResult.h"
#include "Reasoner/Query/CVariableBindingStringResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersOrderedMapResult.h"
#include "Reasoner/Query/CComplexAxiomsClassVariablesAnsweringQuery.h"

#include "Reasoner/Realization/CConceptRealizationInstanceToIndividualVisitor.h"
#include "Reasoner/Realization/CSameRealizationIndividualVisitor.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Query;
		using namespace Realization;

		namespace Answerer {

			namespace Conclusion {







				/*! 
				 *
				 *		\class		CAbstractStreamingComplexQueryFinishingHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CAbstractStreamingComplexQueryFinishingHandler : public CAbstractComplexQueryFinishingHandler {
					// public methods
					public:
						//! Constructor
						CAbstractStreamingComplexQueryFinishingHandler(CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, bool confExtendedLogging, const QString& logDomain, cint64 confMaximumBatchMappingsComputationSize, cint64 confFirstBatchMappingsComputationSize, double confBatchMappingsComputationSizeIncreasingFactor, bool confContinueMappingsCompuationWhenResultsNotWriteable, cint64 confMappingsComputationUnlimitedInterpretationSize);

						virtual ~CAbstractStreamingComplexQueryFinishingHandler();

						virtual bool init();
						virtual bool processQueryResults();


						virtual bool finishQueryProcessing();


						COptimizedComplexVariableCompositionItem* getResultUsingLastVariableCompositionItem();

						COptimizedComplexVariableCompositionItem* mLastVarItem = nullptr;
						COptimizedComplexBuildingVariableCompositionsItem* mVarBuildItem = nullptr;
						COptimizedComplexBuildingIndividualVariableCompositionsItem* mVarBuildIndividualItem = nullptr;
						COptimizedComplexBuildingClassVariableCompositionsItem* mVarBuildClassItem = nullptr;

					// protected methods
					protected:
						class CVariableBindingResultVector : public QVector<CVariableBindingResult*> {
						public:
							cint64 mValidBindingCount = 0;
							cint64 mCurrentBindingIdx = 0;
							CVariableBindingResultVector* mNextIteratingVector = nullptr;
							cint64 mVectorPos = 0;
						};



						void handleBindingCardinalityLinker(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalities);
						void handleBindingCardinalityLinker(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalities,
							CVariableBindingsAnswersResult*& useBindsAnswersResult, CVariableBindingsListAnswerResult*& useBindResListAnswer, CVariableBindingResultVector**& useBindingResultVectorArray,
							cint64& useAnswerNr, cint64& useImplicitMaximalCardinality, cint64& useImplicitTotalCardinality, cint64& useExplicitMaximalCardinality, cint64& useExplicitTotalCardinality);


						virtual void processUpdatedBindingsCardinalityLinkers();

					// protected variables
					protected:

						class CInstanceBindingAddingVisitor : public CSameRealizationIndividualVisitor, public CConceptRealizationInstanceToIndividualVisitor {
						public:
							CInstanceBindingAddingVisitor(CVariableBindingsAnswersResult* bindsAnswersResult, CVariableBindingFilteringAnswerMapping* filteringAnsweringMapping, CComplexVariablesAnsweringQuery* compAssIndVarQuery, CIndividualNameResolver* indiNameResolver, CComplexQueryExpressionProcessingData* queryProcessingData, CVariableBindingsQueryResultWriter* resultWriter) {
								mCompVarQuery = compAssIndVarQuery;
								mBindsAnswersResult = bindsAnswersResult;
								mFilteringAnsweringMapping = filteringAnsweringMapping;
								mIndiNameResolver = indiNameResolver;
								mQueryProcessingData = queryProcessingData;
								mResultWriter = resultWriter;
							}

							~CInstanceBindingAddingVisitor() {
								if (mBinding) {
									delete mBinding;
									mBindResListAnswer->clearResultVariableBindings();
									delete mBindResListAnswer;
								}
							}

							virtual bool visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
								return visitIndividual(indiRef);
							}

							virtual bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealizatio) {
								return visitIndividual(indiRef);
							}


							bool visitIndividual(const CIndividualReference& indiRef) {
								if (!mBinding) {
									mBinding = mResultWriter->createVariableBindingResult(indiRef, mCompVarQuery, mIndiNameResolver);
									mBindResListAnswer = new CVariableBindingsListAnswerResult();
									mBindResListAnswer->addResultVariableBinding(mBinding);
								} else {
									QString indiName = mIndiNameResolver->getIndividualName(indiRef);
									mBinding->initVariableBinding(indiRef, indiName);
								}
								mResultWriter->addReusedVariableBindingAnswerToResultConsideringOffsetLimit(mBindsAnswersResult, mBindResListAnswer, mFilteringAnsweringMapping, mCompVarQuery, mQueryProcessingData, 1);
								return true;
							}

							CVariableBindingsQueryResultWriter* mResultWriter;
							CComplexVariablesAnsweringQuery* mCompVarQuery;
							CVariableBindingsAnswersResult* mBindsAnswersResult;
							CVariableBindingFilteringAnswerMapping* mFilteringAnsweringMapping;
							CIndividualNameResolver* mIndiNameResolver;
							CVariableBindingResult* mBinding = nullptr;
							CComplexQueryExpressionProcessingData* mQueryProcessingData;
							CVariableBindingsListAnswerResult* mBindResListAnswer = new CVariableBindingsListAnswerResult();

						};



						class CInstanceBindingIndividualLambdaCallingVisitor : public CSameRealizationIndividualVisitor {
						public:
							function<bool(const CIndividualReference&)> mCallFunction;

							CInstanceBindingIndividualLambdaCallingVisitor(function<bool(const CIndividualReference&)> callFunction) {
								mCallFunction = callFunction;
							}

							bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
								return mCallFunction(indiRef);
							}
						};



						class CInstanceBindingIndividualCountingVisitor : public CSameRealizationIndividualVisitor {
						public:
							cint64 individualCount = 0;

							bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
								individualCount++;;
								return true;
							}
						};


						//COptimizedComplexVariableCompositionItem* mLastVarItem = nullptr;
						//COptimizedComplexBuildingVariableCompositionsItem* mVarBuildItem = nullptr;
						CVariableBindingsQueryResultWriter mResultWriter;
						CComplexQueryExpressionProcessingData* mQueryProcessingData;
						bool mAbbreviatedIRIs = false;
						QStringList mVarList;
						QHash<CExpressionVariable*, cint64> mDisVarIdHash;
						CComplexVariablesAnsweringQuery* mCompVarQuery;
						CComplexAssertionsIndividualVariablesAnsweringQuery* mCompAssIndVarQuery;
						CComplexAxiomsClassVariablesAnsweringQuery* mCompAxClassVarQuery;
						CVariableBindingFilteringAnswerMapping* mFilteringAnsweringMapping = nullptr;
						bool mAnswersWriteable = true;
						cint64 mLastOnlyCountingWritten = 0;
						COptimizedComplexExpressionOntologyAnsweringItem* mOntoAnsweringItem;
						CVariableBindingsAnswersResult* mBindsAnswersResult = nullptr;
						cint64 mSplitComputedMappingCount = 0;
						bool mDistinct;
						QList<cint64> mAnswerDistinguishedVariableMappingIndexList;
						QList<cint64> mNonAnswerVariableMappingIndexList;
						CSameRealization* mSameRealization;
						CConceptRealization* mConceptRealization;
						cint64 mAnswerVarCount;
						CVariableBindingResultVector** mBindingResultVectorArray;
						CIndividualNameResolver* mIndiNameResolver;
						COptimizedComplexVariableIndividualMappings* mLastVarItemMapping = nullptr;
						COptimizedComplexConceptItem* mConceptItem;
						COptimizedComplexConceptInstanziatedIndividualItemHash* mInstanceItemSet = nullptr;
						COptimizedComplexConceptInstanziatedIndividualItemLinker* mInstanceItemLastProcessingLinker = nullptr;
						COptimizedComplexConceptInstanziatedIndividualItemLinker* mInstanceItemLastProcessedLinker = nullptr;
						COptimizedComplexConceptInstanziatedIndividualItemLinker* mInstanceItemLastRetrievedLinker = nullptr;
						cint64 mInstanceItemLinkerProcessedCount = 0;
						bool mConfExtendedLogging;
						COptimizedComplexVariableCompositionItemDependence* mLastVarItemProcessingDep = nullptr;

						CVariableBindingsAnswersStreamingResult* mExistBindsAnswersStreamingResult = nullptr;
						QString mLogDomain;

						CVariableBindingsListAnswerResult* mBindResListAnswer;

						bool mInitialized = false;



						cint64 mExplicitMaximalCardinality = 0;
						cint64 mExplicitTotalCardinality = 0;

						cint64 mImplicitMaximalCardinality = 0;
						cint64 mImplicitTotalCardinality = 0;

						cint64 mAnswerNr = 0;

						double mNextMappingRequestingCountLimit = 10;
						double mNextMappingRequestingCountLimitIncreasingFactor = 10;

						cint64 mComputationBatchNr = 0;


						cint64 mConfMaximumBatchMappingsComputationSize = 100000000;
						cint64 mConfFirstBatchMappingsComputationSize = 10;
						double mConfBatchMappingsComputationSizeIncreasingFactor = 10.;
						bool mConfContinueMappingsCompuationWhenResultsNotWriteable = false;
						cint64 mConfMappingsComputationUnlimitedInterpretationSize = -1;

						cint64 mLastItemAllBindingsCardinality = 0;


						CInstanceBindingAddingVisitor* mResultGenerator = nullptr;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CONCLUSION_CABSTRACTSTREAMINGCOMPLEXQUERYFINISHINGHANDLER_H
