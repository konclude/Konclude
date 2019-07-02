/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CBranchTriggerPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CBranchTriggerPreProcess::CBranchTriggerPreProcess() {
			}


			CBranchTriggerPreProcess::~CBranchTriggerPreProcess() {
			}


			CConcreteOntology *CBranchTriggerPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.BranchTriggerExtraction.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				if (nonELConstructsUsed || !skipForELOntologies) {

					CMBox *mBox = ontology->getDataBoxes()->getMBox();
					CTBox *tBox = ontology->getDataBoxes()->getTBox();
					CRBox *rBox = ontology->getDataBoxes()->getRBox();
					CABox *aBox = ontology->getDataBoxes()->getABox();

					mConceptVec = tBox->getConceptVector();
					CBranchingTriggerVector* branchTriggVec = mBox->getBranchingTriggerVector();

					CRoleVector *roles = rBox->getRoleVector();

					CConcept *topConcept = mConceptVec->getLocalData(1);

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOnto = ontology;
					mTBox = tBox;
					mRoleDomainTriggerConceptHash = nullptr;

					mStatConceptBranchTriggers = 0;
					mStatRoleBranchTriggers = 0;
					mStatBranchTriggers = 0;


					cint64 conCount = tBox->getConceptCount();
					for (cint64 conIdx = 0; conIdx < conCount; ++conIdx) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						if (concept) {
							cint64 conOpCode = concept->getOperatorCode();
							cint64 opCount = concept->getOperandCount();
							CConceptRoleBranchingTrigger* branchingTriggers = nullptr;
							bool negConTriggers = false;
							if (!branchTriggVec->hasData(conIdx)) {
								if (opCount > 1) {
									if (conOpCode == CCAND || conOpCode == CCEQ) {
										negConTriggers = true;
										branchingTriggers = createBranchingTriggers(concept,negConTriggers);
									} else if (conOpCode == CCOR) {
										branchingTriggers = createBranchingTriggers(concept,negConTriggers);
									}
								}
								if (branchingTriggers) {
									++mStatBranchTriggers;
									branchTriggVec->setData(conIdx,branchingTriggers);
								}
							}
						}
					}

					LOG(INFO,"::Konclude::Reasoner::Preprocess::BranchTriggerExtractor",logTr("Extracted %1 concept and %2 role branch triggers for %3 concepts.").arg(mStatConceptBranchTriggers).arg(mStatRoleBranchTriggers).arg(mStatBranchTriggers),this);
				}
				return ontology;
			}

			CConceptRoleBranchingTrigger *CBranchTriggerPreProcess::createBranchingTriggers(CConcept* concept, bool negated) {
				typedef QPair<CConcept *, bool> TConNegPair;
				CConceptRoleBranchingTrigger* branchingTriggerList = nullptr;

				QSet<TConNegPair> branchingTriggerSet;

				QList<TConNegPair> disjList;
				QList<TConNegPair> disjInitList;
				disjInitList.append(TConNegPair(concept,negated));


				while (!disjInitList.isEmpty()) {
					TConNegPair disConNegPair(disjInitList.takeFirst());
					CConcept *disCon = disConNegPair.first;
					bool disNeg = disConNegPair.second;
					qint64 disConOpCode = disCon->getOperatorCode();
					if (!disNeg && (disConOpCode == CCOR) || disNeg && (disConOpCode == CCAND || disConOpCode == CCEQ)) {		
						CSortedNegLinker<CConcept *> *splitOrOpIt = disCon->getOperandList();
						while (splitOrOpIt) {
							CConcept *splitCon = splitOrOpIt->getData();
							bool splitConNeg = splitOrOpIt->isNegated();
							disjInitList.append(TConNegPair(splitCon,splitConNeg^disNeg));
							splitOrOpIt = splitOrOpIt->getNext();
						}
					} else {
						disjList.append(TConNegPair(disCon,disNeg));
					}
				}		

				while (!disjList.isEmpty()) {
					TConNegPair disConNegPair(disjList.takeFirst());

					CConcept *disCon = disConNegPair.first;
					bool disNeg = disConNegPair.second;
					cint64 conOpCount = disCon->getOperandCount();

					CConceptRoleBranchingTrigger* branchingTrigger = nullptr;

					qint64 disConOpCode = disCon->getDefinitionOperatorTag();
					if (disNeg && (disConOpCode == CCSUB || disConOpCode == CCATOM)) {
						if (!branchingTriggerSet.contains(TConNegPair(disCon,!disNeg))) {
							branchingTriggerSet.insert(TConNegPair(disCon,!disNeg));
							branchingTrigger = CObjectAllocator< CConceptRoleBranchingTrigger >::allocateAndConstruct(mMemMan);
							branchingTrigger->initConceptBranchingTrigger(disCon,!disNeg);
							++mStatConceptBranchTriggers;
						}
					} else if (disConOpCode == CCALL && !disNeg || disConOpCode == CCSOME && disNeg || disConOpCode == CCATMOST && !disNeg || disConOpCode == CCATLEAST && disNeg && conOpCount > 0) {
						CRole* role = disCon->getRole();
						CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
						CConcept* triggerDomConcept = getRoleDomainTriggerConcept(locatedRole);
						if (!branchingTriggerSet.contains(TConNegPair(triggerDomConcept,false))) {
							branchingTriggerSet.insert(TConNegPair(triggerDomConcept,false));
							branchingTrigger = CObjectAllocator< CConceptRoleBranchingTrigger >::allocateAndConstruct(mMemMan);
							branchingTrigger->initConceptBranchingTrigger(triggerDomConcept,false);
							++mStatRoleBranchTriggers;
						}
					}
					if (branchingTrigger) {
						branchingTriggerList = branchingTrigger->append(branchingTriggerList);
					}
				}
				return branchingTriggerList;
			}


			bool CBranchTriggerPreProcess::addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(addedConcept,negated,role->getDomainConceptList());
				role->setDomainConceptList(newBaseOpCon);
				return true;
			}


			CConcept* CBranchTriggerPreProcess::getRoleDomainTriggerConcept(CRole* role) {
				if (!mRoleDomainTriggerConceptHash) {
					mRoleDomainTriggerConceptHash = mTBox->getRoleDomainTriggerConceptHash(true);
				}
				CConcept*& triggerConcept = (*mRoleDomainTriggerConceptHash)[role];
				if (!triggerConcept) {
					triggerConcept = createTriggerConcept();
					addRoleDomainConcept(role,triggerConcept,false);
				}
				return triggerConcept;
			}


			CConcept* CBranchTriggerPreProcess::createTriggerConcept() {
				CConcept* triggerConcept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				triggerConcept->initConcept();
				cint64 nextConTag = mConceptVec->getItemCount();
				triggerConcept->initTag(nextConTag);
				triggerConcept->setOperatorCode(CCIMPLTRIG);
				mConceptVec->setLocalData(nextConTag,triggerConcept);
				return triggerConcept;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
