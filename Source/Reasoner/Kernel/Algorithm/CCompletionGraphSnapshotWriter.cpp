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

#include "CCompletionGraphSnapshotWriter.h"

#include <QFile>
#include <QStringList>
#include <QJsonDocument>

#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CIRIName.h"
#include "Reasoner/Ontology/CConceptOperator.h"
#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Kernel/Process/CConceptProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				static const cint64 SNAPSHOT_MAX_CONCEPT_DEPTH = 4;


				bool CCompletionGraphSnapshotWriter::writeSnapshot(const QString& fileName, const CSnapshotDescription& description, CIndividualProcessNodeVector* nodeVector) {
					if (!nodeVector) {
						return false;
					}
					QJsonArray nodeArray;
					cint64 minIndex = nodeVector->getItemMinIndex();
					cint64 maxIndex = nodeVector->getItemMaxIndex();
					for (cint64 nodeID = minIndex; nodeID <= maxIndex; ++nodeID) {
						CIndividualProcessNode* node = nodeVector->getData(nodeID);
						if (node) {
							nodeArray.append(getNodeJson(node));
						}
					}

					QJsonObject root;
					root.insert("format", "konclude-completion-graph-snapshot-1");
					root.insert("graph", description.mGraphName);
					root.insert("phase", description.mPhase);
					root.insert("note", description.mNote);
					root.insert("maxCachedNodeId", (qint64)description.mMaxCachedNodeID);
					root.insert("nodes", nodeArray);

					QFile file(fileName);
					if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
						return false;
					}
					file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
					file.close();
					return true;
				}


				QJsonObject CCompletionGraphSnapshotWriter::getNodeJson(CIndividualProcessNode* node) {
					QJsonObject nodeJson;
					nodeJson.insert("id", (qint64)node->getIndividualNodeID());
					if (node->isNominalIndividualNode()) {
						nodeJson.insert("kind", "nominal");
						CIndividual* nominalIndividual = node->getNominalIndividual();
						if (nominalIndividual) {
							nodeJson.insert("name", getShortName(CIRIName::getRecentIRIName(nominalIndividual->getIndividualNameLinker())));
						}
					} else if (node->isBlockableIndividual()) {
						nodeJson.insert("kind", "blockable");
					} else {
						nodeJson.insert("kind", "abox");
					}
					if (node->hasMergedIntoIndividualNodeID()) {
						CDependencyTrackPoint* mergeDepTrackPoint = node->getMergedDependencyTrackPoint();
						QJsonObject mergeJson;
						mergeJson.insert("into", (qint64)node->getMergedIntoIndividualNodeID());
						mergeJson.insert("nd", isNonDeterministic(mergeDepTrackPoint));
						mergeJson.insert("branch", (qint64)getBranchTag(mergeDepTrackPoint));
						nodeJson.insert("merge", mergeJson);
					}
					CIndividualProcessNode* blockerNode = node->getBlockerIndividualNode();
					if (blockerNode) {
						nodeJson.insert("blocker", (qint64)blockerNode->getIndividualNodeID());
					}
					nodeJson.insert("blocks", getBlockedNodesJson(node));
					nodeJson.insert("flags", getFlagsJson(node));
					nodeJson.insert("label", getLabelJson(node));
					nodeJson.insert("pending", getPendingQueueJson(node));
					nodeJson.insert("links", getLinksJson(node));
					return nodeJson;
				}


				QJsonArray CCompletionGraphSnapshotWriter::getBlockedNodesJson(CIndividualProcessNode* node) {
					// the nodes that this node registered as blocked by itself (direct blocking) or as waiting for its processing
					QJsonArray blockedArray;
					for (CXLinker<CIndividualProcessNode*>* blockedLinker = node->getBlockedIndividualsLinker(); blockedLinker; blockedLinker = blockedLinker->getNext()) {
						blockedArray.append((qint64)blockedLinker->getData()->getIndividualNodeID());
					}
					for (CXLinker<CIndividualProcessNode*>* blockedLinker = node->getProcessingBlockedIndividualsLinker(); blockedLinker; blockedLinker = blockedLinker->getNext()) {
						blockedArray.append((qint64)blockedLinker->getData()->getIndividualNodeID());
					}
					return blockedArray;
				}

				QJsonArray CCompletionGraphSnapshotWriter::getFlagsJson(CIndividualProcessNode* node) {
					struct CFlagName {
						cint64 mFlag;
						const char* mName;
					};
					static const CFlagName flagNames[] = {
						{ CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED, "CACHED" },
						{ CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED, "INVALIDATED" },
						{ CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODELOCATED, "CACHE-TESTED" },
						{ CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODEEXTENDED, "EXTENDED-BEYOND-CACHE" },
						{ CIndividualProcessNode::PRFDIRECTBLOCKED, "DIRECT-BLOCKED" },
						{ CIndividualProcessNode::PRFINDIRECTBLOCKED, "INDIRECT-BLOCKED" },
						{ CIndividualProcessNode::PRFPROCESSINGBLOCKED, "PROCESSING-BLOCKED" },
						{ CIndividualProcessNode::PRFPROCESSINGCOMPLETED, "COMPLETED" },
						{ CIndividualProcessNode::PRFPURGEDBLOCKED, "PURGED" }
					};
					QJsonArray flagArray;
					for (const CFlagName& flagName : flagNames) {
						if (node->hasPartialProcessingRestrictionFlags(flagName.mFlag)) {
							flagArray.append(QString(flagName.mName));
						}
					}
					return flagArray;
				}


				QJsonArray CCompletionGraphSnapshotWriter::getLabelJson(CIndividualProcessNode* node) {
					QJsonArray labelArray;
					CReapplyConceptLabelSet* labelSet = node->getReapplyConceptLabelSet(false);
					if (!labelSet) {
						return labelArray;
					}
					CReapplyConceptLabelSetIterator labelIt = labelSet->getConceptLabelSetIterator(true, true, false);
					while (labelIt.hasNext()) {
						CConceptDescriptor* conceptDescriptor = labelIt.getConceptDescriptor();
						CDependencyTrackPoint* depTrackPoint = labelIt.getDependencyTrackPoint();
						if (conceptDescriptor) {
							CConcept* concept = conceptDescriptor->getConcept();
							QJsonObject entryJson;
							entryJson.insert("c", getShortConceptString(concept, conceptDescriptor->isNegated()));
							QString conceptKind = getConceptKind(concept);
							entryJson.insert("kind", conceptKind);
							if (conceptKind == "other" || conceptKind == "impl") {
								// concepts without a short notation keep the full text so that they can still be identified
								entryJson.insert("raw", CConceptTextFormater::getConceptString(concept, conceptDescriptor->isNegated()));
							}
							entryJson.insert("tag", (qint64)concept->getConceptTag());
							entryJson.insert("neg", conceptDescriptor->isNegated());
							entryJson.insert("nd", isNonDeterministic(depTrackPoint));
							entryJson.insert("branch", (qint64)getBranchTag(depTrackPoint));
							labelArray.append(entryJson);
						}
						labelIt.moveNext();
					}
					return labelArray;
				}


				QJsonArray CCompletionGraphSnapshotWriter::getPendingQueueJson(CIndividualProcessNode* node) {
					QJsonArray pendingArray;
					CConceptProcessingQueue* conceptQueue = node->getConceptProcessingQueue(false);
					if (!conceptQueue) {
						return pendingArray;
					}
					CConceptProcessingQueueIterator queueIt = conceptQueue->getConceptProcessingQueueIterator();
					while (queueIt.hasNext()) {
						CConceptProcessDescriptor* processDescriptor = queueIt.next();
						if (processDescriptor && processDescriptor->getConceptDescriptor()) {
							CConceptDescriptor* conceptDescriptor = processDescriptor->getConceptDescriptor();
							QJsonObject entryJson;
							entryJson.insert("c", getShortConceptString(conceptDescriptor->getConcept(), conceptDescriptor->isNegated()));
							entryJson.insert("kind", getConceptKind(conceptDescriptor->getConcept()));
							entryJson.insert("nd", isNonDeterministic(processDescriptor->getDependencyTrackPoint()));
							pendingArray.append(entryJson);
						}
					}
					return pendingArray;
				}


				QJsonArray CCompletionGraphSnapshotWriter::getLinksJson(CIndividualProcessNode* node) {
					QJsonArray linkArray;
					// the successor iterator returns one link per neighbour; all links to a neighbour (e.g. r and r⁻ of a
					// two-way connection) are read with the role iterator of that neighbour
					CSuccessorIterator successorIt = node->getSuccessorIterator();
					while (successorIt.hasNext()) {
						cint64 neighbourID = successorIt.nextIndividualID(true);
						CSuccessorRoleIterator roleIt = node->getSuccessorRoleIterator(neighbourID);
						while (roleIt.hasNext()) {
							CIndividualLinkEdge* link = roleIt.next(true);
							CDependencyTrackPoint* depTrackPoint = link->getDependencyTrackPoint();
							QJsonObject linkJson;
							linkJson.insert("to", (qint64)neighbourID);
							linkJson.insert("role", getShortRoleString(link->getLinkRole()));
							linkJson.insert("nd", isNonDeterministic(depTrackPoint));
							linkJson.insert("branch", (qint64)getBranchTag(depTrackPoint));
							linkArray.append(linkJson);
						}
					}
					return linkArray;
				}


				bool CCompletionGraphSnapshotWriter::isNonDeterministic(CDependencyTrackPoint* depTrackPoint) {
					return getBranchTag(depTrackPoint) > 0;
				}


				cint64 CCompletionGraphSnapshotWriter::getBranchTag(CDependencyTrackPoint* depTrackPoint) {
					if (!depTrackPoint) {
						return 0;
					}
					return depTrackPoint->getBranchingTag();
				}


				QString CCompletionGraphSnapshotWriter::getShortName(const QString& iriName) {
					cint64 cutIndex = qMax(iriName.lastIndexOf('#'), iriName.lastIndexOf('/'));
					return iriName.mid(cutIndex + 1);
				}


				QString CCompletionGraphSnapshotWriter::getShortRoleString(CRole* role) {
					if (!role) {
						return QString("?");
					}
					if (role->hasPropertyName()) {
						return getShortName(CIRIName::getRecentIRIName(role->getPropertyNameLinker()));
					}
					CRole* inverseRole = role->getInverseRole();
					if (inverseRole && inverseRole->hasPropertyName()) {
						return getShortName(CIRIName::getRecentIRIName(inverseRole->getPropertyNameLinker())) + QString::fromUtf8("\u207b");
					}
					return QString("role%1").arg(role->getRoleTag());
				}


				QString CCompletionGraphSnapshotWriter::getConceptKind(CConcept* concept) {
					cint64 opCode = concept->getProcessingOperatorTag();
					CConceptOperator* conceptOperator = concept->getConceptOperator();
					if (opCode == CCNOMINAL) {
						return "nominal";
					}
					if (opCode == CCIMPLTRIG || opCode == CCBRANCHTRIG) {
						return "trigger";
					}
					if (opCode == CCATLEAST) {
						return "atleast";
					}
					if (opCode == CCATMOST) {
						return "atmost";
					}
					if (opCode == CCSOME || opCode == CCAQSOME) {
						return "some";
					}
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE)) {
						return "all";
					}
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE)) {
						return "impl";
					}
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AND_AQAND_TYPE)) {
						return concept->hasClassName() ? "class" : "and";
					}
					return concept->hasClassName() ? "class" : "other";
				}


				QString CCompletionGraphSnapshotWriter::getShortOperandsString(CConcept* concept, const QString& separator, bool negated, cint64 depth) {
					QStringList operandStrings;
					CSortedNegLinker<CConcept*>* operandLinker = concept->getOperandList();
					while (operandLinker) {
						operandStrings.append(getShortConceptString(operandLinker->getData(), operandLinker->isNegated() != negated, depth + 1));
						operandLinker = operandLinker->getNext();
					}
					return operandStrings.join(separator);
				}


				QString CCompletionGraphSnapshotWriter::getShortConceptString(CConcept* concept, bool negated, cint64 depth) {
					static const QString notSign = QString::fromUtf8("\u00ac");
					static const QString topSign = QString::fromUtf8("\u22a4");
					QString negation = negated ? notSign : QString();
					if (depth > SNAPSHOT_MAX_CONCEPT_DEPTH) {
						return negation + QString("#%1").arg(concept->getConceptTag());
					}
					if (concept->hasClassName()) {
						QString className = getShortName(CIRIName::getRecentIRIName(concept->getClassNameLinker()));
						if (className == "Thing") {
							className = topSign;
						}
						return negation + className;
					}

					cint64 opCode = concept->getProcessingOperatorTag();
					CConceptOperator* conceptOperator = concept->getConceptOperator();
					if (opCode == CCNOMINAL) {
						return negation + "{" + getShortName(CIRIName::getRecentIRIName(concept->getNominalIndividual()->getIndividualNameLinker())) + "}";
					}
					if (opCode == CCSOME || opCode == CCAQSOME) {
						// the negation of an existential restriction is a universal restriction over the negated filler
						QString quantifier = negated ? QString::fromUtf8("\u2200") : QString::fromUtf8("\u2203");
						return quantifier + getShortRoleString(concept->getRole()) + "." + getShortOperandsString(concept, " , ", negated, depth);
					}
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE)) {
						QString quantifier = negated ? QString::fromUtf8("\u2203") : QString::fromUtf8("\u2200");
						return quantifier + getShortRoleString(concept->getRole()) + "." + getShortOperandsString(concept, " , ", negated, depth);
					}
					if (opCode == CCATLEAST || opCode == CCATMOST) {
						bool atLeast = (opCode == CCATLEAST) != negated;
						cint64 cardinality = concept->getParameter();
						if (negated) {
							cardinality += (opCode == CCATLEAST) ? -1 : 1;
						}
						QString relation = atLeast ? QString::fromUtf8("\u2265") : QString::fromUtf8("\u2264");
						return QString("%1%2 %3.%4").arg(relation).arg(cardinality).arg(getShortRoleString(concept->getRole())).arg(getShortOperandsString(concept, " , ", false, depth));
					}
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AND_AQAND_TYPE) && concept->getOperandList()) {
						bool allOperandsNegated = true;
						CSortedNegLinker<CConcept*>* operandLinker = concept->getOperandList();
						while (operandLinker) {
							allOperandsNegated = allOperandsNegated && operandLinker->isNegated();
							operandLinker = operandLinker->getNext();
						}
						if (negated && allOperandsNegated) {
							// not(not x and not y) is the disjunction of x and y
							return "(" + getShortOperandsString(concept, QString::fromUtf8(" \u2294 "), true, depth) + ")";
						}
						return negation + "(" + getShortOperandsString(concept, QString::fromUtf8(" \u2293 "), false, depth) + ")";
					}
					if (opCode == CCIMPLTRIG) {
						return negation + QString("trigger#%1").arg(concept->getConceptTag());
					}
					return negation + QString("concept#%1").arg(concept->getConceptTag());
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
