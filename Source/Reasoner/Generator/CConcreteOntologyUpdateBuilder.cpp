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


				mNominalConceptCreationRequired = false;
				mLateIndiNominalConceptCreationList.clear();

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
				mBoxContext = mOntoData->getBoxContext();

				mIndividualNameIndividualMapHash = nullptr;
				mIndexedTriplesAssertionAccessor = mOnto->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (!mIndexedTriplesAssertionAccessor) {
					CConcreteOntology* prevOntology = mOnto;
					while (prevOntology && !mIndexedTriplesAssertionAccessor) {
						prevOntology = prevOntology->getIncrementalRevisionData()->getPreviousOntologyVersion();
						if (prevOntology) {
							mIndexedTriplesAssertionAccessor = prevOntology->getOntologyTriplesData()->getTripleAssertionAccessor();
						}
					}
				}
				if (!mIndexedTriplesAssertionAccessor) {
					mIndividualNameIndividualMapHash = mOnto->getStringMapping()->getIndividualNameIndividualMappingHash(false);
					if (!mIndividualNameIndividualMapHash || mIndividualNameIndividualMapHash->isEmpty()) {
						mIndividualNameIndividualMapHash = nullptr;
					}
				}


				mInitialBuild = mOntoData->isInitialBuild();

				mConstructFlags = mOntoData->getExpressionDataBoxMapping()->getBuildConstructFlags();

				mUpdateClassAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdatedClassAxiomTellOrRetractHash();
				mUpdateObjectPropertyAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdateObjectPropertyAxiomHash();
				mUpdateDataPropertyAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdateDataPropertyAxiomHash();
				mUpdateAssertionAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdateAssertionAxiomHash();

				mClassTermClassAxiomSet = mOntoData->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionSet();
				mClassTermClassAxiomHash = mOntoData->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();


				mDatatypeExpDatatypeHash = mOntoData->getExpressionDataBoxMapping()->getDatatypeExpressionDatatypeHash();
				mDatatypeDatatypeExpHash = mOntoData->getExpressionDataBoxMapping()->getDatatypeDatatypeExpessionHash();



				mObjPropTermObjPropAxiomSet = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomSet();
				mObjPropTermObjPropAxiomHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomHash();


				mDataPropTermDataPropAxiomSet = mOntoData->getExpressionDataBoxMapping()->getDataPropertyTermDataPropertyAxiomSet();
				mDataPropTermDataPropAxiomHash = mOntoData->getExpressionDataBoxMapping()->getDataPropertyTermDataPropertyAxiomHash();


				mIndiTermAssertionAxiomSet = mOntoData->getExpressionDataBoxMapping()->getIndividualTermAssertionAxiomSet();
				mIndiTermAssertionAxiomHash = mOntoData->getExpressionDataBoxMapping()->getIndividualTermAssertionAxiomHash();

				mTopRebuildClassTermExpressionsSet = mOntoData->getExpressionDataBoxMapping()->getTopRebuildClassTermExpressionsSet();
				mTopRebuildObjectPropertyTermExpressionsSet = mOntoData->getExpressionDataBoxMapping()->getTopRebuildObjectPropertyTermExpressionsSet();


				mClassTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
				mConceptClassTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();
				mDataRangeTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getDataRangeTermConceptMappingHash();
				mConceptDataRangeTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptDataRangeTermMappingHash();

				mObjPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
				mRoleObjPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();
				mDataPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash();
				mRoleDataPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleDataPropertyTermMappingHash();

				mIndividulTermIndiHash = mOntoData->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash();
				mIndiIndividulTermHash = mOntoData->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();




				mRoleChainObjPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleChainObjectPropertyTermHash();
				mObjPropTermRoleChainHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermRoleChainHash();

				mLastProcessedChangedAxiom = mOntoData->getExpressionDataBoxMapping()->getLastProcessedChangedAxiom();
				mLastProcessedBuildIndividual = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildIndividual();
				mLastProcessedBuildConcept = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildConcept();
				mLastProcessedBuildObjectRole = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildObjectRole();
				mLastProcessedBuildDataRole = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildDataRole();
				mLastProcessedBuildDataRange = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildDataRange();
				mLastProcessedBuildDatatype = mOntoData->getExpressionDataBoxMapping()->getLastProcessedBuildDatatype();

				mLastProcessedExpression = mOntoData->getExpressionDataBoxMapping()->getLastProcessedExpression();
				mLastProcessedInverseProperty = mOntoData->getExpressionDataBoxMapping()->getLastProcessedInverseProperty();

				mLastBuildedIndividual = mOntoData->getExpressionDataBoxMapping()->getLastBuildedIndividual();
				mLastBuildedConcept = mOntoData->getExpressionDataBoxMapping()->getLastBuildedConcept();
				mLastBuildedObjectRole = mOntoData->getExpressionDataBoxMapping()->getLastBuildedObjectRole();
				mLastBuildedDataRole = mOntoData->getExpressionDataBoxMapping()->getLastBuildedDataRole();
				mLastBuildedDataRange = mOntoData->getExpressionDataBoxMapping()->getLastBuildedDataRange();
				mLastBuildedDatatype = mOntoData->getExpressionDataBoxMapping()->getLastBuildedDatatype();

				mLocExpressionBuildHash = mOntoData->getExpressionDataBoxMapping()->getStructuralExpressionBuildHash();
				mLocExpressionBuildContainerList = mOntoData->getExpressionDataBoxMapping()->getExpressionBuildListContainer();
				mLocInverseObjectPropertyHash = mOntoData->getExpressionDataBoxMapping()->getInverseObjectPropertyHash();

				mLocBuildConceptSet = mOntoData->getExpressionDataBoxMapping()->getBuildedConceptSet();
				mLocBuildObjectRoleSet = mOntoData->getExpressionDataBoxMapping()->getBuildedObjectRoleSet();
				mLocBuildDataRoleSet = mOntoData->getExpressionDataBoxMapping()->getBuildedDataRoleSet();
				mLocBuildDataRangeSet = mOntoData->getExpressionDataBoxMapping()->getBuildedDataRangeSet();
				mLocBuildDatatypeSet = mOntoData->getExpressionDataBoxMapping()->getBuildedDatatypeSet();
				mLocBuildIndividualSet = mOntoData->getExpressionDataBoxMapping()->getBuildedIndividualSet();

				mLocBuildConceptList = mOntoData->getExpressionDataBoxMapping()->getBuildedConceptList();
				mLocBuildObjectRoleList = mOntoData->getExpressionDataBoxMapping()->getBuildedObjectRoleList();
				mLocBuildDataRoleList = mOntoData->getExpressionDataBoxMapping()->getBuildedDataRoleList();
				mLocBuildDataRangeList = mOntoData->getExpressionDataBoxMapping()->getBuildedDataRangeList();
				mLocBuildDatatypeList = mOntoData->getExpressionDataBoxMapping()->getBuildedDatatypeList();
				mLocBuildIndividualList = mOntoData->getExpressionDataBoxMapping()->getBuildedIndividualList();

				mRebuildTopConcept = false;

				mClassGCIExpressionSet.clear();
				mLocalisationSet.clear();

				mBuildingConceptSet.clear();
				mNewBuildedConceptSet.clear();

				mNewBuildedObjectRoleSet.clear();
				mNewBuildedDataRoleSet.clear();
				mBuildingObjectRoleSet.clear();
				mBuildingDataRangeSet.clear();
				mBuildingDatatypeSet.clear();
				mBuildingDataRoleSet.clear();

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
				mNewBuildObjectRoleSet.clear();
				mNewBuildDataRoleSet.clear();
				mNewBuildDataRangeSet.clear();
				mNewBuildDatatypeSet.clear();
				mNewBuildIndividualSet.clear();

				return true;
			}



			bool CConcreteOntologyUpdateBuilder::completeBuilding() {

				mOntoBuild->setNextAxiomNumber(mNextAxiomNumber+mNextMaxAxiomNumberOffset);


				CONTOLOGYAXIOMSET<CAxiomExpression*> newTellUpdatedAxiomSet;
				CONTOLOGYAXIOMSET<CAxiomExpression*> newRetractUpdatedAxiomSet;

				if (!mRetractUpdatedAxiomSet->isEmpty()) {
					mConstructFlags->setRetractionUsed();
				}
				//FOREACHIT (CAxiomExpression* axiomExp, *mRetractUpdatedAxiomSet) {
				//	mConstructFlags->setRetractionUsed();
				//	mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
				//	mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
				//}
				//FOREACHIT (CAxiomExpression* axiomExp, *mTellUpdatedAxiomSet) {
				//	mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
				//	mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
				//}

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
				mExpressionBuildHash = mLocExpressionBuildHash;
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



				if (mLastProcessedBuildObjectRole <= 0) {
					*mLocBuildObjectRoleSet = *mBuildObjectRoleSet;
					*mLocBuildObjectRoleList = *mBuildObjectRoleList;
					mLastProcessedBuildObjectRole = mLocBuildObjectRoleList->count();
				} else {
					while (mLastProcessedBuildObjectRole < mBuildObjectRoleList->count()) {
						CObjectPropertyTermExpression* buildExp((*mBuildObjectRoleList)[mLastProcessedBuildObjectRole++]);
						mLocBuildObjectRoleList->append(buildExp);
						mLocBuildObjectRoleSet->insert(buildExp);
					}
				}
				mBuildObjectRoleSet = mLocBuildObjectRoleSet;
				mBuildObjectRoleList = mLocBuildObjectRoleList;




				if (mLastProcessedBuildDataRole <= 0) {
					*mLocBuildDataRoleSet = *mBuildDataRoleSet;
					*mLocBuildDataRoleList = *mBuildDataRoleList;
					mLastProcessedBuildDataRole = mLocBuildDataRoleList->count();
				} else {
					while (mLastProcessedBuildDataRole < mBuildDataRoleList->count()) {
						CDataPropertyTermExpression* buildExp((*mBuildDataRoleList)[mLastProcessedBuildDataRole++]);
						mLocBuildDataRoleList->append(buildExp);
						mLocBuildDataRoleSet->insert(buildExp);
					}
				}
				mBuildDataRoleSet = mLocBuildDataRoleSet;
				mBuildDataRoleList = mLocBuildDataRoleList;





				if (mLastProcessedBuildDataRange <= 0) {
					*mLocBuildDataRangeSet = *mBuildDataRangeSet;
					*mLocBuildDataRangeList = *mBuildDataRangeList;
					mLastProcessedBuildDataRange = mLocBuildDataRangeList->count();
				} else {
					while (mLastProcessedBuildDataRange < mBuildDataRangeList->count()) {
						CDataRangeTermExpression* buildExp((*mBuildDataRangeList)[mLastProcessedBuildDataRange++]);
						mLocBuildDataRangeList->append(buildExp);
						mLocBuildDataRangeSet->insert(buildExp);
					}
				}
				mBuildDataRangeSet = mLocBuildDataRangeSet;
				mBuildDataRangeList = mLocBuildDataRangeList;





				if (mLastProcessedBuildDatatype <= 0) {
					*mLocBuildDatatypeSet = *mBuildDatatypeSet;
					*mLocBuildDatatypeList = *mBuildDatatypeList;
					mLastProcessedBuildDatatype = mLocBuildDatatypeList->count();
				} else {
					while (mLastProcessedBuildDatatype < mBuildDatatypeList->count()) {
						CDatatypeExpression* buildExp((*mBuildDatatypeList)[mLastProcessedBuildDatatype++]);
						mLocBuildDatatypeList->append(buildExp);
						mLocBuildDatatypeSet->insert(buildExp);
					}
				}
				mBuildDatatypeSet = mLocBuildDatatypeSet;
				mBuildDatatypeList = mLocBuildDatatypeList;



				//mUpdateClassAxiomHash->reserve(mTellAxiomSet->size());
				//mUpdateObjectPropertyAxiomHash->reserve(mTellAxiomSet->size());
				//mUpdateDataPropertyAxiomHash->reserve(mTellAxiomSet->size());
				//mDeclarationAxiomSet->reserve(mTellAxiomSet->size());
				//mUpdateAssertionAxiomHash->reserve(mTellAxiomSet->size());

				FOREACHIT (CAxiomExpression* axiomExp, newTellUpdatedAxiomSet) {
					CClassAxiomExpression* classAxiomExp = dynamic_cast<CClassAxiomExpression*>(axiomExp);
					if (classAxiomExp) {
						mUpdateClassAxiomHash->insert(classAxiomExp,true);
					}
					CObjectPropertyAxiomExpression* objPropAxiomExp = dynamic_cast<CObjectPropertyAxiomExpression*>(axiomExp);
					if (objPropAxiomExp) {
						mUpdateObjectPropertyAxiomHash->insert(objPropAxiomExp,true);
					}
					CDataPropertyAxiomExpression* dataPropAxiomExp = dynamic_cast<CDataPropertyAxiomExpression*>(axiomExp);
					if (dataPropAxiomExp) {
						mUpdateDataPropertyAxiomHash->insert(dataPropAxiomExp,true);
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
					CDataPropertyAxiomExpression* dataPropAxiomExp = dynamic_cast<CDataPropertyAxiomExpression*>(axiomExp);
					if (dataPropAxiomExp) {
						mUpdateDataPropertyAxiomHash->insert(dataPropAxiomExp,false);
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




				//mIndiTermAssertionAxiomSet->reserve(mUpdateAssertionAxiomHash->size());
				//mNewBuildIndividualSet.reserve(mUpdateAssertionAxiomHash->size());
				//mIndiTermAssertionAxiomHash->reserve(mUpdateAssertionAxiomHash->size());
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
					} else if (expType == CBuildExpression::BETDATAPROPERTYASSERTION) {
						CDataPropertyAssertionExpression* propAssExp = (CDataPropertyAssertionExpression*)assAxiomExp;
						CIndividualTermExpression* indiTermExp = propAssExp->getIndividualTermExpression();
						mNewBuildIndividualSet.insert(indiTermExp);
						if (!mIndiTermAssertionAxiomSet->contains(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp))) {
							mIndiTermAssertionAxiomSet->insert(QPair<CIndividualTermExpression*,CAssertionAxiomExpression*>(indiTermExp,assAxiomExp));
							if (tellAxiom) {
								mIndiTermAssertionAxiomHash->insertMulti(indiTermExp,assAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION) {
						mConstructFlags->setNonELConstructUsed();
						CNegativeDataPropertyAssertionExpression* propAssExp = (CNegativeDataPropertyAssertionExpression*)assAxiomExp;
						CIndividualTermExpression* indiTermExp = propAssExp->getIndividualTermExpression();
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
							mNewBuildObjectRoleSet.insert(objPropTermExp);
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
							mNewBuildObjectRoleSet.insert(objPropTermExp);
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
							mNewBuildObjectRoleSet.insert(objPropTermExp);
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
							mNewBuildObjectRoleSet.insert(objPropTermExp);
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
						mNewBuildObjectRoleSet.insert(objPropTermExp1);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp1,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp1,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp1,objPropAxiomExp);
							}
						}
						mNewBuildObjectRoleSet.insert(objPropTermExp2);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp2,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp2,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp2,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETFUNCTIONALOBJECTPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CFunctionalObjectPropertyExpression* funcObjPropExp = (CFunctionalObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = funcObjPropExp->getFunctionalObjectPropertyExpression();
						mNewBuildObjectRoleSet.insert(objPropTermExp);
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
						mNewBuildObjectRoleSet.insert(objPropTermExp);
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
						mNewBuildObjectRoleSet.insert(objPropTermExp);
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
						mNewBuildObjectRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETREFLEXIVEPROPERTY) {
						CReflexiveObjectPropertyExpression* objPropExpAxiom = (CReflexiveObjectPropertyExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildObjectRoleSet.insert(objPropTermExp);
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
						mNewBuildObjectRoleSet.insert(objPropTermExp);
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
						mNewBuildObjectRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETOBJECTPROPERTYDOMAIN) {
						CObjectPropertyDomainExpression* objPropExpAxiom = (CObjectPropertyDomainExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildObjectRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETOBJECTPROPERTYRANGE) {
						CObjectPropertyRangeExpression* objPropExpAxiom = (CObjectPropertyRangeExpression*)objPropAxiomExp;
						CObjectPropertyTermExpression* objPropTermExp = objPropExpAxiom->getObjectPropertyTermExpression();
						mNewBuildObjectRoleSet.insert(objPropTermExp);
						if (!mObjPropTermObjPropAxiomSet->contains(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp))) {
							mObjPropTermObjPropAxiomSet->insert(QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>(objPropTermExp,objPropAxiomExp));
							if (tellAxiom) {
								mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp,objPropAxiomExp);
							}
						}
					}
				}





				FOREACHHASHIT (CDataPropertyAxiomExpression* dataPropAxiomExp, bool tellAxiom, *mUpdateDataPropertyAxiomHash) {
					CBuildExpression::ExpressionType expType = dataPropAxiomExp->getType();
					if (expType == CBuildExpression::BETEQUIVALENTDATAPROPERTIES) {
						// all associated properties has to be updated
						CEquivalentDataPropertiesExpression* eqDataPropExp = (CEquivalentDataPropertiesExpression*)dataPropAxiomExp;
						FOREACHIT (CDataPropertyTermExpression* dataPropTermExp, *eqDataPropExp->getDataPropertyTermExpressionList()) {
							mNewBuildDataRoleSet.insert(dataPropTermExp);
							if (!mDataPropTermDataPropAxiomSet->contains(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp))) {
								mDataPropTermDataPropAxiomSet->insert(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp));
								if (tellAxiom) {
									mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETDISJOINTDATAPROPERTIES) {
						mConstructFlags->setNonELConstructUsed();
						// all associated properties has to be updated
						CDisjointDataPropertiesExpression* eqDisjPropExp = (CDisjointDataPropertiesExpression*)dataPropAxiomExp;
						FOREACHIT (CDataPropertyTermExpression* dataPropTermExp, *eqDisjPropExp->getDataPropertyTermExpressionList()) {
							mNewBuildDataRoleSet.insert(dataPropTermExp);
							if (!mDataPropTermDataPropAxiomSet->contains(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp))) {
								mDataPropTermDataPropAxiomSet->insert(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp));
								if (tellAxiom) {
									mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiomExp);
								}
							}
						}
					} else if (expType == CBuildExpression::BETSUBDATAPROPERTYOF) {
						CSubDataPropertyOfExpression* subDataPropExp = (CSubDataPropertyOfExpression*)dataPropAxiomExp;
						CDataPropertyTermExpression* dataPropTermExp = subDataPropExp->getSubDataPropertyTermExpression();
						mNewBuildDataRoleSet.insert(dataPropTermExp);
						if (!mDataPropTermDataPropAxiomSet->contains(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp))) {
							mDataPropTermDataPropAxiomSet->insert(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp));
							if (tellAxiom) {
								mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiomExp);
							}
						}

					} else if (expType == CBuildExpression::BETFUNCTIONALDATAPROPERTY) {
						mConstructFlags->setNonELConstructUsed();
						CFunctionalDataPropertyExpression* funcDataPropExp = (CFunctionalDataPropertyExpression*)dataPropAxiomExp;
						CDataPropertyTermExpression* dataPropTermExp = funcDataPropExp->getFunctionalDataPropertyExpression();
						mNewBuildDataRoleSet.insert(dataPropTermExp);
						if (!mDataPropTermDataPropAxiomSet->contains(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp))) {
							mDataPropTermDataPropAxiomSet->insert(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp));
							if (tellAxiom) {
								mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiomExp);
							}
						}

					} else if (expType == CBuildExpression::BETDATAPROPERTYDOMAIN) {
						CDataPropertyDomainExpression* dataPropExpAxiom = (CDataPropertyDomainExpression*)dataPropAxiomExp;
						CDataPropertyTermExpression* dataPropTermExp = dataPropExpAxiom->getDataPropertyTermExpression();
						mNewBuildDataRoleSet.insert(dataPropTermExp);
						if (!mDataPropTermDataPropAxiomSet->contains(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp))) {
							mDataPropTermDataPropAxiomSet->insert(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp));
							if (tellAxiom) {
								mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiomExp);
							}
						}
					} else if (expType == CBuildExpression::BETDATAPROPERTYRANGE) {
						CDataPropertyRangeExpression* dataPropExpAxiom = (CDataPropertyRangeExpression*)dataPropAxiomExp;
						CDataPropertyTermExpression* dataPropTermExp = dataPropExpAxiom->getDataPropertyTermExpression();
						mNewBuildDataRoleSet.insert(dataPropTermExp);
						if (!mDataPropTermDataPropAxiomSet->contains(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp))) {
							mDataPropTermDataPropAxiomSet->insert(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiomExp));
							if (tellAxiom) {
								mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiomExp);
							}
						}
					}
				}



				while (mLastBuildedConcept < mBuildConceptList->size() || mLastBuildedObjectRole < mBuildObjectRoleList->size() || mLastBuildedDataRole < mBuildDataRoleList->size() || mLastBuildedIndividual < mBuildIndividualList->size()
						|| mLastBuildedDatatype < mBuildDatatypeList->size() || mLastBuildedDataRange < mBuildDataRangeList->size()
						|| !mNewBuildConceptSet.isEmpty() || !mNewBuildObjectRoleSet.isEmpty() || !mNewBuildDataRoleSet.isEmpty() || !mNewBuildIndividualSet.isEmpty() || !mNewBuildDataRangeSet.isEmpty() || !mNewBuildDatatypeSet.isEmpty()) {


					while (mLastBuildedConcept < mBuildConceptList->size()) {
						mNewBuildConceptSet.insert((*mBuildConceptList)[mLastBuildedConcept++]);
					}
					while (mLastBuildedObjectRole < mBuildObjectRoleList->size()) {
						mNewBuildObjectRoleSet.insert((*mBuildObjectRoleList)[mLastBuildedObjectRole++]);
					}
					while (mLastBuildedDataRole < mBuildDataRoleList->size()) {
						mNewBuildDataRoleSet.insert((*mBuildDataRoleList)[mLastBuildedDataRole++]);
					}
					while (mLastBuildedIndividual < mBuildIndividualList->size()) {
						mNewBuildIndividualSet.insert((*mBuildIndividualList)[mLastBuildedIndividual++]);
					}

					while (mLastBuildedDatatype < mBuildDatatypeList->size()) {
						mNewBuildDatatypeSet.insert((*mBuildDatatypeList)[mLastBuildedDatatype++]);
					}

					while (mLastBuildedDataRange < mBuildDataRangeList->size()) {
						mNewBuildDataRangeSet.insert((*mBuildDataRangeList)[mLastBuildedDataRange++]);
					}

					// construct concepts, roles, individuals
					// build all class expressions

					while (!mNewBuildObjectRoleSet.isEmpty()) {
						mBuildingObjectRoleSet = mNewBuildObjectRoleSet;
						mNewBuildObjectRoleSet.clear();

						FOREACHIT (CObjectPropertyTermExpression* objTermExp, mBuildingObjectRoleSet) {
							if (!mNewBuildedObjectRoleSet.contains(objTermExp)) {
								mNewBuildedObjectRoleSet.insert(objTermExp);

								// build role for object property axioms
								buildObjectPropertyRole(objTermExp);
							}
						}
					}



					while (!mNewBuildDataRoleSet.isEmpty()) {
						mBuildingDataRoleSet = mNewBuildDataRoleSet;
						mNewBuildDataRoleSet.clear();

						FOREACHIT (CDataPropertyTermExpression* dataTermExp, mBuildingDataRoleSet) {
							if (!mNewBuildedDataRoleSet.contains(dataTermExp)) {
								mNewBuildedDataRoleSet.insert(dataTermExp);

								// build role for data property axioms
								buildDataPropertyRole(dataTermExp);
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




					while (!mNewBuildDatatypeSet.isEmpty()) {
						mBuildingDatatypeSet = mNewBuildDatatypeSet;
						mNewBuildDatatypeSet.clear();

						FOREACHIT (CDatatypeExpression* datatypeExp, mBuildingDatatypeSet) {
							if (!mNewBuildedDatatypeSet.contains(datatypeExp)) {
								mNewBuildedDatatypeSet.insert(datatypeExp);

								// build datatype
								buildDatatype(datatypeExp);
							}
						}
					}




					while (!mNewBuildDataRangeSet.isEmpty()) {
						mBuildingDataRangeSet = mNewBuildDataRangeSet;
						mNewBuildDataRangeSet.clear();

						FOREACHIT (CDataRangeTermExpression* dataRangeExp, mBuildingDataRangeSet) {
							if (!mNewBuildedDataRangeSet.contains(dataRangeExp)) {
								mNewBuildedDataRangeSet.insert(dataRangeExp);

								CBuildExpression::ExpressionType expType = dataRangeExp->getType();
								CConcept* concept = getConceptForDataRangeTerm(dataRangeExp,true);

								if (expType == CBuildExpression::BETDATATYPE) {
									mConstructFlags->setNonELConstructUsed();
									CDatatypeExpression* datatypeExp = (CDatatypeExpression*)dataRangeExp;
									CDatatype* datatype = getDatatypeForDatatypeExpression(datatypeExp);
									concept->setOperatorCode(CCDATATYPE);
									concept->setDatatype(datatype);

								} else if (expType == CBuildExpression::BETDATALITERAL) {
									mConstructFlags->setNonELConstructUsed();
									CDataLiteralExpression* litExp = (CDataLiteralExpression*)dataRangeExp;
									CDataLiteral* dataLiteral = getDataLiteralForLiteralExpression(litExp);
									concept->setOperatorCode(CCDATALITERAL);
									concept->setDataLiteral(dataLiteral);
								} else if (expType == CBuildExpression::BETDATACOMPLEMENTOF) {
									CDataComplementOfExpression* dataExp = (CDataComplementOfExpression*)dataRangeExp;
									mConstructFlags->setNonELConstructUsed();
									CDataRangeTermExpression* opDataRangeTermExp = dataExp->getDataRangeTermExpression();
									setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExp);
									concept->setOperatorCode(CCNOT);
								} else if (expType == CBuildExpression::BETDATAUNIONOF) {
									CDataUnionOfExpression* dataExp = (CDataUnionOfExpression*)dataRangeExp;
									mConstructFlags->setNonELConstructUsed();
									CEXPRESSIONLIST<CDataRangeTermExpression*>* opDataRangeTermExpList = dataExp->getDataRangeTermExpressionList();
									setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExpList);
									concept->setOperatorCode(CCOR);
								} else if (expType == CBuildExpression::BETDATAONEOF) {
									CDataOneOfExpression* dataExp = (CDataOneOfExpression*)dataRangeExp;
									mConstructFlags->setNonELConstructUsed();
									CEXPRESSIONLIST<CDataRangeTermExpression*>* opDataRangeTermExpList = dataExp->getDataRangeTermExpressionList();
									setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExpList);
									concept->setOperatorCode(CCOR);
								} else if (expType == CBuildExpression::BETDATAINTERSECTIONOF) {
									CDataIntersectionOfExpression* dataExp = (CDataIntersectionOfExpression*)dataRangeExp;
									CEXPRESSIONLIST<CDataRangeTermExpression*>* opDataRangeTermExpList = dataExp->getDataRangeTermExpressionList();
									setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExpList);
									concept->setOperatorCode(CCAND);								
								} else if (expType == CBuildExpression::BETDATATYPERESTRICTION) {
									mConstructFlags->setNonELConstructUsed();
									CDatatypeRestrictionExpression* dataExp = (CDatatypeRestrictionExpression*)dataRangeExp;
									CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* facetExpList = dataExp->getDataFacetRestrictionExpressionList();
									CDatatypeExpression* datatypeExpression = dataExp->getDatatypeExpression();
									if (facetExpList->count() == 1) {
										CDataFacetRestrictionExpression* facetRestrictionExp = facetExpList->first();
										CDataLiteralExpression* litExp = facetRestrictionExp->getDataLiteralExpression();
										CDataFacetExpression* facetExp = facetRestrictionExp->getDataFacetExpression();
										CDataLiteral* dataLiteral = getDataLiteralForLiteralExpression(litExp);
										CDatatype* datatype = getDatatypeForDatatypeExpression(datatypeExpression);
										cint64 facetDataCode = 0;
										const QString& facetIRI = facetExp->getName();
										if (facetIRI == PREFIX_MIN_INCLUSIVE_FACET) {
											facetDataCode = CDFC_MIN_INCLUSIVE;
										} else if (facetIRI == PREFIX_MIN_EXCLUSIVE_FACET) {
											facetDataCode = CDFC_MIN_EXCLUSIVE;
										} else if (facetIRI == PREFIX_MAX_INCLUSIVE_FACET) {
											facetDataCode = CDFC_MAX_INCLUSIVE;
										} else if (facetIRI == PREFIX_MAX_EXCLUSIVE_FACET) {
											facetDataCode = CDFC_MAX_EXCLUSIVE;
										} else if (facetIRI == PREFIX_MAX_LENGTH_FACET) {
											facetDataCode = CDFC_MAX_LENGTH_INCLUSIVE;
										} else if (facetIRI == PREFIX_MIN_LENGTH_FACET) {
											facetDataCode = CDFC_MIN_LENGTH_INCLUSIVE;
										} else if (facetIRI == PREFIX_LENGTH_FACET) {
											facetDataCode = CDFC_LENGTH;
										} else if (facetIRI == PREFIX_PATTERN_FACET) {
											facetDataCode = CDFC_PATTERN;
										} else if (facetIRI == PREFIX_LANGUAGE_RANGE_FACET) {
											facetDataCode = CDFC_LANGUAGE_RANGE;
										}
										concept->setParameter(facetDataCode);
										concept->setDatatype(datatype);
										concept->setDataLiteral(dataLiteral);
										concept->setOperatorCode(CCDATARESTRICTION);								
									} else {
										FOREACHIT (CDataFacetRestrictionExpression* facetExp, *facetExpList) {
											CDatatypeRestrictionExpression* separatedDataExp = getDatatypeRestriction(datatypeExpression,CEXPRESSIONLIST<CDataFacetRestrictionExpression*>()<<facetExp);
											setConceptOperandsFromDataRangeTerms(concept,separatedDataExp);
										}
										concept->setOperatorCode(CCAND);
									}
								}

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

								} else if (expType == CBuildExpression::BETDATASOMEVALUEFROM) {
									CDataSomeValuesFromExpression* dataSomeValueExp = (CDataSomeValuesFromExpression*)classTermExp;
									CDataPropertyTermExpression* dataPropPropExp = dataSomeValueExp->getDataPropertyTermExpression();									
									setConceptRoleFromDataPropertyTerm(concept,dataPropPropExp);
									CDataRangeTermExpression* dataRangeTermExp = dataSomeValueExp->getDataRangeTermExpression();
									if (dataRangeTermExp == nullptr) {
										dataRangeTermExp = mTopDataRangeExpression;
									}
									setConceptOperandsFromDataRangeTerms(concept,dataRangeTermExp);
									concept->setOperatorCode(CCSOME);
								} else if (expType == CBuildExpression::BETDATAALLVALUEFROM) {
									CDataAllValuesFromExpression* dataAllValueExp = (CDataAllValuesFromExpression*)classTermExp;
									CDataPropertyTermExpression* dataPropPropExp = dataAllValueExp->getDataPropertyTermExpression();									
									setConceptRoleFromDataPropertyTerm(concept,dataPropPropExp);
									CDataRangeTermExpression* dataRangeTermExp = dataAllValueExp->getDataRangeTermExpression();
									setConceptOperandsFromDataRangeTerms(concept,dataRangeTermExp);
									concept->setOperatorCode(CCALL);




								} else if (expType == CBuildExpression::BETDATAMINCARDINALITY) {
									CDataMinCardinalityExpression* dataExp = (CDataMinCardinalityExpression*)classTermExp;
									CDataPropertyTermExpression* opDataPropExp = dataExp->getDataPropertyTermExpression();
									setConceptRoleFromDataPropertyTerm(concept,opDataPropExp);
									cint64 cardinality = dataExp->getCardinality();
									CDataRangeTermExpression* opDataRangeTermExp = dataExp->getDataRangeTermExpression();
									if (cardinality == 1) {
										if (opDataRangeTermExp == nullptr) {
											opDataRangeTermExp = mTopDataRangeExpression;
										}
										setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExp);
										concept->setOperatorCode(CCSOME);
									} else {
										if (opDataRangeTermExp != mTopDataRangeExpression) {
											setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExp);
										}
										mConstructFlags->setNonELConstructUsed();
										concept->setParameter(cardinality);
										concept->setOperatorCode(CCATLEAST);
									}
								} else if (expType == CBuildExpression::BETDATAMAXCARDINALITY) {
									mConstructFlags->setNonELConstructUsed();
									CDataMaxCardinalityExpression* dataExp = (CDataMaxCardinalityExpression*)classTermExp;
									CDataPropertyTermExpression* opDataPropExp = dataExp->getDataPropertyTermExpression();
									setConceptRoleFromDataPropertyTerm(concept,opDataPropExp);
									CDataRangeTermExpression* opDataRangeTermExp = dataExp->getDataRangeTermExpression();
									cint64 cardinality = dataExp->getCardinality();
									if (cardinality == 0) {
										if (opDataRangeTermExp == nullptr) {
											opDataRangeTermExp = mTopDataRangeExpression;
										}
										CDataComplementOfExpression* compExp = getDataComplementOf(opDataRangeTermExp);
										setConceptOperandsFromDataRangeTerms(concept,compExp);
										concept->setOperatorCode(CCALL);
									} else {
										if (opDataRangeTermExp != mTopDataRangeExpression) {
											setConceptOperandsFromDataRangeTerms(concept,opDataRangeTermExp);
										}
										concept->setParameter(cardinality);
										concept->setOperatorCode(CCATMOST);
									}
								} else if (expType == CBuildExpression::BETDATAEXACTCARDINALITY) {
									mConstructFlags->setNonELConstructUsed();
									CDataExactCardinalityExpression* dataExp = (CDataExactCardinalityExpression*)classTermExp;
									CDataPropertyTermExpression* opDataPropExp = dataExp->getDataPropertyTermExpression();
									CDataRangeTermExpression* opDataRangeTermExp = dataExp->getDataRangeTermExpression();
									cint64 cardinality = dataExp->getCardinality();
									CDataMaxCardinalityExpression* maxCardExp = getDataMaxCardinality(opDataPropExp,opDataRangeTermExp,cardinality);
									CDataMinCardinalityExpression* minCardExp = getDataMinCardinality(opDataPropExp,opDataRangeTermExp,cardinality);
									CObjectIntersectionOfExpression* maxMinIntersect = getObjectIntersectionOf(CEXPRESSIONLIST<CClassTermExpression*>()<<maxCardExp<<minCardExp);
									setConceptRoleFromDataPropertyTerm(concept,opDataPropExp);
									setConceptOperandsFromClassTerms(concept,maxMinIntersect);
									concept->setParameter(cardinality);
									concept->setOperatorCode(CCAND);

								} else if (expType == CBuildExpression::BETDATAHASVALUE) {
									mConstructFlags->setNonELConstructUsed();
									CDataHasValueExpression* dataExp = (CDataHasValueExpression*)classTermExp;
									CDataPropertyTermExpression* opDataPropExp = dataExp->getDataPropertyTermExpression();
									CDataRangeTermExpression* dataRangeExp = dataExp->getDataRangeTermExpression();
									setConceptRoleFromDataPropertyTerm(concept,opDataPropExp);
									setConceptOperandsFromDataRangeTerms(concept,dataRangeExp);
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
								} else if (expType == CBuildExpression::BETNOMINALINDIVIDUALVARIABLE) {
									mConstructFlags->setNonELConstructUsed();
									mConstructFlags->setNominalSchemaUsed();
									mNominalConceptCreationRequired = true;
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
										concept->setOperatorCode(CCNOMINAL);
										setConceptNominalIndividualFromIndividualTerm(concept,indiTermExp);
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

						if (mNominalConceptCreationRequired && !mLateIndiNominalConceptCreationList.isEmpty()) {
							for (QList<CIndividualTermExpression*>::const_iterator it = mLateIndiNominalConceptCreationList.constBegin(), itEnd = mLateIndiNominalConceptCreationList.constEnd(); it != itEnd; ++it) {
								CIndividualTermExpression* indiTermExp(*it);
								CIndividual* individual = getIndividualForIndividualTerm(indiTermExp,true);
								CClassTermExpression* nominalAssClassTermExp = getObjectOneOf(indiTermExp);
								setIndividualAssertionNominalFromClassTerm(individual,nominalAssClassTermExp,false);
							}
							mLateIndiNominalConceptCreationList.clear();
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
								if (!mNewBuildedObjectRoleSet.contains(rebuildObjectPropertyTermExp) && !mBuildingObjectRoleSet.contains(rebuildObjectPropertyTermExp)) {
									mNewBuildObjectRoleSet.insert(rebuildObjectPropertyTermExp);
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
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildObjectRole(mLastProcessedBuildObjectRole);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildDataRole(mLastProcessedBuildDataRole);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildDataRange(mLastProcessedBuildDataRange);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedBuildDatatype(mLastProcessedBuildDatatype);

				mOntoData->getExpressionDataBoxMapping()->setLastProcessedInverseProperty(mLastProcessedInverseProperty);
				mOntoData->getExpressionDataBoxMapping()->setLastProcessedExpression(mLastProcessedExpression);

				mOntoData->getExpressionDataBoxMapping()->setLastBuildedIndividual(mLastBuildedIndividual);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedConcept(mLastBuildedConcept);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedObjectRole(mLastBuildedObjectRole);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedDataRole(mLastBuildedDataRole);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedDataRange(mLastBuildedDataRange);
				mOntoData->getExpressionDataBoxMapping()->setLastBuildedDatatype(mLastBuildedDatatype);

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

				//CClassTermExpression* nominalAssClassTermExp = nullptr; 
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
								if (classTermExp->getType() == CBuildExpression::BETOBJECTONEOF) {
									CObjectOneOfExpression* objectOneOfExp = (CObjectOneOfExpression*)classTermExp;
									CEXPRESSIONLIST<CIndividualTermExpression*>* indiTermExpList = objectOneOfExp->getIndividualTermExpressionList();
									if (indiTermExpList->count() == 1) {
										CIndividualTermExpression* indiTerm = indiTermExpList->first();
										if (indiTerm == indiTermExp) {
											nominalAssClass = true;
										}
									}
								}
							} else if (expType == CBuildExpression::BETOBJECTPROPERTYASSERTION) {	
								CObjectPropertyAssertionExpression* propAssExp = (CObjectPropertyAssertionExpression*)assAxiom;
								CObjectPropertyTermExpression* objPropTermExp = propAssExp->getObjectPropertyTermExpression();
								CIndividualTermExpression* destIndiTermExp = propAssExp->getSecondIndividualTermExpression();
								setIndividualAssertionRoleFromObjectPropertyTerm(individual,objPropTermExp,destIndiTermExp);
								//CClassTermExpression* classTermExp = getObjectHasValue(objPropTermExp,destIndiTermExp);
								//setIndividualAssertionConceptFromClassTerm(individual,classTermExp,false);

							} else if (expType == CBuildExpression::BETDATAPROPERTYASSERTION) {	
								CDataPropertyAssertionExpression* dataAssExp = (CDataPropertyAssertionExpression*)assAxiom;
								CDataPropertyTermExpression* dataPropTermExp = dataAssExp->getDataPropertyTermExpression();
								CDataLiteralExpression* dataLiteralExp = dynamic_cast<CDataLiteralExpression*>(dataAssExp->getDataLiteralTermExpression());
								setIndividualAssertionDataFromDataLiteralTerm(individual, dataPropTermExp, dataLiteralExp);

							} else if (expType == CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION) {	
								CNegativeDataPropertyAssertionExpression* dataAssExp = (CNegativeDataPropertyAssertionExpression*)assAxiom;
								CDataPropertyTermExpression* dataPropTermExp = dataAssExp->getDataPropertyTermExpression();
								CDataLiteralExpression* dataLiteralExp = dynamic_cast<CDataLiteralExpression*>(dataAssExp->getDataLiteralTermExpression());
								CClassTermExpression* classTermExp = getDataHasValue(dataPropTermExp,dataLiteralExp);
								if (dataLiteralExp) {
									setIndividualAssertionConceptFromClassTerm(individual, classTermExp, true);
								}

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
					if (mConstructFlags->isNominalSchemaUsed()) {
						CClassTermExpression* nominalAssClassTermExp = getObjectOneOf(indiTermExp);
						setIndividualAssertionNominalFromClassTerm(individual,nominalAssClassTermExp,false);
					} else {
						mLateIndiNominalConceptCreationList.append(indiTermExp);
					}
				}

				//if (!nominalAssClass && mConstructFlags->isNominalSchemaUsed()) {
				//	CClassTermExpression* nominalAssClassTermExp = getObjectOneOf(indiTermExp);
				//	setIndividualAssertionNominalFromClassTerm(individual,nominalAssClassTermExp,false);
				//}
				if (!individual->isAnonymousIndividual()) {
					CConceptAssertionLinker* opConLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(mMemManager);
					opConLinker->initNegLinker(tBox->getIndividualTriggerConcept(),false);
					individual->addAssertionConceptLinker(opConLinker);
				}

				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildDatatype(CDatatypeExpression* datatypeExp) {
				return true;
			}


			bool CConcreteOntologyUpdateBuilder::buildDataPropertyRole(CDataPropertyTermExpression* dataPropTermExp) {


				CBUILDLIST<CDataPropertyAxiomExpression*> dataPropAccAxiomList(mDataPropTermDataPropAxiomHash->values(dataPropTermExp));
				mDataPropTermDataPropAxiomHash->remove(dataPropTermExp);


				CRole* role = getRoleForDataPropertyTerm(dataPropTermExp,true);



				FOREACHIT (CDataPropertyAxiomExpression* dataPropAxiom, dataPropAccAxiomList) {
					if (!mRetractUpdatedAxiomSet->contains(dataPropAxiom)) {
						mDataPropTermDataPropAxiomHash->insertMulti(dataPropTermExp,dataPropAxiom);
						CBuildExpression::ExpressionType expType = dataPropAxiom->getType();
						if (expType == CBuildExpression::BETSUBDATAPROPERTYOF) {
							CSubDataPropertyOfExpression* dataPropAxiomExp = (CSubDataPropertyOfExpression*)dataPropAxiom;
							CDataPropertyTermExpression* superDataPropertyExpression = dataPropAxiomExp->getSuperDataPropertyTermExpression();
							if (superDataPropertyExpression != mTopDataPropExpression) {
								CRole* superRole = getRoleForDataPropertyTerm(superDataPropertyExpression);
								if (!role->hasSuperRole(superRole)) {
									CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
									roleLinker->init(superRole,false);
									role->addSuperRoleLinker(roleLinker);
								}
							}
						} else if (expType == CBuildExpression::BETEQUIVALENTDATAPROPERTIES) {
							CEquivalentDataPropertiesExpression* dataPropAxiomExp = (CEquivalentDataPropertiesExpression*)dataPropAxiom;
							CEXPRESSIONLIST<CDataPropertyTermExpression*>* eqDataPropertyExpressionList = dataPropAxiomExp->getDataPropertyTermExpressionList();
							FOREACHIT (CDataPropertyTermExpression* eqDataPropTermExp, *eqDataPropertyExpressionList) {
								if (dataPropTermExp != eqDataPropTermExp) {
									CRole* eqRole = getRoleForDataPropertyTerm(eqDataPropTermExp);
									if (!role->hasEquivalentRole(eqRole)) {
										CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
										roleLinker->init(eqRole,false);
										role->addEquivalentRoleLinker(roleLinker);
									}
								}
							}
						} else if (expType == CBuildExpression::BETDISJOINTDATAPROPERTIES) {
							CDisjointDataPropertiesExpression* dataPropAxiomExp = (CDisjointDataPropertiesExpression*)dataPropAxiom;
							CEXPRESSIONLIST<CDataPropertyTermExpression*>* disDataPropertyExpressionList = dataPropAxiomExp->getDataPropertyTermExpressionList();
							FOREACHIT (CDataPropertyTermExpression* disDataPropTermExp, *disDataPropertyExpressionList) {
								if (dataPropTermExp != disDataPropTermExp) {
									CRole* disRole = getRoleForDataPropertyTerm(disDataPropTermExp);
									if (!role->hasDisjointRole(disRole)) {
										CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
										roleLinker->init(disRole,false);
										role->addDisjointRoleLinker(roleLinker);
									}
								}
							}
						} else if (expType == CBuildExpression::BETDATAPROPERTYDOMAIN) {
							CDataPropertyDomainExpression* dataPropAxiomExp = (CDataPropertyDomainExpression*)dataPropAxiom;
							CClassTermExpression* domainExp = dataPropAxiomExp->getClassTermExpression();
							CConcept* domainCon = getConceptForClassTerm(domainExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(domainCon, domainCon->hasMappingNegation());
							role->addDomainConceptLinker(conceptLinker);
						} else if (expType == CBuildExpression::BETDATAPROPERTYRANGE) {
							CDataPropertyRangeExpression* dataPropAxiomExp = (CDataPropertyRangeExpression*)dataPropAxiom;
							CDataRangeTermExpression* rangeExp = dataPropAxiomExp->getDataRangeTermExpression();
							CConcept* rangeCon = getConceptForDataRangeTerm(rangeExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(rangeCon, rangeCon->hasMappingNegation());
							role->addRangeConceptLinker(conceptLinker);
						} else if (expType == CBuildExpression::BETFUNCTIONALDATAPROPERTY) {
							CFunctionalDataPropertyExpression* dataPropAxiomExp = (CFunctionalDataPropertyExpression*)dataPropAxiom;
							CDataMaxCardinalityExpression* funcCardExp = getDataMaxCardinality(dataPropTermExp,mTopDataPropExpression,1);
							CConcept* domainCon = getConceptForClassTerm(funcCardExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(domainCon, domainCon->hasMappingNegation());
							role->addDomainConceptLinker(conceptLinker);
							role->setFunctional(true);
						}
					} else {
						mDataPropTermDataPropAxiomSet->remove(QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>(dataPropTermExp,dataPropAxiom));
					}
				}
				return true;
			}




			bool CConcreteOntologyUpdateBuilder::buildObjectPropertyRole(CObjectPropertyTermExpression* objPropTermExp) {
				if (mTopRebuildObjectPropertyTermExpressionsSet->contains(objPropTermExp)) {
					if (!mNewBuildedObjectRoleSet.contains(mTopObjPropExpression) && !mBuildingObjectRoleSet.contains(mTopObjPropExpression)) {
						mNewBuildObjectRoleSet.insert(mTopObjPropExpression);
					}
				}

				CBUILDLIST<CObjectPropertyAxiomExpression*> objPropAccAxiomList(mObjPropTermObjPropAxiomHash->values(objPropTermExp));
				mObjPropTermObjPropAxiomHash->remove(objPropTermExp);

				CObjectPropertyTermExpression* inverseObjPropTermExp = mInverseObjectPropertyHash->value(objPropTermExp,nullptr);

				CRole* role = nullptr;
				if (objPropAccAxiomList.isEmpty() && inverseObjPropTermExp) {
					role = mObjPropTermRoleHash->value(objPropTermExp);
					CRole* invRole = mObjPropTermRoleHash->value(inverseObjPropTermExp);
					if (!role && invRole) {
						role = invRole->getInverseRole();
						if (role) {
							mObjPropTermRoleHash->insert(objPropTermExp, role);
						}
					}
				}

				if (!role) {
					role = getRoleForObjectPropertyTerm(objPropTermExp, true);
				}

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

				FOREACHIT(CObjectPropertyAxiomExpression* objPropAxiom, objPropAccAxiomList) {
					if (!mRetractUpdatedAxiomSet->contains(objPropAxiom)) {
						mObjPropTermObjPropAxiomHash->insertMulti(objPropTermExp, objPropAxiom);
						CBuildExpression::ExpressionType expType = objPropAxiom->getType();
						if (expType == CBuildExpression::BETINVERSEOBJECTPROPERTIES) {
							CInverseObjectPropertiesExpression* objPropAxiomExp = (CInverseObjectPropertiesExpression*)objPropAxiom;
							CObjectPropertyTermExpression* invObjPropTermExp = nullptr;
							if (objPropTermExp == objPropAxiomExp->getFirstObjectPropertyTermExpression()) {
								invObjPropTermExp = objPropAxiomExp->getSecondObjectPropertyTermExpression();
							} else {
								invObjPropTermExp = objPropAxiomExp->getFirstObjectPropertyTermExpression();
							}
							mInverseObjectPropertyHash->insert(objPropTermExp, invObjPropTermExp);
							mInverseObjectPropertyHash->insert(invObjPropTermExp, objPropTermExp);
							CRole* invRole = getRoleForObjectPropertyTerm(invObjPropTermExp);
							if (!role->hasInverseRole(invRole)) {
								CSortedNegLinker<CRole*>* roleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(mMemManager);
								roleLinker->init(invRole, true);
								role->addInverseRoleLinker(roleLinker);
							}
						}
					}
				}

				FOREACHIT (CObjectPropertyAxiomExpression* objPropAxiom, objPropAccAxiomList) {
					if (!mRetractUpdatedAxiomSet->contains(objPropAxiom)) {
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
						} else if (expType == CBuildExpression::BETOBJECTPROPERTYDOMAIN) {
							CObjectPropertyDomainExpression* objPropAxiomExp = (CObjectPropertyDomainExpression*)objPropAxiom;
							CClassTermExpression* domainExp = objPropAxiomExp->getClassTermExpression();
							CConcept* domainCon = getConceptForClassTerm(domainExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(domainCon,domainCon->hasMappingNegation());
							role->addDomainConceptLinker(conceptLinker);
						} else if (expType == CBuildExpression::BETOBJECTPROPERTYRANGE) {
							CObjectPropertyRangeExpression* objPropAxiomExp = (CObjectPropertyRangeExpression*)objPropAxiom;
							CClassTermExpression* rangeExp = objPropAxiomExp->getClassTermExpression();
							CConcept* rangeCon = getConceptForClassTerm(rangeExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(rangeCon, rangeCon->hasMappingNegation());
							role->addRangeConceptLinker(conceptLinker);
						} else if (expType == CBuildExpression::BETFUNCTIONALOBJECTPROPERTY) {
							CFunctionalObjectPropertyExpression* objPropAxiomExp = (CFunctionalObjectPropertyExpression*)objPropAxiom;
							CObjectMaxCardinalityExpression* funcCardExp = getObjectMaxCardinality(objPropTermExp,mTopClassExpression,1);
							CConcept* domainCon = getConceptForClassTerm(funcCardExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(domainCon, domainCon->hasMappingNegation());
							role->addDomainConceptLinker(conceptLinker);
							role->setFunctional(true);
						} else if (expType == CBuildExpression::BETINVERSEFUNCTIONALPROPERTY) {
							CInverseFunctionalObjectPropertyExpression* objPropAxiomExp = (CInverseFunctionalObjectPropertyExpression*)objPropAxiom;
							CObjectMaxCardinalityExpression* funcCardExp = getObjectMaxCardinality(getCorrectedInverseObjectPropertyOf(objPropTermExp),mTopClassExpression,1);
							CConcept* rangeCon = getConceptForClassTerm(funcCardExp);
							CSortedNegLinker<CConcept*>* conceptLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
							conceptLinker->init(rangeCon, rangeCon->hasMappingNegation());
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
								role->setInverseRole(role);
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
				bool sameDisCallClassOcc = false;
				FOREACHIT (CClassTermExpression* disClassExp2, disClassExpList) {
					if (disCallClassExp != disClassExp2 || sameDisCallClassOcc) {
						buildConceptSubClassInclusion(disCallClassExp,disClassExp2,true);
					} else {
						sameDisCallClassOcc = true;
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
							break;
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
				if (mInitialBuild && (classTermExp == mTopClassExpression || classTermExp == mBottomClassExpression)) {
					forceLocalisation = true;
				}
				concept = mClassTermConceptHash->value(classTermExp);
				if (concept && concept->getTag() == -1) {
					forceLocalisation = false;
				}
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
						concept->setTag(-1);
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

			CDataLiteral* CConcreteOntologyUpdateBuilder::getDataLiteralForLiteralExpression(CDataLiteralExpression* dataLiteralExp, bool forceLocalisation) {
				CDatatypeExpression* datatypeExp = dataLiteralExp->getDatatypeExpression();
				CDataLexicalValueExpression* lexDatValueExp = dataLiteralExp->getDataLexicalValueExpression();
				CDatatype* datatype = getDatatypeForDatatypeExpression(datatypeExp);
				CDataLiteral* dataLiteral = CObjectParameterizingAllocator<CDataLiteral,CContext*>::allocateAndConstructAndParameterize(mMemManager, mBoxContext);
				dataLiteral->initDataLiteral(lexDatValueExp->getName(),datatype);
				return dataLiteral;
			}


			CDatatype* CConcreteOntologyUpdateBuilder::getDatatypeForDatatypeExpression(CDatatypeExpression* datatypeExp, bool forceLocalisation) {
				CDatatype* datatype = nullptr;
				datatype = mDatatypeExpDatatypeHash->value(datatypeExp);
				bool forceCreation = false;
				if (datatype && forceLocalisation) {
					if (!mLocalisationSet.contains(datatypeExp)) {
						forceCreation = true;
					}
				}
				if (!datatype || forceCreation) {
					if (forceLocalisation) {
						mLocalisationSet.insert(datatypeExp);
					}
					datatype = CObjectAllocator<CDatatype>::allocateAndConstruct(mMemManager);
					datatype->initDatatype(datatypeExp->getName());
					CDatatypeVector* datatypeVec = tBox->getDatatypeVector();
					cint64 nextDatatypeTag = datatypeVec->getItemCount();
					datatype->setDatatypeTag(nextDatatypeTag);
					datatypeVec->setData(nextDatatypeTag,datatype);
					mDatatypeExpDatatypeHash->insert(datatypeExp,datatype);
					mDatatypeDatatypeExpHash->insert(datatype,datatypeExp);

				}
				return datatype;
			}




			CConcept* CConcreteOntologyUpdateBuilder::getConceptForDataRangeTerm(CDataRangeTermExpression* dataRangeExp, bool forceLocalisation) {
				CConcept* concept = nullptr;
				concept = mDataRangeTermConceptHash->value(dataRangeExp);
				bool forceCreation = false;
				if (concept && concept->getTag() == -1) {
					forceLocalisation = false;
				}
				if (concept && forceLocalisation) {
					if (!mLocalisationSet.contains(dataRangeExp)) {
						forceCreation = true;
					}
				}
				if (!concept || forceCreation) {
					CConcept* prevConcept = concept;
					if (forceLocalisation) {
						mLocalisationSet.insert(dataRangeExp);
					}
					concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemManager);
					concept->initConcept();
					if (prevConcept) {
						cint64 prevConceptTag = prevConcept->getConceptTag();
						concept->initTag(prevConceptTag);
						cint64 prevConceptOpCode = prevConcept->getOperatorCode();
						mInstallConceptList.append(concept);
					} else {
						mTaggingConceptList.append(concept);
						concept->setTag(-1);
					}
					mDataRangeTermConceptHash->insert(dataRangeExp,concept);
					mConceptDataRangeTermHash->insert(concept,dataRangeExp);
				}
				return concept;
			}




			CIndividual* CConcreteOntologyUpdateBuilder::getIndividualForIndividualTerm(CIndividualTermExpression* indiTermExp, bool forceLocalisation) {
				CIndividual* indi = nullptr;
				indi = mIndividulTermIndiHash->value(indiTermExp);
				if (!indi && mIndividualNameIndividualMapHash) {
					CNamedIndividualExpression* namedIndiExp = (CNamedIndividualExpression*)indiTermExp;
					indi = mIndividualNameIndividualMapHash->value(namedIndiExp->getName());
				}
				bool forceCreation = false;
				if (indi && indi->getTag() == -1) {
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

					QString indiName;
					if (indiTermExp->getType() == CBuildExpression::BETNAMEDINDIVIDUAL) {
						CNamedIndividualExpression* namedIndiExp = (CNamedIndividualExpression*)indiTermExp;
						indiName = namedIndiExp->getName();
					} else if (indiTermExp->getType() == CBuildExpression::BETANONYMOUSINDIVIDUAL) {
						CAnonymousIndividualExpression* anonymousIndiExp = (CAnonymousIndividualExpression*)indiTermExp;
						indiName = anonymousIndiExp->getName();
					}

					cint64 useIndiTag = -1;
					if (prevIndi) {
						useIndiTag = prevIndi->getIndividualID();
					} else if (mIndexedTriplesAssertionAccessor) {
						useIndiTag = mIndexedTriplesAssertionAccessor->getIndividualId(indiName);
					} 

					if (useIndiTag >= 0) {
						indi->initTag(useIndiTag);
						mInstallIndividualList.append(indi);
					} else {
						mTaggingIndividualSet.append(indi);
						indi->setTag(-1);
					}
					mIndividulTermIndiHash->insert(indiTermExp,indi);
					mIndiIndividulTermHash->insert(indi,indiTermExp);

					updateName(indi, indiName);
					if (indiTermExp->getType() == CBuildExpression::BETANONYMOUSINDIVIDUAL) {
						indi->setAnonymousIndividual(true);
					}


				}
				return indi;
			}




			CRole* CConcreteOntologyUpdateBuilder::getRoleForDataPropertyTerm(CDataPropertyTermExpression* dataPropTermExp, bool forceLocalisation) {
				CRole* role = nullptr;
				role = mDataPropTermRoleHash->value(dataPropTermExp);
				bool forceCreation = false;
				if (mInitialBuild && dataPropTermExp == mTopDataPropExpression) {
					mConstructFlags->setTopDataRoleUsed();
					forceLocalisation = true;
				} else if (mInitialBuild && dataPropTermExp == mBottomDataPropExpression) {
					mConstructFlags->setBottomDataRoleUsed();
					forceLocalisation = true;
				}
				if (role && role->getTag() == -1) {
					forceLocalisation = false;
				}
				if (role && forceLocalisation) {
					if (!mLocalisationSet.contains(dataPropTermExp)) {
						forceCreation = true;
					}
				}
				if (!role || forceCreation) {
					CRole* prevRole = role;
					if (forceLocalisation) {
						mLocalisationSet.insert(dataPropTermExp);
					}
					role = CObjectAllocator<CRole>::allocateAndConstruct(mMemManager);
					role->initRole();
					if (prevRole) {
						role->initTag(prevRole->getRoleTag());
						mInstallRoleList.append(role);
					} else {
						mTaggingRoleSet.insert(role);
						role->setTag(-1);
					}
					role->setDataRole(true);
					mDataPropTermRoleHash->insert(dataPropTermExp,role);
					mRoleDataPropTermHash->insert(role,dataPropTermExp);

					if (dataPropTermExp->getType() == CBuildExpression::BETDATAPROPERTY) {
						// update name
						CDataPropertyExpression* dataPropExp = (CDataPropertyExpression*)dataPropTermExp;
						QString propName = dataPropExp->getName();
						updateName(role,propName);
					}

				}
				return role;
			}


			CRole* CConcreteOntologyUpdateBuilder::getRoleForObjectPropertyTerm(CObjectPropertyTermExpression* objPropTermExp, bool forceLocalisation) {
				CRole* role = nullptr;
				role = mObjPropTermRoleHash->value(objPropTermExp);
				bool forceCreation = false;
				if (mInitialBuild && objPropTermExp == mTopObjPropExpression) {
					mConstructFlags->setTopObjectRoleUsed();
					forceLocalisation = true;
				} else if (mInitialBuild && objPropTermExp == mBottomObjPropExpression) {
					mConstructFlags->setBottomObjectRoleUsed();
					forceLocalisation = true;
				}
				if (role && role->getTag() == -1) {
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
						role->setTag(-1);
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


			bool CConcreteOntologyUpdateBuilder::setConceptNominalIndividualFromIndividualTerm(CConcept* concept, CIndividualTermExpression* indiTermExp) {
				CIndividual* opIndi = getIndividualForIndividualTerm(indiTermExp,false);
				concept->setNominalIndividual(opIndi);
				if (!opIndi->getIndividualNominalConcept()) {
					opIndi->setIndividualNominalConcept(concept);
				}
				return true;
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

			bool CConcreteOntologyUpdateBuilder::setConceptRoleFromDataPropertyTerm(CConcept* concept, CDataPropertyTermExpression* dataPropertyTermExp) {
				CRole* opRole = getRoleForDataPropertyTerm(dataPropertyTermExp);
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
						bool opConNegation = negate;
						CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
						if (opConcept->hasMappingNegation()) {
							opConNegation = !opConNegation;
						}
						opConLinker->init(opConcept, opConNegation);
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

			bool CConcreteOntologyUpdateBuilder::setIndividualAssertionDataFromDataLiteralTerm(CIndividual* individual, CDataPropertyTermExpression* dataPropTermExp, CDataLiteralExpression* dataLitExp) {
				CRole* role = getRoleForDataPropertyTerm(dataPropTermExp);
				CDataLiteral* dataLiteral = getDataLiteralForLiteralExpression(dataLitExp);
				CDataAssertionLinker* dataAssLinker = CObjectAllocator< CDataAssertionLinker >::allocateAndConstruct(mMemManager);
				dataAssLinker->initDataAssertionLinker(role, dataLiteral);
				individual->addAssertionDataLinker(dataAssLinker);
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
						if (opConcept->hasMappingNegation()) {
							negate = !negate;
						}
						opConLinker->init(opConcept,negate);
						concept->addOperandLinker(opConLinker);
						concept->incOperandCount();
					//}
					return true;
				}
				return false;
			}


			bool CConcreteOntologyUpdateBuilder::setConceptOperandsFromDataRangeTerms(CConcept* concept, CDataRangeTermExpression* dataRangeTermExp, bool negate) {
				if (dataRangeTermExp) {
					CConcept* opConcept = getConceptForDataRangeTerm(dataRangeTermExp);
					//if (!concept->hasOperandConcept(opConcept,negate)) {
					CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
					if (opConcept->hasMappingNegation()) {
						negate = !negate;
					}
					opConLinker->init(opConcept,negate);
					concept->addOperandLinker(opConLinker);
					concept->incOperandCount();
					//}
					return true;
				}
				return false;
			}

			bool CConcreteOntologyUpdateBuilder::setConceptOperandsFromDataRangeTerms(CConcept* concept, CEXPRESSIONLIST<CDataRangeTermExpression*>* dataRangeTermExpList, bool negate) {
				FOREACHIT (CDataRangeTermExpression* dataRangeTermExp, *dataRangeTermExpList) {
					CConcept* opConcept = getConceptForDataRangeTerm(dataRangeTermExp);
					//if (!concept->hasOperandConcept(opConcept,negate)) {
					CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
					if (opConcept->hasMappingNegation()) {
						negate = !negate;
					}
					opConLinker->init(opConcept,negate);
					concept->addOperandLinker(opConLinker);
					concept->incOperandCount();
					//}
				}
				return true;
			}

			bool CConcreteOntologyUpdateBuilder::setConceptOperands(CConcept* concept, CConcept* operandConcept, bool negate) {
				//if (!concept->hasOperandConcept(operandConcept,negate)) {
					CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemManager);
					if (operandConcept->hasMappingNegation()) {
						negate = !negate;
					}
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



				class CIndividualTaggingDependingItem : public CIndividualData {
				public:
					CIndividualTaggingDependingItem() {
						mDependent = false;
					}
				public:
					bool mDependent;
				};


				cint64 taggingCount = mTaggingIndividualSet.count();
				CIndividualTaggingDependingItem* taggingItemVec = new CIndividualTaggingDependingItem[taggingCount];

				cint64 itemIdx = 0;
				for (CBUILDLIST<CIndividual*>::const_iterator it1 = mTaggingIndividualSet.constBegin(), it1End = mTaggingIndividualSet.constEnd(); it1 != it1End; ++it1) {
					CIndividual* individual = *it1;
					if (!individual->hasIndividualData()) {
						CIndividualTaggingDependingItem* taggingItem = &taggingItemVec[itemIdx++];
						individual->setIndividualData(taggingItem);
					}
				}

				CBUILDLIST<CIndividual*> dependedIndividualList;
				CBUILDLIST<CIndividual*> directIndividualList;
				CBUILDLIST<CIndividual*> currentIndividualList;

				for (CBUILDLIST<CIndividual*>::const_iterator it1 = mTaggingIndividualSet.constBegin(), it1End = mTaggingIndividualSet.constEnd(); it1 != it1End; ++it1) {
					CIndividual* individual = *it1;
					for (CRoleAssertionLinker* roleAssLinkIt = individual->getAssertionRoleLinker(); roleAssLinkIt; roleAssLinkIt = roleAssLinkIt->getNext()) {
						CIndividual* otIndi = roleAssLinkIt->getIndividual();
						if (otIndi->getIndividualID() <= 0) {
							CIndividualTaggingDependingItem* taggingItem = (CIndividualTaggingDependingItem*)otIndi->getIndividualData();
							if (taggingItem && !taggingItem->mDependent) {
								taggingItem->mDependent = true;
								dependedIndividualList.append(otIndi);
							}
						}
					}
				}
				for (CBUILDLIST<CIndividual*>::const_iterator it1 = mTaggingIndividualSet.constBegin(), it1End = mTaggingIndividualSet.constEnd(); it1 != it1End; ++it1) {
					CIndividual* individual = *it1;
					CIndividualTaggingDependingItem* taggingItem = (CIndividualTaggingDependingItem*)individual->getIndividualData();
					if (!taggingItem->mDependent) {
						directIndividualList.append(individual);
					}
				}
				CBUILDLIST<CIndividual*> taggingIndividualList;
				while (!directIndividualList.isEmpty() || !dependedIndividualList.isEmpty()) {
					CIndividual* individual = nullptr;
					if (currentIndividualList.isEmpty() && !directIndividualList.isEmpty()) {
						individual = directIndividualList.takeFirst();
						currentIndividualList.append(individual);
					}
					if (currentIndividualList.isEmpty() && !dependedIndividualList.isEmpty()) {
						CIndividual* individual = dependedIndividualList.takeFirst();
						CIndividualTaggingDependingItem* taggingItem = (CIndividualTaggingDependingItem*)individual->getIndividualData();
						if (taggingItem && taggingItem->mDependent) {
							taggingItem->mDependent = false;
							currentIndividualList.append(individual);
						}
					}
					CBUILDLIST<CIndividual*> nextTaggingConceptList;
					while (!currentIndividualList.isEmpty()) {
						individual = currentIndividualList.takeFirst();
						nextTaggingConceptList.append(individual);

						for (CRoleAssertionLinker* roleAssLinkIt = individual->getAssertionRoleLinker(); roleAssLinkIt; roleAssLinkIt = roleAssLinkIt->getNext()) {
							CIndividual* otIndi = roleAssLinkIt->getIndividual();
							if (otIndi->getIndividualID() <= 0) {
								CIndividualTaggingDependingItem* taggingItem = (CIndividualTaggingDependingItem*)otIndi->getIndividualData();
								if (taggingItem && taggingItem->mDependent) {
									taggingItem->mDependent = false;
									currentIndividualList.append(otIndi);
								}
							}
						}
					}
					for (CBUILDLIST<CIndividual*>::const_iterator it1 = nextTaggingConceptList.constBegin(), it1End = nextTaggingConceptList.constEnd(); it1 != it1End; ++it1) {
						taggingIndividualList.append(*it1);
					}

				}
				for (CBUILDLIST<CIndividual*>::const_iterator it1 = taggingIndividualList.constBegin(), it1End = taggingIndividualList.constEnd(); it1 != it1End; ++it1) {
					CIndividual* taggingIndividual = *it1;
					taggingIndividual->setIndividualID(nextIndiID++);
					mInstallIndividualList.append(taggingIndividual);
				}

				for (CBUILDLIST<CIndividual*>::const_iterator it1 = mTaggingIndividualSet.constBegin(), it1End = mTaggingIndividualSet.constEnd(); it1 != it1End; ++it1) {
					CIndividual* individual = *it1;
					individual->setIndividualData(nullptr);
				}

				delete [] taggingItemVec;




				//FOREACHIT (CIndividual* indi, mTaggingIndividualSet) {

				//	QString indiName = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
				//	indiName = indiName.replace("file:/C:/temp/s","");
				//	cint64 indiNameID = indiName.toLongLong();
				//	indi->setIndividualID(indiNameID);

				//	//indi->setIndividualID(nextIndiID++);
				//	mInstallIndividualList.append(indi);
				//}
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
						if (opConcept && opConcept->getConceptTag() == -1) {
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
							if (opConcept && opConcept->getConceptTag() == -1) {
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