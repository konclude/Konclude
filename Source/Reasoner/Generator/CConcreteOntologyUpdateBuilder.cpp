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

#include "CConcreteOntologyUpdateBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyUpdateBuilder::CConcreteOntologyUpdateBuilder(CConcreteOntology* updateConcreteOntology) : CConcreteOntologyBuildDataUpdater(updateConcreteOntology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("ConcretOntologyUpdateBuilder initialized."),this);
			}

			CConcreteOntologyUpdateBuilder::~CConcreteOntologyUpdateBuilder() {
			}





			bool CConcreteOntologyUpdateBuilder::initializeBuilding() {
				CConcreteOntologyBuildDataUpdater::initializeBuilding();



				mTellAxiomSet = mOntoBuild->getTellAxiomSet();
				mRetractAxiomSet = mOntoBuild->getRetractAxiomSet();

				mTellUpdatedAxiomSet = mOntoBuild->getUpdatedTellAxiomSet();
				mRetractUpdatedAxiomSet = mOntoBuild->getUpdatedRetractAxiomSet();

				mChangeAxiomList = mOntoBuild->getChangeAxiomList();
				mChangeUpdatedAxiomList = mOntoBuild->getUpdatedChangeAxiomList();



				mOntoData = mOnto->getDataBoxes();
				tBox = mOntoData->getTBox();
				rBox = mOntoData->getRBox();
				aBox = mOntoData->getABox();

				mInitialBuild = mOntoData->isInitialBuild();

				mConstructFlags = mOntoData->getExpressionDataBoxMapping()->getBuildConstructFlags();

				mUpdateClassAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdatedClassAxiomTellOrRetractHash();
				mUpdateObjectPropertyAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdateObjectPropertyAxiomHash();
				mUpdateAssertionAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdateAssertionAxiomHash();

				mClassTermClassAxiomSet = mOntoData->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionSet();
				mClassTermClassAxiomHash = mOntoData->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();


				mObjPropTermObjPropAxiomSet = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomSet();
				mObjPropTermObjPropAxiomHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomHash();

				mIndiTermAssertionAxiomSet = mOntoData->getExpressionDataBoxMapping()->getIndividualTermAssertionAxiomSet();
				mIndiTermAssertionAxiomHash = mOntoData->getExpressionDataBoxMapping()->getIndividualTermAssertionAxiomHash();

				mTopRebuildClassTermExpressionsSet = mOntoData->getExpressionDataBoxMapping()->getTopRebuildClassTermExpressionsSet();
				mTopRebuildObjectPropertyTermExpressionsSet = mOntoData->getExpressionDataBoxMapping()->getTopRebuildObjectPropertyTermExpressionsSet();


				mClassTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
				mConceptClassTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();

				mObjPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
				mRoleObjPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();

				mIndividulTermIndiHash = mOntoData->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash();
				mIndiIndividulTermHash = mOntoData->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();


				mRoleChainObjPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleChainObjectPropertyTermHash();
				mObjPropTermRoleChainHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermRoleChainHash();

				mLastProcessedChangedAxiom = mOntoData->getExpressionDataBoxMapping()->getLastProcessedChangedAxiom();
				mLastProcessedBuildIndividual = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildIndividual();
				mLastProcessedBuildConcept = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildConcept();
				mLastProcessedBuildRole = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildRole();

				mLastProcessedExpression = mOntoData->getExpressionDataBoxMapping()->getLastProcessedExpression();
				mLastProcessedInverseProperty = mOntoData->getExpressionDataBoxMapping()->getLastProcessedInverseProperty();

				mLastBuildedIndividual = mOntoData->getExpressionDataBoxMapping()->getLastBuildedIndividual();
				mLastBuildedConcept = mOntoData->getExpressionDataBoxMapping()->getLastBuildedConcept();
				mLastBuildedRole = mOntoData->getExpressionDataBoxMapping()->getLastBuildedRole();

				mLocExpressionBuildHash = mOntoData->getExpressionDataBoxMapping()->getStructuralExpressionBuildHash();
				mLocExpressionBuildContainerList = mOntoData->getExpressionDataBoxMapping()->getExpressionBuildListContainer();
				mLocInverseObjectPropertyHash = mOntoData->getExpressionDataBoxMapping()->getInverseObjectPropertyHash();

				mLocBuildConceptSet = mOntoData->getExpressionDataBoxMapping()->getBuildedConceptSet();
				mLocBuildRoleSet = mOntoData->getExpressionDataBoxMapping()->getBuildedRoleSet();
				mLocBuildIndividualSet = mOntoData->getExpressionDataBoxMapping()->getBuildedIndividualSet();

				mLocBuildConceptList = mOntoData->getExpressionDataBoxMapping()->getBuildedConceptList();
				mLocBuildRoleList = mOntoData->getExpressionDataBoxMapping()->getBuildedRoleList();
				mLocBuildIndividualList = mOntoData->getExpressionDataBoxMapping()->getBuildedIndividualList();

				mRebuildTopConcept = false;

				mClassGCIExpressionSet.clear();
				mLocalisationSet.clear();

				mBuildingConceptSet.clear();
				mNewBuildedConceptSet.clear();

				mNewBuildedRoleSet.clear();
				mBuildingRoleSet.clear();

				mNewBuildedIndividualSet.clear();
				mBuildingIndividualSet.clear();

				mTaggingConceptList.clear();
				mInstallConceptList.clear();
				mTaggingRoleSet.clear();
				mInstallRoleList.clear();
				mTaggingRoleChainSet.clear();
				mInstallRoleChainList.clear();
				mTaggingIndividualSet.clear();
				mInstallIndividualList.clear();

				mNewBuildConceptSet.clear();
				mNewBuildRoleSet.clear();
				mNewBuildIndividualSet.clear();

				return true;
			}



			bool CConcreteOntologyUpdateBuilder::completeBuilding() {

				mOntoBuild->setNextAxiomNumber(mNextAxiomNumber+mNextMaxAxiomNumberOffset);


				CONTOLOGYAXIOMSET<CAxiomExpression*> newTellUpdatedAxiomSet;
				CONTOLOGYAXIOMSET<CAxiomExpression*> newRetractUpdatedAxiomSet;

				FOREACHIT (CAxiomExpression* axiomExp, *mRetractUpdatedAxiomSet) {
					mConstructFlags->setRetractionUsed();
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
				}
				FOREACHIT (CAxiomExpression* axiomExp, *mTellUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
				}

				if (mLastProcessedChangedAxiom <= 0) {
					mLastProcessedChangedAxiom = mChangeAxiomList->size();
					newTellUpdatedAxiomSet = *mTellAxiomSet;
				} else {
					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> > newChangeUpdatedAxiomList;

					cint64 changeAxiomCount = mChangeAxiomList->size();
					while (mLastProcessedChangedAxiom < changeAxiomCount) {
						newChangeUpdatedAxiomList.append((*mChangeAxiomList)[mLastProcessedChangedAxiom++]);
					}				

					for (CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >::const_iterator it = newChangeUpdatedAxiomList.constBegin(), itEnd = newChangeUpdatedAxiomList.constEnd(); it != itEnd; ++it) {
						QPair<CAxiomExpression*,bool> axiomAdditionPair(*it);
						CAxiomExpression* axiomExp = axiomAdditionPair.first;
						bool axiomAdded = axiomAdditionPair.second;
						if (axiomAdded) {
							if (mTellAxiomSet->contains(axiomExp)) {
								newTellUpdatedAxiomSet.insert(axiomExp);
							}
						} else {
							if (!mTellAxiomSet->contains(axiomExp)) {
								newRetractUpdatedAxiomSet.insert(axiomExp);
							}
						}

					}
				}

				if (mLastProcessedInverseProperty <= 0) {
					*mLocInverseObjectPropertyHash = *mInverseObjectPropertyHash;
					mLastProcessedInverseProperty = mInverseObjectPropertyList->count();
				} else {
					while (mLastProcessedInverseProperty < mInverseObjectPropertyList->count()) {
						const QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> obPropPair((*mInverseObjectPropertyList)[mLastProcessedInverseProperty++]);
						mLocInverseObjectPropertyHash->insert(obPropPair.first,obPropPair.second);
						mLocInverseObjectPropertyHash->insert(obPropPair.second,obPropPair.first);
					}
				}
				mInverseObjectPropertyHash = mLocInverseObjectPropertyHash;

				if (mLastProcessedExpression <= 0) {
					*mLocExpressionBuildHash = *mExpressionBuildHash;
					mLastProcessedExpression = mExpressionBuildContainerList->count();
				} else {
					while (mLastProcessedExpression < mExpressionBuildContainerList->count()) {
						CBuildExpression* buildExp((*mExpressionBuildContainerList)[mLastProcessedExpression++]);
						mLocExpressionBuildHash->insert(CExpressionHasher(buildExp),buildExp);
					}
				}
				mLocExpressionBuildHash = mExpressionBuildHash;
				mExpressionBuildContainerList = mLocExpressionBuildContainerList;

				if (mLastProcessedBuildIndividual <= 0) {
					*mLocBuildIndividualSet = *mBuildIndividualSet;
					*mLocBuildIndividualList = *mBuildIndividualList;
					mLastProcessedBuildIndividual = mLocBuildIndividualList->count();
				} else {
					while (mLastProcessedBuildIndividual < mBuildIndividualList->count()) {
						CIndividualTermExpression* buildExp((*mBuildIndividualList)[mLastProcessedBuildIndividual++]);
						mLocBuildIndividualList->append(buildExp);
						mLocBuildIndividualSet->insert(buildExp);
					}
				}
				mBuildIndividualSet = mLocBuildIndividualSet;
				mBuildIndividualList = mLocBuildIndividualList;

				if (mLastProcessedBuildConcept <= 0) {
					*mLocBuildConceptSet = *mBuildConceptSet;
					*mLocBuildConceptList = *mBuildConceptList;
					mLastProcessedBuildConcept = mLocBuildConceptList->count();
				} else {
					while (mLastProcessedBuildConcept < mBuildConceptList->count()) {
						CClassTermExpression* buildExp((*mBuildConceptList)[mLastProcessedBuildConcept++]);
						mLocBuildConceptList->append(buildExp);
						mLocBuildConceptSet->insert(buildExp);
					}
				}
				mInitialBuildConceptSet = mBuildConceptSet;
				mBuildConceptSet = mLocBuildConceptSet;
				mBuildConceptList = mLocBuildConceptList;

				if (mLastProcessedBuildRole <= 0) {
					*mLocBuildRoleSet = *mBuildRoleSet;
					*mLocBuildRoleList = *mBuildRoleList;
					mLastProcessedBuildRole = mLocBuildRoleList->count();
				} else {
					while (mLastProcessedBuildRole < mBuildRoleList->count()) {
						CObjectPropertyTermExpression* buildExp((*mBuildRoleList)[mLastProcessedBuildRole++]);
						mLocBuildRoleList->append(buildExp);
						mLocBuildRoleSet->insert(buildExp);
					}
				}
				mBuildRoleSet = mLocBuildRoleSet;
				mBuildRoleList = mLocBuildRoleList;


				FOREACHIT (CAxiomExpression* axiomExp, newTellUpdatedAxiomSet) {
					CClassAxiomExpression* classAxiomExp = dynamic_cast<CClassAxiomExpression*>(axiomExp);
					if (classAxiomExp) {
						mUpdateClassAxiomHash->insert(classAxiomExp,true);
					}
					CObjectPropertyAxiomExpression* objPropAxiomExp = dynamic_cast<CObjectPropertyAxiomExpression*>(axiomExp);
					if (objPropAxiomExp) {
						mUpdateObjectPropertyAxiomHash->insert(objPropAxiomExp,true);
					}
					CDeclarationAxiomExpression* declAxiomExp = dynamic_cast<CDeclarationAxiomExpression*>(axiomExp);
					if (declAxiomExp) {
						mDeclarationAxiomSet->insert(declAxiomExp);
					}
					CAssertionAxiomExpression* assAxiomExp = dynamic_cast<CAssertionAxiomExpression*>(axiomExp);
					if (assAxiomExp) {
						mUpdateAssertionAxiomHash->insert(assAxiomExp,true);
					}
				}
				FOREACHIT (CAxiomExpression* axiomExp, newRetractUpdatedAxiomSet) {
					CClassAxiomExpression* classAxiomExp = dynamic_cast<CClassAxiomExpression*>(axiomExp);
					if (classAxiomExp) {
						mUpdateClassAxiomHash->insert(classAxiomExp,false);
					} 
					CObjectPropertyAxiomExpression* objPropAxiomExp = dynamic_cast<CObjectPropertyAxiomExpression*>(axiomExp);
					if (objPropAxiomExp) {
						mUpdateObjectPropertyAxiomHash->insert(objPropAxiomExp,false);
					}
					CDeclarationAxiomExpression* declAxiomExp = dynamic_cast<CDeclarationAxiomExpression*>(axiomExp);
					if (declAxiomExp) {
						mDeclarationAxiomSet->remove(declAxiomExp);
					}
					CAssertionAxiomExpression* assAxiomExp = dynamic_cast<CAssertionAxiomExpression*>(axiomExp);
					if (assAxiomExp) {
						mUpdateAssertionAxiomHash->insert(assAxiomExp,false);
					}
				}




				FOREACHHASHIT (CAssertionAxiomExpression* assAxiomExp, bool tellAxiom, *mUpdateAssertionAxiomHash) {
					CBuildExpression::ExpressionType expType = assAxiomExp->getType();
					if (expType == CBuildExpression::BETCLASSASSERTION) {
						CClassAssertionExpression* classAssExp = (CClassAssertionExpression*)assAxiomExp;
						CIndividualTermExpression* indiTermExp = classAssExp->getIndividualTermExpression();
						mNewBuildIndividualSet.insert(indiTermExp);
						if (!mIndiTermAssertionAxiomSet->contains(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp))) {
							mIndiTermAssertionAxiomSet->insert(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp));
							if (tellAxiom) {
								mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETOBJECTPROPERTYASSERTION) {
						CObjectPropertyAssertionExpression* propAssExp = (CObjectPropertyAssertionExpression*)assAxiomExp;
						CIndividualTermExpression* indiTermExp = propAssExp->getFirstIndividualTermExpression();
						mNewBuildIndividualSet.insert(indiTermExp);
						if (!mIndiTermAssertionAxiomSet->contains(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp))) {
							mIndiTermAssertionAxiomSet->insert(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp));
							if (tellAxiom) {
								mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETNEGATIVEOBJECTPROPERTYASSERTION) {
						mConstructFlags->setNonELConstructUsed();
						CNegativeObjectPropertyAssertionExpression* propAssExp = (CNegativeObjectPropertyAssertionExpression*)assAxiomExp;
						CIndividualTermExpression* indiTermExp = propAssExp->getFirstIndividualTermExpression();
						mNewBuildIndividualSet.insert(indiTermExp);
						if (!mIndiTermAssertionAxiomSet->contains(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp))) {
							mIndiTermAssertionAxiomSet->insert(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp));
							if (tellAxiom) {
								mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETSAMEINDIVIDUAL) {
						mConstructFlags->setNonELConstructUsed();
						CSameIndividualExpression* sameIndiAssExp = (CSameIndividualExpression*)assAxiomExp;
						FOREACHIT (CIndividualTermExpression* indiTermExp, *sameIndiAssExp->getIndividualTermExpressionList()) {
							mNewBuildIndividualSet.insert(indiTermExp);
							if (!mIndiTermAssertionAxiomSet->contains(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp))) {
								mIndiTermAssertionAxiomSet->insert(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp));
								if (tellAxiom) {
									mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETDIFFERENTINDIVIDUAL) {
						mConstructFlags->setNonELConstructUsed();
						CDifferentIndividualsExpression* diffIndiAssExp = (CDifferentIndividualsExpression*)assAxiomExp;
						FOREACHIT (CIndividualTermExpression* indiTermExp, *diffIndiAssExp->getIndividualTermExpressionList()) {
							mNewBuildIndividualSet.insert(indiTermExp);
							if (!mIndiTermAssertionAxiomSet->contains(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp))) {
								mIndiTermAssertionAxiomSet->insert(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp));
								if (tellAxiom) {
									mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiomExp);
								}
							}
						}
					}
				}



				FOREACHHASHIT (CClassAxiomExpression* classAxiomExp, bool tellAxiom, *mUpdateClassAxiomHash) {
					CBuildExpression::ExpressionType expType = classAxiomExp->getType();
					if (expType == CBuildExpression::BETEQUIVALENTCLASSES) { 
						// all associated classes has to be updated
						CEquivalentClassesExpression* eqClassExp = (CEquivalentClassesExpression*)classAxiomExp;
						FOREACHIT (CClassTermExpression* classTermExp, *eqClassExp->getClassTermExpressionList()) {
							mNewBuildConceptSet.insert(classTermExp);
							if (!mClassTermClassAxiomSet->contains(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp))) {
								mClassTermClassAxiomSet->insert(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp));
								if (tellAxiom) {
									mClassTermClassAxiomHash->insertMulti(classTermExp,classAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETESUBCLASSOF) {
						// only sub class expression has to be rebuild
						CSubClassOfExpression* subClassOfExp = (CSubClassOfExpression*)classAxiomExp;
						CClassTermExpression* classTermExp = subClassOfExp->getFirstClassTermExpression();
						mNewBuildConceptSet.insert(classTermExp);
						if (!mClassTermClassAxiomSet->contains(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp))) {
							mClassTermClassAxiomSet->insert(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp));
							if (tellAxiom) {
								mClassTermClassAxiomHash->insertMulti(classTermExp,classAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETDISJOINTCLASSES) {
						// all associated classes has to be updated
						CDisjointClassesExpression* disClassExp = (CDisjointClassesExpression*)classAxiomExp;
						FOREACHIT (CClassTermExpression* classTermExp, *disClassExp->getClassTermExpressionList()) {
							if (classTermExp->getType() != CBuildExpression::BETCLASS) {
								mConstructFlags->setNonELConstructUsed();
							}
							mNewBuildConceptSet.insert(classTermExp);
							if (!mClassTermClassAxiomSet->contains(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp))) {
								mClassTermClassAxiomSet->insert(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp));
								if (tellAxiom) {
									mClassTermClassAxiomHash->insertMulti(classTermExp,classAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETDISJOINTUNION) {
						mConstructFlags->setNonELConstructUsed();
						// all associated classes has to be updated
						CDisjointUnionExpression* disClassExp = (CDisjointUnionExpression*)classAxiomExp;
						FOREACHIT (CClassTermExpression* classTermExp, *disClassExp->getClassTermExpressionList()) {
							mNewBuildConceptSet.insert(classTermExp);
							if (!mClassTermClassAxiomSet->contains(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp))) {
								mClassTermClassAxiomSet->insert(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp));
								if (tellAxiom) {
									mClassTermClassAxiomHash->insertMulti(classTermExp,classAxiomExp);
								}
							}
						}
						CClassTermExpression* classTermExp = disClassExp->getClassExpression();
						mNewBuildConceptSet.insert(classTermExp);
						if (!mClassTermClassAxiomSet->contains(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp))) {
							mClassTermClassAxiomSet->insert(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiomExp));
							if (tellAxiom) {
								mClassTermClassAxiomHash->insertMulti(classTermExp,classAxiomExp);
							}
						}
					}
				}


				FOREACHHASHIT (CObjectPropertyAxiomExpression* objPropAxiomExp, bool tellAxiom, *mUpdateObjectPropertyAxiomHash) {
					CBuildExpression::ExpressionType expType = objPropAxiomExp->getType();
					if (expType == CBuildExpression::BETEQUIVALENTOBJECTPROPERTIES) {
						// all associated properties has to be updated
						CEquivalentObjectPropertiesExpression* eqObjPropExp = (CEquivalentObjectPropertiesExpression*)objPropAxiomExp;
						FOREACHIT (CObjectPropertyTermExpression* objPropTermExp, *eqObjPropExp->getObjectPropertyTermExpressionList()) {
							mNewBuildRoleSet.insert(objPropTermExp);
							if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
								mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
								if (tellAxiom) {
									mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETDISJOINTOBJECTPROPERTIES) {
						mConstructFlags->setNonELConstructUsed();
						// all associated properties has to be updated
						CDisjointObjectPropertiesExpression* eqObjPropExp = (CDisjointObjectPropertiesExpression*)objPropAxiomExp;
						FOREACHIT (CObjectPropertyTermExpression* objPropTermExp, *eqObjPropExp->getObjectPropertyTermExpressionList()) {
							mNewBuildRoleSet.insert(objPropTermExp);
							if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
								mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
								if (tellAxiom) {
									mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETSUBOBJECTPROPERTYOF) {
						CSubObjectPropertyOfExpression* subObjPropExp = (CSubObjectPropertyOfExpression*)objPropAxiomExp;
						FOREACHIT (CObjectPropertyTermExpression* objPropTermExp, *subObjPropExp->getSubObjectPropertyTermExpressionList()) {
							mNewBuildRoleSet.insert(objPropTermExp);
							if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
								mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
								if (tellAxiom) {
									mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
								}
							}
						}
						if (subObjPropExp->getSubObjectPropertyTermExpressionList()->count() >= 2) {
							mConstructFlags->setComplexRoleUsed();
							CObjectPropertyTermExpression* objPropTermExp = subObjPropExp->getSuperObjectPropertyTermExpression();
							mNewBuildRoleSet.insert(objPropTermExp);
							if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
								mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
								if (tellAxiom) {
									mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETINVERSEOBJECTPROPERTIES) {
						mConstructFlags->setNonELConstructUsed();
						// all associated properties has to be updated
						CInverseObjectPropertiesExpression* invObjPropExp = (CInverseObjectPropertiesExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp1 = invObjPropExp->getFirstObjectPropertyTermExpression();
						CObjectPropertyTermExpression* objPropTermExp2 = invObjPropExp->getSecondObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp1);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp1,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp1,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp1,objPropAxiomExp);
							}
						}
						mNewBuildRoleSet.insert(objPropTermExp2);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp2,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp2,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp2,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETFUNCTIONALPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CFunctionalObjectPropertyExpression* funcObjPropExp = (CFunctionalObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = funcObjPropExp->getFunctionalObjectPropertyExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETINVERSEFUNCTIONALPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CInverseFunctionalObjectPropertyExpression* funcObjPropExp = (CInverseFunctionalObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = funcObjPropExp->getInverseFunctionalObjectPropertyExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETSYMMETRICPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CSymmetricObjectPropertyExpression* objPropExpAxiom = (CSymmetricObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETASYMMETRICPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CAsymmetricObjectPropertyExpression* objPropExpAxiom = (CAsymmetricObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETREFLEXIVEPROPERTY) {
						CReflexiveObjectPropertyExpression* objPropExpAxiom = (CReflexiveObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETIRREFLEXIVEPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CIrreflexiveObjectPropertyExpression* objPropExpAxiom = (CIrreflexiveObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETTRANSITIVEOBJECTPROPERTY) {
						mConstructFlags->setComplexRoleUsed();
						CTransetiveObjectPropertyExpression* objPropExpAxiom = (CTransetiveObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getTransetivePropertyExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETOBJECTPROPERTYDOMAIN) {
						CObjectPropertyDomainExpression* objPropExpAxiom = (CObjectPropertyDomainExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETOBJECTPROPERTYRANGE) {
						CObjectPropertyRangeExpression* objPropExpAxiom = (CObjectPropertyRangeExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					}
				}




				while (mLastBuildedConcept < mBuildConceptList->size() || mLastBuildedRole < mBuildRoleList->size() || mLastBuildedIndividual < mBuildIndividualList->size()
						|| !mNewBuildConceptSet.isEmpty() || !mNewBuildRoleSet.isEmpty() || !mNewBuildIndividualSet.isEmpty()) {


					while (mLastBuildedConcept < mBuildConceptList->size()) {
						mNewBuildConceptSet.insert((*mBuildConceptList)[mLastBuildedConcept++]);
					}
					while (mLastBuildedRole < mBuildRoleList->size()) {
						mNewBuildRoleSet.insert((*mBuildRoleList)[mLastBuildedRole++]);
					}
					while (mLastBuildedIndividual < mBuildIndividualList->size()) {
						mNewBuildIndividualSet.insert((*mBuildIndividualList)[mLastBuildedIndividual++]);
					}


					// construct concepts, roles, individuals
					// build all class expressions

					while (!mNewBuildRoleSet.isEmpty()) {
						mBuildingRoleSet = mNewBuildRoleSet;
						mNewBuildRoleSet.clear();

						FOREACHIT (CObjectPropertyTermExpression* objTermExp, mBuildingRoleSet) {
							if (!mNewBuildedRoleSet.contains(objTermExp)) {
								mNewBuildedRoleSet.insert(objTermExp);

								// build role axioms
								buildObjectPropertyRole(objTermExp);
							}
						}
					}




					while (!mNewBuildIndividualSet.isEmpty()) {
						mBuildingIndividualSet = mNewBuildIndividualSet;
						mNewBuildIndividualSet.clear();

						FOREACHIT (CIndividualTermExpression* indiTermExp, mBuildingIndividualSet) {
							if (!mNewBuildedIndividualSet.contains(indiTermExp)) {
								mNewBuildedIndividualSet.insert(indiTermExp);

								buildIndividualIndi(indiTermExp);

							}
						}

					}



					// construct concepts, roles, individuals
					// build all class expressions

					while (!mNewBuildConceptSet.isEmpty()) {
						mBuildingConceptSet = mNewBuildConceptSet;
						mNewBuildConceptSet.clear();

						FOREACHIT (CClassTermExpression* classTermExp, mBuildingConceptSet) {
							if (!mNewBuildedConceptSet.contains(classTermExp)) {
								mNewBuildedConceptSet.insert(classTermExp);
								CBuildExpression::ExpressionType expType = classTermExp->getType();
								CConcept* concept = getConceptForClassTerm(classTermExp,true);
								if (expType == CBuildExpression::BETOBJECTALLVALUEFROM) {	
									mConstructFlags->setNonELConstructUsed();
									CObjectAllValuesFromExpression* objExp = (CObjectAllValuesFromExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									CClassTermExpression* opClassExp = objExp->getClassTermExpression();
									setConceptOperandsFromClassTerms(concept,opClassExp);
									concept->setOperatorCode(CCALL);
								} else if (expType == CBuildExpression::BETOBJECTSOMEVALUEFROM) {
									CObjectSomeValuesFromExpression* objExp = (CObjectSomeValuesFromExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									CClassTermExpression* opClassExp = objExp->getClassTermExpression();
									if (opClassExp == nullptr) {
										opClassExp = mTopClassExpression;
									}
									setConceptOperandsFromClassTerms(concept,opClassExp);
									concept->setOperatorCode(CCSOME);
								} else if (expType == CBuildExpression::BETOBJECTMINCARDINALITY) {
									CObjectMinCardinalityExpression* objExp = (CObjectMinCardinalityExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									cint64 cardinality = objExp->getCardinality();
									CClassTermExpression* opClassExp = objExp->getClassTermExpression();
									if (cardinality == 1) {
										if (opClassExp == nullptr) {
											opClassExp = mTopClassExpression;
										}
										setConceptOperandsFromClassTerms(concept,opClassExp);
										concept->setOperatorCode(CCSOME);
									} else {
										if (opClassExp != mTopClassExpression) {
											setConceptOperandsFromClassTerms(concept,opClassExp);
										}
										mConstructFlags->setNonELConstructUsed();
										concept->setParameter(cardinality);
										concept->setOperatorCode(CCATLEAST);
									}
								} else if (expType == CBuildExpression::BETOBJECTMAXCARDINALITY) {
									mConstructFlags->setNonELConstructUsed();
									CObjectMaxCardinalityExpression* objExp = (CObjectMaxCardinalityExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									CClassTermExpression* opClassExp = objExp->getClassTermExpression();
									cint64 cardinality = objExp->getCardinality();
									if (cardinality == 0) {
										if (opClassExp == nullptr) {
											opClassExp = mTopClassExpression;
										}
										CObjectComplementOfExpression* compExp = getObjectComplementOf(opClassExp);
										setConceptOperandsFromClassTerms(concept,compExp);
										concept->setOperatorCode(CCALL);
									} else {
										if (opClassExp != mTopClassExpression) {
											setConceptOperandsFromClassTerms(concept,opClassExp);
										}
										concept->setParameter(cardinality);
										concept->setOperatorCode(CCATMOST);
									}
								} else if (expType == CBuildExpression::BETOBJECTEXACTLYCARDINALITY) {
									mConstructFlags->setNonELConstructUsed();
									CObjectExactlyCardinalityExpression* objExp = (CObjectExactlyCardinalityExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									CClassTermExpression* opClassExp = objExp->getClassTermExpression();
									cint64 cardinality = objExp->getCardinality();
									CObjectMaxCardinalityExpression* maxCardExp = getObjectMaxCardinality(opObjPropExp,opClassExp,cardinality);
									CObjectMinCardinalityExpression* minCardExp = getObjectMinCardinality(opObjPropExp,opClassExp,cardinality);
									CObjectIntersectionOfExpression* maxMinIntersect = getObjectIntersectionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<maxCardExp<<minCardExp);
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									setConceptOperandsFromClassTerms(concept,maxMinIntersect);
									concept->setParameter(cardinality);
									concept->setOperatorCode(CCAND);
								} else if (expType == CBuildExpression::BETOBJECTCOMPLEMENTOF) {
									CObjectComplementOfExpression* objExp = (CObjectComplementOfExpression*)classTermExp;
									if (!mConstructFlags->isNonELConstructUsed()) {
										if (!mELExceptionExpressionSet.contains(objExp)) {
											mConstructFlags->setNonELConstructUsed();
										}
									}
									CClassTermExpression* opClassExp = objExp->getClassTermExpression();
									setConceptOperandsFromClassTerms(concept,opClassExp);
									concept->setOperatorCode(CCNOT);
								} else if (expType == CBuildExpression::BETOBJECTUNION) {
									CObjectUnionOfExpression* objExp = (CObjectUnionOfExpression*)classTermExp;
									if (!mConstructFlags->isNonELConstructUsed()) {
										if (!mELExceptionExpressionSet.contains(objExp)) {
											mConstructFlags->setNonELConstructUsed();
										}
									}
									CEXPRESSIONLIST<CClassTermExpression*>* opClassExpList = objExp->getClassTermExpressionList();
									setConceptOperandsFromClassTerms(concept,opClassExpList);
									concept->setOperatorCode(CCOR);
								} else if (expType == CBuildExpression::BETOBJECTINTERSECTION) {
									CObjectIntersectionOfExpression* objExp = (CObjectIntersectionOfExpression*)classTermExp;
									CEXPRESSIONLIST<CClassTermExpression*>* opClassExpList = objExp->getClassTermExpressionList();
									setConceptOperandsFromClassTerms(concept,opClassExpList);
									concept->setOperatorCode(CCAND);
								} else if (expType == CBuildExpression::BETOBJECTHASVALUE) {
									mConstructFlags->setNonELConstructUsed();
									CObjectHasValueExpression* objExp = (CObjectHasValueExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									CIndividualTermExpression* indiExp = objExp->getIndividualTermExpression();
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									setConceptIndividualFromIndividualTerm(concept,indiExp);
									concept->setOperatorCode(CCVALUE);
								} else if (expType == CBuildExpression::BETOBJECTHASSELF) {
									mConstructFlags->setNonELConstructUsed();
									CObjectHasSelfExpression* objExp = (CObjectHasSelfExpression*)classTermExp;
									CObjectPropertyTermExpression* opObjPropExp = objExp->getObjectPropertyTermExpression();
									setConceptRoleFromObjectPropertyTerm(concept,opObjPropExp);
									concept->setOperatorCode(CCSELF);
								} else if (expType == CBuildExpression::BETINDIVIDUALVARIABLE) {
									mConstructFlags->setNonELConstructUsed();
									mConstructFlags->setNominalSchemaUsed();
									CObjectIndividualVariableExpression* objExp = (CObjectIndividualVariableExpression*)classTermExp;
									cint64 indiVarID = getIndividualVariableID(concept,objExp);
									concept->setParameter(indiVarID);
									concept->setOperatorCode(CCNOMVAR);
								} else if (expType == CBuildExpression::BETOBJECTONEOF) {
									mConstructFlags->setNonELConstructUsed();
									CObjectOneOfExpression* objExp = (CObjectOneOfExpression*)classTermExp;
									CEXPRESSIONLIST<CIndividualTermExpression*>* indiExpList = objExp->getIndividualTermExpressionList();
									if (indiExpList->count() == 1) {
										CIndividualTermExpression* indiTermExp = indiExpList->first();
										setConceptIndividualFromIndividualTerm(concept,indiTermExp);
										concept->setOperatorCode(CCNOMINAL);
									} else {
										CEXPRESSIONLIST<CClassTermExpression*> nominalClassTermList;
										FOREACHIT (CIndividualTermExpression* indiTermExp, *indiExpList) {
											nominalClassTermList.append(getObjectOneOf(CEXPRESSIONLIST<CIndividualTermExpression*>()<<indiTermExp));
										}
										CObjectUnionOfExpression* objUnionExp = getObjectUnionOf(nominalClassTermList);
										setConceptOperandsFromClassTerms(concept,objUnionExp);
										concept->setOperatorCode(CCAND);
									}
								}


								// build class
								buildClassConcept(classTermExp);
							}
						}

						if (mRebuildTopConcept) {
							CConcept* topConcept = getConceptForClassTerm(mTopClassExpression,true);
							mRebuildTopConcept = false;
							FOREACHIT (CClassTermExpression* gciClassTermExp, mClassGCIExpressionSet) {
								buildConceptSubClassInclusion(mTopClassExpression,gciClassTermExp);
							}
							mClassGCIExpressionSet.clear();
							FOREACHIT (CClassTermExpression* rebuildClassTermExp, *mTopRebuildClassTermExpressionsSet) {
								if (!mNewBuildedConceptSet.contains(rebuildClassTermExp) && !mBuildingConceptSet.contains(rebuildClassTermExp)) {
									mNewBuildConceptSet.insert(rebuildClassTermExp);
								}
							}
							FOREACHIT (CObjectPropertyTermExpression* rebuildObjectPropertyTermExp, *mTopRebuildObjectPropertyTermExpressionsSet) {
								if (!mNewBuildedRoleSet.contains(rebuildObjectPropertyTermExp) && !mBuildingRoleSet.contains(rebuildObjectPropertyTermExp)) {
									mNewBuildRoleSet.insert(rebuildObjectPropertyTermExp);
								}
							}

						}
					}
				}




				buildIndividualIDs();
				buildRoleTags();
				buildConceptTags();
				buildRoleChainTags();

				resortAndInstallConceptsAndRolesAndIndividuals();


				mOntoData->getExpressionDataBoxMapping()->setLastProcessedChangedAxiom(mLastProcessedChangedAxiom);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildIndividual(mLastProcessedBuildIndividual);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildConcept(mLastProcessedBuildConcept);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildRole(mLastProcessedBuildRole);

				mOntoData->getExpressionDataBoxMapping()->setLastProcessedInverseProperty(mLastProcessedInverseProperty);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedExpression(mLastProcessedExpression);

				mOntoData->getExpressionDataBoxMapping()->setLastBuildedIndividual(mLastBuildedIndividual);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedConcept(mLastBuildedConcept);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedRole(mLastBuildedRole);

				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildClassConcept(CClassTermExpression* classTermExp) {
				if (classTermExp == mTopClassExpression) {
					mRebuildTopConcept = true;
				} else if (mTopRebuildClassTermExpressionsSet->contains(classTermExp)) {
					if (!mNewBuildedConceptSet.contains(mTopClassExpression) && !mBuildingConceptSet.contains(mTopClassExpression)) {
						mNewBuildConceptSet.insert(mTopClassExpression);
					}
				}
				if (mClassTermClassAxiomHash->contains(classTermExp)) {
					CBUILDLIST<CClassAxiomExpression*> classAccAxiomList(mClassTermClassAxiomHash->values(classTermExp));
					if (!classAccAxiomList.isEmpty()) {
						mClassTermClassAxiomHash->remove(classTermExp);
						FOREACHIT (CClassAxiomExpression* classAxiom, classAccAxiomList) {
							if (!mRetractUpdatedAxiomSet->contains(classAxiom)) {
								mClassTermClassAxiomHash->insertMulti(classTermExp,classAxiom);
								CBuildExpression::ExpressionType expType = classAxiom->getType();
								if (expType == CBuildExpression::BETESUBCLASSOF) {
									CSubClassOfExpression* axiomExp = (CSubClassOfExpression*)classAxiom;
									buildConceptSubClassInclusion(classTermExp,axiomExp->getSecondClassTermExpression());
								} else if (expType == CBuildExpression::BETEQUIVALENTCLASSES) {
									CEquivalentClassesExpression* axiomExp = (CEquivalentClassesExpression*)classAxiom;
									buildPermutableConceptEquivalentClass(classTermExp,*axiomExp->getClassTermExpressionList());
								} else if (expType == CBuildExpression::BETDISJOINTCLASSES) {
									CDisjointClassesExpression* axiomExp = (CDisjointClassesExpression*)classAxiom;
									buildPermutableConceptDisjointClass(classTermExp,*axiomExp->getClassTermExpressionList());
								} else if (expType == CBuildExpression::BETDISJOINTUNION) {
									CDisjointUnionExpression* axiomExp = (CDisjointUnionExpression*)classAxiom;
									CClassTermExpression* unionClassExp = axiomExp->getClassExpression();
									if (classTermExp == unionClassExp) {
										buildDisjointUnionConceptClass(classTermExp,*axiomExp->getClassTermExpressionList());
									} else {
										buildPermutableConceptDisjointClass(classTermExp,*axiomExp->getClassTermExpressionList());
									}
								}
							} else {
								mClassTermClassAxiomSet->remove(QPair<CClassTermExpression*,CClassAxiomExpression*>(classTermExp,classAxiom));
							}
						}
					}
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildIndividualIndi(CIndividualTermExpression* indiTermExp) {

				CClassTermExpression* nominalAssClassTermExp = getObjectOneOf(indiTermExp);
				bool nominalAssClass = false;

				CIndividual* individual = getIndividualForIndividualTerm(indiTermExp,true);

				if (mIndiTermAssertionAxiomHash->contains(indiTermExp)) {
					CBUILDLIST<CAssertionAxiomExpression*> indiAssAxiomList(mIndiTermAssertionAxiomHash->values(indiTermExp));
					mIndiTermAssertionAxiomHash->remove(indiTermExp);

					FOREACHIT (CAssertionAxiomExpression* assAxiom, indiAssAxiomList) {

						CBuildExpression::ExpressionType expType = assAxiom->getType();
						if (!mRetractUpdatedAxiomSet->contains(assAxiom)) {
							mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiom);

							if (expType == CBuildExpression::BETCLASSASSERTION) {	
								CClassAssertionExpression* classAssExp = (CClassAssertionExpression*)assAxiom;
								CClassTermExpression* classTermExp = classAssExp->getClassTermExpression();
								setIndividualAssertionConceptFromClassTerm(individual,classTermExp,false);
								if (classTermExp == nominalAssClassTermExp) {
									nominalAssClass = true;
								}
							} else if (expType == CBuildExpression::BETOBJECTPROPERTYASSERTION) {	
								CObjectPropertyAssertionExpression* propAssExp = (CObjectPropertyAssertionExpression*)assAxiom;
								CObjectPropertyTermExpression* objPropTermExp = propAssExp->getObjectPropertyTermExpression();
								CIndividualTermExpression* destIndiTermExp = propAssExp->getSecondIndividualTermExpression();
								setIndividualAssertionRoleFromObjectPropertyTerm(individual,objPropTermExp,destIndiTermExp);
								//CClassTermExpression* classTermExp = getObjectHasValue(objPropTermExp,destIndiTermExp);
								//setIndividualAssertionConceptFromClassTerm(individual,classTermExp,false);
							} else if (expType == CBuildExpression::BETNEGATIVEOBJECTPROPERTYASSERTION) {	
								CNegativeObjectPropertyAssertionExpression* propAssExp = (CNegativeObjectPropertyAssertionExpression*)assAxiom;
								CObjectPropertyTermExpression* objPropTermExp = propAssExp->getObjectPropertyTermExpression();
								CIndividualTermExpression* destIndiTermExp = propAssExp->getSecondIndividualTermExpression();
								CClassTermExpression* classTermExp = getObjectHasValue(objPropTermExp,destIndiTermExp);
								setIndividualAssertionConceptFromClassTerm(individual,classTermExp,true);
							} else if (expType == CBuildExpression::BETSAMEINDIVIDUAL) {	
								CSameIndividualExpression* sameIndiAssExp = (CSameIndividualExpression*)assAxiom;
								FOREACHIT (CIndividualTermExpression* indiTermExp2, *sameIndiAssExp->getIndividualTermExpressionList()) {
									if (indiTermExp != indiTermExp2) {
										CClassTermExpression* classTermExp = getObjectOneOf(indiTermExp2);
										setIndividualAssertionConceptFromClassTerm(individual,classTermExp,false);
									}
								}
							} else if (expType == CBuildExpression::BETDIFFERENTINDIVIDUAL) {	
								CDifferentIndividualsExpression* diffIndiAssExp = (CDifferentIndividualsExpression*)assAxiom;
								FOREACHIT (CIndividualTermExpression* indiTermExp2, *diffIndiAssExp->getIndividualTermExpressionList()) {
									if (indiTermExp != indiTermExp2) {
										CClassTermExpression* classTermExp = getObjectOneOf(indiTermExp2);
										setIndividualAssertionConceptFromClassTerm(individual,classTermExp,true);
									}
								}
							}
						}
					}
				}

				if (!nominalAssClass) {
					setIndividualAssertionNominalFromClassTerm(individual,nominalAssClassTermExp,false);
				}
				CConceptAssertionLinker* opConLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemManager);
				opConLinker->initNegLinker(tBox->getIndividualTriggerConcept(),false);
				individual->addAssertionConceptLinker(opConLinker);

				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildObjectPropertyRole(CObjectPropertyTermExpression* objPropTermExp) {
				if (mTopRebuildObjectPropertyTermExpressionsSet->contains(objPropTermExp)) {
					if (!mNewBuildedConceptSet.contains(mTopClassExpression) && !mBuildingConceptSet.contains(mTopClassExpression)) {
						mNewBuildConceptSet.insert(mTopClassExpression);
					}
				}

				CBUILDLIST<CObjectPropertyAxiomExpression*> objPropAccAxiomList(mObjPropTermObjPropAxiomHash->values(objPropTermExp));
				mObjPropTermObjPropAxiomHash->remove(objPropTermExp);

				CObjectPropertyTermExpression* inverseObjPropTermExp = mInverseObjectPropertyHash->value(objPropTermExp,nullptr);

				if (objPropTermExp != mTopObjPropExpression || !objPropAccAxiomList.isEmpty() || inverseObjPropTermExp) {

					CRole* role = getRoleForObjectPropertyTerm(objPropTermExp,true);

					if (inverseObjPropTermExp) {
						CRole* invRole = getRoleForObjectPropertyTerm(inverseObjPropTermExp);
						if (!role->hasInverseRole(invRole)) {
							if (!role->getInverseRole()) {
								role->setInverseRole(invRole);
							}
							CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
							roleLinker->init(invRole,true);
							role->addInverseRoleLinker(roleLinker);
						}
						if (!invRole->hasInverseRole(role)) {
							if (!invRole->getInverseRole()) {
								invRole->setInverseRole(role);
							}
							CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
							roleLinker->init(role,true);
							invRole->addInverseRoleLinker(roleLinker);
						}
					}

					FOREACHIT (CObjectPropertyAxiomExpression* objPropAxiom, objPropAccAxiomList) {
						if (!mRetractUpdatedAxiomSet->contains(objPropAxiom)) {
							mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiom);
							CBuildExpression::ExpressionType expType = objPropAxiom->getType();
							if (expType == CBuildExpression::BETSUBOBJECTPROPERTYOF) {
								if (objPropTermExp != mBottomObjPropExpression) {
									CSubObjectPropertyOfExpression* objPropAxiomExp = (CSubObjectPropertyOfExpression*)objPropAxiom;
									CEXPRESSIONLIST<CObjectPropertyTermExpression*>* subObjectPropertyExpressionList = objPropAxiomExp->getSubObjectPropertyTermExpressionList();
									// build only once
									CObjectPropertyTermExpression* superObjectPropertyExpression = objPropAxiomExp->getSuperObjectPropertyTermExpression();
									if (subObjectPropertyExpressionList->count() <= 1) {
										// build simple sub role relation
										if (superObjectPropertyExpression != mTopObjPropExpression) {
											CRole* superRole = getRoleForObjectPropertyTerm(superObjectPropertyExpression);
											if (!role->hasSuperRole(superRole)) {
												CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
												roleLinker->init(superRole,false);
												role->addSuperRoleLinker(roleLinker);
											}
										}
									} else {
										// build role chain
										CRoleChain* roleChain = buildRoleChainForSubObjectPropertyOfExpression(objPropAxiomExp);
										CXLinker<CRoleChain*>* roleChainLinker = CObjectAllocator< CXLinker<CRoleChain*> >::allocateAndConstruct(mMemManager);
										roleChainLinker->initLinker(roleChain);
										if (objPropTermExp == superObjectPropertyExpression) {
											// role is super role
											role->addRoleChainSuperSharingLinker(roleChainLinker);
										} else {
											role->addRoleChainSubSharingLinker(roleChainLinker);
										}
									}
								}
							} else if (expType == CBuildExpression::BETEQUIVALENTOBJECTPROPERTIES) {
								CEquivalentObjectPropertiesExpression* objPropAxiomExp = (CEquivalentObjectPropertiesExpression*)objPropAxiom;
								CEXPRESSIONLIST<CObjectPropertyTermExpression*>* eqObjectPropertyExpressionList = objPropAxiomExp->getObjectPropertyTermExpressionList();
								FOREACHIT (CObjectPropertyTermExpression* eqObjPropTermExp, *eqObjectPropertyExpressionList) {
									if (objPropTermExp != eqObjPropTermExp) {
										CRole* eqRole = getRoleForObjectPropertyTerm(eqObjPropTermExp);
										if (!role->hasEquivalentRole(eqRole)) {
											CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
											roleLinker->init(eqRole,false);
											role->addEquivalentRoleLinker(roleLinker);
										}
									}
								}
							} else if (expType == CBuildExpression::BETDISJOINTOBJECTPROPERTIES) {
								CDisjointObjectPropertiesExpression* objPropAxiomExp = (CDisjointObjectPropertiesExpression*)objPropAxiom;
								CEXPRESSIONLIST<CObjectPropertyTermExpression*>* disObjectPropertyExpressionList = objPropAxiomExp->getObjectPropertyTermExpressionList();
								FOREACHIT (CObjectPropertyTermExpression* disObjPropTermExp, *disObjectPropertyExpressionList) {
									if (objPropTermExp != disObjPropTermExp) {
										CRole* disRole = getRoleForObjectPropertyTerm(disObjPropTermExp);
										if (!role->hasDisjointRole(disRole)) {
											CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
											roleLinker->init(disRole,false);
											role->addDisjointRoleLinker(roleLinker);
										}
									}
								}
							} else if (expType == CBuildExpression::BETINVERSEOBJECTPROPERTIES) {
								CInverseObjectPropertiesExpression* objPropAxiomExp = (CInverseObjectPropertiesExpression*)objPropAxiom;
								CObjectPropertyTermExpression* invObjPropTermExp = nullptr;
								if (objPropTermExp == objPropAxiomExp->getFirstObjectPropertyTermExpression()) {
									invObjPropTermExp = objPropAxiomExp->getSecondObjectPropertyTermExpression();
								} else {
									invObjPropTermExp = objPropAxiomExp->getFirstObjectPropertyTermExpression();
								}
								CRole* invRole = getRoleForObjectPropertyTerm(invObjPropTermExp);
								if (!role->hasInverseRole(invRole)) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
									roleLinker->init(invRole,true);
									role->addInverseRoleLinker(roleLinker);
								}
							} else if (expType == CBuildExpression::BETOBJECTPROPERTYDOMAIN) {
								CObjectPropertyDomainExpression* objPropAxiomExp = (CObjectPropertyDomainExpression*)objPropAxiom;
								CClassTermExpression* domainExp = objPropAxiomExp->getClassTermExpression();
								CConcept* domainCon = getConceptForClassTerm(domainExp);
								CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
								conceptLinker->init(domainCon,false);
								role->addDomainConceptLinker(conceptLinker);
							} else if (expType == CBuildExpression::BETOBJECTPROPERTYRANGE) {
								CObjectPropertyRangeExpression* objPropAxiomExp = (CObjectPropertyRangeExpression*)objPropAxiom;
								CClassTermExpression* rangeExp = objPropAxiomExp->getClassTermExpression();
								CConcept* rangeCon = getConceptForClassTerm(rangeExp);
								CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
								conceptLinker->init(rangeCon,false);
								role->addRangeConceptLinker(conceptLinker);
							} else if (expType == CBuildExpression::BETFUNCTIONALPROPERTY) {
								CFunctionalObjectPropertyExpression* objPropAxiomExp = (CFunctionalObjectPropertyExpression*)objPropAxiom;
								CObjectMaxCardinalityExpression* funcCardExp = getObjectMaxCardinality(objPropTermExp,mTopClassExpression,1);
								CConcept* domainCon = getConceptForClassTerm(funcCardExp);
								CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
								conceptLinker->init(domainCon,false);
								role->addDomainConceptLinker(conceptLinker);
								role->setFunctional(true);
							} else if (expType == CBuildExpression::BETINVERSEFUNCTIONALPROPERTY) {
								CInverseFunctionalObjectPropertyExpression* objPropAxiomExp = (CInverseFunctionalObjectPropertyExpression*)objPropAxiom;
								CObjectMaxCardinalityExpression* funcCardExp = getObjectMaxCardinality(getCorrectedInverseObjectPropertyOf(objPropTermExp),mTopClassExpression,1);
								CConcept* rangeCon = getConceptForClassTerm(funcCardExp);
								CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
								conceptLinker->init(rangeCon,false);
								role->addRangeConceptLinker(conceptLinker);
								role->setInverseFunctional(true);
							} else if (expType == CBuildExpression::BETREFLEXIVEPROPERTY) {
								CReflexiveObjectPropertyExpression* objPropAxiomExp = (CReflexiveObjectPropertyExpression*)objPropAxiom;
								CObjectHasSelfExpression* gciSelfExp = getObjectHasSelf(objPropTermExp);
								buildGeneralConceptInclusionClassExpression(gciSelfExp);
								role->setReflexive(true);
							} else if (expType == CBuildExpression::BETIRREFLEXIVEPROPERTY) {
								CIrreflexiveObjectPropertyExpression* objPropAxiomExp = (CIrreflexiveObjectPropertyExpression*)objPropAxiom;
								CObjectHasSelfExpression* selfExp = getObjectHasSelf(objPropTermExp);
								CObjectComplementOfExpression* gciNotSelfExp = getObjectComplementOf(selfExp);
								buildGeneralConceptInclusionClassExpression(gciNotSelfExp);
								role->setIrreflexive(true);
							} else if (expType == CBuildExpression::BETSYMMETRICPROPERTY) {
								// has it self as inverse
								CSymmetricObjectPropertyExpression* objPropAxiomExp = (CSymmetricObjectPropertyExpression*)objPropAxiom;
								CRole* invRole = role;
								if (!role->hasInverseRole(invRole)) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
									roleLinker->init(invRole,true);
									role->addInverseRoleLinker(roleLinker);
								}
								role->setSymmetric(true);
							} else if (expType == CBuildExpression::BETASYMMETRICPROPERTY) {
								// disjoint to the inverse
								CAsymmetricObjectPropertyExpression* objPropAxiomExp = (CAsymmetricObjectPropertyExpression*)objPropAxiom;
								CObjectPropertyTermExpression* invObjPropTermExp = getCorrectedInverseObjectPropertyOf(objPropTermExp);
								CRole* invRole = getRoleForObjectPropertyTerm(invObjPropTermExp);
								if (!role->hasDisjointRole(invRole)) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
									roleLinker->init(invRole,false);
									role->addDisjointRoleLinker(roleLinker);
								}
								if (!invRole->hasDisjointRole(role)) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
									roleLinker->init(role,false);
									invRole->addDisjointRoleLinker(roleLinker);
								}
								role->setAsymmetric(true);
							} else if (expType == CBuildExpression::BETTRANSITIVEOBJECTPROPERTY) {
								// TODO: build RR [= R ??
								CTransetiveObjectPropertyExpression* objPropAxiomExp = (CTransetiveObjectPropertyExpression*)objPropAxiom;
								CRoleChain* roleChain = buildRoleChainForTransetiveObjectPropertyExpression(objPropAxiomExp);

								CXLinker<CRoleChain*>* superRoleChainLinker = CObjectAllocator< CXLinker<CRoleChain*> >::allocateAndConstruct(mMemManager);
								superRoleChainLinker->initLinker(roleChain);
								role->addRoleChainSuperSharingLinker(superRoleChainLinker);
								CXLinker<CRoleChain*>* subRoleChainLinker = CObjectAllocator< CXLinker<CRoleChain*> >::allocateAndConstruct(mMemManager);
								subRoleChainLinker->initLinker(roleChain);
								role->addRoleChainSubSharingLinker(subRoleChainLinker);
								role->setTransitive(true);
							} 
						} else {
							mObjPropTermObjPropAxiomSet->remove(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiom));
						}
					}
				}
				return true;
			}



			CRoleChain* CConcreteOntologyUpdateBuilder::buildRoleChainForTransetiveObjectPropertyExpression(CTransetiveObjectPropertyExpression* objPropAxiomExp) {
				CRoleChain* roleChain = nullptr;
				roleChain = mObjPropTermRoleChainHash->value(objPropAxiomExp,nullptr);
				if (!roleChain) {
					roleChain = CObjectAllocator< CRoleChain >::allocateAndConstruct(mMemManager);
					roleChain->initRoleChain();
					mObjPropTermRoleChainHash->insert(objPropAxiomExp,roleChain);
					mRoleChainObjPropTermHash->insert(roleChain,objPropAxiomExp);
					CObjectPropertyTermExpression* transObjectProperty = objPropAxiomExp->getTransetivePropertyExpression();
					CRole* transRole = getRoleForObjectPropertyTerm(transObjectProperty);
					CXLinker<CRole*>* chainedRoleLinker1 = CObjectAllocator< CXLinker<CRole*> >::allocateAndConstruct(mMemManager);
					chainedRoleLinker1->initLinker(transRole);
					roleChain->appendRoleChainLinker(chainedRoleLinker1);
					CXLinker<CRole*>* chainedRoleLinker2 = CObjectAllocator< CXLinker<CRole*> >::allocateAndConstruct(mMemManager);
					chainedRoleLinker2->initLinker(transRole);
					roleChain->appendRoleChainLinker(chainedRoleLinker2);
					mTaggingRoleChainSet.insert(roleChain);
				}
				return roleChain;
			}


			CRoleChain* CConcreteOntologyUpdateBuilder::buildRoleChainForSubObjectPropertyOfExpression(CSubObjectPropertyOfExpression* subObPropExp) {
				CRoleChain* roleChain = nullptr;
				roleChain = mObjPropTermRoleChainHash->value(subObPropExp,nullptr);
				if (!roleChain) {
					roleChain = CObjectAllocator< CRoleChain >::allocateAndConstruct(mMemManager);
					roleChain->initRoleChain();
					mObjPropTermRoleChainHash->insert(subObPropExp,roleChain);
					mRoleChainObjPropTermHash->insert(roleChain,subObPropExp);
					CEXPRESSIONLIST<CObjectPropertyTermExpression*>* subObjPropList = subObPropExp->getSubObjectPropertyTermExpressionList();
					FOREACHIT (CObjectPropertyTermExpression* subObjProp, *subObjPropList) {
						CRole* subRole = getRoleForObjectPropertyTerm(subObjProp);
						CXLinker<CRole*>* chainedRoleLinker = CObjectAllocator< CXLinker<CRole*> >::allocateAndConstruct(mMemManager);
						chainedRoleLinker->initLinker(subRole);
						roleChain->appendRoleChainLinker(chainedRoleLinker);
					}
					mTaggingRoleChainSet.insert(roleChain);
				}
				return roleChain;
			}


			bool CConcreteOntologyUpdateBuilder::buildDisjointUnionConceptClass(CClassTermExpression* disCallClassExp, const CEXPRESSIONLIST<CClassTermExpression*>& disClassExpList) {
				cint64 disjCount = disClassExpList.count();
				if (disjCount > 0) {
					if (disjCount == 1) {
						CClassTermExpression* disjClassExp = disClassExpList.first();
						buildConceptSubClassInclusion(disCallClassExp,disjClassExp);
						buildConceptSubClassInclusion(disjClassExp,disCallClassExp);
					} else {
						CClassTermExpression* orDisjExp = getObjectUnionOf(disClassExpList);
						buildConceptSubClassInclusion(disCallClassExp,orDisjExp);
						buildConceptSubClassInclusion(orDisjExp,disCallClassExp);
					}
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildPermutableConceptEquivalentClass(CClassTermExpression* eqCallClassExp, const CEXPRESSIONLIST<CClassTermExpression*>& eqClassExpList) {
				if (eqClassExpList.count() == 2) {
					CClassTermExpression* firstEqClassExp = eqClassExpList.first();
					CClassTermExpression* secondEqClassExp = eqClassExpList.last();
					buildPermutableConceptEquivalentClass(firstEqClassExp,secondEqClassExp);
				} else {
					FOREACHIT (CClassTermExpression* eqClassExp2, eqClassExpList) {
						if (eqClassExp2 != eqCallClassExp) {
							buildConceptSubClassInclusion(eqCallClassExp,eqClassExp2);
						}
					}
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildPermutableConceptDisjointClass(CClassTermExpression* disCallClassExp, const CEXPRESSIONLIST<CClassTermExpression*>& disClassExpList) {
				FOREACHIT (CClassTermExpression* disClassExp2, disClassExpList) {
					if (disCallClassExp != disClassExp2) {
						buildConceptSubClassInclusion(disCallClassExp,disClassExp2,true);
					}
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildPermutableConceptEquivalentClass(CClassTermExpression* classExp1, CClassTermExpression* classExp2) {
				CConcept* concept1 = getConceptForClassTerm(classExp1);
				CConcept* concept2 = getConceptForClassTerm(classExp2);
				CClassTermExpression* defEqClassExp = nullptr;
				CConcept* defEqConcept = nullptr;
				CClassTermExpression* eqClassExp = nullptr;
				CConcept* eqConcept = nullptr;
				if (concept1->getOperatorCode() == CCEQ && concept1->getOperandCount() == 1 && concept1->hasOperandConcept(concept2,false) || concept2->getOperatorCode() == CCEQ && concept2->getOperandCount() == 1 && concept2->hasOperandConcept(concept1,false)) {
					// equivalent class is already build
					return true;
				} else {
					if (classExp1->getType() == CBuildExpression::BETCLASS && concept1->getOperatorCode() == CCNONE) {
						defEqClassExp = classExp1;
						defEqConcept = concept1;
						eqClassExp = classExp2;
						eqConcept = concept2;
					} else {
						defEqClassExp = classExp2;
						defEqConcept = concept2;
						eqClassExp = classExp1;
						eqConcept = concept1;
					}
					return buildConceptEquivalentClass(defEqClassExp,eqClassExp);
				}
			} 

			bool CConcreteOntologyUpdateBuilder::buildConceptEquivalentClass(CClassTermExpression* classExp1, CClassTermExpression* classExp2) {
				CConcept* concept1 = getConceptForClassTerm(classExp1);
				CConcept* concept2 = getConceptForClassTerm(classExp2);
				CClassTermExpression* defEqClassExp = nullptr;
				CConcept* defEqConcept = nullptr;
				CClassTermExpression* eqClassExp = nullptr;
				CConcept* eqConcept = nullptr;
				defEqClassExp = classExp1;
				defEqConcept = concept1;
				eqClassExp = classExp2;
				eqConcept = concept2;


				if (defEqClassExp->getType() == CBuildExpression::BETCLASS && defEqConcept->getOperatorCode() == CCNONE) {
					// can build completely defined equivalent concept 
					defEqConcept->setOperatorCode(CCEQ);
					setConceptOperandsFromClassTerms(defEqConcept,eqClassExp);
				} else {

					// build each other inclusion with SubClassOf expressions
					buildConceptSubClassInclusion(defEqClassExp,eqClassExp);
					buildConceptSubClassInclusion(eqClassExp,defEqClassExp);
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildConceptSubClassInclusion(CClassTermExpression* subClassExp, CClassTermExpression* inclusionClassExp, bool negate) {
				if (subClassExp != mBottomClassExpression) {
					CConcept* concept = getConceptForClassTerm(subClassExp);
					qint64 opCode = concept->getOperatorCode();
					if (opCode == CCEQ) {
						CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();

						concept->setOperatorCode(CCSUB);
						opCode = CCSUB;

						// there is only one equivalent concept and is not negated by the linker
						CConcept* eqConcept = opConLinker->getData();
						CClassTermExpression* eqClassExp = mConceptClassTermHash->value(eqConcept);
						if (eqConcept == CCATOM) {
							concept->setOperandCount(0);
							concept->setOperandList(nullptr);
							// order rebuild equivalent class
							if (mNewBuildedConceptSet.contains(eqClassExp)) {
								mNewBuildedConceptSet.remove(eqClassExp);
							}
							mNewBuildConceptSet.insert(eqClassExp);
						} else {
							buildConceptSubClassInclusion(eqClassExp,subClassExp);
						}

					} 
					if (opCode == CCSUB || opCode == CCTOP) {
						// build simple subclass inclusion
						setConceptOperandsFromClassTerms(concept,inclusionClassExp,negate);
					} else if (opCode == CCNONE && subClassExp->getType() == CBuildExpression::BETCLASS) {
						// build simple subclass inclusion
						setConceptOperandsFromClassTerms(concept,inclusionClassExp,negate);
						concept->setOperatorCode(CCSUB);
						// not necessary
						//CConcept* atomicSubClassUnfoldConcept = getAtomicSubClassConcept(subClassExp);
						//setConceptOperands(concept,atomicSubClassUnfoldConcept);
					} else {
						// build GCI
						CClassTermExpression* compSubClassExp = getObjectComplementOf(subClassExp);
						if (!mConstructFlags->isNonELConstructUsed() && !mInitialBuildConceptSet->contains(compSubClassExp)) {
							mELExceptionExpressionSet.insert(compSubClassExp);
						}
						CClassTermExpression* neIinclusionClassExp = inclusionClassExp;
						if (negate) {
							neIinclusionClassExp = getObjectComplementOf(inclusionClassExp);
							mELExceptionExpressionSet.insert(neIinclusionClassExp);
						}
						CClassTermExpression* gciIncusionExp = getObjectUnionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<compSubClassExp<<neIinclusionClassExp);
						if (!mConstructFlags->isNonELConstructUsed() && !mInitialBuildConceptSet->contains(gciIncusionExp)) {
							mELExceptionExpressionSet.insert(gciIncusionExp);
						}
						buildGeneralConceptInclusionClassExpression(gciIncusionExp);
					}
				}
				return true;
			}

			bool CConcreteOntologyUpdateBuilder::buildGeneralConceptInclusionClassExpression(CClassTermExpression* gciClassTermExp) {
				mClassGCIExpressionSet.insert(gciClassTermExp);
				mRebuildTopConcept = true;
				return true;
			}


			CConcept* CConcreteOntologyUpdateBuilder::getAtomicSubClassConcept(CClassTermExpression* subClassExp) {
				CConcept* concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemManager);
				concept->initConcept();
				mTaggingConceptList.append(concept);
				return concept;
			}

			bool CConcreteOntologyUpdateBuilder::updateName(CNamedItem* item, const QString& name) {
				bool nameAdded = false;
				if (!CIRIName::hasIRIName(item->getNameLinker(),name)) {
					cint64 lastNameID = CName::getLastNameVersionID(item->getNameLinker());
					if (lastNameID < 0) {
						lastNameID = 0;
					}
					FOREACHHASHIT (QString prefix, CNamePrefix* namePrefix, *mNamePrefixMapHash) {
						if (name.startsWith(prefix)) {
							CAbbreviatedIRIName* newName = CObjectAllocator<CAbbreviatedIRIName>::allocateAndConstruct(mMemManager);
							QString abbString = name.mid(prefix.length());
							newName->init(namePrefix,abbString);
							newName->setNameVersionID(lastNameID);
							CLinker<CName*>* newNameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(mMemManager);
							newNameLinker->init(newName);
							item->addNameLinker(newNameLinker);
							nameAdded = true;
						}
					}
					if (!nameAdded) {
						CIRIName* newName = CObjectAllocator<CIRIName>::allocateAndConstruct(mMemManager);
						newName->init(name);
						newName->setNameVersionID(lastNameID);
						CLinker<CName*>* newNameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(mMemManager);
						newNameLinker->init(newName);
						item->addNameLinker(newNameLinker);
					}
				}	
				return nameAdded;
			}



			CConcept* CConcreteOntologyUpdateBuilder::getConceptForClassTerm(CClassTermExpression* classTermExp, bool forceLocalisation) {
				CConcept* concept = nullptr;
				bool forceCreation = false;
				if (classTermExp == mTopClassExpression || classTermExp == mBottomClassExpression) {
					forceLocalisation = true;
				} else if (mInitialBuild) {
					forceLocalisation = false;
				}
				concept = mClassTermConceptHash->value(classTermExp);
				if (concept && forceLocalisation) {
					if (!mLocalisationSet.contains(classTermExp)) {
						forceCreation = true;
					}
				}
				if (!concept || forceCreation) {
					CConcept* prevConcept = concept;
					if (forceLocalisation) {
						mLocalisationSet.insert(classTermExp);
					}
					concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemManager);
					concept->initConcept();
					if (prevConcept) {
						cint64 prevConceptTag = prevConcept->getConceptTag();
						concept->initTag(prevConceptTag);
						cint64 prevConceptOpCode = prevConcept->getOperatorCode();
						if (prevConceptOpCode == CCTOP || prevConceptOpCode == CCBOTTOM) {
							concept->setOperatorCode(prevConceptOpCode);
						}	
						mInstallConceptList.append(concept);
					} else {
						mTaggingConceptList.append(concept);
					}
					mClassTermConceptHash->insert(classTermExp,concept);
					mConceptClassTermHash->insert(concept,classTermExp);

					if (classTermExp->getType() == CBuildExpression::BETCLASS) {
						// update name
						CClassExpression* classExp = (CClassExpression*)classTermExp;
						QString className = classExp->getName();
						updateName(concept,className);
					}
				}
				return concept;
			}



			CIndividual* CConcreteOntologyUpdateBuilder::getIndividualForIndividualTerm(CIndividualTermExpression* indiTermExp, bool forceLocalisation) {
				CIndividual* indi = nullptr;
				indi = mIndividulTermIndiHash->value(indiTermExp);
				bool forceCreation = false;
				if (mInitialBuild) {
					forceLocalisation = false;
				}
				if (indi && forceLocalisation) {
					if (!mLocalisationSet.contains(indiTermExp)) {
						forceCreation = true;
					}
				}
				if (!indi || forceCreation) {
					CIndividual* prevIndi = indi;
					if (forceLocalisation) {
						mLocalisationSet.insert(indiTermExp);
					}
					indi = CObjectAllocator<CIndividual>::allocateAndConstruct(mMemManager);
					indi->initIndividual();
					if (prevIndi) {
						indi->initTag(prevIndi->getIndividualID());
						mInstallIndividualList.append(indi);
					} else {
						mTaggingIndividualSet.insert(indi);
					}
					mIndividulTermIndiHash->insert(indiTermExp,indi);
					mIndiIndividulTermHash->insert(indi,indiTermExp);

					if (indiTermExp->getType() == CBuildExpression::BETNAMEDINDIVIDUAL) {
						// update name
						CNamedIndividualExpression* namedIndiExp = (CNamedIndividualExpression*)indiTermExp;
						QString indiName = namedIndiExp->getName();
						updateName(indi,indiName);
					}


				}
				return indi;
			}



			CRole* CConcreteOntologyUpdateBuilder::getRoleForObjectPropertyTerm(CObjectPropertyTermExpression* objPropTermExp, bool forceLocalisation) {
				CRole* role = nullptr;
				role = mObjPropTermRoleHash->value(objPropTermExp);
				bool forceCreation = false;
				if (objPropTermExp == mTopObjPropExpression) {
					mConstructFlags->setTopRoleUsed();
					forceLocalisation = true;
				} else if (objPropTermExp == mBottomObjPropExpression) {
					mConstructFlags->setBottomRoleUsed();
					forceLocalisation = true;
				} else if (mInitialBuild) {
					forceLocalisation = false;
				}
				if (role && forceLocalisation) {
					if (!mLocalisationSet.contains(objPropTermExp)) {
						forceCreation = true;
					}
				}
				if (!role || forceCreation) {
					CRole* prevRole = role;
					if (forceLocalisation) {
						mLocalisationSet.insert(objPropTermExp);
					}
					role = CObjectAllocator<CRole>::allocateAndConstruct(mMemManager);
					role->initRole();
					if (prevRole) {
						role->initTag(prevRole->getRoleTag());
						mInstallRoleList.append(role);
					} else {
						mTaggingRoleSet.insert(role);
					}
					mObjPropTermRoleHash->insert(objPropTermExp,role);
					mRoleObjPropTermHash->insert(role,objPropTermExp);

					if (objPropTermExp->getType() == CBuildExpression::BETOBJECTPROPERTY) {
						// update name
						CObjectPropertyExpression* objPropExp = (CObjectPropertyExpression*)objPropTermExp;
						QString propName = objPropExp->getName();
						updateName(role,propName);
					}

				}
				return role;
			}


			bool CConcreteOntologyUpdateBuilder::setConceptIndividualFromIndividualTerm(CConcept* concept, CIndividualTermExpression* indiTermExp) {
				CIndividual* opIndi = getIndividualForIndividualTerm(indiTermExp,false);
				concept->setNominalIndividual(opIndi);
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::setConceptRoleFromObjectPropertyTerm(CConcept* concept, CObjectPropertyTermExpression* objectPropertyTermExp) {
				CRole* opRole = getRoleForObjectPropertyTerm(objectPropertyTermExp);
				concept->setRole(opRole);
				return true;
			}



			cint64 CConcreteOntologyUpdateBuilder::getIndividualVariableID(CConcept* concept, CObjectIndividualVariableExpression* objectIndVarExp) {
				cint64 indVarID = mIndividualVariableIDHash->value(objectIndVarExp,0);
				if (indVarID <= 0) {
					indVarID = mIndividualVariableIDHash->count()+1;
					mIndividualVariableIDHash->insert(objectIndVarExp,indVarID);
				}
				return indVarID;
			}



			bool CConcreteOntologyUpdateBuilder::setConceptOperandsFromClassTerms(CConcept* concept, CEXPRESSIONLIST<CClassTermExpression*>* classTermList, bool negate) {
				FOREACHIT (CClassTermExpression* classTermExp, *classTermList) {
					CConcept* opConcept = getConceptForClassTerm(classTermExp);
					//if (!concept->hasOperandConcept(opConcept,negate)) {
						CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
						opConLinker->init(opConcept,negate);
						concept->addOperandLinker(opConLinker);
						concept->incOperandCount();
					//}
				}
				return true;
			}

			bool CConcreteOntologyUpdateBuilder::setIndividualAssertionRoleFromObjectPropertyTerm(CIndividual* individual, CObjectPropertyTermExpression* objPropTermExp, CIndividualTermExpression* destIndiTermExp) {
				CIndividual* desIndividual = getIndividualForIndividualTerm(destIndiTermExp);
				CRole* role = getRoleForObjectPropertyTerm(objPropTermExp);
				CRoleAssertionLinker* roleAssLinker = CObjectAllocator< CRoleAssertionLinker >::allocateAndConstruct(mMemManager);
				roleAssLinker->initRoleAssertionLinker(role,desIndividual);
				individual->addAssertionRoleLinker(roleAssLinker);
				return true;
			}

			bool CConcreteOntologyUpdateBuilder::setIndividualAssertionConceptFromClassTerm(CIndividual* individual, CClassTermExpression* classTermExp, bool negate) {
				CConcept* opConcept = getConceptForClassTerm((CClassTermExpression*)classTermExp);
				CConceptAssertionLinker* opConLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemManager);
				opConLinker->initNegLinker(opConcept,negate);
				individual->addAssertionConceptLinker(opConLinker);
				return true;
			}

			bool CConcreteOntologyUpdateBuilder::setIndividualAssertionNominalFromClassTerm(CIndividual* individual, CClassTermExpression* classTermExp, bool negate) {
				CConcept* opConcept = getConceptForClassTerm((CClassTermExpression*)classTermExp);
				individual->setIndividualNominalConcept(opConcept);
				CConceptAssertionLinker* opConLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemManager);
				opConLinker->initNegLinker(opConcept,negate);
				individual->addAssertionConceptLinker(opConLinker);
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::setConceptOperandsFromClassTerms(CConcept* concept, CClassTermExpression* classTermExp, bool negate) {
				if (classTermExp) {
					CConcept* opConcept = getConceptForClassTerm((CClassTermExpression*)classTermExp);
					//if (!concept->hasOperandConcept(opConcept,negate)) {
						CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
						opConLinker->init(opConcept,negate);
						concept->addOperandLinker(opConLinker);
						concept->incOperandCount();
					//}
					return true;
				}
				return false;
			}

			bool CConcreteOntologyUpdateBuilder::setConceptOperands(CConcept* concept, CConcept* operandConcept, bool negate) {
				//if (!concept->hasOperandConcept(operandConcept,negate)) {
					CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
					opConLinker->init(operandConcept,negate);
					concept->addOperandLinker(opConLinker);
					concept->incOperandCount();
				//}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::tellOntologyAxiom(CAxiomExpression* axiom) {
				if (!mTellAxiomSet->contains(axiom)) {
					mRetractAxiomSet->remove(axiom);
					mTellAxiomSet->insert(axiom);
					if (mRetractUpdatedAxiomSet->contains(axiom)) {
						mRetractUpdatedAxiomSet->remove(axiom);
					} else {
						mTellUpdatedAxiomSet->insert(axiom);
					}
				}
				return true;
			}

			bool CConcreteOntologyUpdateBuilder::retractOntologyAxiom(CAxiomExpression* axiom) {
				if (mTellAxiomSet->contains(axiom)) {
					mTellAxiomSet->remove(axiom);
					mRetractAxiomSet->insert(axiom);
					if (mTellUpdatedAxiomSet->contains(axiom)) {
						mTellUpdatedAxiomSet->remove(axiom);
					} else {
						mRetractUpdatedAxiomSet->insert(axiom);
					}
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildRoleTags() {
				cint64 nextRoleTag = rBox->getRoleCount();
				FOREACHIT (CRole* role, mTaggingRoleSet) {
					role->setRoleTag(nextRoleTag++);
					mInstallRoleList.append(role);
				}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildIndividualIDs() {
				cint64 nextIndiID = aBox->getIndividualCount();
				FOREACHIT (CIndividual* indi, mTaggingIndividualSet) {
					indi->setIndividualID(nextIndiID++);
					mInstallIndividualList.append(indi);
				}
				return true;
			}



			bool CConcreteOntologyUpdateBuilder::buildRoleChainTags() {
				cint64 nextRoleChainTag = rBox->getRoleChainCount();
				FOREACHIT (CRoleChain* roleChain, mTaggingRoleChainSet) {
					roleChain->setRoleChainTag(nextRoleChainTag++);
					mInstallRoleChainList.append(roleChain);
				}
				return true;
			}



			bool CConcreteOntologyUpdateBuilder::buildConceptTags() {
				cint64 lastFreeConceptTag = tBox->getConceptCount();

				class CConceptTaggingDependingItem : public CConceptData {
					public:
						CConceptTaggingDependingItem() {
							mDependent = false;
						}
					public:
						bool mDependent;
				};


				cint64 taggingCount = mTaggingConceptList.count();
				CConceptTaggingDependingItem* taggingItemVec = new CConceptTaggingDependingItem[taggingCount];

				cint64 itemIdx = 0;
				for (CBUILDLIST<CConcept*>::const_iterator it1 = mTaggingConceptList.constBegin(), it1End = mTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
					CConcept* concept = *it1;
					if (!concept->hasConceptData()) {
						CConceptTaggingDependingItem* taggingItem = &taggingItemVec[itemIdx++];
						concept->setConceptData(taggingItem);
					}
				}

				CBUILDLIST<CConcept*> dependedConceptList;
				CBUILDLIST<CConcept*> directConceptList;
				CBUILDLIST<CConcept*> currentConceptList;

				for (CBUILDLIST<CConcept*>::const_iterator it1 = mTaggingConceptList.constBegin(), it1End = mTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
					CConcept* concept = *it1;
					CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
					while (opConLinker) {
						CConcept* opConcept = opConLinker->getData();
						if (opConcept) {
							CConceptTaggingDependingItem* taggingItem = (CConceptTaggingDependingItem*)opConcept->getConceptData();
							if (taggingItem && !taggingItem->mDependent) {
								taggingItem->mDependent = true;
								dependedConceptList.append(opConcept);
							}
						}
						opConLinker = opConLinker->getNext();
					}
				}
				for (CBUILDLIST<CConcept*>::const_iterator it1 = mTaggingConceptList.constBegin(), it1End = mTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
					CConcept* concept = *it1;
					CConceptTaggingDependingItem* taggingItem = (CConceptTaggingDependingItem*)concept->getConceptData();
					if (!taggingItem->mDependent) {
						directConceptList.append(concept);
					}
				}
				CBUILDLIST<CConcept*> taggingConceptList;
				while (!directConceptList.isEmpty() || !dependedConceptList.isEmpty()) {
					CConcept* concept = nullptr;
					if (currentConceptList.isEmpty() && !directConceptList.isEmpty()) {
						concept = directConceptList.takeFirst();
						currentConceptList.append(concept);
					}
					if (currentConceptList.isEmpty() && !dependedConceptList.isEmpty()) {
						CConcept* concept = dependedConceptList.takeFirst();
						CConceptTaggingDependingItem* taggingItem = (CConceptTaggingDependingItem*)concept->getConceptData();
						if (taggingItem && taggingItem->mDependent) {
							taggingItem->mDependent = false;
							currentConceptList.append(concept);
						}
					}
					CBUILDLIST<CConcept*> nextTaggingConceptList;
					while (!currentConceptList.isEmpty()) {
						concept = currentConceptList.takeFirst();
						nextTaggingConceptList.prepend(concept);

						CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
						while (opConLinker) {
							CConcept* opConcept = opConLinker->getData();
							if (opConcept) {
								CConceptTaggingDependingItem* taggingItem = (CConceptTaggingDependingItem*)opConcept->getConceptData();
								if (taggingItem && taggingItem->mDependent) {
									taggingItem->mDependent = false;
									currentConceptList.append(opConcept);
								}
							}
							opConLinker = opConLinker->getNext();
						}
					}
					for (CBUILDLIST<CConcept*>::const_iterator it1 = nextTaggingConceptList.constBegin(), it1End = nextTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
						taggingConceptList.append(*it1);
					}

				}
				for (CBUILDLIST<CConcept*>::const_iterator it1 = taggingConceptList.constBegin(), it1End = taggingConceptList.constEnd(); it1 != it1End; ++it1) {
					CConcept* taggingConcept = *it1;
					taggingConcept->setConceptTag(lastFreeConceptTag++);
					mInstallConceptList.append(taggingConcept);
				}

				for (CBUILDLIST<CConcept*>::const_iterator it1 = mTaggingConceptList.constBegin(), it1End = mTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
					CConcept* concept = *it1;
					concept->setConceptData(nullptr);
				}

				delete [] taggingItemVec;




				//CBUILDSET<CConcept*> dependedConceptSet;
				//CBUILDSET<CConcept*> taggedConceptSet;
				//CBUILDLIST<CConcept*> directConceptList;
				//CBUILDLIST<CConcept*> currentConceptList;
				//for (CBUILDSET<CConcept*>::const_iterator it1 = mTaggingConceptSet.constBegin(), it1End = mTaggingConceptSet.constEnd(); it1 != it1End; ++it1) {
				//	CConcept* concept = *it1;
				//	CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				//	while (opConLinker) {
				//		CConcept* opConcept = opConLinker->getData();
				//		if (opConcept && mTaggingConceptSet.contains(opConcept)) {
				//			dependedConceptSet.insert(opConcept);
				//		}
				//		opConLinker = opConLinker->getNext();
				//	}
				//}
				//for (CBUILDSET<CConcept*>::const_iterator it1 = mTaggingConceptSet.constBegin(), it1End = mTaggingConceptSet.constEnd(); it1 != it1End; ++it1) {
				//	CConcept* concept = *it1;
				//	if (!dependedConceptSet.contains(concept)) {
				//		directConceptList.append(concept);
				//	}
				//}
				//CBUILDLIST<CConcept*> taggingConceptList;
				//while (!directConceptList.isEmpty() || !dependedConceptSet.isEmpty()) {
				//	CConcept* concept = nullptr;
				//	if (currentConceptList.isEmpty() && !directConceptList.isEmpty()) {
				//		concept = directConceptList.takeFirst();
				//		currentConceptList.append(concept);
				//	}
				//	if (currentConceptList.isEmpty() && !dependedConceptSet.isEmpty()) {
				//		CBUILDSET<CConcept*>::iterator firstDepConIt = dependedConceptSet.begin();
				//		concept = *firstDepConIt;
				//		currentConceptList.append(concept);
				//		dependedConceptSet.erase(firstDepConIt);
				//	}
				//	CBUILDLIST<CConcept*> nextTaggingConceptList;
				//	while (!currentConceptList.isEmpty()) {
				//		concept = currentConceptList.takeFirst();
				//		nextTaggingConceptList.prepend(concept);

				//		CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				//		while (opConLinker) {
				//			CConcept* opConcept = opConLinker->getData();
				//			if (opConcept && dependedConceptSet.contains(opConcept)) {
				//				dependedConceptSet.remove(opConcept);
				//				currentConceptList.prepend(opConcept);
				//			}
				//			opConLinker = opConLinker->getNext();
				//		}
				//	}
				//	for (CBUILDLIST<CConcept*>::const_iterator it1 = nextTaggingConceptList.constBegin(), it1End = nextTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
				//		taggingConceptList.append(*it1);
				//	}

				//}
				//for (CBUILDLIST<CConcept*>::const_iterator it1 = taggingConceptList.constBegin(), it1End = taggingConceptList.constEnd(); it1 != it1End; ++it1) {
				//	CConcept* taggingConcept = *it1;
				//	taggingConcept->setConceptTag(lastFreeConceptTag++);
				//	mInstallConceptList.append(taggingConcept);
				//}



				//cint64 nextUsedConceptTag = lastFreeConceptTag+mTaggingConceptSet.count()-1;
				//CBUILDSET<CConcept*> directConceptSet;
				//CBUILDHASH<CConcept*,CConcept*> succConceptHash;
				//CBUILDHASH<CConcept*,cint64> prevConceptCountHash;

				//FOREACHIT (CConcept* concept, mTaggingConceptSet) {
				//	if (prevConceptCountHash.value(concept,0) <= 0) {
				//		directConceptSet.insert(concept);
				//	}
				//	CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
				//	while (opConLinker) {
				//		CConcept* opConcept = opConLinker->getData();
				//		if (opConcept && mTaggingConceptSet.contains(opConcept)) {
				//			succConceptHash.insertMulti(concept,opConcept);
				//			cint64 prevCount = prevConceptCountHash.value(opConcept,0);
				//			if (prevCount <= 0) {
				//				directConceptSet.remove(opConcept);
				//			}
				//			++prevCount;
				//			prevConceptCountHash.insert(opConcept,prevCount);
				//		}
				//		opConLinker = opConLinker->getNext();
				//	}
				//}
				//CBUILDLIST<CConcept*> nextConceptTaggingList;
				//FOREACHIT (CConcept* concept, directConceptSet) {
				//	nextConceptTaggingList.append(concept);
				//}
				//if (nextConceptTaggingList.isEmpty() && nextUsedConceptTag >= lastFreeConceptTag) {
				//	// search next concept for tagging with minimal dependency
				//	cint64 minPrevCount = 0;
				//	CConcept* minPrevConcept = nullptr;
				//	FOREACHHASHIT (CConcept* concept, cint64 prevCount, prevConceptCountHash) {
				//		if (!minPrevConcept || prevCount < minPrevCount && prevCount > 0) {
				//			minPrevCount = prevCount;
				//			minPrevConcept = concept;
				//		}
				//	}
				//	nextConceptTaggingList.append(minPrevConcept);
				//}
				//while (!nextConceptTaggingList.isEmpty()) {
				//	CConcept* taggingConcept = nextConceptTaggingList.takeLast();
				//	taggingConcept->setConceptTag(nextUsedConceptTag--);
				//	mInstallConceptList.append(taggingConcept);
				//	prevConceptCountHash.insert(taggingConcept,0);

				//	FOREACHIT (CConcept* succConcept, succConceptHash.values(taggingConcept)) {
				//		cint64 prevCount = prevConceptCountHash.value(succConcept)-1;
				//		if (prevCount == 0) {
				//			nextConceptTaggingList.append(succConcept);
				//		}
				//		prevConceptCountHash.insert(succConcept,prevCount);
				//	}

				//	if (nextConceptTaggingList.isEmpty() && nextUsedConceptTag >= lastFreeConceptTag) {
				//		// search next concept for tagging with minimal dependency
				//		cint64 minPrevCount = 0;
				//		CConcept* minPrevConcept = nullptr;
				//		FOREACHHASHIT (CConcept* concept, cint64 prevCount, prevConceptCountHash) {
				//			if ((!minPrevConcept || prevCount < minPrevCount) && prevCount > 0) {
				//				minPrevCount = prevCount;
				//				minPrevConcept = concept;
				//			}
				//		}
				//		nextConceptTaggingList.append(minPrevConcept);
				//	}

				//}
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::resortAndInstallConceptsAndRolesAndIndividuals() {
				CConceptVector* conVec = tBox->getConceptVector();
				FOREACHIT (CConcept* concept, mInstallConceptList) {
					CSortedNegLinker<CConcept*>* opConceptLinkerIt = concept->getOperandList();
					opConceptLinkerIt = resortConceptLinkersByTagging(opConceptLinkerIt,concept->getOperandCount());
					concept->setOperandList(opConceptLinkerIt);
					conVec->setData(concept->getConceptTag(),concept);
				}
				CRoleVector* roleVec = rBox->getRoleVector();
				FOREACHIT (CRole* role, mInstallRoleList) {
					CSortedNegLinker<CRole*>* roleLinkerIt = nullptr;

					roleLinkerIt = role->getSuperRoleList();
					roleLinkerIt = resortRoleLinkersByTagging(roleLinkerIt);
					role->setSuperRoleList(roleLinkerIt);

					roleLinkerIt = role->getDisjointRoleList();
					roleLinkerIt = resortRoleLinkersByTagging(roleLinkerIt);
					role->setDisjointRoleList(roleLinkerIt);

					roleLinkerIt = role->getInverseEquivalentRoleList();
					roleLinkerIt = resortRoleLinkersByTagging(roleLinkerIt);
					role->setInverseEquivalentRoleList(roleLinkerIt);

					CSortedNegLinker<CConcept*>* conceptLinkerIt = nullptr;

					conceptLinkerIt = role->getDomainConceptList();
					conceptLinkerIt = resortConceptLinkersByTagging(conceptLinkerIt);
					role->setDomainConceptList(conceptLinkerIt);

					conceptLinkerIt = role->getRangeConceptList();
					conceptLinkerIt = resortConceptLinkersByTagging(conceptLinkerIt);
					role->setRangeConceptList(conceptLinkerIt);

					roleVec->setData(role->getRoleTag(),role);
				}

				CRoleChainVector* roleChainVec = rBox->getRoleChainVector();
				FOREACHIT (CRoleChain* roleChain, mInstallRoleChainList) {
					roleChainVec->setData(roleChain->getRoleChainTag(),roleChain);
				}

				CIndividualVector* indiVec = aBox->getIndividualVector();
				FOREACHIT (CIndividual* indi, mInstallIndividualList) {
					indiVec->setData(indi->getIndividualID(),indi);
				}
				return false;
			}


			CSortedNegLinker<CConcept*>* CConcreteOntologyUpdateBuilder::resortConceptLinkersByTagging(CSortedNegLinker<CConcept*>* conceptLinkers, cint64 linkerCount) {
				if (linkerCount > 100) {
					QMap<cint64,CSortedNegLinker<CConcept*>*> linkerSortMap;
					CSortedNegLinker<CConcept*>* conceptLinkerIt = conceptLinkers;
					while (conceptLinkerIt) {
						CSortedNegLinker<CConcept*>* tmpConceptLinker = conceptLinkerIt;
						conceptLinkerIt = conceptLinkerIt->getNext();
						tmpConceptLinker->setNext(nullptr);
						linkerSortMap.insertMulti(tmpConceptLinker->getData()->getConceptTag(),tmpConceptLinker);
					}
					CSortedNegLinker<CConcept*>* newConceptLinkers = nullptr;
					CSortedNegLinker<CConcept*>* lastNewConceptLinkers = nullptr;
					for (QMap<cint64,CSortedNegLinker<CConcept*>*>::const_iterator it = linkerSortMap.constBegin(), itEnd = linkerSortMap.constEnd(); it != itEnd; ++it) {
						CSortedNegLinker<CConcept*>* tmpConceptLinker = *it;
						if (!newConceptLinkers) {
							lastNewConceptLinkers = newConceptLinkers = tmpConceptLinker;
						} else {
							lastNewConceptLinkers->setNext(tmpConceptLinker);
							lastNewConceptLinkers = tmpConceptLinker;
						}
					}
					return newConceptLinkers;
				} else {
					CSortedNegLinker<CConcept*>* newConceptLinkers = nullptr;
					while (conceptLinkers) {
						CSortedNegLinker<CConcept*>* tmpConceptLinker = conceptLinkers;
						conceptLinkers = conceptLinkers->getNext();
						tmpConceptLinker->setNext(nullptr);
						if (!newConceptLinkers) {
							newConceptLinkers = tmpConceptLinker;
						} else {
							newConceptLinkers = newConceptLinkers->insertSortedNextSorted(tmpConceptLinker);
						}
					}
					return newConceptLinkers;
				}
			}


			CSortedNegLinker<CRole*>* CConcreteOntologyUpdateBuilder::resortRoleLinkersByTagging(CSortedNegLinker<CRole*>* roleLinkers) {
				CSortedNegLinker<CRole*>* newRoleLinkers = nullptr;
				while (roleLinkers) {
					CSortedNegLinker<CRole*>* tmpRoleLinker = roleLinkers;
					roleLinkers = roleLinkers->getNext();
					tmpRoleLinker->setNext(nullptr);
					if (!newRoleLinkers) {
						newRoleLinkers = tmpRoleLinker;
					} else {
						newRoleLinkers = newRoleLinkers->insertSortedNextSorted(tmpRoleLinker);
					}
				}
				return newRoleLinkers;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude