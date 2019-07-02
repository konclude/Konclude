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

#include "CRoleChainAutomataTransformationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CRoleChainAutomataTransformationPreProcess::CRoleChainAutomataTransformationPreProcess() {
				mConfSaveTransitiveTransitions = true;
			}


			CRoleChainAutomataTransformationPreProcess::~CRoleChainAutomataTransformationPreProcess() {
			}


			CConcreteOntology *CRoleChainAutomataTransformationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				bool complexRoleConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isComplexRoleUsed();
				if (complexRoleConstructsUsed) {
					mOntology = ontology;
					CTBox* tbox = ontology->getDataBoxes()->getTBox();
					CABox *abox = ontology->getDataBoxes()->getABox();
					CRBox *rbox = ontology->getDataBoxes()->getRBox();
					CMBox* mBox = ontology->getDataBoxes()->getMBox();

					mStatAutomateTransitionConceptCount = 0;
					mStatAutomateStateConceptCount = 0;
					mStatAutomateTransformedConceptCount = 0;
					mStatAutomateTransitiveSavedCount = 0;
					mStatCreatedDomainPropagationCount = 0;
					mStatCreatedRangePropagationCount = 0;
					mStatPropagatedAlreadyInDomainRangeCount = 0;
					mStatPropagationAlreadyInDomainRangeCount = 0;
					mStatDomainPropagationCount = 0;
					mStatRangePropagationCount = 0;


					mRoleVec = rbox->getRoleVector();
					mConVec = tbox->getConceptVector();
					mNextConceptTag = mConVec->getItemCount();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();

					mRoleObjPropTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();
					mObjPropTermRoleHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
					mExpressionBuildContainerList = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getExpressionBuildListContainer();
					mInverseObjectPropertyHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getInverseObjectPropertyHash();
					mExpressionBuildHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getStructuralExpressionBuildHash();


					collectSubRoleChains();

					createMissingInverseChainedRoles();

					createInverseRoleChainLinkers();

					transformVALUERestrictions();

					createDomainRangePropagations();

					createRecursiveTraversalData();


					cint64 conCount = mConVec->getItemCount();
					for (qint64 i = 0; i < conCount; ++i) {
						CConcept *concept = mConVec->getData(i);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCALL || opCode == CCSOME || opCode == CCIMPLALL || opCode == CCBRANCHALL || opCode == CCVARBINDALL || opCode == CCPBINDALL || opCode == CCVARPBACKALL) {
								CRole* role = concept->getRole();
								role = mRoleVec->getData(role->getRoleTag());
								if (role->isComplexRole()) {
									concept = CConceptRoleIndividualLocator::getLocatedConcept(concept,ontology);
									// translate into concept role automat
									++mStatAutomateTransformedConceptCount;
									convertAutomatConcept(concept);
								}
							}
						}
					}

					if (mStatAutomateTransformedConceptCount > 0) {
						LOG(INFO,"::Konclude::Reasoner::Preprocess::RoleChainAutomataTransformation",logTr("Transformed %1 concepts to automate.").arg(mStatAutomateTransformedConceptCount),this);
						LOG(INFO,"::Konclude::Reasoner::Preprocess::RoleChainAutomataTransformation",logTr("Generated %1 automate state and %2 automate transaction concepts.").arg(mStatAutomateStateConceptCount).arg(mStatAutomateTransitionConceptCount),this);
					}

				}

				return ontology;
			}


			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::transformVALUERestrictions() {
				cint64 conCount = mConVec->getItemCount();
				for (qint64 i = 0; i < conCount; ++i) {
					bool localFlag = false;
					CConcept *concept = mConVec->getData(i,&localFlag);
					if (concept) {
						bool transformToExistentialNominal = false;
						cint64 opCode = concept->getOperatorCode();
						if (opCode == CCVALUE) {
							CRole* role = concept->getRole();
							role = mRoleVec->getData(role->getRoleTag());
							if (role->isComplexRole()) {
								transformToExistentialNominal = true;
							}
						}

						if (transformToExistentialNominal) {
							if (!localFlag) {
								concept = CConceptRoleIndividualLocator::getLocatedConcept(concept,mOntology);
							}
							CIndividual* nomIndividual = concept->getNominalIndividual();
							concept->setOperatorCode(CCSOME);
							concept->setNominalIndividual(nullptr);
							CConcept* nomIndividualConcept = nomIndividual->getIndividualNominalConcept();
							CSortedNegLinker<CConcept*>* opLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
							opLinker->init(nomIndividualConcept,false);
							concept->setOperandList(opLinker);
						}
					}
				}
				return this;
			}



			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::createDomainRangePropagations() {



				QSet<CConcept*> domRangePropCocneptSet;
				qint64 itemCounts = mRoleVec->getItemCount();
				for (qint64 i = 0; i < itemCounts; ++i) {
					CRole *role = mRoleVec->getData(i);
					if (role && role->isComplexRole()) {

						CRole* inverseRole = nullptr;

						for (CSortedNegLinker<CConcept*>* domainConLinker = role->getDomainConceptList(); domainConLinker; domainConLinker = domainConLinker->getNext()) {
							CConcept* domCon = domainConLinker->getData();
							bool domConNeg = domainConLinker->isNegated();

							if (domConNeg || !domRangePropCocneptSet.contains(domCon)) {

								++mStatRangePropagationCount;
								if (!inverseRole) {
									inverseRole = getInverseRole(role);
								}

								QList<CRoleSubRoleChainDataItem> subRoleItemList;
								foreach (CRoleSubRoleChainData data, mRoleSubRoleChainDataHash.values(role)) {
									subRoleItemList.append(CRoleSubRoleChainDataItem(data));
								}
								CConcept* propConcept = nullptr;

								while (!subRoleItemList.isEmpty()) {
									CRoleSubRoleChainData subRoleData = nullptr;
									bool allowPropagatedConcept = true;
									bool inversedToOrginal = false;
									CRoleSubRoleChainDataItem dataItem(subRoleItemList.takeFirst());
									subRoleData = dataItem.mChainData;
									allowPropagatedConcept = dataItem.mAllowPropagated;
									inversedToOrginal = dataItem.mNegated ^ subRoleData.mInverse;
									CRoleChain* roleChain = subRoleData.mRoleChain;

									CRole* firstSubRole = nullptr;
									CRole* lastSubRole = nullptr;

									if (inversedToOrginal) {
										firstSubRole = getInverseRole(roleChain->getInverseRoleChainLinker()->getData());
										lastSubRole = getInverseRole(roleChain->getInverseRoleChainLinker()->getLastListLink()->getData());
									} else {
										firstSubRole = roleChain->getRoleChainLinker()->getData();
										lastSubRole = roleChain->getRoleChainLinker()->getLastListLink()->getData();
									}


									if (!hasPropagationConcept(domConNeg,domCon,inverseRole,lastSubRole->getIndirectSuperRoleList(),true) && (!allowPropagatedConcept || !hasPropagatedConcept(domConNeg,domCon,firstSubRole->getIndirectSuperRoleList(),true))) {
										if (!propConcept) {
											propConcept = createTransitionConcept(inverseRole,TTNORMAL);
											propConcept->setOperatorCode(CCALL);
											appendTransitionOperandConceptLinker(propConcept,createTransitionOperandConceptLinker(domCon,domConNeg));
										}
										domRangePropCocneptSet.insert(propConcept);
										CSortedNegLinker<CConcept*>* newConLinker = createTransitionOperandConceptLinker(propConcept,false);

										lastSubRole->addRangeConceptLinker(newConLinker);
										++mStatCreatedDomainPropagationCount;

										foreach (CRoleSubRoleChainData data, mRoleSubRoleChainDataHash.values(lastSubRole)) {
											subRoleItemList.append(CRoleSubRoleChainDataItem(data,false));
										}
									}
								}
							}
						}


						for (CSortedNegLinker<CConcept*>* rangeConLinker = role->getRangeConceptList(); rangeConLinker; rangeConLinker = rangeConLinker->getNext()) {
							CConcept* rangeCon = rangeConLinker->getData();
							bool rangeConNeg = rangeConLinker->isNegated();

							if (rangeConNeg || !domRangePropCocneptSet.contains(rangeCon)) {

								++mStatDomainPropagationCount;
								QList<CRoleSubRoleChainDataItem> subRoleItemList;
								foreach (CRoleSubRoleChainData data, mRoleSubRoleChainDataHash.values(role)) {
									subRoleItemList.append(CRoleSubRoleChainDataItem(data));
								}
								CConcept* propConcept = nullptr;

								while (!subRoleItemList.isEmpty()) {
									CRoleSubRoleChainData subRoleData = nullptr;
									bool allowPropagatedConcept = true;
									bool inversedToOrginal = false;
									CRoleSubRoleChainDataItem dataItem(subRoleItemList.takeFirst());
									subRoleData = dataItem.mChainData;
									allowPropagatedConcept = dataItem.mAllowPropagated;
									inversedToOrginal = dataItem.mNegated ^ subRoleData.mInverse;
									CRoleChain* roleChain = subRoleData.mRoleChain;

									CRole* firstSubRole = nullptr;
									CRole* lastSubRole = nullptr;

									if (inversedToOrginal) {
										firstSubRole = getInverseRole(roleChain->getInverseRoleChainLinker()->getData());
										lastSubRole = getInverseRole(roleChain->getInverseRoleChainLinker()->getLastListLink()->getData());
									} else {
										firstSubRole = roleChain->getRoleChainLinker()->getData();
										lastSubRole = roleChain->getRoleChainLinker()->getLastListLink()->getData();
									}

									if (!hasPropagationConcept(rangeConNeg,rangeCon,role,firstSubRole->getIndirectSuperRoleList(),false) && (!allowPropagatedConcept || !hasPropagatedConcept(rangeConNeg,rangeCon,lastSubRole->getIndirectSuperRoleList(),false))) {
										if (!propConcept) {
											propConcept = createTransitionConcept(role,TTNORMAL);
											propConcept->setOperatorCode(CCALL);
											appendTransitionOperandConceptLinker(propConcept,createTransitionOperandConceptLinker(rangeCon,rangeConNeg));
										}
										domRangePropCocneptSet.insert(propConcept);
										CSortedNegLinker<CConcept*>* newConLinker = createTransitionOperandConceptLinker(propConcept,false);
										
										firstSubRole->addDomainConceptLinker(newConLinker);
										++mStatCreatedRangePropagationCount;

										foreach (CRoleSubRoleChainData data, mRoleSubRoleChainDataHash.values(firstSubRole)) {
											subRoleItemList.append(CRoleSubRoleChainDataItem(data,false));
										}
									}
								}
							}
						}

					}
				}
				return this;
			}

			bool CRoleChainAutomataTransformationPreProcess::hasPropagatedConcept(bool negated, CConcept* concept, CSortedNegLinker<CRole*>* roleList, bool inverseDomRange) {
				for (CSortedNegLinker<CRole*>* roleListLinker = roleList; roleListLinker; roleListLinker = roleListLinker->getNext()) {
					CRole* superRole = roleListLinker->getData();
					bool superRoleNeg = roleListLinker->isNegated();
					bool switchDomainRange = inverseDomRange ^ superRoleNeg;

					CSortedNegLinker<CConcept*>* conList = superRole->getDomainRangeConceptList(!switchDomainRange);
					for (CSortedNegLinker<CConcept*>* conListLinker = conList; conListLinker; conListLinker = conListLinker->getNext()) {
						CConcept* con = conListLinker->getData();
						bool conNeg = conListLinker->isNegated();
						cint64 conCode = con->getOperatorCode();
						if (con == concept && conNeg == negated) {
							++mStatPropagatedAlreadyInDomainRangeCount;
							return true;
						}
					}
				}
				return false;
			}

			bool CRoleChainAutomataTransformationPreProcess::hasPropagationConcept(bool negated, CConcept* concept, CRole* role, CSortedNegLinker<CRole*>* roleList, bool inverseDomRange) {
				for (CSortedNegLinker<CRole*>* roleListLinker = roleList; roleListLinker; roleListLinker = roleListLinker->getNext()) {
					CRole* superRole = roleListLinker->getData();
					bool superRoleNeg = roleListLinker->isNegated();
					bool switchDomainRange = inverseDomRange ^ superRoleNeg;

					CSortedNegLinker<CConcept*>* conList = superRole->getDomainRangeConceptList(switchDomainRange);
					for (CSortedNegLinker<CConcept*>* conListLinker = conList; conListLinker; conListLinker = conListLinker->getNext()) {
						CConcept* con = conListLinker->getData();
						bool conNeg = conListLinker->isNegated();
						if (con->getRole() == role) {
							cint64 conCode = con->getOperatorCode();
							if ((conCode == CCALL || conCode == CCAQALL || conCode == CCIMPLALL) && !conNeg || conNeg && (conCode == CCSOME)) {
								for (CSortedNegLinker<CConcept*>* opListLinker = con->getOperandList(); opListLinker; opListLinker = opListLinker->getNext()) {
									if (opListLinker->getData() == concept && opListLinker->isNegated()^conNeg == negated) {
										++mStatPropagationAlreadyInDomainRangeCount;
										return true;
									}
								}
							}
						}
					}
				}
				return false;
			}


			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::convertAutomatConcept(CConcept* concept) {
				CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				cint64 opCount = concept->getOperandCount();
				CRole* role = concept->getRole();
				role = mRoleVec->getData(role->getRoleTag());
				bool existNegation = false;
				if (concept->getOperatorCode() == CCSOME) {
					existNegation = true;
				}

				//if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "#LocativeAttribute") {
				//	if (CIRIName::getRecentIRIName(concept->getOperandList()->getData()->getClassNameLinker()) == "#Heart") {
				//		bool bug = true;
				//	}
				//}

				// transformation into automat concept

				bool implTransisition = false;
				TRANSLATIONTYPE transType = TTNORMAL;
				cint64 opCode = concept->getOperatorCode();
				if (opCode == CCIMPLALL) {
					// implication transition is never negated
					transType = TTIMPL;
				} else if (opCode == CCBRANCHALL) {
					transType = TTBRANCH;
				} else if (opCode == CCPBINDALL) {
					transType = TTPROPBIND;
				} else if (opCode == CCVARPBACKALL) {
					transType = TTBACKPROP;
				} else if (opCode == CCVARBINDALL) {
					transType = TTVARBIND;
				}

				CConcept* generatingConcept = nullptr;
				CConcept* beginState = nullptr;
				CConcept* propCon = nullptr;
				CConcept* endState = nullptr;

				if (transType == TTNORMAL) {
					concept->setOperatorCode(CCAQCHOOCE);
					generatingConcept = createAutomatGeneratingConcept(opConLinker,!existNegation,role);
					beginState = createStateConcept(transType);
				} else {
					beginState = concept;
					cint64 opCode = CCAQAND;
					if (transType == TTIMPL) {
						opCode = CCIMPLAQAND;
					} else if (transType == TTBRANCH) {
						opCode = CCBRANCHAQAND;
					} else if (transType == TTPROPBIND) {
						opCode = CCPBINDAQAND;
					} else if (transType == TTBACKPROP) {
						opCode = CCVARPBACKAQAND;
					} else if (transType == TTVARBIND) {
						opCode = CCVARBINDAQAND;
					} 
					beginState->setOperatorCode(opCode);
					beginState->setRole(nullptr);
				}
				concept->setOperandList(nullptr);
				concept->setOperandCount(0);

				propCon = createTransitionConcept(role,transType);
				endState = createStateConcept(transType);



				CSortedNegLinker<CConcept*>* opLinkerIt = opConLinker;
				while (opLinkerIt) {
					CConcept* opConcept = opLinkerIt->getData();
					bool opNegation = opLinkerIt->isNegated() ^ existNegation;
					appendTransitionOperandConceptLinker(endState,createTransitionOperandConceptLinker(opConcept,opNegation));
					opLinkerIt = opLinkerIt->getNext();
				}

				if (transType == TTNORMAL) {
					appendTransitionOperandConceptLinker(concept,createTransitionOperandConceptLinker(generatingConcept,!existNegation));
					appendTransitionOperandConceptLinker(concept,createTransitionOperandConceptLinker(beginState,existNegation));
				}


				appendTransitionOperandConceptLinker(beginState,createTransitionOperandConceptLinker(propCon,false));
				appendTransitionOperandConceptLinker(propCon,createTransitionOperandConceptLinker(endState,false));


				//if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "#isInLawOf") {
				//	bool bug = true;
				//}


				//const QList<CRoleSubRoleChainData> subRoleChainDataList(mRoleSubRoleChainDataHash.values(role));
				const CRecTravSubRoleChainDataItem& recTravSubRoleChainDataItem(mRoleRecTravSubRoleChainDataHash.value(role));
				QSet<CRole*> locUnfoldRoleSet;
				generateRoleChainAutomatConcept(role,recTravSubRoleChainDataItem,locUnfoldRoleSet,beginState,endState,transType);

				// search fitting sub role chains
				//bool hasFittingSubRole = false;
				//QList<CRoleSubRoleChainData> subRoleChainDataList(mRoleSubRoleChainDataHash.values(role));
				//hasFittingSubRole = !subRoleChainDataList.isEmpty();
				//if (hasFittingSubRole) {

				//	bool transitiveRole = true;
				//	for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
				//		CRoleSubRoleChainData subRoleChainData(*it);
				//		CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
				//		bool subRoleNegation = subRoleChainData.mInverse;
				//		CRole* superRole = subRoleChainData.mRole;

				//		CXLinker<CRole*>* roleChainLinkerIt = subRoleChain->getRoleChainLinker();
				//		while (roleChainLinkerIt) {
				//			CRole* chainRole = roleChainLinkerIt->getData();
				//			if (chainRole != superRole) {
				//				transitiveRole = false;
				//				break;
				//			}
				//			roleChainLinkerIt = roleChainLinkerIt->getNext();
				//		}
				//	}


				//	if (transitiveRole && subRoleChainDataList.count() > 1) {

				//		QHash<CRole*,cint64> roleCountHash;
				//		QHash<CRole*,CRoleSubRoleChainData> roleChainDataHash;
				//		for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
				//			CRoleSubRoleChainData subRoleChainData(*it);
				//			CRole* superRole = subRoleChainData.mRole;
				//			roleCountHash.insert(superRole,0);
				//			roleChainDataHash.insert(superRole,subRoleChainData);
				//		}
				//		for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
				//			CRoleSubRoleChainData subRoleChainData(*it);
				//			CRole* superRole = subRoleChainData.mRole;
				//			CSortedNegLinker<CRole*>* superSuperLinkerIt = superRole->getIndirectSuperRoleList();
				//			while (superSuperLinkerIt) {
				//				CRole* superSuperRole = superSuperLinkerIt->getData();
				//				cint64 count = roleCountHash.value(superSuperRole,-1);
				//				if (count >= 0) {
				//					roleCountHash.insert(superSuperRole,count+1);
				//				}
				//				superSuperLinkerIt = superSuperLinkerIt->getNext();
				//			}
				//		}
				//		QMap<cint64,CRole*> countRoleMap;
				//		for (QHash<CRole*,cint64>::const_iterator it = roleCountHash.constBegin(), itEnd = roleCountHash.constEnd(); it != itEnd; ++it) {
				//			CRole* role = it.key();
				//			cint64 count = it.value();
				//			countRoleMap.insertMulti(-count,role);
				//		}

				//		QSet<CRole*> locUnfoldRoleSet;
				//		for (QMap<cint64,CRole*>::const_iterator it = countRoleMap.constBegin(), itEnd = countRoleMap.constEnd(); it != itEnd; ++it) {
				//			CRole* superRole = it.value();
				//			CRoleSubRoleChainData subRoleChainData(roleChainDataHash.value(superRole));
				//			CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
				//			bool subRoleNegation = subRoleChainData.mInverse;
				//			bool requiresAutomatGeneration = true;

				//			CSortedNegLinker<CRole*>* superSuperLinkerIt = superRole->getIndirectSuperRoleList();
				//			while (superSuperLinkerIt) {
				//				CRole* superSuperRole = superSuperLinkerIt->getData();
				//				if (locUnfoldRoleSet.contains(superSuperRole)) {
				//					requiresAutomatGeneration = false;
				//					break;
				//				}
				//				superSuperLinkerIt = superSuperLinkerIt->getNext();
				//			}

				//			if (requiresAutomatGeneration) {
				//				locUnfoldRoleSet.insert(superRole);
				//				generateRoleChainAutomatConcept(role,superRole,subRoleChain,subRoleNegation,locUnfoldRoleSet,beginState,endState,transType);
				//			} else {
				//				++mStatAutomateTransitiveSavedCount;
				//			}
				//		}



				//	} else {					 
				//		for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
				//			CRoleSubRoleChainData subRoleChainData(*it);
				//			CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
				//			bool subRoleNegation = subRoleChainData.mInverse;
				//			CRole* superRole = subRoleChainData.mRole;
				//			CSortedNegLinker<CRole*>* superSuperLinkerIt = superRole->getIndirectSuperRoleList();
				//			QSet<CRole*> locUnfoldRoleSet;
				//			locUnfoldRoleSet.insert(role);
				//			while (superSuperLinkerIt) {
				//				CRole* superSuperRole = superSuperLinkerIt->getData();
				//				locUnfoldRoleSet.insert(superSuperRole);
				//				superSuperLinkerIt = superSuperLinkerIt->getNext();
				//			}
				//			generateRoleChainAutomatConcept(role,superRole,subRoleChain,subRoleNegation,locUnfoldRoleSet,beginState,endState,transType);
				//		}
				//	}
				//}



				return this;
			}



			CConcept* CRoleChainAutomataTransformationPreProcess::createAutomatGeneratingConcept(CSortedNegLinker<CConcept*>* opLinker, bool negate, CRole* role) {
				CConcept* concept = nullptr;
				concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 conTag = mNextConceptTag++;
				concept->setConceptTag(conTag);
				mConVec->setData(conTag,concept);
				concept->setOperatorCode(CCAQSOME);
				concept->setRole(role);
				CSortedNegLinker<CConcept*>* opLinkerIt = opLinker;
				while (opLinkerIt) {
					CConcept* opConcept = opLinkerIt->getData();
					bool opNegation = opLinkerIt->isNegated() ^ negate;
					appendTransitionOperandConceptLinker(concept,createTransitionOperandConceptLinker(opConcept,opNegation));
					opLinkerIt = opLinkerIt->getNext();
				}
				return concept;
			}



			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::createInverseRoleChainLinkers() {
				FOREACHIT (CRoleChain* roleChain, mInverseUpdateRoleChainSet) {
					if (!roleChain->getInverseRoleChainLinker()) {
						CXLinker<CRole*>* roleLinkerIt = roleChain->getRoleChainLinker();
						while (roleLinkerIt) {
							CRole* chainedRole = roleLinkerIt->getData();
							CXLinker<CRole*>* invRoleLinker = CObjectAllocator< CXLinker<CRole*> >::allocateAndConstruct(mMemMan);
							invRoleLinker->initLinker(chainedRole);
							roleChain->prependInverseRoleChainLinker(invRoleLinker);
							roleLinkerIt = roleLinkerIt->getNext();
						}
					}
				}
				return this;
			}


			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::createMissingInverseChainedRoles() {

				// collect direct and indirect sub roles
				QHash<qint64, QPair<cint64,bool> > roleTagIndirectSubRoleTagNegationHash;
				QHash<qint64, QPair<cint64,bool> > roleTagSubRoleTagNegationHash;
				qint64 itemCounts = mRoleVec->getItemCount();
				for (qint64 i = 0; i < itemCounts; ++i) {
					CRole *role = mRoleVec->getData(i);
					if (role) {
						cint64 roleTag = role->getRoleTag();
						CSortedNegLinker<CRole*>* inSuperRoleLinkerIt = role->getIndirectSuperRoleList();
						while (inSuperRoleLinkerIt) {
							CRole* superRole = inSuperRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = inSuperRoleLinkerIt->isNegated();
							roleTagIndirectSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(roleTag,superRoleNeg));
							inSuperRoleLinkerIt = inSuperRoleLinkerIt->getNext();
						}
						CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
						while (superRoleLinkerIt) {
							CRole* superRole = superRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = superRoleLinkerIt->isNegated();
							roleTagSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(roleTag,superRoleNeg));
							superRoleLinkerIt = superRoleLinkerIt->getNext();
						}
					}
				}

				// build missing inverse roles
				QSet<CRole*> needsInverseRoleSet;
				for (qint64 i = 0; i < itemCounts; ++i) {
					CRole *role = mRoleVec->getLocalData(i);
					if (role) {
						if (role->isComplexRole()) {

							if (role->getDomainConceptList()) {
								needsInverseRoleSet.insert(role);
							}

							if (hasInverseRole(role) || role->getDomainConceptList() || role->getRangeConceptList()) {
								// check whether it is necessary to build additional inverse roles
								QList<CRole*> subRoleInvSearchList;
								QSet<CRole*> subRoleInvSearchSet;
								subRoleInvSearchList.append(role);
								subRoleInvSearchSet.insert(role);
								while (!subRoleInvSearchList.isEmpty()) {
									CRole* subRoleInvSearchRole = subRoleInvSearchList.takeFirst();
									QList<CRoleSubRoleChainData> subRoleChainDataList(mRoleSubRoleChainDataHash.values(subRoleInvSearchRole));
									for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
										CRoleSubRoleChainData subRoleChainData(*it);
										CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
										bool subRoleNegation = subRoleChainData.mInverse;
										CRole* superRole = subRoleChainData.mRole;
										needsInverseRoleSet.insert(superRole);

										CXLinker<CRole*>* chainedSubRoleLinkerIt = subRoleChain->getRoleChainLinker();
										while (chainedSubRoleLinkerIt) {
											CRole* subRole = chainedSubRoleLinkerIt->getData();
											needsInverseRoleSet.insert(subRole);
											if (!subRoleInvSearchSet.contains(subRole)) {
												subRoleInvSearchSet.insert(subRole);
												subRoleInvSearchList.append(subRole);
											}
											chainedSubRoleLinkerIt = chainedSubRoleLinkerIt->getNext();
										}
									}
								}
							}
						}
					}
				}

				bool updatedInverseRoles = false;
				FOREACHIT (CRole* role, needsInverseRoleSet) {
					if (!hasInverseRole(role)) {
						// build additional inverse role
						role = getLocalizedRole(role);

						updatedInverseRoles = true;
						CRole* inverseRole = CObjectAllocator< CRole >::allocateAndConstruct(mMemMan);
						inverseRole->initRole();
						cint64 inverseRoleTag = mRoleVec->getItemCount();
						inverseRole->setRoleTag(inverseRoleTag);
						inverseRole->setRoleComplexity(role->getRoleComplexity());
						mRoleVec->setData(inverseRole->getRoleTag(),inverseRole);

						CSortedNegLinker<CRole*>* invRoleLinker1 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
						invRoleLinker1->init(role,true);
						inverseRole->addSuperRoleLinker(invRoleLinker1);
						CSortedNegLinker<CRole*>* invRoleLinker2 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
						invRoleLinker2->init(inverseRole,true);
						role->addSuperRoleLinker(invRoleLinker2);




						CObjectPropertyTermExpression* buildExpression = mRoleObjPropTermHash->value(role);
						CInverseObjectPropertyOfExpression* expression = nullptr;
						expression = new CInverseObjectPropertyOfExpression(buildExpression);
						mExpressionBuildHash->insert(CExpressionHasher(expression),expression);
						mInverseObjectPropertyHash->insert(buildExpression,expression);
						mInverseObjectPropertyHash->insert(expression,buildExpression);
						mExpressionBuildContainerList->append(expression);
						mRoleObjPropTermHash->insert(role,expression);
						mObjPropTermRoleHash->insert(expression,role);


						QList< QPair<cint64,bool> > subRoleTagList(roleTagSubRoleTagNegationHash.values(role->getRoleTag()));
						for (QList< QPair<cint64,bool> >::const_iterator it = subRoleTagList.constBegin(), itEnd = subRoleTagList.constEnd(); it != itEnd; ++it) {
							cint64 subRoleTag = it->first;
							CRole* subRole = getLocalizedRole(subRoleTag);
							// add new inverse super role
							CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
							superRoleLinker->init(inverseRole,!it->second);
							subRole->addSuperRoleLinker(superRoleLinker);
						}

						QList< QPair<cint64,bool> > indSubRoleTagList(roleTagIndirectSubRoleTagNegationHash.values(role->getRoleTag()));
						for (QList< QPair<cint64,bool> >::const_iterator it = indSubRoleTagList.constBegin(), itEnd = indSubRoleTagList.constEnd(); it != itEnd; ++it) {
							cint64 subRoleTag = it->first;
							CRole* subRole = getLocalizedRole(subRoleTag);
							// add new inverse super role
							CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
							superRoleLinker->init(inverseRole,!it->second);
							subRole->addIndirectSuperRoleLinker(superRoleLinker);
						}

						// copy indirect super role linker to new inverse role
						CSortedNegLinker<CRole*>* inSuperRoleLinkerIt = role->getIndirectSuperRoleList();
						while (inSuperRoleLinkerIt) {
							CRole* superRole = inSuperRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = inSuperRoleLinkerIt->isNegated();

							CSortedNegLinker<CRole*>* invSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
							invSuperRoleLinker->init(superRole,!superRoleNeg);
							inverseRole->addIndirectSuperRoleLinker(invSuperRoleLinker);

							roleTagIndirectSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(inverseRoleTag,!superRoleNeg));
							inSuperRoleLinkerIt = inSuperRoleLinkerIt->getNext();
						}
						// copy super role linker to new inverse role
						CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
						while (superRoleLinkerIt) {
							CRole* superRole = superRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = superRoleLinkerIt->isNegated();

							CSortedNegLinker<CRole*>* invSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemMan);
							invSuperRoleLinker->init(superRole,!superRoleNeg);
							inverseRole->addSuperRoleLinker(invSuperRoleLinker);

							roleTagSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(inverseRoleTag,!superRoleNeg));
							superRoleLinkerIt = superRoleLinkerIt->getNext();
						}
					}
				}

				if (updatedInverseRoles) {
					mRoleSubRoleChainDataHash.clear();
					collectSubRoleChains();
				}
				return this;
			}




			bool CRoleChainAutomataTransformationPreProcess::hasInverseRole(CRole* role, bool searchInverseEquivalent) {
				return getInverseRole(role,searchInverseEquivalent) != nullptr;
			}


			CRole* CRoleChainAutomataTransformationPreProcess::getInverseRole(CRole* role, bool searchInverseEquivalent) {
				CRole* inverseRole = role->getInverseRole();
				if (inverseRole) {
					return inverseRole;
				}
				CSortedNegLinker< CRole* >* inEqRoleLinkerIt = role->getInverseEquivalentRoleList();
				while (inEqRoleLinkerIt) {
					CRole* invEqRole = inEqRoleLinkerIt->getData();
					bool invEqRoleNeg = inEqRoleLinkerIt->isNegated();
					if (invEqRoleNeg) {
						return invEqRole;
					}
					inEqRoleLinkerIt = inEqRoleLinkerIt->getNext();
				}
				CSortedNegLinker< CRole* >* suberRoleLinkerIt = role->getSuperRoleList();
				while (suberRoleLinkerIt) {
					CRole* superRole = suberRoleLinkerIt->getData();
					bool superRoleNeg = suberRoleLinkerIt->isNegated();
					if (superRoleNeg) {	
						if (!searchInverseEquivalent) {
							return superRole;
						} else {
							CSortedNegLinker< CRole* >* invSuberRoleLinkerIt = superRole->getSuperRoleList();
							while (invSuberRoleLinkerIt) {
								if (invSuberRoleLinkerIt->getData()->getRoleTag() == role->getRoleTag()) {
									return superRole;
								}
								invSuberRoleLinkerIt = invSuberRoleLinkerIt->getNext();
							}
						}
					}
					suberRoleLinkerIt = suberRoleLinkerIt->getNext();
				}
				return nullptr;
			}



			bool CRoleChainAutomataTransformationPreProcess::hasInversedOrNonInversedSuperRole(CRole* role, CRole* testingSuperRole) {
				return hasSuperRole(role,testingSuperRole,false,false);
			}

			bool CRoleChainAutomataTransformationPreProcess::hasInversedSuperRole(CRole* role, CRole* testingSuperRole) {
				return hasSuperRole(role,testingSuperRole,true,true);
			}

			bool CRoleChainAutomataTransformationPreProcess::hasNonInversedSuperRole(CRole* role, CRole* testingSuperRole) {
				return hasSuperRole(role,testingSuperRole,true,false);
			}

			bool CRoleChainAutomataTransformationPreProcess::hasSuperRole(CRole* role, CRole* testingSuperRole, bool superRoleInversedRequired) {
				return hasSuperRole(role,testingSuperRole,true,superRoleInversedRequired);
			}

			bool CRoleChainAutomataTransformationPreProcess::hasSuperRole(CRole* role, CRole* testingSuperRole, bool testInversed, bool superRoleInversedRequired) {
				for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
					CRole* superRole = superRoleLinkerIt->getData();
					if (superRole == testingSuperRole) {
						if (testInversed) {
							if (superRoleLinkerIt->isNegated() == superRoleInversedRequired) {
								return true;
							}
						} else {
							return true;
						}
					}
				}
				return false;
			}


			bool CRoleChainAutomataTransformationPreProcess::isChainLinkerImplicit(CXLinker<CRole*>* testingImplicitChainLinker, CXLinker<CRole*>* chainLinker, bool inversedTesting) {
				CXLinker<CRole*>* testingChainLinkerIt = testingImplicitChainLinker;
				CXLinker<CRole*>* chainLinkerIt = chainLinker;
				for (; testingChainLinkerIt && chainLinkerIt; testingChainLinkerIt = testingChainLinkerIt->getNext(), chainLinkerIt = chainLinkerIt->getNext()) {

						CRole* testingChainRole = testingChainLinkerIt->getData();
						CRole* chainRole = chainLinkerIt->getData();

						if (!hasSuperRole(testingChainRole,chainRole,inversedTesting)) {
							return false;
						}
				}
				if (testingChainLinkerIt || chainLinkerIt) {
					return false;
				}
				return true;
			}


			bool CRoleChainAutomataTransformationPreProcess::isTransitiveChainData(const CRoleSubRoleChainData& chainData) {
				cint64 transChainedRoleCount = 0;
				CRole* chainSuperRole = chainData.mRole;
				for (CXLinker<CRole*>* chainLinkerIt = chainData.mRoleChain->getRoleChainLinker(); chainLinkerIt ; chainLinkerIt = chainLinkerIt->getNext()) {
					CRole* chainedRole = chainLinkerIt->getData();
					if (chainedRole != chainSuperRole) {
						return false;
					}
					++transChainedRoleCount;
				}
				if (transChainedRoleCount == 2) {
					return true;
				}
				return false;
			}


			bool CRoleChainAutomataTransformationPreProcess::isChainDataImplicit(const CRoleSubRoleChainData& testingImplicitChainData, const CRoleSubRoleChainData& chainData) {
				// the testingImplicitChainData is implicit if every used role is a subrole of the corresponding role in chainData
				CRole* testingChainSuperRole = testingImplicitChainData.mRole;
				CRole* chainSuperRole = chainData.mRole;
				bool inversedTesting = chainData.mInverse ^ testingImplicitChainData.mInverse;
				if (hasInversedOrNonInversedSuperRole(testingChainSuperRole,chainSuperRole)) {

					if (!inversedTesting && hasNonInversedSuperRole(testingChainSuperRole,chainSuperRole)) {
						CXLinker<CRole*>* testingChainLinker = testingImplicitChainData.mRoleChain->getRoleChainLinker();
						CXLinker<CRole*>* chainLinker = chainData.mRoleChain->getRoleChainLinker();
						if (isChainLinkerImplicit(testingChainLinker,chainLinker,false)) {
							return true;
						}
					}

					if (inversedTesting && hasInversedSuperRole(testingChainSuperRole,chainSuperRole)) {
						CXLinker<CRole*>* testingChainLinker = testingImplicitChainData.mRoleChain->getRoleChainLinker();
						CXLinker<CRole*>* chainLinker = chainData.mRoleChain->getInverseRoleChainLinker();
						if (isChainLinkerImplicit(testingChainLinker,chainLinker,true)) {
							return true;
						}
					}
				}
				return false;
			}

			bool CRoleChainAutomataTransformationPreProcess::isChainDataRecursiveTraversalCritical(CRole* role, const CRoleSubRoleChainData& chainData) {
				// test first and last chain linker
				CRole* chainSuperRole = chainData.mRole;
				if (chainSuperRole != role) {
					if (!hasInversedOrNonInversedSuperRole(role,chainSuperRole)) {
						CXLinker<CRole*>* chainLinkerIt = chainData.mRoleChain->getRoleChainLinker();
						if (chainLinkerIt->getData() == chainSuperRole) {
							return true;
						}
						if (chainLinkerIt->getLastListLink()->getData() == chainSuperRole) {
							return true;
						}
					}
				}
				return false;
			}





			QList<CRoleSubRoleChainData> CRoleChainAutomataTransformationPreProcess::getRelevantChainDataList(CRole* role, const QList<CRoleSubRoleChainData>& roleSubChainDataList) {
				QList<CRoleSubRoleChainData> relevantChainDataList;
				for (QList<CRoleSubRoleChainData>::const_iterator it = roleSubChainDataList.constBegin(), itEnd = roleSubChainDataList.constEnd(); it != itEnd; ++it) {
					const CRoleSubRoleChainData& roleChainData(*it);
					bool hasImplicitRoleChainData = false;
					QList<CRoleSubRoleChainData>::const_iterator it2 = it;
					for (++it2; it2 != itEnd && !hasImplicitRoleChainData; ++it2) {
						const CRoleSubRoleChainData& roleChainData2(*it2);
						if (isChainDataImplicit(roleChainData,roleChainData2)) {
							hasImplicitRoleChainData = true;
						}
					}
					for (QList<CRoleSubRoleChainData>::const_iterator it2 = relevantChainDataList.constBegin(), it2End = relevantChainDataList.constEnd(); it2 != it2End && !hasImplicitRoleChainData; ++it2) {
						const CRoleSubRoleChainData& roleChainData2(*it2);
						if (isChainDataImplicit(roleChainData,roleChainData2)) {
							hasImplicitRoleChainData = true;
						}
					}
					if (!hasImplicitRoleChainData) {
						relevantChainDataList.append(roleChainData);
					}
				}
				return relevantChainDataList;
			}


			QHash<CRole*,bool> CRoleChainAutomataTransformationPreProcess::getRelevantRecursiveTraversalCriticalRoles(QHash<CRole*,bool>* criticalRoleNegationHash) {
				// return the most general roles
				QHash<CRole*,bool> criticalGeneralRoleNegationHash;
				for (QHash<CRole*,bool>::const_iterator it = criticalRoleNegationHash->constBegin(), itEnd = criticalRoleNegationHash->constEnd(); it != itEnd; ++it) {
					CRole* role = it.key();
					bool roleInversed = it.value();
					bool hasCriticalSuperRole = false;
					QHash<CRole*,bool>::const_iterator it2 = it;
					for (++it2; it2 != itEnd && !hasCriticalSuperRole; ++it2) {
						CRole* role2 = it2.key();
						bool role2Inversed = it2.value();
						if (role != role2) {
							if (hasInversedOrNonInversedSuperRole(role,role2)) {
								hasCriticalSuperRole = true;
							}
						}
					}
					for (QHash<CRole*,bool>::const_iterator it2 = criticalGeneralRoleNegationHash.constBegin(), it2End = criticalGeneralRoleNegationHash.constEnd(); it2 != it2End && !hasCriticalSuperRole; ++it2) {
						CRole* role2 = it2.key();
						bool role2Inversed = it2.value();
						if (role != role2) {
							if (hasInversedOrNonInversedSuperRole(role,role2)) {
								hasCriticalSuperRole = true;
							}
						}
					}
					if (!hasCriticalSuperRole) {
						criticalGeneralRoleNegationHash.insert(role,roleInversed);
					}
				}
				return criticalGeneralRoleNegationHash;
			}


			bool CRoleChainAutomataTransformationPreProcess::collectRecursiveTraversalCriticalRoles(CRole* role, const CRoleSubRoleChainData& chainData, QHash<CRole*,bool>* criticalRoleNegationHash) {
				if (isChainDataRecursiveTraversalCritical(role,chainData)) {
					CRole* chainSuperRole = chainData.mRole;
					bool negatedSuperRole = chainData.mInverse;
					criticalRoleNegationHash->insert(chainSuperRole,negatedSuperRole);
					return true;
				}
				return false;
			}



			bool CRoleChainAutomataTransformationPreProcess::requiresRecursiveTraversalForRole(CRole* role, const CRoleSubRoleChainData& chainData, QHash<CRole*,bool>* criticalRoleNegationHash) {
				CRole* chainSuperRole = chainData.mRole;
				for (CSortedNegLinker<CRole*>* superRoleIt = chainSuperRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
					CRole* superRole = superRoleIt->getData();
					bool superRoleNeg = superRoleIt->isNegated() ^ chainData.mInverse;
					if (criticalRoleNegationHash->contains(superRole)) {
						return true;
					}
				}
				return false;
			}




			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::collectSubRoleChains() {
				qint64 itemCounts = mRoleVec->getItemCount();
				for (qint64 i = 0; i < itemCounts; ++i) {
					CRole *role = mRoleVec->getData(i);
					if (role) {
						if (role->isComplexRole()) {
							CXLinker<CRoleChain*>* roleChainLinkerIt = role->getRoleChainSuperSharingLinker();
							while (roleChainLinkerIt) {
								CRoleChain* roleChain = roleChainLinkerIt->getData();
								if (!roleChain->getInverseRoleChainLinker()) {
									mInverseUpdateRoleChainSet.insert(roleChain);
								}
								CSortedNegLinker< CRole* >* suberRoleLinkerIt = role->getIndirectSuperRoleList();
								while (suberRoleLinkerIt) {
									CRole* superRole = suberRoleLinkerIt->getData();
									bool superRoleNeg = suberRoleLinkerIt->isNegated();
									superRole = mRoleVec->getData(superRole->getRoleTag());
									mRoleSubRoleChainDataHash.insertMulti(superRole,CRoleSubRoleChainData(role,roleChain,superRoleNeg));
									suberRoleLinkerIt = suberRoleLinkerIt->getNext();
								}
								roleChainLinkerIt = roleChainLinkerIt->getNext();
							}
						}
					}
				}
				return this;
			}





			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::createRecursiveTraversalData() {
				CRole* currentRole = nullptr;
				QList<CRoleSubRoleChainData> roleSubChainDataList;
				for (QHash<CRole*,CRoleSubRoleChainData>::const_iterator it = mRoleSubRoleChainDataHash.constBegin(), itEnd = mRoleSubRoleChainDataHash.constEnd(); it != itEnd; ++it) {
					CRole* role(it.key());
					const CRoleSubRoleChainData& roleSubChainData(it.value());
					if (currentRole != role) {
						if (currentRole) {
							addRecursiveTraversalData(currentRole,roleSubChainDataList);
							currentRole = nullptr;
							roleSubChainDataList.clear();
						}
						currentRole = role;
					}
					roleSubChainDataList.append(roleSubChainData);
				}
				if (currentRole) {
					addRecursiveTraversalData(currentRole,roleSubChainDataList);
				}
				return this;
			}



			CRoleChainAutomataTransformationPreProcess* CRoleChainAutomataTransformationPreProcess::addRecursiveTraversalData(CRole* role, const QList<CRoleSubRoleChainData>& roleSubChainDataList) {
				//if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "#isInLawOf") {
				//	bool debug = true;
				//}
				QList<CRoleSubRoleChainData> relevantChainDataList(getRelevantChainDataList(role,roleSubChainDataList));
				CRecTravSubRoleChainDataItem& recTravSubRoleChainData = mRoleRecTravSubRoleChainDataHash[role];
				recTravSubRoleChainData.mRole = role;
				QHash<CRole*,bool> criticalRoleNegationHash;
				bool criticalRecursiveTraversalRoles = false;
				for (QList<CRoleSubRoleChainData>::const_iterator it = relevantChainDataList.constBegin(), itEnd = relevantChainDataList.constEnd(); it != itEnd; ++it) {
					const CRoleSubRoleChainData& chainData(*it);
					criticalRecursiveTraversalRoles |= collectRecursiveTraversalCriticalRoles(role,chainData,&criticalRoleNegationHash);
				}
				if (!criticalRecursiveTraversalRoles) {
					recTravSubRoleChainData.mDirectSubRoleChainDataList = relevantChainDataList;
				} else {
					QHash<CRole*,bool> relevantCriticalRoleNegationHash(getRelevantRecursiveTraversalCriticalRoles(&criticalRoleNegationHash));
					for (QList<CRoleSubRoleChainData>::const_iterator it = relevantChainDataList.constBegin(), itEnd = relevantChainDataList.constEnd(); it != itEnd; ++it) {
						const CRoleSubRoleChainData& chainData(*it);
						if (!requiresRecursiveTraversalForRole(role,chainData,&relevantCriticalRoleNegationHash)) {
							recTravSubRoleChainData.mDirectSubRoleChainDataList.append(chainData);
						}
					}
					for (QHash<CRole*,bool>::const_iterator it = relevantCriticalRoleNegationHash.constBegin(), itEnd = relevantCriticalRoleNegationHash.constEnd(); it != itEnd; ++it) {
						CRole* role(it.key());
						bool inversed(it.value());
						recTravSubRoleChainData.mRecTraversalSubRoleList.append(TRoleNegationPair(role,inversed));
					}
				}
				return this;
			}


			CConcept* CRoleChainAutomataTransformationPreProcess::createTransitionConcept(CRole* role, TRANSLATIONTYPE transType) {
				CConcept* concept = nullptr;
				concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 conTag = mNextConceptTag++;
				concept->setConceptTag(conTag);
				mConVec->setData(conTag,concept);
				concept->setRole(role);
				cint64 opCode = CCAQALL;
				if (transType == TTNORMAL) {
					opCode = CCAQALL;
				} else if (transType == TTIMPL) {
					opCode = CCIMPLAQALL;
				} else if (transType == TTBRANCH) {
					opCode = CCBRANCHAQALL;
				} else if (transType == TTPROPBIND) {
					opCode = CCPBINDAQALL;
				} else if (transType == TTBACKPROP) {
					opCode = CCVARPBACKAQALL;
				} else if (transType == TTVARBIND) {
					opCode = CCVARBINDAQALL;
				}
				concept->setOperatorCode(opCode);
				++mStatAutomateTransitionConceptCount;
				return concept;
			}


			CConcept* CRoleChainAutomataTransformationPreProcess::createStateConcept(TRANSLATIONTYPE transType) {
				CConcept* concept = nullptr;
				concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 conTag = mNextConceptTag++;
				concept->setConceptTag(conTag);
				mConVec->setData(conTag,concept);
				cint64 opCode = CCAQAND;
				if (transType == TTNORMAL) {
					opCode = CCAQAND;
				} else if (transType == TTIMPL) {
					opCode = CCIMPLAQAND;
				} else if (transType == TTBRANCH) {
					opCode = CCBRANCHAQAND;
				} else if (transType == TTPROPBIND) {
					opCode = CCPBINDAQAND;
				} else if (transType == TTBACKPROP) {
					opCode = CCVARPBACKAQAND;
				} else if (transType == TTVARBIND) {
					opCode = CCVARBINDAQAND;
				}
				concept->setOperatorCode(opCode);
				++mStatAutomateStateConceptCount;
				return concept;
			}


			CSortedNegLinker<CConcept*>* CRoleChainAutomataTransformationPreProcess::createTransitionOperandConceptLinker(CConcept* operandConcept, bool negation) {
				CSortedNegLinker<CConcept*>* opConceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				opConceptLinker->init(operandConcept,negation);
				return opConceptLinker;
			}

			CConcept* CRoleChainAutomataTransformationPreProcess::appendTransitionOperandConceptLinker(CConcept* concept, CSortedNegLinker<CConcept*>* opLinker) {

				//for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
				//	CConcept* opCon = opLinkerIt->getData();
				//	if (opCon == opLinker->getData()) {
				//		if (opLinkerIt->isNegated() == opLinker->isNegated()) {
				//			bool bug = true;
				//		}
				//	}
				//}

				concept->addOperandLinker(opLinker);
				concept->incOperandCount();
				return concept;
			}

			bool CRoleChainAutomataTransformationPreProcess::generateRoleChainAutomatConcept(CRole* lastRole, const CRecTravSubRoleChainDataItem& recTravSubRoleChainDataItem, QSet<CRole*>& alreadyUnfoldRoleSet, CConcept* beginConcept, CConcept* endConcept, TRANSLATIONTYPE transType) {
				const QList<CRoleSubRoleChainData>& subRoleChainDataList(recTravSubRoleChainDataItem.mDirectSubRoleChainDataList);
				generateRoleChainAutomatConcept(lastRole,subRoleChainDataList,alreadyUnfoldRoleSet,beginConcept,endConcept,transType);

				const QList<TRoleNegationPair>& recTravSubRoleList(recTravSubRoleChainDataItem.mRecTraversalSubRoleList);
				for (QList<TRoleNegationPair>::const_iterator it = recTravSubRoleList.constBegin(), itEnd = recTravSubRoleList.constEnd(); it != itEnd; ++it) {
					TRoleNegationPair recTravSubRoleNegPair(*it);
					CRole* recTravSubRole(recTravSubRoleNegPair.first);

					if (!alreadyUnfoldRoleSet.contains(recTravSubRole)) {
						bool recTravSubRoleInversed(recTravSubRoleNegPair.second);
						if (recTravSubRoleInversed) {
							recTravSubRole = getInverseRole(recTravSubRole);
						}

						QSet<CRole*> locUnfoldRoleSet(alreadyUnfoldRoleSet);
						locUnfoldRoleSet.insert(recTravSubRole);
						CSortedNegLinker<CRole*>* superSuperLinkerIt = recTravSubRole->getIndirectSuperRoleList();
						while (superSuperLinkerIt) {
							CRole* superSuperRole = superSuperLinkerIt->getData();
							locUnfoldRoleSet.insert(superSuperRole);
							superSuperLinkerIt = superSuperLinkerIt->getNext();
						}

						CConcept* recTravSubBeginConcept = createStateConcept(transType);
						CConcept* recTravSubEndConcept = createStateConcept(transType);
						appendTransitionOperandConceptLinker(beginConcept,createTransitionOperandConceptLinker(recTravSubBeginConcept,false));
						appendTransitionOperandConceptLinker(recTravSubEndConcept,createTransitionOperandConceptLinker(endConcept,false));
						CConcept* recTravPropCon = createTransitionConcept(recTravSubRole,transType);
						appendTransitionOperandConceptLinker(recTravSubBeginConcept,createTransitionOperandConceptLinker(recTravPropCon,false));
						appendTransitionOperandConceptLinker(recTravPropCon,createTransitionOperandConceptLinker(recTravSubEndConcept,false));
						const CRecTravSubRoleChainDataItem& nextRecTravSubRoleChainDataItem(mRoleRecTravSubRoleChainDataHash.value(recTravSubRole));
						generateRoleChainAutomatConcept(recTravSubRole,nextRecTravSubRoleChainDataItem,locUnfoldRoleSet,recTravSubBeginConcept,recTravSubEndConcept,transType);
					}
				}
				return true;
			}

			bool CRoleChainAutomataTransformationPreProcess::generateRoleChainAutomatConcept(CRole* lastRole, const QList<CRoleSubRoleChainData>& subRoleChainDataList, QSet<CRole*>& alreadyUnfoldRoleSet, CConcept* beginConcept, CConcept* endConcept, TRANSLATIONTYPE transType) {
				for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
					CRoleSubRoleChainData subRoleChainData(*it);
					CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
					bool subRoleNegation = subRoleChainData.mInverse;
					CRole* superRoleChain = subRoleChainData.mRole;

					QSet<CRole*> locUnfoldRoleSet(alreadyUnfoldRoleSet);
					locUnfoldRoleSet.insert(lastRole);
					CSortedNegLinker<CRole*>* superSuperLinkerIt = superRoleChain->getIndirectSuperRoleList();
					while (superSuperLinkerIt) {
						CRole* superSuperRole = superSuperLinkerIt->getData();
						locUnfoldRoleSet.insert(superSuperRole);
						superSuperLinkerIt = superSuperLinkerIt->getNext();
					}

					generateRoleChainAutomatConcept(lastRole,superRoleChain,subRoleChain,subRoleNegation,locUnfoldRoleSet,beginConcept,endConcept,transType);
				}
				return true;
			}


			bool CRoleChainAutomataTransformationPreProcess::generateRoleChainAutomatConcept(CRole* lastRole, CRole* superRole, CRoleChain* descendingRoleChain, bool negatedChain, QSet<CRole*>& alreadyUnfoldRoleSet, CConcept* beginConcept, CConcept* endConcept, TRANSLATIONTYPE transType) {
				// TODO:
				// generate for each role in the role chain a concept 
				// connect all concepts with each other,
				// append the first connected concept to the appending list
				// return later the last connected concept 
				// for each generated concept in the role chain
				//  check whether related role is complex and is not already used above (hash)
				//	 recursive call to generate concepts for each role chain and sub role

				CXLinker<CRole*>* chainedRoleLinker = nullptr;
				CXLinker<CRole*>* inverseChainedRoleLinker = nullptr;
				bool useInverseRoles = negatedChain;
				if (!negatedChain) {
					chainedRoleLinker = descendingRoleChain->getRoleChainLinker();
					inverseChainedRoleLinker = descendingRoleChain->getInverseRoleChainLinker();
				} else {
					chainedRoleLinker = descendingRoleChain->getInverseRoleChainLinker();
					inverseChainedRoleLinker = descendingRoleChain->getRoleChainLinker();
				}
				CConcept* connectBeginCon = nullptr;
				CConcept* connectEndCon = nullptr;
				CConcept* firstConcept = nullptr;
				CConcept* lastConcept = nullptr;
				bool transStart = false;
				bool transEnd = false;

				CRole* testRole = superRole;
				if (useInverseRoles && lastRole != superRole) {
					testRole = getInverseRole(superRole);
				}


				while (chainedRoleLinker) {
					CRole* subRole = chainedRoleLinker->getData();
					if (useInverseRoles) {
						// search inverse role
						subRole = getInverseRole(subRole);
					}

					subRole = mRoleVec->getData(subRole->getRoleTag());
					if (!firstConcept && subRole == testRole && !transStart) {
						// transitive start
						transStart = true;
					} else if (!chainedRoleLinker->hasNext() && subRole == testRole) {
						// transitive end
						transEnd = true;
					} else if (!alreadyUnfoldRoleSet.contains(subRole)) {
						// test recursive call

						CConcept* subBeginConcept = createStateConcept(transType);
						CConcept* propCon = createTransitionConcept(subRole,transType);
						CConcept* subEndConcept = createStateConcept(transType);
						if (!connectBeginCon) {
							connectBeginCon = subBeginConcept;
						}
						if (connectEndCon) {
							appendTransitionOperandConceptLinker(connectEndCon,createTransitionOperandConceptLinker(subBeginConcept,false));
						}
						connectEndCon = subEndConcept;

						if (!firstConcept) {
							firstConcept = propCon;
						}
						appendTransitionOperandConceptLinker(subBeginConcept,createTransitionOperandConceptLinker(propCon,false));
						appendTransitionOperandConceptLinker(propCon,createTransitionOperandConceptLinker(subEndConcept,false));



						const CRecTravSubRoleChainDataItem& recTravSubRoleChainDataItem(mRoleRecTravSubRoleChainDataHash.value(subRole));
						generateRoleChainAutomatConcept(subRole,recTravSubRoleChainDataItem,alreadyUnfoldRoleSet,subBeginConcept,subEndConcept,transType);

						//const QList<CRoleSubRoleChainData>& subRoleChainDataList(recTravSubRoleChainDataItem.mDirectSubRoleChainDataList);
						//generateRoleChainAutomatConcept(subRole,subRoleChainDataList,alreadyUnfoldRoleSet,subBeginConcept,subEndConcept,transType);

						////for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
						////	CRoleSubRoleChainData subRoleChainData(*it);
						////	CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
						////	bool subRoleNegation = subRoleChainData.mInverse;
						////	CRole* superRoleChain = subRoleChainData.mRole;

						////	QSet<CRole*> locUnfoldRoleSet(alreadyUnfoldRoleSet);
						////	locUnfoldRoleSet.insert(subRole);
						////	CSortedNegLinker<CRole*>* superSuperLinkerIt = superRoleChain->getIndirectSuperRoleList();
						////	while (superSuperLinkerIt) {
						////		CRole* superSuperRole = superSuperLinkerIt->getData();
						////		locUnfoldRoleSet.insert(superSuperRole);
						////		superSuperLinkerIt = superSuperLinkerIt->getNext();
						////	}

						////	generateRoleChainAutomatConcept(subRole,superRoleChain,subRoleChain,subRoleNegation,locUnfoldRoleSet,subBeginConcept,subEndConcept,transType);
						////}

						//const QList<TRoleNegationPair>& recTravSubRoleList(recTravSubRoleChainDataItem.mRecTraversalSubRoleList);
						//for (QList<TRoleNegationPair>::const_iterator it = recTravSubRoleList.constBegin(), itEnd = recTravSubRoleList.constEnd(); it != itEnd; ++it) {
						//	TRoleNegationPair recTravSubRoleNegPair(*it);
						//	CRole* recTravSubRole(recTravSubRoleNegPair.first);
						//	bool recTravSubRoleInversed(recTravSubRoleNegPair.second);
						//	if (recTravSubRoleInversed) {
						//		recTravSubRole = getInverseRole(recTravSubRole);
						//	}
						//	CConcept* recTravSubBeginConcept = createStateConcept(transType);
						//	CConcept* recTravSubEndConcept = createStateConcept(transType);
						//	appendTransitionOperandConceptLinker(subBeginConcept,createTransitionOperandConceptLinker(recTravSubBeginConcept,false));
						//	appendTransitionOperandConceptLinker(recTravSubEndConcept,createTransitionOperandConceptLinker(subEndConcept,false));
						//	CConcept* recTravPropCon = createTransitionConcept(recTravSubRole,transType);
						//	appendTransitionOperandConceptLinker(recTravSubBeginConcept,createTransitionOperandConceptLinker(recTravPropCon,false));
						//	appendTransitionOperandConceptLinker(recTravPropCon,createTransitionOperandConceptLinker(recTravSubEndConcept,false));
						//	generateRoleChainAutomatConcept(recTravSubRole,subRoleChainDataList,alreadyUnfoldRoleSet,recTravSubBeginConcept,recTravSubEndConcept,transType);
						//}

						//// search fitting sub role chains
						////bool hasFittingSubRole = false;
						////QList<CRoleSubRoleChainData> subRoleChainDataList(mRoleSubRoleChainDataHash.values(subRole));
						////hasFittingSubRole = !subRoleChainDataList.isEmpty();
						////if (hasFittingSubRole) {
						////	for (QList<CRoleSubRoleChainData>::const_iterator it = subRoleChainDataList.constBegin(), itEnd = subRoleChainDataList.constEnd(); it != itEnd; ++it) {
						////		CRoleSubRoleChainData subRoleChainData(*it);
						////		CRoleChain* subRoleChain = subRoleChainData.mRoleChain;
						////		bool subRoleNegation = subRoleChainData.mInverse;
						////		CRole* superRoleChain = subRoleChainData.mRole;

						////		QSet<CRole*> locUnfoldRoleSet(alreadyUnfoldRoleSet);
						////		locUnfoldRoleSet.insert(subRole);
						////		CSortedNegLinker<CRole*>* superSuperLinkerIt = superRoleChain->getIndirectSuperRoleList();
						////		while (superSuperLinkerIt) {
						////			CRole* superSuperRole = superSuperLinkerIt->getData();
						////			locUnfoldRoleSet.insert(superSuperRole);
						////			superSuperLinkerIt = superSuperLinkerIt->getNext();
						////		}

						////		generateRoleChainAutomatConcept(subRole,superRoleChain,subRoleChain,subRoleNegation,locUnfoldRoleSet,subBeginConcept,subEndConcept,transType);
						////	}
						////}
						lastConcept = propCon;
					} else {
						// error, not allowed construct
						bool bug = true;
					}

					chainedRoleLinker = chainedRoleLinker->getNext();
				}
				if (transStart && !transEnd) {
					//if (lastRole != testRole) {
					//	CConcept* tmpEndConcept = createStateConcept(transType);
					//	appendTransitionOperandConceptLinker(tmpEndConcept,createTransitionOperandConceptLinker(endConcept,false));
					//	endConcept = tmpEndConcept;
					//}
					appendTransitionOperandConceptLinker(endConcept,createTransitionOperandConceptLinker(connectBeginCon,false));
					appendTransitionOperandConceptLinker(connectEndCon,createTransitionOperandConceptLinker(endConcept,false));
				} else if (transEnd && !transStart) {
					//if (lastRole != testRole) {
					//	CConcept* tmpBeginConcept = createStateConcept(transType);
					//	appendTransitionOperandConceptLinker(beginConcept,createTransitionOperandConceptLinker(tmpBeginConcept,false));
					//	beginConcept = tmpBeginConcept;
					//}
					appendTransitionOperandConceptLinker(connectEndCon,createTransitionOperandConceptLinker(beginConcept,false));
					appendTransitionOperandConceptLinker(beginConcept,createTransitionOperandConceptLinker(connectBeginCon,false));
				} else if (transStart && transEnd && !firstConcept) {
					//if (lastRole != testRole) {
					//	CConcept* tmpBeginConcept = createStateConcept(transType);
					//	appendTransitionOperandConceptLinker(beginConcept,createTransitionOperandConceptLinker(tmpBeginConcept,false));
					//	beginConcept = tmpBeginConcept;
					//	CConcept* tmpEndConcept = createStateConcept(transType);
					//	appendTransitionOperandConceptLinker(tmpEndConcept,createTransitionOperandConceptLinker(endConcept,false));
					//	endConcept = tmpEndConcept;
					//	CConcept* propCon = createTransitionConcept(testRole,transType);
					//	appendTransitionOperandConceptLinker(tmpBeginConcept,createTransitionOperandConceptLinker(propCon,false));
					//	appendTransitionOperandConceptLinker(propCon,createTransitionOperandConceptLinker(tmpEndConcept,false));
					//}
					appendTransitionOperandConceptLinker(endConcept,createTransitionOperandConceptLinker(beginConcept,false));
				} else if (!transStart && !transEnd) {
					if (connectBeginCon && connectEndCon) {
						appendTransitionOperandConceptLinker(beginConcept,createTransitionOperandConceptLinker(connectBeginCon,false));
						appendTransitionOperandConceptLinker(connectEndCon,createTransitionOperandConceptLinker(endConcept,false));
					}
				} else {
					// error, not allowed construct
				}

				return true;
			}



			CRole* CRoleChainAutomataTransformationPreProcess::getLocalizedRole(CRole* role) {
				return getLocalizedRole(role->getRoleTag());
			}


			CRole* CRoleChainAutomataTransformationPreProcess::getLocalizedRole(cint64 roleTag) {
				CRole* role = mRoleVec->getData(roleTag);
				if (!mRoleVec->hasLocalData(roleTag)) {
					role = CConceptRoleIndividualLocator::getLocatedRole(role,mOntology);
				}
				return role;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
