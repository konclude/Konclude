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

#include "CCompletionGraphRandomWalkQueryGenerator.h"


namespace Konclude {


	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {



				CCompletionGraphRandomWalkQueryGenerator::CCompletionGraphRandomWalkQueryGenerator() : CLogIdentifier("::Konclude::Test::CompletionGraphRandomWalkQueryGenerator", this) {
				}


				CCompletionGraphRandomWalkQueryGenerator::~CCompletionGraphRandomWalkQueryGenerator() {
				}





				void CCompletionGraphRandomWalkQueryGenerator::generateQueries(CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool treefoldedCycleQueries = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.TreeFoldedCylceQueries");
					bool realCycleQueries = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.RealCycleQueries");
					bool treeShapedQueries = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.TreeShapedQueries");
					cint64 queryGenerationCount = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.MaximumQueryGenerationCount");
					cint64 maxOntologyQueryCount = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.MaximumOntologyQueryCount");

					while (queryGenerationCount > 0 && getQueryForOntologyCount(satCalcTask) < maxOntologyQueryCount) {
						if (treefoldedCycleQueries && queryGenerationCount > 0 && getQueryForOntologyCount(satCalcTask) < maxOntologyQueryCount) {
							queryGenerationCount--;
							generateTreeFoldedCycleQuery(satCalcTask);
						}
						if (realCycleQueries && queryGenerationCount > 0 && getQueryForOntologyCount(satCalcTask) < maxOntologyQueryCount) {
							queryGenerationCount--;
							generateRealCycleQuery(satCalcTask);
						}
					}
					LOG(INFO, getLogDomain(), logTr("Generated %1 real-cycle queries, %2 failed extractions").arg(mRealCycleQueryGenerationCount).arg(mRealCycleQueryFailedExtractionCount), this);
					LOG(INFO, getLogDomain(), logTr("Generated %1 tree-folded-cycle queries, %2 failed extractions").arg(mTreeFoldedCycleQueryGenerationCount).arg(mTreeFoldedCycleQueryFailedExtractionCount), this);
				}




				CIndividualProcessNode* CCompletionGraphRandomWalkQueryGenerator::getUpToDateIndividual(cint64 indiId, CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
					return indiNodeVec->getData(indiId);
				}




				QList<cint64> CCompletionGraphRandomWalkQueryGenerator::getShortestPathBetweenNodes(cint64 nodeId1, cint64 nodeId2, CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes");

					class ProcessingData {
					public:
						cint64 indiId = 0;
						QList<cint64> visitedIdList;

						ProcessingData(cint64 newIndiId, QList<cint64> prevVisitedIdList) {
							indiId = newIndiId;
							visitedIdList = prevVisitedIdList;
							visitedIdList.append(newIndiId);
						}
					};

					QSet<cint64> visitedNodeSet;
					QList<ProcessingData> visitingNodeList;
					QList<ProcessingData> pathList;

					visitedNodeSet.insert(nodeId1);
					if (nodeId1 != nodeId2) {
						visitingNodeList.append(ProcessingData(nodeId1, QList<cint64>()));
					} else {
						pathList.append(ProcessingData(nodeId1, QList<cint64>()));
					}


					while (!visitingNodeList.isEmpty()) {
						ProcessingData procData = visitingNodeList.takeFirst();
						CIndividualProcessNode* testingNode = getUpToDateIndividual(procData.indiId, satCalcTask);

						CSuccessorIterator succIt = testingNode->getSuccessorIterator();
						while (succIt.hasNext()) {
							cint64 succIndiID = succIt.nextIndividualID(true);
							CIndividualProcessNode* succIndiNode = getUpToDateIndividual(succIndiID, satCalcTask);

							if (!succIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE) && !succIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && (!onlyIndividualNodesWalking || succIndiNode->getNominalIndividual())) {

								ProcessingData succProcData(succIndiID, procData.visitedIdList);

								if (succIndiID == nodeId2) {
									pathList.append(succProcData);
									break;
								} else if (!visitedNodeSet.contains(succIndiID)) {
									visitedNodeSet.insert(succIndiID);
									visitingNodeList.append(succProcData);
								}
							}
						}
					}

					if (!pathList.isEmpty()) {
						return pathList.first().visitedIdList;
					}

					return QList<cint64>();
				}



				QList<cint64> CCompletionGraphRandomWalkQueryGenerator::getRandomizedPathBetweenNodes(QList<cint64> pathList, CSatisfiableCalculationTask* satCalcTask) {
					//srand(time(NULL));
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes");

					QList<cint64> randomizedPathList(pathList);
					cint64 maxPathSize = 3;
					cint64 expansionCount = 0;
					if (maxPathSize - pathList.size() > 0) {
						expansionCount = (rand() % (maxPathSize - pathList.size())) + pathList.size();
					}

					for (cint64 i = 0; i < expansionCount; ++i) {
						cint64 index = rand() % randomizedPathList.size();
						cint64 indiId = randomizedPathList.at(index);
						CIndividualProcessNode* testingNode = getUpToDateIndividual(indiId, satCalcTask);
						CSuccessorIterator succIt = testingNode->getSuccessorIterator();
						QList<cint64> succIndiIds;
						while (succIt.hasNext()) {
							cint64 succIndiID = succIt.nextIndividualID(true);
							if (succIndiID != indiId) {
								CIndividualProcessNode* succNode = getUpToDateIndividual(succIndiID, satCalcTask);
								if (!succNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE) && !succNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && (!onlyIndividualNodesWalking || succNode->getNominalIndividual())) {
									succIndiIds.append(succIndiID);
								}
							}
						}
						if (!succIndiIds.isEmpty()) {
							cint64 succExpIndex = rand() % succIndiIds.size();
							cint64 expandingSuccIndiId = succIndiIds.at(succExpIndex);
							randomizedPathList.insert(index + 1, indiId);
							randomizedPathList.insert(index + 1, expandingSuccIndiId);
						}
					}

