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

#include "CTriggeredImplicationGCIAbsorberPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
	#define KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(a)
#else
	//#define KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(a) a
	#define KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(a)
#endif



			CTriggeredImplicationGCIAbsorberPreProcess::CTriggeredImplicationGCIAbsorberPreProcess() {
				mConfRequiresExistingInverseRole = false;
				mConfAbsorbEqClassDefinitions = true;
				mConfAbsorbDisjunctions = true;
				mConfAbsorbSimpleDisjunctions = true;
				mConfPartialGCIAbsorption = true;
			}


			CTriggeredImplicationGCIAbsorberPreProcess::~CTriggeredImplicationGCIAbsorberPreProcess() {
			}


			CConcreteOntology *CTriggeredImplicationGCIAbsorberPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				CMBox *mBox = ontology->getDataBoxes()->getMBox();
				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CRBox *rBox = ontology->getDataBoxes()->getRBox();
				CABox *aBox = ontology->getDataBoxes()->getABox();

				mConceptVec = tBox->getConceptVector();
				cint64 conceptCount = mConceptVec->getItemCount();

				CRoleVector *roles = rBox->getRoleVector();

				CConcept *topConcept = tBox->getTopConcept();
				mBottomConcept = tBox->getBottomConcept();

				mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
				mOnto = ontology;


				CConfiguration* config = context->getConfiguration();
				mConfAbsorbDisjunctions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DisjunctionToImplicationAbsorption",true);
				mConfAbsorbSimpleDisjunctions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DisjunctionToImplicationAbsorptionByExistingTriggers",true);
				mConfAbsorbEqClassDefinitions = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.EquivalentDefinitionToSubclassImplicationAbsorption",true);
				mConfPartialGCIAbsorption = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.PartialGCIImplicationAbsorption",true);

				
				CImplicationReplacementVector* impRepVec = nullptr;
				if (mConfAbsorbDisjunctions || mConfAbsorbSimpleDisjunctions) {
					impRepVec = mBox->getImplicationReplacementVector(true);
				}

				mTriggerImplHash = tBox->getTriggerImplicationHash(true);


				mStatInvRolesGenerated = 0;
				mStatGCIAbsorbed = 0;
				mStatGCINormalAbsorbed = 0;
				mStatGCITotal = 0;
				mStatEqAbsorbed = 0;
				mStatEqTotal = 0;
				mStatImplConceptsGenerated = 0;
				mStatGCIAbsorptionPartialExtended = 0;
				mStatGCIPartialAbsorbed = 0;

				if (mConfAbsorbEqClassDefinitions) {
					QList<CConcept*> equivConceptAbsorbList;
					// try to absorb equivalent classes expressions
					for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCEQ) {
								++mStatEqTotal;
								if (isEquivalenceConceptTriggeredImplicationAbsorbable(concept,false)) {
									// absorb to subclass and implication trigger
									concept->setOperatorCode(CCSUB);
									equivConceptAbsorbList.append(concept);
									++mStatEqAbsorbed;
								} 
							}
						}
					}

					foreach (CConcept* concept, equivConceptAbsorbList) {
						createEquivalenceAbsorbedTriggeredImplication(concept,false);
					}
				}

				mFirstNewConceptTag = mConceptVec->getItemCount();
				mLastNewConceptTag = mFirstNewConceptTag;


				if (topConcept) {
					CSortedNegLinker<CConcept*>* lastgciConLinker = nullptr;
					CSortedNegLinker<CConcept*>* gciConLinker = topConcept->getOperandList();
					CSortedNegLinker<CConcept*>* gciConLinkerIt = gciConLinker;
					while (gciConLinkerIt) {
						CConcept* opCon = gciConLinkerIt->getData();
						bool opNegation = gciConLinkerIt->isNegated();
						++mStatGCITotal;
						bool absorbed = false;

						mFirstNewConceptTag = mLastNewConceptTag;

						KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastGCIString = generateDebugGCIConceptString(opCon,opNegation));

						if (absorbGCIConceptsToTriggeredImplications(opCon,opNegation)) {
							++mStatGCINormalAbsorbed;
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCITechniqueString = QString("implication absorbed"));
							absorbed = true;
						}


						if (!absorbed && mConfPartialGCIAbsorption) {
							if (absorbPartialGCIConceptsToTriggeredImplications(opCon,opNegation)) {
								++mStatGCIPartialAbsorbed;
								KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCITechniqueString = QString("partial implication absorbed"));
								absorbed = true;
							}
						}


						if (absorbed) {
							++mStatGCIAbsorbed;
							mLastNewConceptTag = mConceptVec->getItemCount();
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCIConceptString = generateDebugGeneratedAbsorbedImplicationString(mFirstNewConceptTag,mLastNewConceptTag));
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugLastAbsorbedGCIString = QString("GCI:<br>\r\n<br>\r\n%1<br>\r\n%2 to<br>\r\n<br>\r\n%3<p>\r\n\r\n").arg(mDebugLastGCIString).arg(mDebugLastAbsorbedGCITechniqueString).arg(mDebugLastAbsorbedGCIConceptString));
							KONCLUCE_PREPROCESSING_TRIGGERED_IMPLICATION_STRING_INSTRUCTION(mDebugTotalAbsorbedGCIString += mDebugLastAbsorbedGCIString);
							if (lastgciConLinker) {
								lastgciConLinker->setNext(gciConLinkerIt->getNext());
							} else {
								topConcept->setOperandList(gciConLinkerIt->getNext());
							}
							topConcept->incOperandCount(-1);
						} else {
							lastgciConLinker = gciConLinkerIt;
						}

						gciConLinkerIt = gciConLinkerIt->getNext();
					}
				}


				if (mConfAbsorbDisjunctions || mConfAbsorbSimpleDisjunctions) {
					for (cint64 conIdx = 0; conIdx < conceptCount; conIdx++) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						bool localRepData = false;
						CReplacementData* prevReplData = impRepVec->getData(conIdx,&localRepData);
						if (concept && (!prevReplData || !localRepData)) {
							cint64 opCode = concept->getOperatorCode();
							bool absorbable = false;
							bool absorbNeg = false;
							bool absorbPos = false;
							cint64 opCount = concept->getOperandCount();
							if (opCount > 1) {
								if (opCode == CCAND) {
									absorbNeg = true;
									absorbPos = true;
								} else if (opCode == CCOR) {
									absorbNeg = false;
									absorbPos = true;
								}
								if (absorbPos) {
									if (mConfAbsorbSimpleDisjunctions && isDisjunctionConceptSimpleTriggeredImplicationAbsorbable(concept,absorbNeg)) {
										absorbable = true;
									} else if (mConfAbsorbDisjunctions && isDisjunctionConceptTriggeredImplicationAbsorbable(concept,absorbNeg)) {
										// add implication trigger absorption
										absorbable = true;
									}
								}
								if (absorbable) {
									CConcept* absorbedImpCon = createDisjunctionAbsorbedTriggeredImplication(concept,absorbNeg);

									CReplacementData* replData = prevReplData;
									if (!localRepData) {
										replData = CObjectAllocator<CReplacementData>::allocateAndConstruct(mMemMan);
										replData->initReplacementData(prevReplData);
										impRepVec->setData(concept->getConceptTag(),replData);
									}
									replData->setImplicationReplacementConcept(absorbedImpCon);
								}
							}
						}
					}
				}
				
				LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 of %2 equivalence class definitions.").arg(mStatEqAbsorbed).arg(mStatEqTotal),this);
				LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 of %2 GCI definitions.").arg(mStatGCIAbsorbed).arg(mStatGCITotal),this);
				if (mStatGCITotal > 0) {
					LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Absorbed %1 GCI definitions only partially and extended %2 absorptions partially.").arg(mStatGCIPartialAbsorbed).arg(mStatGCIAbsorptionPartialExtended),this);
				}
				LOG(INFO,"::Konclude::Reasoner::Preprocess::TriggeredImplicationAbsorber",logTr("Generated %1 triggered implication concepts and %2 additional inverse roles.").arg(mStatImplConceptsGenerated).arg(mStatInvRolesGenerated),this);
	
				return ontology;
			}




			QString CTriggeredImplicationGCIAbsorberPreProcess::generateDebugGCIConceptString(CConcept* concept, bool conceptNegation) {
				mDebugGCIString = CConceptTextFormater::getConceptString(concept,conceptNegation);
				return mDebugGCIString;
			}


			QString CTriggeredImplicationGCIAbsorberPreProcess::generateDebugGeneratedAbsorbedImplicationString(cint64 minNewConceptTag, cint64 lastNewConceptTag) {
				QString absorbedString;
				for (cint64 tag = minNewConceptTag; tag <= lastNewConceptTag; ++tag) {
					CConcept* concept = mConceptVec->getData(tag);
					if (concept) {
						QString tmpString = CConceptTextFormater::getConceptString(concept,false);
						absorbedString += QString("%1<br>\r\n").arg(tmpString);
					}
				}
				mDebugAbsorbedString = absorbedString;
				return absorbedString;
			}


			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createImpliedConcept() {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 nextConTag = mConceptVec->getItemCount();
				concept->initTag(nextConTag);
				concept->setOperatorCode(CCAND);
				mConceptVec->setLocalData(nextConTag,concept);
				return concept;
			}


			bool CTriggeredImplicationGCIAbsorberPreProcess::addConceptToImplied(CConcept* impliedConcept, CConcept* concept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(concept,negated,impliedConcept->getOperandList());
				impliedConcept->setOperandList(newBaseOpCon);
				impliedConcept->incOperandCount();
				return true;
			}

			bool CTriggeredImplicationGCIAbsorberPreProcess::addSubClassConcept(CConcept* subclassConcept, CConcept* addedConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(addedConcept,negated,subclassConcept->getOperandList());
				subclassConcept->setOperandList(newBaseOpCon);
				subclassConcept->incOperandCount();
				subclassConcept->setOperatorCode(CCSUB);
				return true;
			}


			bool CTriggeredImplicationGCIAbsorberPreProcess::addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(addedConcept,negated,role->getDomainConceptList());
				role->setDomainConceptList(newBaseOpCon);
				return true;
			}



			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createImplicationConcept(CConcept* impliedConcept, bool negated) {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 nextConTag = mConceptVec->getItemCount();
				concept->initTag(nextConTag);
				concept->setOperatorCode(CCIMPL);

				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(impliedConcept,negated);
				concept->setOperandList(newBaseOpCon);
				concept->incOperandCount();

				mConceptVec->setLocalData(nextConTag,concept);
				++mStatImplConceptsGenerated;
				return concept;
			}



			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createImplicationTriggerConcept(CConcept* impConcept, bool negated) {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 nextConTag = mConceptVec->getItemCount();
				concept->initTag(nextConTag);
				concept->setOperatorCode(CCIMPLTRIG);
				mConceptVec->setLocalData(nextConTag,concept);
				if (impConcept) {
					addConceptToImplied(concept,impConcept,negated);
				}
				return concept;
			}

			bool CTriggeredImplicationGCIAbsorberPreProcess::addImplicationTrigger(CConcept* implConcept, CConcept* triggerConcept, bool negated) {
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(triggerConcept,negated,implConcept->getOperandList()->getNext());
				implConcept->getOperandList()->setNext(newTriggerOpCon);
				implConcept->incOperandCount();
				mTriggerImplHash->insertMulti(triggerConcept,implConcept);
				return true;
			}


			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createTriggerPropagationConcept(CConcept* destConcept, CRole* backwardPropRole) {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				CRole* invRole = getInverseRole(backwardPropRole);
				concept->initConcept();
				cint64 nextConTag = mConceptVec->getItemCount();
				concept->initTag(nextConTag);
				concept->setOperatorCode(CCIMPLALL);
				concept->setRole(invRole);
				CSortedNegLinker<CConcept*>* newTriggerOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newTriggerOpCon->init(destConcept,false);
				concept->setOperandList(newTriggerOpCon);
				concept->incOperandCount();
				mConceptVec->setLocalData(nextConTag,concept);
				return concept;
			}


			CRole* CTriggeredImplicationGCIAbsorberPreProcess::getInverseRole(CRole* role) {
				CSortedNegLinker<CRole*>* eqInvRoleIt = role->getInverseEquivalentRoleList();
				while (eqInvRoleIt) {
					if (eqInvRoleIt->isNegated()) {
						return eqInvRoleIt->getData();
					}
					eqInvRoleIt = eqInvRoleIt->getNext();
				}
				CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
				while (superRoleIt) {
					if (superRoleIt->isNegated()) {
						CRole* superRole = superRoleIt->getData();
						CSortedNegLinker<CRole*>* superRoleSuperRoleIt = superRole->getIndirectSuperRoleList();
						while (superRoleSuperRoleIt) {
							if (superRoleSuperRoleIt->isNegated() && superRoleSuperRoleIt->getData() == role) {
								return superRole;
							}
							superRoleSuperRoleIt = superRoleSuperRoleIt->getNext();
						}
					}
					superRoleIt = superRoleIt->getNext();
				}

				// create inverse role
				CRole* invRole = CConcreteOntologyInverseRoleBuilder::createInverseRole(role,mOnto);
				++mStatInvRolesGenerated;
				return invRole;
			}



			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createEquivalenceAbsorbedTriggeredImplication(CConcept* eqConcept, bool negated) {
				CConcept* impliedConcept = eqConcept;
				CConcept* implicationConcept = createImplicationConcept(impliedConcept,false);


				//if (eqConcept->hasClassName()) {
				//	QString iriClassNameString = CIRIName::getRecentIRIName(eqConcept->getClassNameLinker());
				//	if (iriClassNameString == QString("http://www.owllink.org/testsuite/galen#LungPathology")) {
				//		bool bug = true;
				//	}
				//}


				CSortedNegLinker<CConcept*>* opConLinkerIt = eqConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					createImplicationAddedToTrigger(implicationConcept,nullptr,opConcept,opNegated);
					opConLinkerIt = opConLinkerIt->getNext();
				}
				return implicationConcept;
			}




			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createDisjunctionAbsorbedTriggeredImplication(CConcept* orConcept, bool negated) {
				CConcept* impliedConcept = createImpliedConcept();
				CConcept* implicationConcept = createImplicationConcept(impliedConcept,false);

				bool oneNotTriggered = false;

				CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (!oneNotTriggered && !opConLinkerIt->hasNext() || !isConceptImplicationTriggerable(opConcept,!(negated^opNegated))) {
						oneNotTriggered = true;
						addConceptToImplied(impliedConcept,opConcept,negated^opNegated);
					} else {
						createTrigger(implicationConcept,nullptr,opConcept,!(negated^opNegated),nullptr,nullptr);
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				if (impliedConcept->getOperandCount() >= 1) {
					impliedConcept->setOperatorCode(CCOR);
				} else {
					impliedConcept->setOperatorCode(CCAND);
				}
				return implicationConcept;
			}



			cint64 CTriggeredImplicationGCIAbsorberPreProcess::createImplicationAddedToTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated) {
				cint64 triggerCount = 0;
				cint64 maxImpAddTriggerCount = 0;
				CConcept* maxImpAddTriggerConcept = nullptr;
				if (!implicationConcept) {
					implicationConcept = createImplicationConcept(triggerConcept,false);
					triggerConcept = nullptr;
				}

				triggerCount = createTrigger(implicationConcept,triggerConcept,concept,negated,&maxImpAddTriggerCount,&maxImpAddTriggerConcept);

				if (maxImpAddTriggerConcept) {
					cint64 opCode = maxImpAddTriggerConcept->getOperatorCode();
					if (opCode != CCNONE) {
						addConceptToImplied(maxImpAddTriggerConcept,implicationConcept,false);
					} else {
						addSubClassConcept(maxImpAddTriggerConcept,implicationConcept,false);
					}
				}

				return triggerCount;
			}



			cint64 CTriggeredImplicationGCIAbsorberPreProcess::createTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated, cint64* impAddTriggerCount, CConcept** impAddTriggerConcept) {
				cint64 triggerCount = 0;
				cint64 triggerForAddingImpCount = 0;
				CConcept* triggerForAddingImpConcept = nullptr;
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CRole* role = concept->getRole();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				if (!negated && (opCode == CCIMPLALL || opCode == CCIMPL || opCode == CCIMPLTRIG)) {
					// nothing to do
				} else if (!negated && (opCode == CCATOM || opCode == CCSUB)) {
					CConcept* locatedConcept = CConceptRoleIndividualLocator::getLocatedConcept(concept,mOnto);

					//if (concept->hasClassName()) {
					//	QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
					//	if (iriClassNameString == QString("http://www.owllink.org/testsuite/galen#nonNormal")) {
					//		bool bug = true;
					//	}
					//}

					++triggerForAddingImpCount;
					++triggerCount;
					triggerForAddingImpConcept = locatedConcept;
					if (implicationConcept) {
						addImplicationTrigger(implicationConcept,concept,!negated);
					} else {
						addSubClassConcept(locatedConcept,triggerConcept,false);
					}
				} else if (!negated && (opCode == CCAND || opCode == CCEQ) || negated && (opCode == CCOR)) {
					// and trigger
					if (triggerConcept) {
						return createImplicationAddedToTrigger(implicationConcept,triggerConcept,concept,negated);
					}

					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;

						cint64 tmpImpAddTriggerCount = 0;
						CConcept* tmpImpAddTriggerConcept = nullptr;

						triggerCount += createTrigger(implicationConcept,triggerConcept,opConcept,opNegated,&tmpImpAddTriggerCount,&tmpImpAddTriggerConcept);

						if (triggerForAddingImpConcept == nullptr || tmpImpAddTriggerCount > triggerForAddingImpCount) {
							triggerForAddingImpCount = tmpImpAddTriggerCount;
							triggerForAddingImpConcept = tmpImpAddTriggerConcept;
						}

						opConLinkerIt = opConLinkerIt->getNext();
					}
				} else {					
					CConcept* conTriggerConcept = mConceptTriggerHash.value(QPair<CConcept*,bool>(concept,negated));
					if (implicationConcept && conTriggerConcept) {
						addImplicationTrigger(implicationConcept,conTriggerConcept,true);
						triggerForAddingImpCount += mTriggerCountHash.value(conTriggerConcept,0);
						triggerCount += triggerForAddingImpCount;
						triggerForAddingImpConcept = conTriggerConcept;
					} else {
						if (implicationConcept) {
							triggerConcept = createImplicationTriggerConcept(nullptr,false);
							addImplicationTrigger(implicationConcept,triggerConcept,true);
							mConceptTriggerHash.insert(QPair<CConcept*,bool>(concept,negated),triggerConcept);
							implicationConcept = nullptr;
						}
						if (!negated && (opCode == CCNOMINAL)) {
							CIndividual* individual = concept->getNominalIndividual();
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual,mOnto);
							triggerCount += 2;
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(triggerConcept,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);
						} else if (!negated && (opCode == CCOR) || negated && (opCode == CCAND || opCode == CCEQ)) {
							// or trigger

							cint64 minTriggerCount = 0;

							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegated = opConLinkerIt->isNegated()^negated;

								cint64 tmpTriggerCount = 0;

								tmpTriggerCount = createTrigger(implicationConcept,triggerConcept,opConcept,opNegated,nullptr,nullptr);

								if (minTriggerCount == 0 || tmpTriggerCount > 0 && tmpTriggerCount < minTriggerCount) {
									minTriggerCount = tmpTriggerCount;
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}

							triggerCount += minTriggerCount;

						} else if (!negated && (opCode == CCSOME || opCode == CCSELF) || negated && (opCode == CCALL)) {
							if (opConLinker) {
								CConcept* nextLevelTrigger = createTriggerPropagationConcept(triggerConcept,role);
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated()^negated;
									triggerCount += createTrigger(implicationConcept,nextLevelTrigger,opConcept,opNegated,nullptr,nullptr);
									opConLinkerIt = opConLinkerIt->getNext();
								}
							} else {
								CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
								addRoleDomainConcept(locatedRole,triggerConcept,false);
							}
							// add one for role
							triggerCount += 1;
						} else if (!negated && (opCode == CCVALUE)) {
							CConcept* nextLevelTrigger = createTriggerPropagationConcept(triggerConcept,role);

							CIndividual* individual = concept->getNominalIndividual();
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual,mOnto);
							triggerCount += 2;
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(nextLevelTrigger,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);

							// add one for role
							triggerCount += 1;
						} else if (!negated && (opCode == CCATLEAST && parm == 1) || negated && (opCode == CCATMOST && parm == 2)) {
							if (opConLinker) {
								CConcept* nextLevelTrigger = createTriggerPropagationConcept(triggerConcept,role);
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated();
									triggerCount += createTrigger(implicationConcept,nextLevelTrigger,opConcept,opNegated,nullptr,nullptr);
									opConLinkerIt = opConLinkerIt->getNext();
								}
							} else {
								CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
								addRoleDomainConcept(locatedRole,triggerConcept,false);
							}
							triggerCount += 1;
						}
						mTriggerCountHash.insert(triggerConcept,triggerCount);
						triggerForAddingImpConcept = triggerConcept;
						triggerForAddingImpCount = triggerCount;
					}
				}

				if (impAddTriggerConcept && impAddTriggerCount) {
					if (*impAddTriggerCount < triggerForAddingImpCount) {
						*impAddTriggerCount = triggerForAddingImpCount;
						*impAddTriggerConcept = triggerForAddingImpConcept;
					}
				}

				return triggerCount;
			}


			bool CTriggeredImplicationGCIAbsorberPreProcess::isDisjunctionConceptSimpleTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated) {
				cint64 notAbsorbableCount = 0;
				cint64 absorbableCount = 0;
				CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptSimpleImplicationTriggerable(opConcept,!opNegated^negated)) {
						absorbableCount++;
					} else {
						notAbsorbableCount++;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				if (notAbsorbableCount <= 1) {
					return true;
				}
				return false;
			}




			bool CTriggeredImplicationGCIAbsorberPreProcess::isDisjunctionConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated) {
				cint64 notAbsorbableCount = 0;
				cint64 absorbableCount = 0;
				CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
				while (opConLinkerIt) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opNegated = opConLinkerIt->isNegated();
					if (isConceptImplicationTriggerable(opConcept,!opNegated^negated)) {
						absorbableCount++;
					} else {
						notAbsorbableCount++;
					}
					opConLinkerIt = opConLinkerIt->getNext();
				}
				if (notAbsorbableCount <= 1) {
					return true;
				}
				return false;
			}



			bool CTriggeredImplicationGCIAbsorberPreProcess::isEquivalenceConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount) {
				cint64 notAbsorbableCount = 0;
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCEQ) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (!isConceptImplicationTriggerable(opConcept,opNegated)) {
							return false;
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				return true;
			}


			bool CTriggeredImplicationGCIAbsorberPreProcess::isGCIConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount) {
				bool absorbable = false;
				cint64 notAbsorbableCount = 0;
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated();
						if (isConceptImplicationTriggerable(opConcept,!opNegated)) {
							absorbable = true;
						} else {
							notAbsorbableCount++;
						}
						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				if (notAbsorbableOperandsCount) {
					*notAbsorbableOperandsCount = notAbsorbableCount;
				}
				return absorbable;
			}


			bool CTriggeredImplicationGCIAbsorberPreProcess::isConceptSimpleImplicationTriggerable(CConcept* concept, bool negated) {
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				if (!negated && (opCode == CCSUB || opCode == CCATOM)) {
					return true;
				} else if (!negated && (opCode == CCAND || opCode == CCEQ) || negated && opCode == CCOR) {
					if (opCount >= 1) {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptSimpleImplicationTriggerable(opConcept,opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						return allOpsTriggerable;
					}
				} else {
					CConcept* conTriggerConcept = mConceptTriggerHash.value(QPair<CConcept*,bool>(concept,negated));
					if (conTriggerConcept) {
						return true;
					}
				}
				return false;
			}


			bool CTriggeredImplicationGCIAbsorberPreProcess::isConceptImplicationTriggerable(CConcept* concept, bool negated) {
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				if (!negated && (opCode == CCSUB || opCode == CCATOM)) {
					return true;
				} else if (!negated && (opCode == CCNOMINAL)) {
					return true;
				} else if (!negated && (opCode == CCVALUE)) {
					return true;
				} else  if (!negated && (opCode == CCSOME || opCode == CCSELF) || negated && (opCode == CCALL)) {
					if (!opConLinker) {
						return true;
					} else {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						return allOpsTriggerable;
					}
				} else  if (!negated && (opCode == CCATLEAST && parm == 1) || negated && (opCode == CCATMOST && parm == 0)) {
					if (!opConLinker) {
						return true;
					} else {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated();
							allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						return allOpsTriggerable;
					}
				} else  if (opCode == CCAND || opCode == CCEQ || opCode == CCOR) {
					if (opCount >= 1) {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptImplicationTriggerable(opConcept,opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						return allOpsTriggerable;
					}
				}
				return false;
			}








			bool CTriggeredImplicationGCIAbsorberPreProcess::absorbGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated) {
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					QList< QPair<CConcept*,bool> > candList;
					if (addAbsorbableDisjunctCandidates(orConcept,negated,candList)) {
						if (candList.count() >= 1) {
							QList< QPair<CConcept*,bool> > absorbList(splitAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {
								createGCIAbsorbedTriggeredImplication(orConcept,negated,absorbList,candList);
								return true;
							}
						}
					}
				}
				return false;
			}






			bool CTriggeredImplicationGCIAbsorberPreProcess::isConceptPartialImplicationTriggerable(CConcept* concept, bool negated) {
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				if (!negated && (opCode == CCSUB || opCode == CCATOM)) {
					return true;
				} else if (!negated && (opCode == CCNOMINAL)) {
					return true;
				} else if (!negated && (opCode == CCVALUE)) {
					return true;
				} else  if (!negated && (opCode == CCSOME || opCode == CCSELF) || negated && (opCode == CCALL)) {
					return true;
				} else  if (!negated && (opCode == CCATLEAST && parm >= 1) || negated && (opCode == CCATMOST && parm >= 0)) {
					return true;
				} else  if (!negated && (opCode == CCAND || opCode == CCEQ) || negated && opCode == CCOR) {
					// OR, at least one operand has to be triggerable
					if (opCount >= 1) {
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							if (isConceptPartialImplicationTriggerable(opConcept,opNegated)) {
								return true;
							}
							opConLinkerIt = opConLinkerIt->getNext();
						}
					}
				} else  if (negated && (opCode == CCAND || opCode == CCEQ) || !negated && opCode == CCOR) {
					// OR, all operands have to be triggerable
					if (opCount >= 1) {
						bool allOpsTriggerable = true;
						CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
						while (opConLinkerIt && allOpsTriggerable) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegated = opConLinkerIt->isNegated()^negated;
							allOpsTriggerable &= isConceptPartialImplicationTriggerable(opConcept,opNegated);
							opConLinkerIt = opConLinkerIt->getNext();
						}
						return allOpsTriggerable;
					}
				}
				return false;
			}




			bool CTriggeredImplicationGCIAbsorberPreProcess::absorbPartialGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated) {
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					QList< QPair<CConcept*,bool> > candList;
					if (addAbsorbableDisjunctCandidates(orConcept,negated,candList)) {
						if (candList.count() >= 1) {
							QList< QPair<CConcept*,bool> > absorbList(getPartialAbsorbableDisjuncts(candList));
							if (!absorbList.isEmpty()) {
								createGCIPartialAbsorbedTriggeredImplication(orConcept,negated,absorbList);
								return true;
							}
						}
					}
				}
				return false;
			}



			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createGCIPartialAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, const QList< QPair<CConcept*,bool> >& absorbList) {
				CConcept* implicationConcept = createImplicationConcept(orConcept,negated);
				for (QList< QPair<CConcept*,bool> >::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					QPair<CConcept*,bool> conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					createPartialImplicationAddedToTrigger(implicationConcept,nullptr,concept,!negated);
				}
				return implicationConcept;
			}



			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createPartialExtendedGCIAbsorbedTriggeredImplication(CConcept* implicationConcept, const QList< QPair<CConcept*,bool> >& absorbList) {
				cint64 maxImpAddTriggerCount = 0;
				CConcept* maxImpAddTriggerConcept = nullptr;
				for (QList< QPair<CConcept*,bool> >::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					QPair<CConcept*,bool> conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					createPartialTrigger(implicationConcept,nullptr,concept,!negated,&maxImpAddTriggerCount,&maxImpAddTriggerConcept);
				}
				return implicationConcept;
			}



			CConcept* CTriggeredImplicationGCIAbsorberPreProcess::createGCIAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList< QPair<CConcept*,bool> >& absorbList, QList< QPair<CConcept*,bool> >& candList) {
				CConcept* impliedConcept = nullptr;
				bool impliedNegation = false; 
				QList< QPair<CConcept*,bool> > partialAbsorbableList;

				if (candList.isEmpty()) {
					//candList.append(mBottomConcept);
					candList.append(absorbList.takeFirst());
				}
				if (candList.count() > 1) {
					impliedConcept = createImpliedConcept();
					for (QList< QPair<CConcept*,bool> >::const_iterator it = candList.constBegin(), itEnd = candList.constEnd(); it != itEnd; ++it) {
						QPair<CConcept*,bool> conNegPair(*it);
						CConcept* concept = conNegPair.first;
						bool negated = conNegPair.second;
						addConceptToImplied(impliedConcept,concept,negated);
					}
					impliedConcept->setOperatorCode(CCOR);
					if (mConfPartialGCIAbsorption) {
						partialAbsorbableList = getPartialAbsorbableDisjuncts(candList);
					}
				} else {
					QPair<CConcept*,bool> conNegPair(candList.takeFirst());
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					impliedNegation = negated;
					impliedConcept = concept;
				}

				CConcept* implicationConcept = createImplicationConcept(impliedConcept,impliedNegation);

				if (!partialAbsorbableList.isEmpty()) {
					createPartialExtendedGCIAbsorbedTriggeredImplication(implicationConcept,partialAbsorbableList);
					++mStatGCIAbsorptionPartialExtended;
				}

				for (QList< QPair<CConcept*,bool> >::const_iterator it = absorbList.constBegin(), itEnd = absorbList.constEnd(); it != itEnd; ++it) {
					QPair<CConcept*,bool> conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					createImplicationAddedToTrigger(implicationConcept,nullptr,concept,!negated);
				}
				return implicationConcept;
			}




			cint64 CTriggeredImplicationGCIAbsorberPreProcess::createPartialImplicationAddedToTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated) {
				cint64 triggerCount = 0;
				cint64 maxImpAddTriggerCount = 0;
				CConcept* maxImpAddTriggerConcept = nullptr;
				if (!implicationConcept) {
					implicationConcept = createImplicationConcept(triggerConcept,false);
					triggerConcept = nullptr;
				}

				triggerCount = createPartialTrigger(implicationConcept,triggerConcept,concept,negated,&maxImpAddTriggerCount,&maxImpAddTriggerConcept);

				if (maxImpAddTriggerConcept) {
					cint64 opCode = maxImpAddTriggerConcept->getOperatorCode();
					if (opCode != CCNONE) {
						addConceptToImplied(maxImpAddTriggerConcept,implicationConcept,false);
					} else {
						addSubClassConcept(maxImpAddTriggerConcept,implicationConcept,false);
					}
				}

				return triggerCount;
			}




			cint64 CTriggeredImplicationGCIAbsorberPreProcess::createPartialTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated, cint64* impAddTriggerCount, CConcept** impAddTriggerConcept) {
				cint64 triggerCount = 0;
				cint64 triggerForAddingImpCount = 0;
				CConcept* triggerForAddingImpConcept = nullptr;
				cint64 opCode = concept->getOperatorCode();
				cint64 parm = concept->getParameter();
				cint64 opCount = concept->getOperandCount();
				CRole* role = concept->getRole();
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				if (!negated && (opCode == CCIMPLALL || opCode == CCIMPL || opCode == CCIMPLTRIG)) {
					// nothing to do
				} else if (!negated && (opCode == CCATOM || opCode == CCSUB)) {
					CConcept* locatedConcept = CConceptRoleIndividualLocator::getLocatedConcept(concept,mOnto);

					//if (concept->hasClassName()) {
					//	QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
					//	if (iriClassNameString == QString("http://www.owllink.org/testsuite/galen#nonNormal")) {
					//		bool bug = true;
					//	}
					//}

					++triggerForAddingImpCount;
					++triggerCount;
					triggerForAddingImpConcept = locatedConcept;
					if (implicationConcept) {
						addImplicationTrigger(implicationConcept,concept,!negated);
					} else {
						addSubClassConcept(locatedConcept,triggerConcept,false);
					}
				} else if (!negated && (opCode == CCAND || opCode == CCEQ) || negated && (opCode == CCOR)) {
					// and trigger
					if (triggerConcept) {
						return createPartialImplicationAddedToTrigger(implicationConcept,triggerConcept,concept,negated);
					}

					CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;

						if (isConceptPartialImplicationTriggerable(opConcept,opNegated)) {

							cint64 tmpImpAddTriggerCount = 0;
							CConcept* tmpImpAddTriggerConcept = nullptr;

							triggerCount += createPartialTrigger(implicationConcept,triggerConcept,opConcept,opNegated,&tmpImpAddTriggerCount,&tmpImpAddTriggerConcept);

							if (triggerForAddingImpConcept == nullptr || tmpImpAddTriggerCount > triggerForAddingImpCount) {
								triggerForAddingImpCount = tmpImpAddTriggerCount;
								triggerForAddingImpConcept = tmpImpAddTriggerConcept;
							}
						}

						opConLinkerIt = opConLinkerIt->getNext();
					}
				} else {					
					CConcept* conTriggerConcept = mConceptTriggerHash.value(QPair<CConcept*,bool>(concept,negated));
					if (implicationConcept && conTriggerConcept) {
						addImplicationTrigger(implicationConcept,conTriggerConcept,true);
						triggerForAddingImpCount += mTriggerCountHash.value(conTriggerConcept,0);
						triggerCount += triggerForAddingImpCount;
						triggerForAddingImpConcept = conTriggerConcept;
					} else {
						if (implicationConcept) {
							triggerConcept = createImplicationTriggerConcept(nullptr,false);
							addImplicationTrigger(implicationConcept,triggerConcept,true);
							mConceptTriggerHash.insert(QPair<CConcept*,bool>(concept,negated),triggerConcept);
							implicationConcept = nullptr;
						}
						if (!negated && (opCode == CCNOMINAL)) {
							CIndividual* individual = concept->getNominalIndividual();
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual,mOnto);
							triggerCount += 2;
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(triggerConcept,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);
						} else if (!negated && (opCode == CCOR) || negated && (opCode == CCAND || opCode == CCEQ)) {
							// or trigger

							cint64 minTriggerCount = 0;

							CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegated = opConLinkerIt->isNegated()^negated;

								cint64 tmpTriggerCount = 0;

								tmpTriggerCount = createPartialTrigger(implicationConcept,triggerConcept,opConcept,opNegated,nullptr,nullptr);

								if (minTriggerCount == 0 || tmpTriggerCount > 0 && tmpTriggerCount < minTriggerCount) {
									minTriggerCount = tmpTriggerCount;
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}

							triggerCount += minTriggerCount;

						} else if (!negated && (opCode == CCSOME || opCode == CCSELF) || negated && (opCode == CCALL)) {
							bool absorbOperands = true;
							if (opConLinker) {
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated()^negated;
									if (!isConceptPartialImplicationTriggerable(opConcept,opNegated)) {
										absorbOperands = false;
									}
									opConLinkerIt = opConLinkerIt->getNext();
								}
							} else {
								absorbOperands = false;
							}
							if (absorbOperands) {
								CConcept* nextLevelTrigger = createTriggerPropagationConcept(triggerConcept,role);
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated()^negated;
									triggerCount += createPartialTrigger(implicationConcept,nextLevelTrigger,opConcept,opNegated,nullptr,nullptr);
									opConLinkerIt = opConLinkerIt->getNext();
								}
							} else {
								CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
								addRoleDomainConcept(locatedRole,triggerConcept,false);
							}
							// add one for role
							triggerCount += 1;
						} else if (!negated && (opCode == CCVALUE)) {
							CConcept* nextLevelTrigger = createTriggerPropagationConcept(triggerConcept,role);

							CIndividual* individual = concept->getNominalIndividual();
							CIndividual* locatedIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual,mOnto);
							triggerCount += 2;
							CConceptAssertionLinker* assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemMan);
							assLinker->initNegLinker(nextLevelTrigger,false);
							locatedIndividual->addAssertionConceptLinker(assLinker);

							// add one for role
							triggerCount += 1;
						} else if (!negated && (opCode == CCATLEAST && parm >= 1) || negated && (opCode == CCATMOST && parm >= 2)) {
							bool absorbOperands = true;
							if (opConLinker) {
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated();
									if (!isConceptPartialImplicationTriggerable(opConcept,opNegated)) {
										absorbOperands = false;
									}
									opConLinkerIt = opConLinkerIt->getNext();
								}
							} else {
								absorbOperands = false;
							}
							if (absorbOperands) {
								CConcept* nextLevelTrigger = createTriggerPropagationConcept(triggerConcept,role);
								CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker;
								while (opConLinkerIt) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegated = opConLinkerIt->isNegated();
									triggerCount += createPartialTrigger(implicationConcept,nextLevelTrigger,opConcept,opNegated,nullptr,nullptr);
									opConLinkerIt = opConLinkerIt->getNext();
								}
							} else {
								CRole* locatedRole = CConceptRoleIndividualLocator::getLocatedRole(role,mOnto);
								addRoleDomainConcept(locatedRole,triggerConcept,false);
							}
							triggerCount += 1;
						}
						mTriggerCountHash.insert(triggerConcept,triggerCount);
						triggerForAddingImpConcept = triggerConcept;
						triggerForAddingImpCount = triggerCount;
					}
				}

				if (impAddTriggerConcept && impAddTriggerCount) {
					if (*impAddTriggerCount < triggerForAddingImpCount) {
						*impAddTriggerCount = triggerForAddingImpCount;
						*impAddTriggerConcept = triggerForAddingImpConcept;
					}
				}

				return triggerCount;
			}



			bool CTriggeredImplicationGCIAbsorberPreProcess::addAbsorbableDisjunctCandidates(CConcept* orConcept, bool negated, QList< QPair<CConcept*,bool> >& list) {
				bool absorbableDisjuncts = false;
				cint64 opCode = orConcept->getOperatorCode();
				if (!negated && opCode == CCOR || negated && opCode == CCAND) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = orConcept->getOperandList();
					while (opConLinkerIt) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opNegated = opConLinkerIt->isNegated()^negated;

						cint64 opOpCode = opConcept->getOperatorCode();
						if (!opNegated && opOpCode == CCOR || opNegated && opOpCode == CCAND) {
							absorbableDisjuncts |= addAbsorbableDisjunctCandidates(opConcept,opNegated,list);
						} else {
							list.append(QPair<CConcept*,bool>(opConcept,opNegated));
							absorbableDisjuncts = true;
						}

						opConLinkerIt = opConLinkerIt->getNext();
					}
				}
				return absorbableDisjuncts;
			}

			QList< QPair<CConcept*,bool> > CTriggeredImplicationGCIAbsorberPreProcess::splitAbsorbableDisjuncts(QList< QPair<CConcept*,bool> >& list) {
				QList< QPair<CConcept*,bool> > absorbableList;
				cint64 count = list.count();
				for (cint64 nr = 0; nr < count; ++nr) {
					QPair<CConcept*,bool> conNegPair(list.takeFirst());
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					if (isConceptImplicationTriggerable(concept,!negated)) {
						absorbableList.append(conNegPair);
					} else {
						list.append(conNegPair);
					}
				}
				return absorbableList;
			}

			QList< QPair<CConcept*,bool> > CTriggeredImplicationGCIAbsorberPreProcess::getPartialAbsorbableDisjuncts(QList< QPair<CConcept*,bool> >& list) {
				QList< QPair<CConcept*,bool> > absorbableList;
				cint64 count = list.count();
				for (QList< QPair<CConcept*,bool> >::const_iterator it = list.constBegin(), itEnd = list.constEnd(); it != itEnd; ++it) {
					QPair<CConcept*,bool> conNegPair(*it);
					CConcept* concept = conNegPair.first;
					bool negated = conNegPair.second;
					if (isConceptPartialImplicationTriggerable(concept,!negated)) {
						absorbableList.append(conNegPair);
					}
				}
				return absorbableList;
			}

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
