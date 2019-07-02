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

#include "CMappingUpdaterPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CMappingUpdaterPreProcess::CMappingUpdaterPreProcess() {
			}


			CMappingUpdaterPreProcess::~CMappingUpdaterPreProcess() {
			}


			CConcreteOntology *CMappingUpdaterPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				if (ontology) {
					tbox = ontology->getDataBoxes()->getTBox();
					abox = ontology->getDataBoxes()->getABox();
					rbox = ontology->getDataBoxes()->getRBox();
					mBox = ontology->getDataBoxes()->getMBox();


					mBuildConstructFlags = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags();

					mRoleVec = rbox->getRoleVector();
					mConceptVec = tbox->getConceptVector();

					mRoleObjPropTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();
					mObjPropTermRoleHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();

					mRoleDataPropTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleDataPropertyTermMappingHash();
					mDataPropTermRoleHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash();

					mClassTermConceptHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
					mConceptClassTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();

					mIndiTermIndiHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash();
					mIndiIndiTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();

					mDeclarationAxiomSet = ontology->getBuildData()->getDeclarationAxiomSet();

					mClassTermClassAxiomHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();
					mObjPropTermObjPropAxiomHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomHash();

					mNameConceptHash = ontology->getStringMapping()->getClassNameConceptMappingHash();

					mActiveEntityVector = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getActiveEntityVector();

					qint64 rolesStartUpdateInd = mRoleVec->getReferencedItemCount();
					qint64 rolesCounts = mRoleVec->getItemCount();
					for (qint64 i = rolesStartUpdateInd; i < rolesCounts; ++i) {
						CRole *role = mRoleVec->getLocalData(i);
						if (role) {
							CObjectPropertyTermExpression* objPropTerm = mRoleObjPropTermHash->value(role,nullptr);
							if (objPropTerm) {
								CRole* prevRole = mRoleVec->getReferenceData(i);
								if (prevRole) {
									objPropTerm = mRoleObjPropTermHash->value(prevRole,nullptr);
								}
								if (objPropTerm) {
									mRoleObjPropTermHash->insert(role,objPropTerm);
									mObjPropTermRoleHash->insert(objPropTerm,role);
								}
							}
						}
					}

					qint64 conStartUpdateInd = mConceptVec->getReferencedItemCount();
					qint64 conCounts = mConceptVec->getItemCount();
					for (qint64 i = 0; i < conCounts; ++i) {
						CConcept *concept = mConceptVec->getLocalData(i);
						if (concept) {
							if (concept->hasClassName()) {
								mNameConceptHash->insert(CIRIName::getRecentIRIName(concept->getClassNameLinker()),concept);
							}
							if (i < conStartUpdateInd) {
								CClassTermExpression* classTerm = mConceptClassTermHash->value(concept,nullptr);
								if (classTerm) {
									CConcept* prevConcept = mConceptVec->getReferenceData(i);
									if (prevConcept) {
										classTerm = mConceptClassTermHash->value(prevConcept,nullptr);
									}
									if (classTerm) {
										mConceptClassTermHash->insert(concept,classTerm);
										mClassTermConceptHash->insert(classTerm,concept);
									}
								}
							}
						}
					}
				}

				collectActiveOntologyItemsFromActiveEntities();

				return ontology;
			}

			bool CMappingUpdaterPreProcess::collectActiveOntologyItemsFromActiveEntities() {
				CBOXSET<CConcept*>* activeClassConceptSet = tbox->getActiveClassConceptSet();
				activeClassConceptSet->clear();

				CBOXSET<CIndividual*>* activeIndividualSet = abox->getActiveIndividualSet();
				activeIndividualSet->clear();

				CBOXSET<CRole*>* activeRoleSet = rbox->getActivePropertyRoleSet();
				activeRoleSet->clear();

				CActiveEntityIterator activeEntityIterator(mActiveEntityVector->getActiveEntityIterator());
				while (activeEntityIterator.hasNextActiveEntity()) {
					CExpressionEntity* expressionEntity = activeEntityIterator.getNextActiveEntity(true);
					if (expressionEntity) {
						CClassExpression* classExpression = dynamic_cast<CClassExpression*>(expressionEntity);
						if (classExpression) {
							CConcept* concept = mClassTermConceptHash->value(classExpression);
							if (concept) {
								activeClassConceptSet->insert(concept);
							}
						} else {
							CNamedIndividualExpression* namedIndiExpression = dynamic_cast<CNamedIndividualExpression*>(expressionEntity);
							if (namedIndiExpression) {
								CIndividual* individual = mIndiTermIndiHash->value(namedIndiExpression);
								if (individual) {
									activeIndividualSet->insert(individual);
								}
							} else {
								CAnonymousIndividualExpression* anoIndiExpression = dynamic_cast<CAnonymousIndividualExpression*>(expressionEntity);
								if (anoIndiExpression) {
									CIndividual* individual = mIndiTermIndiHash->value(anoIndiExpression);
									if (individual) {
										activeIndividualSet->insert(individual);
									}
								} else {
									CObjectPropertyExpression* objPropertyExpression = dynamic_cast<CObjectPropertyExpression*>(expressionEntity);
									if (objPropertyExpression) {
										CRole* role = mObjPropTermRoleHash->value(objPropertyExpression);
										if (role) {
											activeRoleSet->insert(role);
										}
									} else {
										CDataPropertyExpression* dataPropertyExpression = dynamic_cast<CDataPropertyExpression*>(expressionEntity);
										if (dataPropertyExpression) {
											CRole* role = mDataPropTermRoleHash->value(dataPropertyExpression);
											if (role) {
												activeRoleSet->insert(role);
											}
										}
									}
								}
							}
						}
					}
				}
				
				return true;
			}

			bool CMappingUpdaterPreProcess::collectActiveOntologyItems() {

				mUpdateConceptSet.insert(tbox->getTopConcept());
				mUpdateConceptList.append(tbox->getTopConcept());
				mUpdateConceptSet.insert(tbox->getBottomConcept());
				mUpdateConceptList.append(tbox->getBottomConcept());

				mUpdateRoleSet.insert(rbox->getTopObjectRole());
				mUpdateRoleList.append(rbox->getTopObjectRole());
				mUpdateRoleSet.insert(rbox->getBottomObjectRole());
				mUpdateRoleList.append(rbox->getBottomObjectRole());


				if (!mBuildConstructFlags->isRetractionUsed()) {
					qint64 conCounts = mConceptVec->getItemCount();
					for (qint64 i = 2; i < conCounts; ++i) {
						CConcept *concept = mConceptVec->getLocalData(i);
						if (concept) {
							if (concept->hasClassName()) {
								if (!mUpdateConceptSet.contains(concept)) {
									mUpdateConceptSet.insert(concept);
									mUpdateConceptList.append(concept);
								}
							}
						}
					}
				} else {
					FOREACHIT (CDeclarationAxiomExpression* declAxiom, *mDeclarationAxiomSet) {
						CExpressionEntity* entity = declAxiom->getEntity();
						CClassExpression* classExp = dynamic_cast<CClassExpression*>(entity);
						if (classExp) {
							CConcept* concept = mClassTermConceptHash->value(classExp,nullptr);
							if (concept) {
								if (!mUpdateConceptSet.contains(concept)) {
									mUpdateConceptSet.insert(concept);
									mUpdateConceptList.append(concept);
								}
							}
						}
						CObjectPropertyExpression* objPropExp = dynamic_cast<CObjectPropertyExpression*>(entity);
						if (objPropExp) {
							CRole* role = mObjPropTermRoleHash->value(objPropExp,nullptr);
							if (role) {
								if (!mUpdateRoleSet.contains(role)) {
									mUpdateRoleSet.insert(role);
									mUpdateRoleList.append(role);
								}
							}
						}
						//CIndividualTermExpression* indiExp = dynamic_cast<CIndividualTermExpression*>(entity);
						//if (indiExp) {
						//	CIndividual* individual = mIndiTermIndiHash->value(indiExp);
						//	if (individual) {
						//		if (!mUpdateIndividualSet.contains(individual)) {
						//			mUpdateIndividualSet.insert(individual);
						//			mUpdateIndividualList.append(individual);
						//		}
						//	}
						//}

					}

					// consider all Abox individual, not only those which are referenced by definitions
					// if an individual is removed from the KB, then there are no asserted concepts
					cint64 indiCount = abox->getIndividualCount();
					CIndividualVector* indiVec = abox->getIndividualVector(false);
					if (indiVec) {
						for (cint64 indiID = 0; indiID < indiCount; ++indiID) {
							CIndividual* individual = indiVec->getData(indiID);
							if (individual) {
								for (CConceptAssertionLinker* conAssIt = individual->getAssertionConceptLinker(); conAssIt; conAssIt = conAssIt->getNext()) {
									CConcept* concept = conAssIt->getData();
									if (!mUpdateConceptSet.contains(concept)) {
										mUpdateConceptSet.insert(concept);
										mUpdateConceptList.append(concept);
									}
								}
							}
						}
					}

					FOREACHIT (CClassTermExpression* classTermExp, mClassTermClassAxiomHash->keys()) {
						if (mClassTermClassAxiomHash->value(classTermExp) != nullptr) {
							CConcept* concept = mClassTermConceptHash->value(classTermExp,nullptr);
							if (concept) {
								if (!mUpdateConceptSet.contains(concept)) {
									mUpdateConceptSet.insert(concept);
									mUpdateConceptList.append(concept);
								}
							}
						}
					}
					FOREACHIT (CObjectPropertyTermExpression* objPropTermExp, mObjPropTermObjPropAxiomHash->keys()) {
						if (mObjPropTermObjPropAxiomHash->value(objPropTermExp) != nullptr) {
							CRole* role = mObjPropTermRoleHash->value(objPropTermExp,nullptr);
							if (role) {
								if (!mUpdateRoleSet.contains(role)) {
									mUpdateRoleSet.insert(role);
									mUpdateRoleList.append(role);
								}
							}
						}
					}

					while (!mUpdateConceptList.isEmpty() || !mUpdateRoleList.isEmpty() || !mUpdateIndividualList.isEmpty()) {
						while (!mUpdateConceptList.isEmpty()) {
							CConcept* concept = mUpdateConceptList.takeFirst();
							CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
							while (opLinkerIt) {
								CConcept* opCon = opLinkerIt->getData();
								if (!mUpdateConceptSet.contains(opCon)) {
									mUpdateConceptSet.insert(opCon);
									mUpdateConceptList.append(opCon);
								}
								opLinkerIt = opLinkerIt->getNext();
							}
							CRole* role = concept->getRole();
							if (role && !mUpdateRoleSet.contains(role)) {
								mUpdateRoleSet.insert(role);
								mUpdateRoleList.append(role);
							}
							//CIndividual* individual = concept->getNominalIndividual();
							//if (individual && !mUpdateIndividualSet.contains(individual)) {
							//	mUpdateIndividualSet.insert(individual);
							//	mUpdateIndividualList.append(individual);
							//}
						}



						//while (!mUpdateIndividualList.isEmpty()) {
						//	CIndividual* individual = mUpdateIndividualList.takeFirst();

						//	for (CConceptAssertionLinker* conAssIt = individual->getAssertionConceptLinker(); conAssIt; conAssIt = conAssIt->getNext()) {
						//		CConcept* concept = conAssIt->getData();
						//		if (!mUpdateConceptSet.contains(concept)) {
						//			mUpdateConceptSet.insert(concept);
						//			mUpdateConceptList.append(concept);
						//		}
						//	}
						//	for (CRoleAssertionLinker* roleAssIt = individual->getAssertionRoleLinker(); roleAssIt; roleAssIt = roleAssIt->getNext()) {
						//		CRole* role = roleAssIt->getRole();
						//		if (!mUpdateRoleSet.contains(role)) {
						//			mUpdateRoleSet.insert(role);
						//			mUpdateRoleList.append(role);
						//		}
						//		CIndividual* individual2 = roleAssIt->getIndividual();
						//		if (!mUpdateIndividualSet.contains(individual2)) {
						//			mUpdateIndividualSet.insert(individual2);
						//			mUpdateIndividualList.append(individual2);
						//		}
						//	}
						//}


						while (!mUpdateRoleList.isEmpty()) {
							CRole* role = mUpdateRoleList.takeFirst();
							CSortedNegLinker<CRole*>* roleLinkerIt = role->getIndirectSuperRoleList();
							while (roleLinkerIt) {
								CRole* superRole = roleLinkerIt->getData();
								if (!mUpdateRoleSet.contains(superRole)) {
									mUpdateRoleSet.insert(superRole);
									mUpdateRoleList.append(superRole);
								}
								roleLinkerIt = roleLinkerIt->getNext();
							}
							CSortedNegLinker<CRole*>* disRoleLinkerIt = role->getDisjointRoleList();
							while (disRoleLinkerIt) {
								CRole* disRole = disRoleLinkerIt->getData();
								if (!mUpdateRoleSet.contains(disRole)) {
									mUpdateRoleSet.insert(disRole);
									mUpdateRoleList.append(disRole);
								}
								disRoleLinkerIt = disRoleLinkerIt->getNext();
							}
							CSortedNegLinker<CConcept*>* domainConLinkerIt = role->getDomainConceptList();
							while (domainConLinkerIt) {
								CConcept* opCon = domainConLinkerIt->getData();
								if (!mUpdateConceptSet.contains(opCon)) {
									mUpdateConceptSet.insert(opCon);
									mUpdateConceptList.append(opCon);
								}
								domainConLinkerIt = domainConLinkerIt->getNext();
							}
							CSortedNegLinker<CConcept*>* rangeConLinkerIt = role->getRangeConceptList();
							while (rangeConLinkerIt) {
								CConcept* opCon = rangeConLinkerIt->getData();
								if (!mUpdateConceptSet.contains(opCon)) {
									mUpdateConceptSet.insert(opCon);
									mUpdateConceptList.append(opCon);
								}
								rangeConLinkerIt = rangeConLinkerIt->getNext();
							}
						}
					}
				}

				CBOXSET<CConcept*>* activeClassConceptSet = tbox->getActiveClassConceptSet();
				activeClassConceptSet->clear();
				FOREACHIT (CConcept* concept, mUpdateConceptSet) {
					if (concept->hasClassName()) {
						activeClassConceptSet->insert(concept);
					}
				}

				return true;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
