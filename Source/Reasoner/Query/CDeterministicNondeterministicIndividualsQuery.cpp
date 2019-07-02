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

#include "CDeterministicNondeterministicIndividualsQuery.h"


#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Query {


			CDeterministicNondeterministicIndividualsQuery::CDeterministicNondeterministicIndividualsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QSet<CIndividual*>& individualSet, const QString& queryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mQueryName = queryName;
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mConsistencyCalcError = false;
				mQueryConstructError = false;

				mCalcConfig = configuration;

				mIndividualSet = individualSet;
				mDeterministicIndividuals = false;

				mRequiresConceptRealisation = true;
				mRequiresSameIndividualRealisation = true;
			}

			CDeterministicNondeterministicIndividualsQuery::~CDeterministicNondeterministicIndividualsQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CDeterministicNondeterministicIndividualsQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CDeterministicNondeterministicIndividualsQuery::constructResult(CRealization* realization) {
				return constructIndividualsResult(realization);
			}



			bool CDeterministicNondeterministicIndividualsQuery::addIndividualResultData(CIndividual* indi, CIndividualProcessNodeVector* indiNodeVec, QSet<QPair<CIndividual*,CConcept*> >* possAssSet) {
				cint64 indiID = -indi->getIndividualID();
				CBOXSET<CConcept*>* eqNonCanSet = mOntology->getTBox()->getEquivalentConceptNonCandidateSet(false);

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


						for (CConceptDescriptor* conDesIt = conLabelSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
							CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();
							CConcept* concept = conDesIt->getConcept();
							bool conNegation = conDesIt->isNegated();
							if (nonDeterministicMerged || !depTrackPoint || depTrackPoint->getBranchingTag() > 0) {
								if (!conNegation && concept && concept->hasClassName()) {
									possAssSet->insert( QPair<CIndividual*,CConcept*>(indi,concept) );
								}
							}
							if (concept->getOperatorCode() == CCEQCAND) {
								CConcept* eqConcept = concept->getOperandList()->getData();
								bool negation = false;
								if (!conLabelSet->containsConcept(eqConcept,negation)) {
									possAssSet->insert( QPair<CIndividual*,CConcept*>(indi,eqConcept) );
								}
							}
						}

						if (eqNonCanSet) {
							for (CBOXSET<CConcept*>::const_iterator it = eqNonCanSet->constBegin(), itEnd = eqNonCanSet->constEnd(); it != itEnd; ++it) {
								CConcept* eqConcept(*it);
								bool negation = false;
								if (!conLabelSet->containsConcept(eqConcept,negation)) {
									possAssSet->insert( QPair<CIndividual*,CConcept*>(indi,eqConcept) );
								}
							}
						}
					}
					return true;
				}
				return false;
			}


			CIndividualsResult *CDeterministicNondeterministicIndividualsQuery::constructIndividualsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				CConsistence* consistence = mOntology->getConsistence();
				if (consistence && realization) {
					mResult = new CIndividualsResult();

					CConceptRealization* conRealisation = realization->getConceptRealization();
					CPossibleAssertionsCollectionSet* possAssCollSet = conRealisation->getPossibleAssertionCollectionSet();

					QSet< QPair<CIndividual*,CConcept*> > possAssSet;
					QSet< QPair<CIndividual*,CConcept*> > knownAssSet;

					CIndividualVector* indiVec = mOntology->getABox()->getIndividualVector();
					cint64 indiCount = indiVec->getItemCount();

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

									if (possAssCollSet) {
										for (CPossibleAssertionsCollectionSet::const_iterator it = possAssCollSet->constBegin(), itEnd = possAssCollSet->constEnd(); it != itEnd; ++it) {
											const QPair<CIndividual*,CConcept*>& possAssPair(*it);
											possAssSet.insert( QPair<CIndividual*,CConcept*>(possAssPair.first,possAssPair.second) );
										}
									}

									for (cint64 indiID = 0; indiID < indiCount; ++indiID) {
										CIndividual* individual = indiVec->getData(indiID);
										if (individual) {
											addIndividualResultData(individual,indiNodeVec,&possAssSet);
											CIndividualConceptAssertionSetCollector indiConAssColl(individual,&knownAssSet);
											conRealisation->visitAllTypes(individual,&indiConAssColl);
										}
									}

								} else {
									if (possAssCollSet) {
										for (CPossibleAssertionsCollectionSet::const_iterator it = possAssCollSet->constBegin(), itEnd = possAssCollSet->constEnd(); it != itEnd; ++it) {
											const QPair<CIndividual*,CConcept*>& possAssPair(*it);
											CIndividual* individual(possAssPair.first);
											if (mIndividualSet.contains(individual)) {
												possAssSet.insert( QPair<CIndividual*,CConcept*>(possAssPair.first,possAssPair.second) );
											}
										}
									}


									for (QSet<CIndividual*>::const_iterator it = mIndividualSet.constBegin(), itEnd = mIndividualSet.constEnd(); it  != itEnd; ++it) {
										CIndividual* individual(*it);
										addIndividualResultData(individual,indiNodeVec,&possAssSet);
										CIndividualConceptAssertionSetCollector indiConAssColl(individual,&knownAssSet);
										conRealisation->visitAllTypes(individual,&indiConAssColl);
									}

								}
							}
						}
					}

					QSet<CIndividual*> nonDetIndiSet;
					for (QSet< QPair<CIndividual*,CConcept*> >::const_iterator it = possAssSet.constBegin(), itEnd = possAssSet.constEnd(); it != itEnd; ++it) {
						const QPair<CIndividual*,CConcept*>& indiConPair(*it);
						if (!knownAssSet.contains(indiConPair)) {
							CIndividual* indi = indiConPair.first;
							nonDetIndiSet.insert(indi);
						}						
					}

					if (mIndividualSet.isEmpty()) {
						for (cint64 indiID = 0; indiID < indiCount; ++indiID) {
							CIndividual* individual = indiVec->getData(indiID);
							if (individual) {
								bool addIndiToResult = false;
								if (mDeterministicIndividuals && !nonDetIndiSet.contains(individual)) {
									addIndiToResult = true;
								} else if (!mDeterministicIndividuals && nonDetIndiSet.contains(individual)) {
									addIndiToResult = true;
								}
								if (addIndiToResult) {
									QString indiName;
									if (mUseAbbreviatedIRIs) {
										indiName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
									} else {
										indiName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
									}
									mResult->addIndividual(indiName);
								}
							}
						}
					} else {
						for (QSet<CIndividual*>::const_iterator it = mIndividualSet.constBegin(), itEnd = mIndividualSet.constEnd(); it  != itEnd; ++it) {
							CIndividual* individual(*it);
							bool addIndiToResult = false;
							if (mDeterministicIndividuals && !nonDetIndiSet.contains(individual)) {
								addIndiToResult = true;
							} else if (!mDeterministicIndividuals && nonDetIndiSet.contains(individual)) {
								addIndiToResult = true;
							}
							if (addIndiToResult) {
								QString indiName;
								if (mUseAbbreviatedIRIs) {
									indiName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
								} else {
									indiName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
								}
								mResult->addIndividual(indiName);
							}
						}
					}

				}
				return mResult;
			}


			QString CDeterministicNondeterministicIndividualsQuery::getQueryName() {
				return mQueryName;
			}

			QString CDeterministicNondeterministicIndividualsQuery::getQueryString() {
				return mQueryString;
			}

			bool CDeterministicNondeterministicIndividualsQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CDeterministicNondeterministicIndividualsQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CDeterministicNondeterministicIndividualsQuery::hasError() {
				return mConsistencyCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
