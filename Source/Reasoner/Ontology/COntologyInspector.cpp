/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "COntologyInspector.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyInspector::COntologyInspector() {
			}


			COntologyInspector::~COntologyInspector() {
			}


			COntologyStructureSummary *COntologyInspector::createStructureSummary(CConcreteOntology *ontology) {
				COntologyStructureSummary *ontStrSum = inspectOntology(ontology);
				if (ontStrSum) {
					ontology->setStructureSummary(ontStrSum);
				}
				return ontStrSum;
			}


			CBOXSET<CConcept*> *COntologyInspector::createConceptContainsSet(CConcept *concept, CTBox *tBox, CBOXSET<CConcept*> *containsSet) {
				if (!containsSet) {
					containsSet = new CBOXSET<CConcept*>();
				}
				QList<CConcept *> checkList;
				checkList.append(concept);
				CSortedNegLinker<CConcept *> *conIt = 0;
				while (!checkList.isEmpty()) {
					CConcept *checkConcept = checkList.takeFirst();
					if (!containsSet->contains(checkConcept)) {
						containsSet->insert(checkConcept);
						// collect new dependencies
						CRole *role = checkConcept->getRole();
						if (role) {
							CSortedNegLinker<CRole *> *superRoleIt = role->getIndirectSuperRoleList();
							while (superRoleIt) {
								CRole *superRole = superRoleIt->getData();
								conIt = superRole->getDomainConceptList();
								while (conIt) {
									CConcept *con = conIt->getData();
									checkList.append(con);
									conIt = conIt->getNext();
								}
								conIt = superRole->getRangeConceptList();
								while (conIt) {
									CConcept *con = conIt->getData();
									checkList.append(con);
									conIt = conIt->getNext();
								}
								superRoleIt = superRoleIt->getNext();
							}
						}
						conIt = checkConcept->getOperandList();
						while (conIt) {
							CConcept *con = conIt->getData();
							checkList.append(con);
							conIt = conIt->getNext();
						}
					}
				}
				return containsSet;
			}


			COntologyInspector *COntologyInspector::createGCIConceptSet(CTBox *tBox) {
				CConceptVector *conVec = tBox->getConceptVector();
				qint64 conCount = conVec->getItemCount();
				CConcept *topCon = conVec->getData(1);
				if (topCon) {
					CContext* tBoxContext = tBox->getBoxContext();
					CBOXSET<CConcept*> *gciContainsSet = createConceptContainsSet(topCon,tBox,CObjectParameterizingAllocator< CBOXSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(tBoxContext),tBoxContext));
					tBox->setGCIConceptSet(gciContainsSet);
				}
				return this;
			}


			COntologyStructureSummary* COntologyInspector::inspectConcept(CConcreteOntology *ontology, CConcept* concept, bool negation, COntologyStructureSummary *ontStructSum) {
				qint64 conOpTag = concept->getDefinitionOperatorTag();
				bool isClass = concept->hasClassName();
				bool opCount = concept->getOperandCount() > 0;
				cint64 parm = concept->getParameter();
				CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
				for (CSortedNegLinker<CConcept*>* opLinkerIt = opLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
					if (opLinkerIt->isNegated()) {
						CConcept* opConcept = opLinkerIt->getData();
						CConceptOperator* conOperator = opConcept->getConceptOperator();
						if (!conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)) {
							ontStructSum->setNegationDisjunctionsOccurrence();
						}
					}
				}
				CRole* role = concept->getRole();
				if (role && role->isDataRole()) {
					ontStructSum->setDatatypeOccurrence();
				}
				if (isClass) {
					ontStructSum->incClassCount();
				}
				if (conOpTag == CCATOM) {
					ontStructSum->incPrimitiveConceptCount();
					if (isClass) {
						ontStructSum->incAtomicClassCount();
					}
				} else {
					ontStructSum->incComplexConceptCount();
					if (isClass) {
						ontStructSum->incComplexClassCount();
					}
					if (conOpTag == CCPBINDGROUND || conOpTag == CCNOMTEMPLREF || conOpTag == CCVARBINDGROUND) {
						ontStructSum->setGroundingOccurrence();
					} else if (conOpTag == CCNOT) {
						ontStructSum->incConceptNegCount();
						ontStructSum->setNegationDisjunctionsOccurrence();
					} else if (conOpTag == CCAND) {
						ontStructSum->incConceptIntersectionCount();
					} else if (conOpTag == CCOR) {
						ontStructSum->incConceptUnionCount();
						ontStructSum->setNegationDisjunctionsOccurrence();
					} else if (conOpTag == CCATMOST) {
						ontStructSum->incConceptAtmostCount();
						if (opCount) {
							ontStructSum->incConceptQualifiedAtmostCount();
						} else {
							ontStructSum->incConceptNotQualifiedAtmostCount();
						}
						if (parm == 1) {
							ontStructSum->setFunctionalityOccurrence();
							ontStructSum->incConceptFunctionalAtmostCount();
						} else if (opCount) {
							ontStructSum->setQualifiedCardinalityOccurrence();
						} else {
							ontStructSum->setCardinalityOccurrence();
						}
					} else if (conOpTag == CCATLEAST) {
						ontStructSum->setExistensialOccurrence();
						ontStructSum->incConceptAtleastCount();
						if (opCount) {
							ontStructSum->incConceptQualifiedAtleastCount();
							if (parm != 1) {
								ontStructSum->setQualifiedCardinalityOccurrence();
							}
						} else {
							ontStructSum->incConceptNotQualifiedAtleastCount();
							if (parm != 1) {
								ontStructSum->setCardinalityOccurrence();
							}
						}
					} else if (conOpTag == CCALL) {
						ontStructSum->incConceptOnlyCount();
						ontStructSum->setExistensialOccurrence();
					} else if (conOpTag == CCSOME || conOpTag == CCAQSOME) {
						ontStructSum->incConceptSomeCount();
						ontStructSum->setExistensialOccurrence();
					} else if (conOpTag == CCEQ) {
						ontStructSum->setNegationDisjunctionsOccurrence();
						ontStructSum->incConceptEquivalenceCount();
					} else if (conOpTag == CCSUB) {
						ontStructSum->incConceptSubclassCount();
					} else if (conOpTag == CCVALUE) {
						ontStructSum->incConceptValueCount();
						ontStructSum->setNominalIndividualOccurrence();
					} else if (conOpTag == CCNOMINAL) {
						ontStructSum->incConceptNominalIndividualCount();
						ontStructSum->setNominalIndividualOccurrence();
						ontStructSum->setNegationDisjunctionsOccurrence();
					} else if (conOpTag == CCSELF) {
						ontStructSum->incConceptSelfCount();
					} else if (conOpTag == CCDATATYPE || CCDATALITERAL || CCDATARESTRICTION) {
					}
				} 
				return ontStructSum;
			}

			COntologyStructureSummary *COntologyInspector::inspectOntology(CConcreteOntology *ontology) {
				COntologyStructureSummary *ontStructSum = new COntologyStructureSummary();

				CRBox *rBox = ontology->getDataBoxes()->getRBox();
				CRoleVector *roleVec = rBox->getRoleVector();
				qint64 roleCount = roleVec->getItemCount();
				for (qint64 index = 0; index < roleCount; ++index) {
					CRole* role = roleVec->getData(index);
					if (role) {
						CSortedNegLinker<CRole*>* invEqRoleLinkerIt = role->getInverseEquivalentRoleList();
						while (invEqRoleLinkerIt) {
							bool neg = invEqRoleLinkerIt->isNegated();
							CRole* inEqRole = invEqRoleLinkerIt->getData();
							if (neg) {
								ontStructSum->setInverseOccurrence();
								ontStructSum->incRoleInverseCount();
							} else {
								ontStructSum->incRoleEquivalenceCount();
							}
							invEqRoleLinkerIt = invEqRoleLinkerIt->getNext();
						}
						CSortedNegLinker<CRole*>* disRoleLinkerIt = role->getDisjointRoleList();
						while (disRoleLinkerIt) {
							ontStructSum->incRoleDisjointCount();
							disRoleLinkerIt = disRoleLinkerIt->getNext();
						}
						CSortedNegLinker<CConcept*>* domConLinkerIt = role->getDomainConceptList();
						while (domConLinkerIt) {
							ontStructSum->incRoleDomainConceptCount();
							domConLinkerIt = domConLinkerIt->getNext();
						}
						CSortedNegLinker<CConcept*>* rangeConLinkerIt = role->getRangeConceptList();
						while (rangeConLinkerIt) {
							ontStructSum->incRoleRangeConceptCount();
							rangeConLinkerIt = rangeConLinkerIt->getNext();
						}
						if (role->isAsymmetric()) {
							ontStructSum->incRoleAsymmetricCount();
						}
						if (role->isSymmetric()) {
							ontStructSum->incRoleSymmetricCount();
						}
						if (role->isTransitive()) {
							ontStructSum->setTransitivityOccurrence();
							ontStructSum->incRoleTransetivCount();
						}
						if (role->isReflexive()) {
							ontStructSum->incRoleReflexivityCount();
						}
						if (role->isIrreflexive()) {
							ontStructSum->incRoleIrreflexivityCount();
						}
						if (role->isFunctional()) {
							ontStructSum->setFunctionalityOccurrence();
							ontStructSum->incRoleFunctionalCount();
						}
						if (role->isInverseFunctional()) {
							ontStructSum->setFunctionalityOccurrence();
							ontStructSum->incRoleInverseFunctionalCount();
						}
						if (role->hasRoleChainSuperSharing()) {
							bool allTransChaincs = true;
							CXLinker<CRoleChain*>* roleChainLinkerIt = role->getRoleChainSuperSharingLinker();
							while (roleChainLinkerIt && allTransChaincs) {
								CRoleChain* roleChain = roleChainLinkerIt->getData();
								CXLinker<CRole*>* chainRoleLinker = roleChain->getRoleChainLinker();
								while (chainRoleLinker) {
									CRole* chainRole = chainRoleLinker->getData();
									if (chainRole != role) {
										ontStructSum->setComplexSubRoleOccurrence();
										allTransChaincs = false;
										break;
									}
									chainRoleLinker = chainRoleLinker->getNext();
								}
								roleChainLinkerIt = roleChainLinkerIt->getNext();
							}
							if (allTransChaincs) {
								ontStructSum->setTransitivityOccurrence();
							}
						}
						CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
						while (superRoleLinkerIt) {
							if (!superRoleLinkerIt->isNegated()) {
								//if (!role->hasEquivalentRole(superRoleLinkerIt->getData())) {
								//}
								ontStructSum->setRoleHierarchyOccurrence();
							}
							superRoleLinkerIt = superRoleLinkerIt->getNext();
						}
					}
				}

				CABox *aBox = ontology->getDataBoxes()->getABox();
				CIndividualVector *indiVec = aBox->getIndividualVector();
				qint64 indiCount = indiVec->getItemCount();


				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CConceptVector *conVec = tBox->getConceptVector();
				qint64 conCount = conVec->getItemCount();
				for (qint64 index = 0; index < conCount; ++index) {
					CConcept *concept = conVec->getData(index);
					if (concept) {
						ontStructSum->incConceptCount();
						inspectConcept(ontology,concept,false,ontStructSum);
					}
				}

				return ontStructSum;
			}



			bool COntologyInspector::testOntologyForNonDeterministicConcepts(CConcreteOntology *ontology, COntologyStructureSummary *ontStructSum) {
				CStructureFlags structureFlags;

				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CABox* aBox = ontology->getDataBoxes()->getABox();
				CConcept *topConcept = ontology->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = ontology->getDataBoxes()->getBottomConcept();

				QSet< QPair<CConcept*,bool> > singleConNegSet;

				CDatatypeValueSpacesTriggers* valueSpaceTriggers = ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
				if (valueSpaceTriggers) {
					structureFlags.mValidDeterministic = false;
				}

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd && (structureFlags.mValidDeterministic); ++it) {
						CConcept *concept = (*it);
						if (concept) {
							if (concept->getOperatorCode() == CCEQ) {
								structureFlags.mValidDeterministic = false;
							} else {
								analyseConceptStructureFlags(concept,false,singleConNegSet,nullptr,structureFlags);
							}
						}
					}
				}
				if (structureFlags.mValidDeterministic && (structureFlags.mUniversalRoleOccurence || structureFlags.mNominalOccurence)) {
					CBOXSET<CIndividual*>* individualSet = aBox->getActiveIndividualSet(false);
					if (individualSet) {
						for (CBOXSET<CIndividual*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd && (structureFlags.mValidDeterministic); ++it) {
							CIndividual *indi = (*it);
							if (indi) {
								CConceptAssertionLinker* assConIt = indi->getAssertionConceptLinker();
								while (assConIt) {
									CConcept* assCon = assConIt->getData();
									bool assConNeg = assConIt->isNegated();
									analyseConceptStructureFlags(assCon,assConNeg,singleConNegSet,nullptr,structureFlags);
									assConIt = assConIt->getNext();
								}
							}
						}
					}
				}

				if (ontStructSum) {
					ontStructSum->setOnlyDeterministicConceptClasses(structureFlags.mValidDeterministic);
				}

				return structureFlags.mValidDeterministic;
			}


			bool COntologyInspector::testOntologyForNonELConcepts(CConcreteOntology *ontology, COntologyStructureSummary *ontStructSum) {
				CStructureFlags structureFlags;

				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CConcept *topConcept = ontology->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = ontology->getDataBoxes()->getBottomConcept();

				QSet< QPair<CConcept*,bool> > singleConNegSet;
				QHash<CRole*,bool> existRoleHash;

				CDatatypeValueSpacesTriggers* valueSpaceTriggers = ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
				if (valueSpaceTriggers) {
					structureFlags.mValidEL = false;
				}

				CBOXSET<CConcept*> *conceptHash = tBox->getActiveClassConceptSet(false);
				if (conceptHash) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptHash->constBegin(), itEnd = conceptHash->constEnd(); it != itEnd && (structureFlags.mValidEL); ++it) {
						CConcept *concept = (*it);
						if (concept) {
							if (concept->getOperatorCode() == CCEQ) {
								structureFlags.mValidEL = false;
							} else {
								analyseConceptStructureFlags(concept,false,singleConNegSet,&existRoleHash,structureFlags);
							}
						}
					}
				}

				if (ontStructSum) {
					ontStructSum->setOnlyELConceptClasses(structureFlags.mValidEL);
				}

				return structureFlags.mValidEL;
			}


			bool COntologyInspector::analyzeOntologyConceptsStructureFlags(CConcreteOntology *ontology, COntologyStructureSummary *ontStructSum) {

				CStructureFlags structureFlags;

				CTBox* tBox = ontology->getDataBoxes()->getTBox();
				CABox* aBox = ontology->getDataBoxes()->getABox();
				CConcept *topConcept = ontology->getDataBoxes()->getTopConcept();
				CConcept *bottomConcept = ontology->getDataBoxes()->getBottomConcept();

				QSet< QPair<CConcept*,bool> > singleConNegSet;
				QHash<CRole*,bool> existRoleHash;

				CDatatypeValueSpacesTriggers* valueSpaceTriggers = ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
				if (valueSpaceTriggers) {
					structureFlags.mValidEL = false;
					structureFlags.mValidDeterministic = false;
				}

				CBOXSET<CConcept*>* conceptSet = tBox->getActiveClassConceptSet(false);
				if (conceptSet) {
					for (CBOXSET<CConcept*>::const_iterator it = conceptSet->constBegin(), itEnd = conceptSet->constEnd(); it != itEnd && (structureFlags.mValidEL || structureFlags.mValidDeterministic || !structureFlags.mNominalOccurence); ++it) {
						CConcept *concept = (*it);
						if (concept) {
							if (concept->getOperatorCode() == CCEQ) {
								structureFlags.mValidEL = false;
								structureFlags.mValidDeterministic = false;
							} else {
								analyseConceptStructureFlags(concept,false,singleConNegSet,&existRoleHash,structureFlags);
							}
						}
					}
				}

				
				if (structureFlags.mValidDeterministic && (structureFlags.mUniversalRoleOccurence || structureFlags.mNominalOccurence)) {
					CBOXSET<CIndividual*>* individualSet = aBox->getActiveIndividualSet(false);
					if (individualSet) {
						for (CBOXSET<CIndividual*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd && (structureFlags.mValidEL || structureFlags.mValidDeterministic || !structureFlags.mNominalOccurence); ++it) {
							CIndividual *indi = (*it);
							if (indi) {
								CConceptAssertionLinker* assConIt = indi->getAssertionConceptLinker();
								while (assConIt) {
									CConcept* assCon = assConIt->getData();
									bool assConNeg = assConIt->isNegated();
									analyseConceptStructureFlags(assCon,assConNeg,singleConNegSet,&existRoleHash,structureFlags);
									assConIt = assConIt->getNext();
								}
							}
						}
					}
				}

				if (ontStructSum) {
					ontStructSum->setNominalOccurrence(structureFlags.mNominalOccurence);
					ontStructSum->setOnlyELConceptClasses(structureFlags.mValidEL);
					ontStructSum->setOnlyDeterministicConceptClasses(structureFlags.mValidDeterministic);
				}

				return structureFlags.mValidEL || structureFlags.mValidDeterministic;
			}




			bool COntologyInspector::analyseConceptStructureFlags(CConcept* concept, bool negated, QSet< QPair<CConcept*,bool> >& singleConNegSet, QHash<CRole*,bool>* existRoleHash, CStructureFlags& structureFlags) {
				singleConNegSet.insert(QPair<CConcept*,bool>(concept,negated));
				QList< QPair<CConcept*,bool> > checkConNegList;
				checkConNegList.append(QPair<CConcept*,bool>(concept,negated));

				while (!checkConNegList.isEmpty()) {
					QPair<CConcept*,bool> conNegPair(checkConNegList.takeFirst());
					CConcept* con = conNegPair.first;
					bool conNeg = conNegPair.second;
					CRole* role = con->getRole();
					if (role) {
						if (role->getRoleTag() == 1) {
							structureFlags.mUniversalRoleOccurence = true;
						}
						CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
						while (superRoleIt) {
							CRole* superRole = superRoleIt->getData();
							CSortedNegLinker<CConcept*>* domConIt = superRole->getDomainConceptList();
							while (domConIt) {
								CConcept* domCon = domConIt->getData();
								bool domConNeg = domConIt->isNegated();
								if (!singleConNegSet.contains(QPair<CConcept*,bool>(domCon,domConNeg))) {
									singleConNegSet.insert(QPair<CConcept*,bool>(domCon,domConNeg));
									checkConNegList.append(QPair<CConcept*,bool>(domCon,domConNeg));
								}
								domConIt = domConIt->getNext();
							}
							CSortedNegLinker<CConcept*>* ranConIt = superRole->getRangeConceptList();
							if (ranConIt) {
								structureFlags.mValidEL = false;
								while (ranConIt) {
									CConcept* ranCon = ranConIt->getData();
									bool ranConNeg = ranConIt->isNegated();
									if (!singleConNegSet.contains(QPair<CConcept*,bool>(ranCon,ranConNeg))) {
										singleConNegSet.insert(QPair<CConcept*,bool>(ranCon,ranConNeg));
										checkConNegList.append(QPair<CConcept*,bool>(ranCon,ranConNeg));
									}
									ranConIt = ranConIt->getNext();
								}
							}
							superRoleIt = superRoleIt->getNext();
						}
					}
					CIndividual* indi = con->getNominalIndividual();
					if (indi) {
						structureFlags.mValidEL = false;
						structureFlags.mNominalOccurence = true;
					}


					cint64 opCode = con->getOperatorCode();
					CConceptOperator* conOperator = con->getConceptOperator();
					cint64 param = con->getParameter();
					cint64 opCount = con->getOperandCount();
					if (!con->hasClassName()) {
						if (!conNeg && (opCode == CCOR) && opCount > 1 || conNeg && (opCode == CCAND || opCode == CCEQ) && opCount > 1) {
							structureFlags.mValidDeterministic = false;
							structureFlags.mValidEL = false;

						} else if (!conNeg && (opCode == CCATMOST) || conNeg && (opCode == CCATLEAST)) {
							structureFlags.mValidEL = false;
							if (opCount >= 1 || !conNeg && (opCode == CCATMOST) && param > 1 || conNeg && (opCode == CCATLEAST) && param > 2) {
								structureFlags.mValidDeterministic = false;
							}
						} else {
							if (!existRoleHash) {
								if (!conNeg && opCode == CCALL) {
									structureFlags.mValidEL = false;
								}
							}
							// TODO: check el, SOME r. and SOME r^-1 is not allowed
							// TODO: check el, ALL r. translated into automata concept AQALL also not allowed
						}
					}

					if (existRoleHash) {
						if (conNeg && (opCode == CCSOME) || !conNeg && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE))) {
							if (existRoleHash->contains(role)) {
								if (existRoleHash->value(role) == true) {
									structureFlags.mValidEL = false;
								}
							} else {
								existRoleHash->insert(role,false);
							}
						}
						if (conNeg && (opCode == CCALL) || !conNeg && (opCode == CCSOME || opCode == CCAQSOME)) {
							if (existRoleHash->contains(role)) {
								if (existRoleHash->value(role) == false) {
									structureFlags.mValidEL = false;
								}
							} else {
								existRoleHash->insert(role,true);
							}
						}
					}
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_DATATYPE_RELATED_TYPE)) {
						structureFlags.mValidEL = false;
					}

					bool inspectOperandsNegated = false;
					bool inspectOnlySameNegation = false;
					if (opCode == CCAQCHOOCE) {
						inspectOnlySameNegation = true;
					}
					if (conNeg && (opCode == CCSOME || opCode == CCALL || opCode == CCAND || opCode == CCEQ || opCode == CCOR)) {
						inspectOperandsNegated = true;
					}
					cint64 inspectOperandCount = opCount;
					if (opCode == CCVARBINDJOIN || opCode == CCPBINDCYCLE || conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE)) {
						inspectOperandCount = 1;
					}
					CSortedNegLinker<CConcept*>* opConIt = con->getOperandList();
					while (opConIt) {
						CConcept* opCon = opConIt->getData();
						bool opConNeg = opConIt->isNegated()^inspectOperandsNegated;
						if (inspectOperandCount-- > 0) {
							if (!inspectOnlySameNegation || opConNeg == conNeg) {
								if (!singleConNegSet.contains(QPair<CConcept*,bool>(opCon,opConNeg))) {
									singleConNegSet.insert(QPair<CConcept*,bool>(opCon,opConNeg));
									checkConNegList.append(QPair<CConcept*,bool>(opCon,opConNeg));
								}
							}
						}
						opConIt = opConIt->getNext();
					}

				}


				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