					return randomizedPathList;
				}



				bool CCompletionGraphRandomWalkQueryGenerator::generateTreeFoldedCycleQuery(CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
					cint64 indiStart = indiNodeVec->getItemMinIndex();
					cint64 indiEnd = indiNodeVec->getItemMaxIndex();

					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes");

					cint64 treeFoldedQueryGenerationTryCount = mTreeFoldedCycleQueryGenerationTryCount;

					while (treeFoldedQueryGenerationTryCount-- > 0 && (indiEnd - indiStart) > 0) {
						cint64 indiIdx = (rand() % (indiEnd - indiStart)) + indiStart;
						CIndividualProcessNode* indiNode = getUpToDateIndividual(indiIdx, satCalcTask);

						if (indiNode && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE) && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && (!onlyIndividualNodesWalking || indiNode->getNominalIndividual())) {


							QHash<QPair<cint64, cint64>, QString> indiVarIdNameHash;
							QHash<QString, cint64> statisticsHash = getInitialStatisticsHash(satCalcTask);
							statisticsHash["RequestedConjunctiveQueryTreeFoldedCycle"]++;


							cint64 nextVariableId = 0;
							cint64 nextTreeExtensionNr = 0;

							QHash<cint64, cint64> variableNodeIdHash;
							QHash<cint64, QList<cint64>> variableNeighboursHash;
							variableNodeIdHash[nextVariableId++] = indiIdx;

							cint64 cylceCount = (rand() % 4) + 1;
							bool queryExtractionFailed = false;

							for (cint64 i = 0; i < cylceCount && !queryExtractionFailed; ++i) {

								cint64 varId1 = rand() % variableNodeIdHash.size();
								cint64 varId2 = rand() % variableNodeIdHash.size();

								cint64 indiId1 = variableNodeIdHash[varId1];
								cint64 indiId2 = variableNodeIdHash[varId2];

								QList<cint64> path = getShortestPathBetweenNodes(indiId1, indiId2, satCalcTask);
								if (path.size() >= 1) {
									statisticsHash["RequestedConjunctiveQueryCycleCount"]++;
									path = getRandomizedPathBetweenNodes(path, satCalcTask);

									path.removeFirst();
									if (!path.isEmpty()) {
										path.removeLast();
									}

									cint64 lastNodeId = indiId1;
									cint64 lastVarId = varId1;
									for (cint64 nodeId : path) {
										cint64 nextVarId = nextVariableId++;
										variableNodeIdHash[nextVarId] = nodeId;
										variableNeighboursHash[lastVarId].append(nextVarId);
										lastVarId = nextVarId;
										lastNodeId = nodeId;
									}
									variableNeighboursHash[lastVarId].append(varId2);
								} else {
									queryExtractionFailed = true;
								}

							}



							if (variableNodeIdHash.size() <= 1) {
								queryExtractionFailed = true;
							}

							QHash<QString, cint64> varQueryUseCount;

							QStringList queryTerms;
							QSet<QString> queryTermSet;
							QSet<QString> selectableVarSet;
							QSet<QString> queryParameters;
							for (QHash<cint64, QList<cint64>>::const_iterator it = variableNeighboursHash.constBegin(), itEnd = variableNeighboursHash.constEnd(); it != itEnd && !queryExtractionFailed; ++it) {

								cint64 varId = it.key();
								QList<cint64> linkedVarIdList = it.value();

								cint64 indiId = variableNodeIdHash[varId];
								QString indiVar = getIndividualNodeQueryVariable(indiId, varId, indiVarIdNameHash, statisticsHash, satCalcTask);
								CIndividualProcessNode* indiNode = getUpToDateIndividual(indiId, satCalcTask);

								if (indiNode && indiNode->getNominalIndividual()) {
									selectableVarSet.insert(indiVar);
								}



								generateConceptTerms(indiNode, indiVar, queryParameters, queryTermSet, queryTerms, statisticsHash, satCalcTask);


								generateTreeExpansion(indiNode, indiVar, queryParameters, queryTermSet, queryTerms, statisticsHash, nextTreeExtensionNr, satCalcTask);


								for (cint64 linkedVarId : linkedVarIdList) {

									cint64 linkedIndiId = variableNodeIdHash[linkedVarId];
									QString linkedIndiVar = getIndividualNodeQueryVariable(linkedIndiId, linkedVarId, indiVarIdNameHash, statisticsHash, satCalcTask);


									if (!generateRoleTerm(indiNode, indiVar, getUpToDateIndividual(linkedIndiId, satCalcTask), linkedIndiVar, queryParameters, queryTermSet, queryTerms, varQueryUseCount, statisticsHash, satCalcTask)) {
										queryExtractionFailed = true;
										break;
									}

								}
							}


							bool hasAllIndisAtleastTwice = true;
							for (cint64 usedCount : varQueryUseCount) {
								if (usedCount <= 1) {
									hasAllIndisAtleastTwice = false;
									queryExtractionFailed = true;
									break;
								}
							}


							if (!queryExtractionFailed) {

								if (!writeQuery(queryTerms, "tree-folded-cycle", selectableVarSet.toList(), queryParameters.toList(), statisticsHash, satCalcTask)) {
									queryExtractionFailed = true;
								} else {
									mTreeFoldedCycleQueryGenerationCount++;
									return true;
								}

							}
							if (queryExtractionFailed) {
								mTreeFoldedCycleQueryFailedExtractionCount++;
							}
						} else {
							mTreeFoldedCycleQueryFailedExtractionCount++;
						}

					}
					return false;
				}






				cint64 CCompletionGraphRandomWalkQueryGenerator::getQueryForOntologyCount(CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();

					QString queryFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.QueryFileBasePath");
					QString ontologyFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.OntologyFileBasePath");

					QStringList loadedOntologyIris = processingDataBox->getOntology()->getLoadData()->getLoadedOntologyIRIs();
					QString loadedOntologyIri;
					if (!loadedOntologyIris.isEmpty()) {
						loadedOntologyIri = loadedOntologyIris.first();
					}


					loadedOntologyIri = loadedOntologyIri.replace(ontologyFileBasePath, "");
					QStringList loadedOntologyFilePathElements = loadedOntologyIri.split("/");
					QString queryDestPath = queryFileBasePath + loadedOntologyFilePathElements.join("/");

					QDir queryDestDir(queryDestPath);
					cint64 queryfileCount = queryDestDir.entryList(QDir::Files).size();
					return queryfileCount;
				}




				bool CCompletionGraphRandomWalkQueryGenerator::writeQueryStatistics(const QString& ontologyFileName, const QString& queryFileName, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					QString ontologyStatisticsFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.OntologyStatisticsFileBasePath");
					QString queryStatisticsFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.QueryStatisticsFileBasePath");

					QStringList loadedOntologyFilePathElements = ontologyFileName.split("/");
					QString queryStatisticsDestPath = queryStatisticsFileBasePath + loadedOntologyFilePathElements.join("/");
					QDir currDir;
					currDir.mkpath(queryStatisticsDestPath);

					QString ontStatsString;
					QFile ontStatFile(ontologyStatisticsFileBasePath + ontologyFileName + ".txt");
					if (ontStatFile.open(QIODevice::ReadOnly)) {
						ontStatsString = ontStatFile.readAll();
						ontStatFile.close();
					}

					QFile queryStatFile(queryStatisticsDestPath + "/" + queryFileName + ".txt");
					if (queryStatFile.open(QIODevice::WriteOnly)) {
						queryStatFile.write(ontStatsString.toUtf8());

						for (QHash<QString, cint64>::const_iterator it = statisticsHash.constBegin(), itEnd = statisticsHash.constEnd(); it != itEnd; ++it) {
							QString statName = it.key();
							cint64 statValue = it.value();
							QString writeString = QString("%1:\t%2\r\n").arg(statName).arg(statValue);
							queryStatFile.write(writeString.toUtf8());
						}

						queryStatFile.close();
						return true;
					}
					return false;

				}



				bool CCompletionGraphRandomWalkQueryGenerator::writeQuery(const QStringList& queryTerms, const QString& queryStructureString, QList<QString> selectableVariables, const QList<QString>& queryParameters, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satCalcTask) {


					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();


					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes");
					QString queryFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.QueryFileBasePath");
					QString ontologyFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.OntologyFileBasePath");
					QString queryTemplateFileName = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.QueryTemplateFile");
					QString loadFileBasePath = CConfigDataReader::readConfigString(config, "Konclude.Test.ConjunctiveQueryGeneration.LoadFileBasePath");


					QStringList loadedOntologyIris = processingDataBox->getOntology()->getLoadData()->getLoadedOntologyIRIs();
					QString loadedOntologyIri;
					if (!loadedOntologyIris.isEmpty()) {
						loadedOntologyIri = loadedOntologyIris.first();
					}


					loadedOntologyIri = loadedOntologyIri.replace(ontologyFileBasePath, "");


					QStringList loadedOntologyFilePathElements = loadedOntologyIri.split("/");
					QString ontologyName = loadedOntologyFilePathElements.last();
					ontologyName = ontologyName.replace(".owl.fss", "");
					ontologyName = ontologyName.replace(".owl.xml", "");
					ontologyName = ontologyName.replace(".owl", "");





					QString queryTemplateString;
					QFile templateFile(queryTemplateFileName);
					if (templateFile.open(QIODevice::ReadOnly)) {
						queryTemplateString = templateFile.readAll();
						templateFile.close();
					}
					queryTemplateString = queryTemplateString.replace("$QUERYTERMS", queryTerms.join("\r\n"));



					queryTemplateString = queryTemplateString.replace("$QUERYONTOLOGYLOAD", loadFileBasePath + loadedOntologyIri);


					QString queryDestPath = queryFileBasePath + loadedOntologyFilePathElements.join("/");

					QDir currDir;
					currDir.mkpath(queryDestPath);


					QString queryTypeString;
					cint64 ontologyQueryNr = getQueryForOntologyCount(satCalcTask);

					QString querySelectionString = "ASK";
					queryTypeString = "ASK";
					cint64 minVarSelCount = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.MinimalVariableSelectCount");
					cint64 maxVarSelCount = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.MaximumVariableSelectCount");
					if (maxVarSelCount > 0) {
						queryTypeString = "SELECT";
						querySelectionString = "SELECT ";
						QStringList selectVarStringList;
						maxVarSelCount = qMin(maxVarSelCount, (cint64)selectableVariables.size());
						cint64 selectTermsCount = ((rand() % (maxVarSelCount - minVarSelCount)) + minVarSelCount);
						for (cint64 i = 0; i < selectTermsCount; ++i) {
							QString varName;
							cint64 nextVarPos = rand() % selectableVariables.size();

							while (nextVarPos-- > 0) {
								varName = selectableVariables.takeFirst();
								selectableVariables.append(varName);
							}
							varName = selectableVariables.takeFirst();
							selectVarStringList.append(varName);
							statisticsHash["RequestedConjunctiveQuerySelectedVariableCount"]++;


						}
						querySelectionString += selectVarStringList.join(" ") + "\r\nWHERE";
					}

					queryTemplateString = queryTemplateString.replace("$QUERYSELECTION", querySelectionString);

					QList<QString> queryPars = queryParameters;
					qSort(queryPars.begin(), queryPars.end());
					QString parameters = queryPars.join("-and-");

					QString queryFileName = QString("%1--query-nr-%5-%2-%3-%4.sparql").arg(ontologyName).arg(queryStructureString).arg(parameters).arg(queryTypeString).arg(ontologyQueryNr + 1);
					QString queryPathFileName = QString("%1/%2").arg(queryDestPath).arg(queryFileName);
					QFile queryFile(queryPathFileName);
					if (queryFile.open(QIODevice::WriteOnly)) {
						QByteArray data = queryTemplateString.toUtf8();
						queryFile.write(data);
						queryFile.close();

						writeQueryStatistics(loadedOntologyIri, queryFileName, statisticsHash, satCalcTask);
						return true;
					} else {
						return false;
					}

				}




				bool CCompletionGraphRandomWalkQueryGenerator::generateRoleTerm(CIndividualProcessNode* indiNode1, QString indiVar1, CIndividualProcessNode* indiNode2, QString indiVar2, QSet<QString>& queryParameters, QSet<QString>& queryTermSet, QStringList& queryTerms, QHash<QString, cint64>& varQueryUseCount, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satCalcTask) {

					CSuccessorRoleIterator succRoleIt = indiNode1->getSuccessorRoleIterator(indiNode2->getIndividualNodeID());
					bool foundRole = false;
					QList<QPair<QString, bool>> roleInverseList;
					while (succRoleIt.hasNext()) {
						CIndividualLinkEdge* link = succRoleIt.next(true);
						CRole* role = link->getLinkRole();
						if (role->getRoleTag() != 1) {
							if (role->getPropertyNameLinker()) {
								QString roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
								roleInverseList.append(QPair<QString, bool>(roleName, false));
								foundRole = true;
							} else if (role->getInverseRole() && role->getInverseRole()->getPropertyNameLinker()) {
								QString roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
								roleInverseList.append(QPair<QString, bool>(roleName, true));
								foundRole = true;
							}
						}
					}


					if (foundRole) {

						cint64 roleId = rand() % roleInverseList.size();
						QString roleName = roleInverseList.at(roleId).first;
						bool inversed = roleInverseList.at(roleId).second;

						QString queryTermString;
						if (!inversed) {
							queryTermString = QString(" %1 <%2> %3 . ").arg(indiVar1).arg(roleName).arg(indiVar2);
						} else {
							queryTermString = QString(" %1 <%2> %3 . ").arg(indiVar2).arg(roleName).arg(indiVar1);
						}
						if (!queryTermSet.contains(queryTermString)) {
							statisticsHash["RequestedConjunctiveQueryRoleTermCount"]++;
							statisticsHash["RequestedConjunctiveQueryTermCount"]++;
							queryTermSet.insert(queryTermString);

							varQueryUseCount[indiVar1]++;
							varQueryUseCount[indiVar2]++;

							queryTerms.append(queryTermString);
						}
						return true;
					} else {
						return false;
					}

				}




				void CCompletionGraphRandomWalkQueryGenerator::generateConceptTerms(CIndividualProcessNode* indiNode, QString indiVar, QSet<QString>& queryParameters, QSet<QString>& queryTermSet, QStringList& queryTerms, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satCalcTask) {

					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool generateConceptTerms = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.GenerateConceptTerms");

					if (generateConceptTerms) {
						CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
						QList<QString> conceptList;
						auto conSetIt = conSet->getConceptLabelSetIterator(false, false, false);
						while (conSetIt.hasNext()) {
							CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
							CConcept* concept = conDes->getConcept();
							if (concept->getOperatorCode() == CCATOM || concept->getOperatorCode() == CCNONE || concept->getOperatorCode() == CCSUB || concept->getOperatorCode() == CCEQ) {
								if (!conDes->getNegation()) {
									QString conceptName = CIRIName::getRecentIRIName(concept->getClassNameLinker());
									conceptList.append(conceptName);
								}
							}
							conSetIt.moveNext();
						}
						queryParameters.insert("with-concept-terms");
						cint64 conTerms = ((rand() % 200) + 50) / 100;
						for (cint64 i = 0; i < conTerms && conceptList.size() > 0; ++i) {
							cint64 conId = rand() % conceptList.size();
							QString conName = conceptList.at(conId);

							QString queryTermString = QString(" %1 rdf:type <%2> . ").arg(indiVar).arg(conName);
							if (!queryTermSet.contains(queryTermString)) {
								queryTermSet.insert(queryTermString);
								queryTerms.append(queryTermString);
								statisticsHash["RequestedConjunctiveQueryConceptTermCount"]++;
								statisticsHash["RequestedConjunctiveQueryAtomicConceptTermCount"]++;
								statisticsHash["RequestedConjunctiveQueryTermCount"]++;
							}
						}
					}

				}






				void CCompletionGraphRandomWalkQueryGenerator::generateTreeExpansion(CIndividualProcessNode* indiNode, QString indiVar, QSet<QString>& queryParameters, QSet<QString>& queryTermSet, QStringList& queryTerms, QHash<QString, cint64>& statisticsHash, cint64& nextTreeExpansionNr, CSatisfiableCalculationTask* satCalcTask) {

					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool generateTreeExtensions = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.GenerateTreeExtensions");
					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.TreeExtensionsOnlyOverIndividualNodes");


					cint64 existentialVariableConversionRation = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.TreeExtensionExistentialVariableConversionRatio");

					cint64 extensionCreationRatio = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.TreeExtensionCreationRatio");


					cint64 treeExpansionVarId = 0;
					QHash<QPair<cint64, cint64>, QString> treeExtIndiVarIdNameHash;

					class CTreeExpansionNodeData {
					public:
						CTreeExpansionNodeData(CIndividualProcessNode* indiNode, QString varName, cint64 depth) {
							mIndiNode = indiNode;
							mVarName = varName;
							mExistential = mVarName.startsWith("_:");
							mDepth = depth;
						}

						CIndividualProcessNode* mIndiNode;
						QString mVarName;
						bool mExistential;
						cint64 mDepth;
					};
					cint64 creationRatio = rand() % 100;
					if (generateTreeExtensions && creationRatio < extensionCreationRatio) {
						cint64 maxTreeExtensionSize = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.MaximumTreeExtensionSize");
						cint64 teeExtensionSize = rand() % maxTreeExtensionSize;

						QList<CTreeExpansionNodeData> nodeVarNamePairList;
						nodeVarNamePairList.append(CTreeExpansionNodeData(indiNode, indiVar, 0));
						if (teeExtensionSize > 0) {

							statisticsHash["RequestedConjunctiveQueryTreeExpansionCount"]++;
							queryParameters.insert("with-tree-extensions");
							cint64 treeExpansionNr = nextTreeExpansionNr++;

							while (teeExtensionSize-- > 0) {
								cint64 extIdx = rand() % nodeVarNamePairList.size();
								CTreeExpansionNodeData extIndiVarNameData = nodeVarNamePairList.at(extIdx);
								CIndividualProcessNode* extIndiNode = extIndiVarNameData.mIndiNode;
								QString extVarName = extIndiVarNameData.mVarName;
								bool existential = extIndiVarNameData.mExistential;
								cint64 depth = extIndiVarNameData.mDepth;

								QList<CIndividualProcessNode*> succNodeList;
								CSuccessorIterator succIt = extIndiNode->getSuccessorIterator();
								while (succIt.hasNext()) {
									cint64 succIndiID = succIt.nextIndividualID(true);

									CIndividualProcessNode* succIndiNode = getUpToDateIndividual(succIndiID, satCalcTask);
									if (!succIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE) && !succIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && (!onlyIndividualNodesWalking || extIndiNode->getNominalIndividual())) {

										succNodeList.append(succIndiNode);

									}
								}


								if (!succNodeList.isEmpty()) {
									cint64 extSuccIdx = rand() % succNodeList.size();
									CIndividualProcessNode* succIndiNode = succNodeList.at(extSuccIdx);

									statisticsHash["RequestedConjunctiveQueryTreeExpandedVariableCount"]++;


									bool createDistinguishedVariable = true;
									if (existentialVariableConversionRation > 0) {
										createDistinguishedVariable = rand() % 100 > existentialVariableConversionRation;
									}
									if (createDistinguishedVariable && existential) {
										createDistinguishedVariable = false;
									}

									QString succIndiVarName = getIndividualNodeQueryVariable(succIndiNode->getIndividualNodeID(), ++treeExpansionVarId, treeExtIndiVarIdNameHash, statisticsHash, createDistinguishedVariable, QString("tree%1_d%2").arg(treeExpansionNr).arg(depth), satCalcTask);


									QHash<QString, cint64> varQueryUseCount;
									if (generateRoleTerm(extIndiNode, extVarName, succIndiNode, succIndiVarName, queryParameters, queryTermSet, queryTerms, varQueryUseCount, statisticsHash, satCalcTask)) {
										CTreeExpansionNodeData treeExpNodeData = CTreeExpansionNodeData(succIndiNode, succIndiVarName, depth + 1);
										nodeVarNamePairList.append(treeExpNodeData);

										generateConceptTerms(succIndiNode, succIndiVarName, queryParameters, queryTermSet, queryTerms, statisticsHash, satCalcTask);

										if (treeExpNodeData.mExistential) {
											statisticsHash["RequestedConjunctiveQueryTreeExpandedNondistinguishedVariableCount"]++;
										} else {
											statisticsHash["RequestedConjunctiveQueryTreeExpandedDistinguishedVariableCount"]++;
										}
									}




									
								}

							}

						}
					}

				}



				QHash<QString, cint64> CCompletionGraphRandomWalkQueryGenerator::getInitialStatisticsHash(CSatisfiableCalculationTask* satTask) {
					QHash<QString, cint64> statisticsHash;
					statisticsHash["RequestedConjunctiveQueryCount"] = 1;
					statisticsHash["RequestedConjunctiveQueryTermCount"] = 1;
					statisticsHash["RequestedConjunctiveQueryConceptTermCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryAtomicConceptTermCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryComplexConceptTermCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryRoleTermCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryCycleCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryNondistinguishedVariablesCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryNominalNodeBasedVariablesCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryBlockableNodeBasedVariablesCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryDistinguishedVariablesCount"] = 0;
					statisticsHash["RequestedConjunctiveQuerySelectedVariableCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryRealCycle"] = 0;
					statisticsHash["RequestedConjunctiveQueryTreeFoldedCycle"] = 0;
					statisticsHash["RequestedConjunctiveQueryTreeExpansionCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryTreeExpandedVariableCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryTreeExpandedDistinguishedVariableCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryTreeExpandedNondistinguishedVariableCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryVariableCount"] = 0;
					statisticsHash["RequestedConjunctiveQueryConstantCount"] = 0;
					return statisticsHash;
				}



				bool CCompletionGraphRandomWalkQueryGenerator::generateRealCycleQuery(CSatisfiableCalculationTask* satCalcTask) {
					CProcessingDataBox* processingDataBox = satCalcTask->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();
					cint64 indiStart = indiNodeVec->getItemMinIndex();
					cint64 indiEnd = indiNodeVec->getItemMaxIndex();

					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();

					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes");


					class ReachabilityDistanceData {
					public:
						cint64 indiId = 0;
						cint64 reachCount = 0;
						QList<cint64> depList;
						cint64 minDistance = 999999;
						cint64 maxDistance = -9999999;
					};

					class ProcessingDistanceData {
					public:
						cint64 prevIndiId = 0;
						cint64 indiId = 0;
						cint64 distance = 0;
						ProcessingDistanceData(cint64 id, cint64 distance, cint64 prevIndiId) {
							indiId = id;
							this->distance = distance;
							this->prevIndiId = prevIndiId;
						}
					};

					cint64 queryGenerationTryCount = mRealCycleQueryGenerationTryCount;

					while (queryGenerationTryCount-- > 0 && (indiEnd - indiStart) > 0) {
						cint64 indiIdx = (rand() % (indiEnd - indiStart)) + indiStart;
						CIndividualProcessNode* indiNode = getUpToDateIndividual(indiIdx, satCalcTask);
						if (indiNode && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE) && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && (!onlyIndividualNodesWalking || indiNode->getNominalIndividual())) {
							QHash<cint64, ReachabilityDistanceData> reachabilityCountHash;
							QSet<cint64> processedSet;
							QList<ProcessingDistanceData> processingList;
							if (indiNode) {
								processedSet.insert(indiNode->getIndividualNodeID());
								processingList.append(ProcessingDistanceData(indiNode->getIndividualNodeID(), 0, indiNode->getIndividualNodeID()));


								QHash<QPair<cint64, cint64>, QString> indiVarIdNameHash;
								QHash<QString, cint64> statisticsHash = getInitialStatisticsHash(satCalcTask);
								statisticsHash["RequestedConjunctiveQueryRealCycle"]++;

								cint64 connection = 0;

								cint64 nextTreeExtensionNr = 0;

								while (!processingList.isEmpty()) {
									ProcessingDistanceData processingData = processingList.takeFirst();

									CIndividualProcessNode* indiNode = getUpToDateIndividual(processingData.indiId, satCalcTask);
									CSuccessorIterator succIt = indiNode->getSuccessorIterator();
									while (succIt.hasNext()) {
										cint64 succIndiID = succIt.nextIndividualID(true);

										CIndividualProcessNode* succIndiNode = getUpToDateIndividual(succIndiID, satCalcTask);
										if (!succIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCONCRETEDATAINDINODE) && !succIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && (!onlyIndividualNodesWalking || succIndiNode->getNominalIndividual())) {

											if (succIndiID != processingData.indiId && processingData.prevIndiId != succIndiID) {
												ReachabilityDistanceData& reachData = reachabilityCountHash[succIndiID];
												reachData.indiId = succIndiID;
												if (connection < 6) {
													++reachData.reachCount;
													if (reachData.reachCount > 1) {
														connection++;
													}
													reachData.depList.append(processingData.indiId);
													reachData.minDistance = qMin(reachData.minDistance, processingData.distance + 1);
													reachData.maxDistance = qMax(reachData.maxDistance, processingData.distance + 1);


													if (processingData.distance + 1 < 10 && connection < 3) {
														if (!processedSet.contains(succIndiID)) {
															processedSet.insert(succIndiID);
															processingList.append(ProcessingDistanceData(succIndiID, processingData.distance + 1, processingData.indiId));
														}
													}
												}
											}
										}

									}
								}


								class TracingBackData {
								public:
									cint64 indiId = 0;
									cint64 tracedBackIndiId = 0;
									TracingBackData(cint64 indiId, cint64 tracedBackIndiId) {
										this->indiId = indiId;
										this->tracedBackIndiId = tracedBackIndiId;
									}
								};

								cint64 indiQueryCount = 0;
								for (ReachabilityDistanceData& reachData : reachabilityCountHash) {
									QStringList queryTerms;
									if (reachData.reachCount > 1) {
										// generate query by tracking back

										QList<TracingBackData> tracingBackDataList;
										for (cint64 depIndi : reachData.depList) {
											tracingBackDataList.append(TracingBackData(reachData.indiId, depIndi));
										}

										bool queryExtractionFailed = false;
										QSet<QPair<cint64, cint64>> tracedBackPairsSet;
										QSet<QString> queryParameters;
										QSet<QString> queryTermSet;
										QSet<QString> selectableVarSet;
										QHash<QString, cint64> varQueryUseCount;
										QSet<cint64> trackedbackIndiSet;
										while (!tracingBackDataList.isEmpty() && !queryExtractionFailed) {
											TracingBackData tracingBackData = tracingBackDataList.takeFirst();

											if (trackedbackIndiSet.contains(tracingBackData.indiId)) {
												statisticsHash["RequestedConjunctiveQueryCycleCount"]++;
											} else {
												trackedbackIndiSet.insert(tracingBackData.indiId);
											}


											QString indiVar = getIndividualNodeQueryVariable(tracingBackData.indiId, 0, indiVarIdNameHash, statisticsHash, satCalcTask);
											QString traceBackIndiVar = getIndividualNodeQueryVariable(tracingBackData.tracedBackIndiId, 0, indiVarIdNameHash, statisticsHash, satCalcTask);

											CIndividualProcessNode* indiNode = getUpToDateIndividual(tracingBackData.indiId, satCalcTask);
											if (indiNode && indiNode->getNominalIndividual()) {
												selectableVarSet.insert(indiVar);
											}


											generateConceptTerms(indiNode, indiVar, queryParameters, queryTermSet, queryTerms, statisticsHash, satCalcTask);


											generateTreeExpansion(indiNode, indiVar, queryParameters, queryTermSet, queryTerms, statisticsHash, nextTreeExtensionNr, satCalcTask);

											if (!generateRoleTerm(indiNode, indiVar, getUpToDateIndividual(tracingBackData.tracedBackIndiId, satCalcTask), traceBackIndiVar, queryParameters, queryTermSet, queryTerms, varQueryUseCount, statisticsHash, satCalcTask)) {
												queryExtractionFailed = true;
											}




											ReachabilityDistanceData& depReachData = reachabilityCountHash[tracingBackData.tracedBackIndiId];
											for (cint64 depIndi : depReachData.depList) {
												if (!tracedBackPairsSet.contains(QPair<cint64, cint64>(depReachData.indiId, depIndi))) {
													tracedBackPairsSet.insert(QPair<cint64, cint64>(depReachData.indiId, depIndi));
													tracingBackDataList.append(TracingBackData(depReachData.indiId, depIndi));
												}
											}
										}



										bool hasAllIndisAtleastTwice = true;
										for (cint64 usedCount : varQueryUseCount) {
											if (usedCount <= 1) {
												hasAllIndisAtleastTwice = false;
												queryExtractionFailed = true;
												break;
											}
										}


										if (!queryExtractionFailed) {
											++indiQueryCount;



											if (!writeQuery(queryTerms, "real-cycle", selectableVarSet.toList(), queryParameters.toList(), statisticsHash, satCalcTask)) {
												queryExtractionFailed = true;
											} else {
												mRealCycleQueryGenerationCount++;
												return true;
											}

										}
										if (queryExtractionFailed) {
											mRealCycleQueryFailedExtractionCount++;
										}

										if (indiQueryCount > 10) {
											break;
										}


									}
								}
							}



						} else {
							mRealCycleQueryFailedExtractionCount++;
						}
					}

					return false;
				}




				QString CCompletionGraphRandomWalkQueryGenerator::getIndividualNodeQueryVariable(cint64 indiId, cint64 varId, QHash<QPair<cint64, cint64>, QString>& indiVarIdNameHash, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satCalcTask) {
					CCalculationConfigurationExtension* config = satCalcTask->getCalculationConfiguration();
					bool onlyIndividualNodesWalking = CConfigDataReader::readConfigBoolean(config, "Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes");
					cint64 existentialVariableInterpRation = CConfigDataReader::readConfigInteger(config, "Konclude.Test.ConjunctiveQueryGeneration.IndividualNodeAsExistentialVariableInterpretationRatio");

					bool createDistinguishedVariable = true;
					if (existentialVariableInterpRation > 0) {
						createDistinguishedVariable = rand() % 100 < existentialVariableInterpRation;
					}
					return getIndividualNodeQueryVariable(indiId, varId, indiVarIdNameHash, statisticsHash, createDistinguishedVariable, "cyc", satCalcTask);
				}


				QString CCompletionGraphRandomWalkQueryGenerator::getIndividualNodeQueryVariable(cint64 indiId, cint64 varId, QHash<QPair<cint64,cint64>, QString>& indiVarIdNameHash, QHash<QString, cint64>& statisticsHash, bool createDistinguishedVariableIfPossible, const QString& varPrefixString, CSatisfiableCalculationTask* satCalcTask) {

					QPair<cint64, cint64> indiVarIdPair(indiId, varId);
					if (indiVarIdNameHash.contains(indiVarIdPair)) {
						return indiVarIdNameHash.value(indiVarIdPair);
					}

					bool isDistinguishedVariable = false;
					CIndividualProcessNode* indiNode = getUpToDateIndividual(indiId, satCalcTask);
					QString nodeSuffix;
					if (indiNode->getNominalIndividual() && createDistinguishedVariableIfPossible) {
						QString indiName = CIRIName::getRecentIRIName(indiNode->getNominalIndividual()->getIndividualNameLinker());
						indiName = indiName.replace("-", "_");
						indiName = indiName.replace(",", "_");
						indiName = indiName.replace(".", "_");
						indiName = indiName.replace("+", "_");
						indiName = indiName.replace("*", "_");
						indiName = indiName.replace("'", "_");
						int lastPos = indiName.lastIndexOf("#");
						lastPos = qMax(lastPos, indiName.lastIndexOf("/"));
						lastPos = qMax(lastPos, indiName.lastIndexOf(":"));
						lastPos = qMax(lastPos, indiName.lastIndexOf("\\"));
						QString lastIndiNamePart = indiName.mid(lastPos + 1);
						for (cint64 i = 0; i < lastIndiNamePart.length(); ++i) {
							QChar nameChar = lastIndiNamePart.at(i);
							if (nameChar.isLetterOrNumber()) {
								nodeSuffix += nameChar;
							}
						}
						while (indiName.contains("__")) {
							indiName = indiName.replace("__", "_");
						}
						if (nodeSuffix.length() > 0) {
							nodeSuffix += "__";
						}
						nodeSuffix += QString("n%1").arg(qAbs(indiId));
						isDistinguishedVariable = true;
						statisticsHash["RequestedConjunctiveQueryDistinguishedVariablesCount"]++;
					} else {
						nodeSuffix += QString("b%1").arg(qAbs(indiId));
						statisticsHash["RequestedConjunctiveQueryNondistinguishedVariablesCount"]++;
					}
					QString indiVar;
					if (isDistinguishedVariable) {
						indiVar = QString("?%1_v%2__%3").arg(varPrefixString).arg(varId).arg(nodeSuffix);
					} else {
						indiVar = QString("_:%1_v%2__%3").arg(varPrefixString).arg(varId).arg(nodeSuffix);
					}
					indiVarIdNameHash.insert(indiVarIdPair, indiVar);

					statisticsHash["RequestedConjunctiveQueryVariableCount"]++;
					if (indiNode->getNominalIndividual()) {
						statisticsHash["RequestedConjunctiveQueryNominalNodeBasedVariablesCount"]++;
					} else {
						statisticsHash["RequestedConjunctiveQueryBlockableNodeBasedVariablesCount"]++;
					}

					return indiVar;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel


	}; // end namespace Reasoner

}; // end namespace Konclude
