/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CClassAssertionQuery.h"


#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Query {


			CClassAssertionQuery::CClassAssertionQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QSet<CIndividual*>& individualSet, const QString& queryName)
					: CConsistencePremisingQuery(ontology,configuration) {
				mQueryName = queryName;
				if (mQueryName.isEmpty()) {
					mQueryName = QString("Get Concept Assertions");
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mConsistencyCalcError = false;
				mQueryConstructError = false;
				mDeterministicAssertions = false;
				mNondeterministicAssertions = false;

				mIndividualSet = individualSet;

				mCalcConfig = configuration;
			}

			CClassAssertionQuery::~CClassAssertionQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CClassAssertionQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CClassAssertionQuery::constructResult(CConsistence* consistence) {
				return constructAssertionResult(consistence);
			}


			bool CClassAssertionQuery::addIndividualResultData(cint64 indiID, CIndividualProcessNodeVector* indiNodeVec) {
				CBOXSET<CConcept*>* eqNonCanSet = ontology->getTBox()->getEquivalentConceptNonCandidateSet(false);
				bool indiConAssertionAdded = false;
				CIndividualProcessNode* baseIndiNode = indiNodeVec->getData(indiID);
				CIndividualProcessNode* indiNode = baseIndiNode;
				if (indiNode && indiNode->getNominalIndividual()) {
					cint64 mergedIndiID = indiID;
					bool nonDeterministicMerged = false;
					while (mergedIndiID != indiNode->getMergedIntoIndividualNodeID()) {
						mergedIndiID = indiNode->getMergedIntoIndividualNodeID();
						CDependencyTrackPoint* mergedDepTrackPoint = indiNode->getMergedDependencyTrackPoint();
						if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
							nonDeterministicMerged = true;
						}
						indiNode = indiNodeVec->getData(mergedIndiID);
					}

					CReapplyConceptLabelSet* conLabelSet = indiNode->getReapplyConceptLabelSet(false);
					if (conLabelSet) {
						bool allNonDeterministic = nonDeterministicMerged;
						QString indiName;
						if (mUseAbbreviatedIRIs) {
							indiName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(baseIndiNode->getNominalIndividual()->getIndividualNameLinker());
						} else {
							indiName = CIRIName::getRecentIRIName(baseIndiNode->getNominalIndividual()->getIndividualNameLinker());
						}

						for (CConceptDescriptor* conDesIt = conLabelSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
							CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();
							bool conNegation = conDesIt->isNegated();
							CConcept* concept = conDesIt->getConcept();
							if (mDeterministicAssertions && depTrackPoint && depTrackPoint->getBranchingTag() <= 0 && !allNonDeterministic || mNondeterministicAssertions && (allNonDeterministic || !depTrackPoint || depTrackPoint->getBranchingTag() > 0)) {
								if (!conNegation && concept && concept->hasClassName()) {

									QString conceptName;
									if (mUseAbbreviatedIRIs) {
										conceptName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(concept->getClassNameLinker());
									} else {
										conceptName = CIRIName::getRecentIRIName(concept->getClassNameLinker());
									}
									mResult->addIndividualConceptAssertion(indiName,conceptName);
									indiConAssertionAdded = true;

								}
							}
							if (mNondeterministicAssertions && concept->getOperatorCode() == CCEQCAND) {
								CConcept* eqConcept = concept->getOperandList()->getData();
								bool negation = false;
								if (!conLabelSet->containsConcept(eqConcept,&negation)) {

									QString conceptName;
									if (mUseAbbreviatedIRIs) {
										conceptName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(eqConcept->getClassNameLinker());
									} else {
										conceptName = CIRIName::getRecentIRIName(eqConcept->getClassNameLinker());
									}
									mResult->addIndividualConceptAssertion(indiName,conceptName);
									indiConAssertionAdded = true;
								}
							}
						}

						if (eqNonCanSet) {
							for (CBOXSET<CConcept*>::const_iterator it = eqNonCanSet->constBegin(), itEnd = eqNonCanSet->constEnd(); it != itEnd; ++it) {
								CConcept* eqConcept(*it);
								bool negation = false;
								if (!conLabelSet->containsConcept(eqConcept,&negation)) {

									QString conceptName;
									if (mUseAbbreviatedIRIs) {
										conceptName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(eqConcept->getClassNameLinker());
									} else {
										conceptName = CIRIName::getRecentIRIName(eqConcept->getClassNameLinker());
									}
									mResult->addIndividualConceptAssertion(indiName,conceptName);
									indiConAssertionAdded = true;
								}
							}
						}
					}
				}
				return indiConAssertionAdded;
			}


			CIndividualClassAssertionsResult *CClassAssertionQuery::constructAssertionResult(CConsistence* consistence) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (consistence) {
					mResult = new CIndividualClassAssertionsResult();
					CConsistenceTaskData* consTaskData = (CConsistenceTaskData*)consistence->getConsistenceModelData();
					if (consTaskData) {
						CSatisfiableCalculationTask* detTask = consTaskData->getDeterministicSatisfiableTask();
						CSatisfiableCalculationTask* nonDetTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
						if (detTask != nonDetTask) {
							CProcessingDataBox* dataBox = nonDetTask->getProcessingDataBox();
							if (dataBox) {
								CIndividualProcessNodeVector* indiNodeVec = dataBox->getIndividualProcessNodeVector();
								cint64 indiNodeCount = indiNodeVec->getItemCount();
								if (mIndividualSet.isEmpty()) {
									for (cint64 indiID = 0; indiID < indiNodeCount; ++indiID) {
										addIndividualResultData(indiID,indiNodeVec);
									}
								} else {
									for (QSet<CIndividual*>::const_iterator it = mIndividualSet.constBegin(), itEnd = mIndividualSet.constEnd(); it  != itEnd; ++it) {
										CIndividual* individual(*it);
										cint64 indiID = individual->getIndividualID();
										addIndividualResultData(indiID,indiNodeVec);
									}
								}
							}
						}
					}
				}
				return mResult;
			}


			QString CClassAssertionQuery::getQueryName() {
				return mQueryName;
			}

			QString CClassAssertionQuery::getQueryString() {
				return mQueryString;
			}

			bool CClassAssertionQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CClassAssertionQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Concept Assertions");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CClassAssertionQuery::hasError() {
				return mConsistencyCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
