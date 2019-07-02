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

#include "CConcreteOntologyMergingBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyMergingBuilder::CConcreteOntologyMergingBuilder(CConcreteOntology* baseConcreteOntology, CConcreteOntology* mergingConcreteOntology, CConcreteOntology* prevConcreteOntology) : CConcreteOntologyBuildDataUpdater(mergingConcreteOntology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcretOntologyMergingBuilder",logTr("ConcretOntologyMergingBuilder initialized."),this);

				mMergingConcreteOntology = mergingConcreteOntology;
				mBaseConcreteOntology = baseConcreteOntology;
				mPrevConcreteOntology = prevConcreteOntology;

				initJumpingHash();
			}

			CConcreteOntologyMergingBuilder::~CConcreteOntologyMergingBuilder() {
			}





			bool CConcreteOntologyMergingBuilder::initializeBuilding() {
				CConcreteOntologyBuildDataUpdater::initializeBuilding();

				mTellAxiomSet = mOntoBuild->getTellAxiomSet();
				mRetractAxiomSet = mOntoBuild->getRetractAxiomSet();

				mTellUpdatedAxiomSet = mOntoBuild->getUpdatedTellAxiomSet();
				mRetractUpdatedAxiomSet = mOntoBuild->getUpdatedRetractAxiomSet();

				mChangeAxiomList = mOntoBuild->getChangeAxiomList();
				mChangeUpdatedAxiomList = mOntoBuild->getUpdatedChangeAxiomList();

				cint64 nextNewAxiom = mPrevConcreteOntology->getBuildData()->getChangeAxiomList()->count();
				while (nextNewAxiom < mChangeUpdatedAxiomList->count()) {
					QPair<CAxiomExpression*,bool> changedAxiomPair((*mChangeUpdatedAxiomList)[nextNewAxiom++]);
					mMergeChangeAxiomList.append( QPair<CAxiomExpression*,bool>(changedAxiomPair) );
				}

				cint64 nextNewPrefix = mPrevConcreteOntology->getStringMapping()->getPrefixList()->count();
				while (nextNewPrefix < mPrefixList->count()) {
					CNamePrefix* namePrefix = (*mPrefixList)[nextNewPrefix++];
					mMergePrefixList.append( namePrefix );
				}

				COntologyStringMapping* baseOntoStrings = mBaseConcreteOntology->getStringMapping();
				COntologyBuildData* baseOntoBuild = mBaseConcreteOntology->getBuildData();

				mTopClassExpression = baseOntoBuild->getTopClassExpression();
				mBottomClassExpression = baseOntoBuild->getBottomClassExpression();

				mTopObjPropExpression = baseOntoBuild->getTopObjectPropertyExpression();
				mBottomObjPropExpression = baseOntoBuild->getBottomObjectPropertyExpression();

				*mDeclarationAxiomSet = *baseOntoBuild->getDeclarationAxiomSet();

				*mBuildConceptSet = *baseOntoBuild->getBuildConceptSet();
				*mBuildObjectRoleSet = *baseOntoBuild->getBuildObjectRoleSet();
				*mBuildIndividualSet = *baseOntoBuild->getBuildIndividualSet();

				*mBuildConceptList = *baseOntoBuild->getBuildConceptList();
				*mBuildObjectRoleList = *baseOntoBuild->getBuildObjectRoleList();
				*mBuildIndividualList = *baseOntoBuild->getBuildIndividualList();

				*mInverseObjectPropertyHash = *baseOntoBuild->getInverseObjectPropertyHash();
				*mInverseObjectPropertyList = *baseOntoBuild->getInverseObjectPropertyList();

				*mExpressionBuildHash = *baseOntoBuild->getStructuralExpressionBuildHash();

				*mClassBuildHash = *baseOntoBuild->getClassEntityBuildHash();
				*mObjectPropertyBuildHash = *baseOntoBuild->getObjectPropertyEntityBuildHash();
				*mIndividualBuildHash = *baseOntoBuild->getIndividualEntityBuildHash();
				*mAnoIndividualBuildHash = *baseOntoBuild->getAnonymousIndividualBuildHash();
				*mNominalIndividualVariableBuildHash = *baseOntoBuild->getNominalIndividualVariableBuildHash();
				*mNamedIndividualVariableBuildHash = *baseOntoBuild->getNamedIndividualVariableBuildHash();
				*mAnonymousIndividualVariableBuildHash = *baseOntoBuild->getAnonymousIndividualVariableBuildHash();

				*mAbbreviatedNamePrefixMapHash = *baseOntoStrings->getAbbreviatedNamePrefixHash();
				*mNamePrefixMapHash = *baseOntoStrings->getNamePrefixHash();
				*mPrefixList = *baseOntoStrings->getPrefixList();

				*mIndividualVariableIDHash = *baseOntoBuild->getIndividualVariableIDHash();

				*mTellAxiomSet = *baseOntoBuild->getTellAxiomSet();
				*mRetractAxiomSet = *baseOntoBuild->getRetractAxiomSet();

				*mTellUpdatedAxiomSet = *baseOntoBuild->getUpdatedTellAxiomSet();
				*mRetractUpdatedAxiomSet = *baseOntoBuild->getUpdatedRetractAxiomSet();

				*mChangeAxiomList = *baseOntoBuild->getChangeAxiomList();
				*mChangeUpdatedAxiomList = *baseOntoBuild->getUpdatedChangeAxiomList();


				return true;
			}


			CMergeExpressionStackObject* CConcreteOntologyMergingBuilder::createMergeExpressionStackObject(CBuildExpression* expression) {
				CMergeExpressionStackObject* mergeExpStackObj = new CMergeExpressionStackObject(expression);
				mMergeExpContainer.append(mergeExpStackObj);
				const CMergeExpressionFunctionPair& mergeExpFuncPair(mMergeExpFunctionJumpHash.value((cint64)expression->getType()));
				mergeExpStackObj->mEndFunction = mergeExpFuncPair.mEndFunction;
				mergeExpStackObj->mStartFunction = mergeExpFuncPair.mStartFunction;
				if (mergeExpStackObj->mStartFunction) {
					FStartMergeExpressionFunction startFunction = mergeExpStackObj->mStartFunction;
					(this->*startFunction)(mergeExpStackObj);							
				}
				return mergeExpStackObj;
			}


			CBuildExpression* CConcreteOntologyMergingBuilder::handleMergeEpxressionStack(CBuildExpression* expression) {
				CBuildExpression* newMergeExpStackObj = nullptr;


				CMergeExpressionStackObject* nextMergeExpStackObj = createMergeExpressionStackObject(expression);
				mMergeExpStack.push(nextMergeExpStackObj);

				while (!mMergeExpStack.isEmpty()) {
					nextMergeExpStackObj = mMergeExpStack.top();


					if (nextMergeExpStackObj->hasProcessingExpressions()) {
						CBuildExpression* nextExpression = nextMergeExpStackObj->mProcessingExpList.takeFirst();
						mMergeExpStack.push(createMergeExpressionStackObject(nextExpression));
					} else {
						nextMergeExpStackObj = mMergeExpStack.pop();
						if (nextMergeExpStackObj->mEndFunction) {
							FEndMergeExpressionFunction endFunction = nextMergeExpStackObj->mEndFunction;
							(this->*endFunction)(nextMergeExpStackObj);		
						}
						CBuildExpression* replacedExpression = nextMergeExpStackObj->mReplaceExpression;
						if (!mMergeExpStack.isEmpty()) {
							nextMergeExpStackObj = mMergeExpStack.top();
							nextMergeExpStackObj->addReplacedBuildExpression(replacedExpression);
						} else {
							newMergeExpStackObj = replacedExpression;
						}
					}
				}

				return newMergeExpStackObj;
			}



			bool CConcreteOntologyMergingBuilder::mergingBuilding() {
				for (CMAPPINGLIST<CNamePrefix*>::const_iterator it = mMergePrefixList.constBegin(), itEnd = mMergePrefixList.constEnd(); it != itEnd; ++it) {
					CNamePrefix* namePrefix(*it);
					mAbbreviatedNamePrefixMapHash->insert(namePrefix->getAbbreviatedPrefixString(),namePrefix);
					mNamePrefixMapHash->insert(namePrefix->getPrefixString(),namePrefix);
				}

				for (CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >::const_iterator it = mMergeChangeAxiomList.constBegin(), itEnd = mMergeChangeAxiomList.constEnd(); it != itEnd; ++it) {
					QPair<CAxiomExpression*,bool> changeAxiomPair(*it);
					CAxiomExpression* axiomExp(changeAxiomPair.first);
					bool axiomAdded(changeAxiomPair.second);

					CAxiomExpression* newAxiomExp = (CAxiomExpression*)handleMergeEpxressionStack(axiomExp);
					if (axiomAdded) {
						tellOntologyAxiom(newAxiomExp);
					} else {
						retractOntologyAxiom(newAxiomExp);
					}

					mNextAxiomNumber++;
				}
				return true;
			}


			bool CConcreteOntologyMergingBuilder::completeBuilding() {


				FOREACHIT (CAxiomExpression* axiomExp, *mRetractUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
				}
				FOREACHIT (CAxiomExpression* axiomExp, *mTellUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
				}

				mOntoBuild->setNextAxiomNumber(mNextAxiomNumber+mNextMaxAxiomNumberOffset);

				return true;
			}



			bool CConcreteOntologyMergingBuilder::tellOntologyAxiom(CAxiomExpression* axiom) {
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

			bool CConcreteOntologyMergingBuilder::retractOntologyAxiom(CAxiomExpression* axiom) {
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

			CConcreteOntologyMergingBuilder* CConcreteOntologyMergingBuilder::initJumpingHash() {
				mMergeExpFunctionJumpHash.insert((cint64)CClassExpression::BETCLASS,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseClassStart,&CConcreteOntologyMergingBuilder::parseClassEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectIndividualVariableExpression::BETNOMINALINDIVIDUALVARIABLE, CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseNominalIndividualVariableStart, &CConcreteOntologyMergingBuilder::parseNominalIndividualVariableEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectIndividualVariableExpression::BETNAMEDINDIVIDUALVARIABLE, CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseNamedIndividualVariableStart, &CConcreteOntologyMergingBuilder::parseNamedIndividualVariableEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectIndividualVariableExpression::BETDATAVALUEVARIABLE, CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseDataValueVariableStart, &CConcreteOntologyMergingBuilder::parseDataValueVariableEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectIndividualVariableExpression::BETDATALITERALVARIABLE, CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseDataLiteralVariableStart, &CConcreteOntologyMergingBuilder::parseDataLiteralVariableEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectIndividualVariableExpression::BETANONYMOUSINDIVIDUALVARIABLE, CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseAnonymousIndividualVariableStart, &CConcreteOntologyMergingBuilder::parseAnonymousIndividualVariableEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CEquivalentClassesExpression::BETEQUIVALENTCLASSES,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseEquivalentClassesStart,&CConcreteOntologyMergingBuilder::parseEquivalentClassesEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CSubClassOfExpression::BETESUBCLASSOF,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseSubClassOfStart,&CConcreteOntologyMergingBuilder::parseSubClassOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CDisjointClassesExpression::BETDISJOINTCLASSES,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseDisjointClassesStart,&CConcreteOntologyMergingBuilder::parseDisjointClassesEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CDisjointUnionExpression::BETDISJOINTUNION,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseDisjointUnionStart,&CConcreteOntologyMergingBuilder::parseDisjointUnionEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectComplementOfExpression::BETOBJECTCOMPLEMENTOF,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectComplementOfStart,&CConcreteOntologyMergingBuilder::parseObjectComplementOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectIntersectionOfExpression::BETOBJECTINTERSECTION,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectIntersectionOfStart,&CConcreteOntologyMergingBuilder::parseObjectIntersectionOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectUnionOfExpression::BETOBJECTUNION,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectUnionOfStart,&CConcreteOntologyMergingBuilder::parseObjectUnionOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectMaxCardinalityExpression::BETOBJECTMAXCARDINALITY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectMaxCardinalityStart,&CConcreteOntologyMergingBuilder::parseObjectMaxCardinalityEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectMinCardinalityExpression::BETOBJECTMINCARDINALITY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectMinCardinalityStart,&CConcreteOntologyMergingBuilder::parseObjectMinCardinalityEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectExactlyCardinalityExpression::BETOBJECTEXACTLYCARDINALITY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectExactlyCardinalityStart,&CConcreteOntologyMergingBuilder::parseObjectExactlyCardinalityEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectAllValuesFromExpression::BETOBJECTALLVALUEFROM,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectAllValuesFromStart,&CConcreteOntologyMergingBuilder::parseObjectAllValuesFromEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectSomeValuesFromExpression::BETOBJECTSOMEVALUEFROM,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectSomeValuesFromStart,&CConcreteOntologyMergingBuilder::parseObjectSomeValuesFromEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectOneOfExpression::BETOBJECTONEOF,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectOneOfStart,&CConcreteOntologyMergingBuilder::parseObjectOneOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectHasValueExpression::BETOBJECTHASVALUE,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectHasValueStart,&CConcreteOntologyMergingBuilder::parseObjectHasValueEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectHasSelfExpression::BETOBJECTHASSELF,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectHasSelfStart,&CConcreteOntologyMergingBuilder::parseObjectHasSelfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectPropertyExpression::BETOBJECTPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectProbertyStart,&CConcreteOntologyMergingBuilder::parseObjectProbertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CSubObjectPropertyOfExpression::BETSUBOBJECTPROPERTYOF,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseSubObjectPropertyOfStart,&CConcreteOntologyMergingBuilder::parseSubObjectPropertyOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectPropertyDomainExpression::BETOBJECTPROPERTYDOMAIN,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectPropertyDomainStart,&CConcreteOntologyMergingBuilder::parseObjectPropertyDomainEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectPropertyRangeExpression::BETOBJECTPROPERTYRANGE,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectPropertyRangeStart,&CConcreteOntologyMergingBuilder::parseObjectPropertyRangeEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CInverseObjectPropertiesExpression::BETINVERSEOBJECTPROPERTIES,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseInverseObjectPropertiesStart,&CConcreteOntologyMergingBuilder::parseInverseObjectPropertiesEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CTransetiveObjectPropertyExpression::BETTRANSITIVEOBJECTPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseTransetiveObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseTransetiveObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CFunctionalObjectPropertyExpression::BETFUNCTIONALOBJECTPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseFunctionalObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseFunctionalObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CInverseFunctionalObjectPropertyExpression::BETINVERSEFUNCTIONALPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseInverseFunctionalObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseInverseFunctionalObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CInverseObjectPropertyOfExpression::BETINVERSEOBJECTPROPERTYOF,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseInverseObjectPropertyOfStart,&CConcreteOntologyMergingBuilder::parseInverseObjectPropertyOfEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectPropertyChainExpression::BETOBJECTPROPERTYCHAIN,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectPropertyChainStart,&CConcreteOntologyMergingBuilder::parseObjectPropertyChainEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CEquivalentObjectPropertiesExpression::BETEQUIVALENTOBJECTPROPERTIES,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseEquivalentObjectPropertiesStart,&CConcreteOntologyMergingBuilder::parseEquivalentObjectPropertiesEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CDisjointObjectPropertiesExpression::BETDISJOINTOBJECTPROPERTIES,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseDisjointObjectPropertiesStart,&CConcreteOntologyMergingBuilder::parseDisjointObjectPropertiesEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CSymmetricObjectPropertyExpression::BETSYMMETRICPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseSymmetricObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseSymmetricObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CAsymmetricObjectPropertyExpression::BETASYMMETRICPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseAsymmetricObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseAsymmetricObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CReflexiveObjectPropertyExpression::BETREFLEXIVEPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseReflexiveObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseReflexiveObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CIrreflexiveObjectPropertyExpression::BETIRREFLEXIVEPROPERTY,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseIrreflexiveObjectPropertyStart,&CConcreteOntologyMergingBuilder::parseIrreflexiveObjectPropertyEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CNamedIndividualExpression::BETNAMEDINDIVIDUAL,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseNamedIndividualStart,&CConcreteOntologyMergingBuilder::parseNamedIndividualEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CAnonymousIndividualExpression::BETANONYMOUSINDIVIDUAL,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseAnonymousIndividualStart,&CConcreteOntologyMergingBuilder::parseAnonymousIndividualEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CClassAssertionExpression::BETCLASSASSERTION,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseClassAssertionStart,&CConcreteOntologyMergingBuilder::parseClassAssertionEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CObjectPropertyAssertionExpression::BETOBJECTPROPERTYASSERTION,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseObjectPropertyAssertionStart,&CConcreteOntologyMergingBuilder::parseObjectPropertyAssertionEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CNegativeObjectPropertyAssertionExpression::BETNEGATIVEOBJECTPROPERTYASSERTION,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseNegativeObjectPropertyAssertionStart,&CConcreteOntologyMergingBuilder::parseNegativeObjectPropertyAssertionEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CSameIndividualExpression::BETSAMEINDIVIDUAL,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseSameIndividualStart,&CConcreteOntologyMergingBuilder::parseSameIndividualEnd));
				mMergeExpFunctionJumpHash.insert((cint64)CDifferentIndividualsExpression::BETDIFFERENTINDIVIDUAL,CMergeExpressionFunctionPair(&CConcreteOntologyMergingBuilder::parseDifferentIndividualsStart,&CConcreteOntologyMergingBuilder::parseDifferentIndividualsEnd));
				return this;
			}

			void CConcreteOntologyMergingBuilder::addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CClassTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					mergeStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyMergingBuilder::addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CBuildExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					mergeStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyMergingBuilder::addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CObjectPropertyTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CObjectPropertyTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					mergeStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyMergingBuilder::addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CIndividualTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CIndividualTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					mergeStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyMergingBuilder::addExpressions(CMergeExpressionStackObject* mergeStackObj, CBuildExpression* expression) {
				mergeStackObj->addProcessingBuildExpression(expression);
			}


			void CConcreteOntologyMergingBuilder::parseClassStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseNominalIndividualVariableStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseNamedIndividualVariableStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseAnonymousIndividualVariableStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseDataValueVariableStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseDataLiteralVariableStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseEquivalentClassesStart(CMergeExpressionStackObject* mergeStackObj) {
				CEquivalentClassesExpression* expression = (CEquivalentClassesExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseSubClassOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CSubClassOfExpression* expression = (CSubClassOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getSubClassTermExpression());
				addExpressions(mergeStackObj,expression->getSuperClassTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseDisjointClassesStart(CMergeExpressionStackObject* mergeStackObj) {
				CDisjointClassesExpression* expression = (CDisjointClassesExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseDisjointUnionStart(CMergeExpressionStackObject* mergeStackObj) {
				CDisjointUnionExpression* expression = (CDisjointUnionExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassExpression());
				addExpressions(mergeStackObj,expression->getClassTermExpressionList());
		}

			void CConcreteOntologyMergingBuilder::parseObjectComplementOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectComplementOfExpression* expression = (CObjectComplementOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectIntersectionOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectIntersectionOfExpression* expression = (CObjectIntersectionOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseObjectUnionOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectUnionOfExpression* expression = (CObjectUnionOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseObjectMaxCardinalityStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectMaxCardinalityExpression* expression = (CObjectMaxCardinalityExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectMinCardinalityStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectMinCardinalityExpression* expression = (CObjectMinCardinalityExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectExactlyCardinalityStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectExactlyCardinalityExpression* expression = (CObjectExactlyCardinalityExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectAllValuesFromStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectAllValuesFromExpression* expression = (CObjectAllValuesFromExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectSomeValuesFromStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectSomeValuesFromExpression* expression = (CObjectSomeValuesFromExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getClassTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectOneOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectOneOfExpression* expression = (CObjectOneOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getIndividualTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseObjectHasValueStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectHasValueExpression* expression = (CObjectHasValueExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
				addExpressions(mergeStackObj,expression->getIndividualTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectHasSelfStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectHasSelfExpression* expression = (CObjectHasSelfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectProbertyStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseSubObjectPropertyOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CSubObjectPropertyOfExpression* expression = (CSubObjectPropertyOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getSubObjectPropertyTermExpressionList());
				addExpressions(mergeStackObj,expression->getSuperObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyDomainStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyDomainExpression* expression = (CObjectPropertyDomainExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
				addExpressions(mergeStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyRangeStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyRangeExpression* expression = (CObjectPropertyRangeExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
				addExpressions(mergeStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseInverseObjectPropertiesStart(CMergeExpressionStackObject* mergeStackObj) {
				CInverseObjectPropertiesExpression* expression = (CInverseObjectPropertiesExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getFirstObjectPropertyTermExpression());
				addExpressions(mergeStackObj,expression->getSecondObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseTransetiveObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CTransetiveObjectPropertyExpression* expression = (CTransetiveObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseFunctionalObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CFunctionalObjectPropertyExpression* expression = (CFunctionalObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseInverseFunctionalObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CInverseFunctionalObjectPropertyExpression* expression = (CInverseFunctionalObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseInverseObjectPropertyOfStart(CMergeExpressionStackObject* mergeStackObj) {
				CInverseObjectPropertyOfExpression* expression = (CInverseObjectPropertyOfExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyChainStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyChainExpression* expression = (CObjectPropertyChainExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getChainObjectPropertyTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseEquivalentObjectPropertiesStart(CMergeExpressionStackObject* mergeStackObj) {
				CEquivalentObjectPropertiesExpression* expression = (CEquivalentObjectPropertiesExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseDisjointObjectPropertiesStart(CMergeExpressionStackObject* mergeStackObj) {
				CDisjointObjectPropertiesExpression* expression = (CDisjointObjectPropertiesExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseSymmetricObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CSymmetricObjectPropertyExpression* expression = (CSymmetricObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseAsymmetricObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CAsymmetricObjectPropertyExpression* expression = (CAsymmetricObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseReflexiveObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CReflexiveObjectPropertyExpression* expression = (CReflexiveObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseIrreflexiveObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj) {
				CIrreflexiveObjectPropertyExpression* expression = (CIrreflexiveObjectPropertyExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseNamedIndividualStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseAnonymousIndividualStart(CMergeExpressionStackObject* mergeStackObj) {
			}

			void CConcreteOntologyMergingBuilder::parseClassAssertionStart(CMergeExpressionStackObject* mergeStackObj) {
				CClassAssertionExpression* expression = (CClassAssertionExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getIndividualTermExpression());
				addExpressions(mergeStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyAssertionStart(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyAssertionExpression* expression = (CObjectPropertyAssertionExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getFirstIndividualTermExpression());
				addExpressions(mergeStackObj,expression->getSecondIndividualTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseNegativeObjectPropertyAssertionStart(CMergeExpressionStackObject* mergeStackObj) {
				CNegativeObjectPropertyAssertionExpression* expression = (CNegativeObjectPropertyAssertionExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getFirstIndividualTermExpression());
				addExpressions(mergeStackObj,expression->getSecondIndividualTermExpression());
				addExpressions(mergeStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyMergingBuilder::parseSameIndividualStart(CMergeExpressionStackObject* mergeStackObj) {
				CSameIndividualExpression* expression = (CSameIndividualExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getIndividualTermExpressionList());
			}

			void CConcreteOntologyMergingBuilder::parseDifferentIndividualsStart(CMergeExpressionStackObject* mergeStackObj) {
				CDifferentIndividualsExpression* expression = (CDifferentIndividualsExpression*)mergeStackObj->mExpression;
				addExpressions(mergeStackObj,expression->getIndividualTermExpressionList());
			}



			void CConcreteOntologyMergingBuilder::parseClassEnd(CMergeExpressionStackObject* mergeStackObj) {
				CClassExpression* expression = (CClassExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getClass(expression->getName());
			}

			void CConcreteOntologyMergingBuilder::parseNominalIndividualVariableEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectIndividualVariableExpression* expression = (CObjectIndividualVariableExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getNominalIndividualVariable(expression->getName(),mNextAxiomNumber);
			}


			void CConcreteOntologyMergingBuilder::parseNamedIndividualVariableEnd(CMergeExpressionStackObject* mergeStackObj) {
				CNamedIndividualVariableExpression* expression = (CNamedIndividualVariableExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getIndividualVariable(expression->getName(),false);
			}

			void CConcreteOntologyMergingBuilder::parseAnonymousIndividualVariableEnd(CMergeExpressionStackObject* mergeStackObj) {
				CAnonymousIndividualVariableExpression* expression = (CAnonymousIndividualVariableExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getIndividualVariable(expression->getName(), true);
			}


			void CConcreteOntologyMergingBuilder::parseDataValueVariableEnd(CMergeExpressionStackObject* mergeStackObj) {
				CDataValueVariableExpression* expression = (CDataValueVariableExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getDataValueVariable(expression->getName());
			}


			void CConcreteOntologyMergingBuilder::parseDataLiteralVariableEnd(CMergeExpressionStackObject* mergeStackObj) {
				CDataLiteralVariableExpression* expression = (CDataLiteralVariableExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getDataLiteralVariable(expression->getName());
			}

			void CConcreteOntologyMergingBuilder::parseEquivalentClassesEnd(CMergeExpressionStackObject* mergeStackObj) {
				CEquivalentClassesExpression* expression = (CEquivalentClassesExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getEquivalentClasses(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseSubClassOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CEquivalentClassesExpression* expression = (CEquivalentClassesExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getSubClassOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseDisjointClassesEnd(CMergeExpressionStackObject* mergeStackObj) {
				CDisjointClassesExpression* expression = (CDisjointClassesExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getDisjointClasses(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseDisjointUnionEnd(CMergeExpressionStackObject* mergeStackObj) {
				CDisjointUnionExpression* expression = (CDisjointUnionExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getDisjointUnion(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectComplementOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectComplementOfExpression* expression = (CObjectComplementOfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectComplementOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectIntersectionOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectIntersectionOfExpression* expression = (CObjectIntersectionOfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectIntersectionOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectUnionOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectUnionOfExpression* expression = (CObjectUnionOfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectUnionOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectMaxCardinalityEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectMaxCardinalityExpression* expression = (CObjectMaxCardinalityExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectMaxCardinality(*mergeStackObj->getReplacedExpressions(),expression->getCardinality());
			}

			void CConcreteOntologyMergingBuilder::parseObjectMinCardinalityEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectMinCardinalityExpression* expression = (CObjectMinCardinalityExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectMinCardinality(*mergeStackObj->getReplacedExpressions(),expression->getCardinality());
			}

			void CConcreteOntologyMergingBuilder::parseObjectExactlyCardinalityEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectExactlyCardinalityExpression* expression = (CObjectExactlyCardinalityExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectExactlyCardinality(*mergeStackObj->getReplacedExpressions(),expression->getCardinality());
			}

			void CConcreteOntologyMergingBuilder::parseObjectAllValuesFromEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectAllValuesFromExpression* expression = (CObjectAllValuesFromExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectAllValuesFrom(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectSomeValuesFromEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectSomeValuesFromExpression* expression = (CObjectSomeValuesFromExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectSomeValuesFrom(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectOneOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectOneOfExpression* expression = (CObjectOneOfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectOneOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectHasValueEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectHasValueExpression* expression = (CObjectHasValueExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectHasValue(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectHasSelfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectHasSelfExpression* expression = (CObjectHasSelfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectHasSelf(*mergeStackObj->getReplacedExpressions());
			}


			void CConcreteOntologyMergingBuilder::parseObjectProbertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyExpression* expression = (CObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectProberty(expression->getName());
			}

			void CConcreteOntologyMergingBuilder::parseSubObjectPropertyOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CSubObjectPropertyOfExpression* expression = (CSubObjectPropertyOfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getSubObjectPropertyOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyDomainEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyDomainExpression* expression = (CObjectPropertyDomainExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectPropertyDomainExpression(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyRangeEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyRangeExpression* expression = (CObjectPropertyRangeExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectPropertyRangeExpression(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseInverseObjectPropertiesEnd(CMergeExpressionStackObject* mergeStackObj) {
				CInverseObjectPropertiesExpression* expression = (CInverseObjectPropertiesExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getInverseObjectProperties(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseTransetiveObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CTransetiveObjectPropertyExpression* expression = (CTransetiveObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getTransetiveObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseFunctionalObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CFunctionalObjectPropertyExpression* expression = (CFunctionalObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getFunctionalObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseInverseFunctionalObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CInverseFunctionalObjectPropertyExpression* expression = (CInverseFunctionalObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getInverseFunctionalObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseInverseObjectPropertyOfEnd(CMergeExpressionStackObject* mergeStackObj) {
				CInverseObjectPropertyOfExpression* expression = (CInverseObjectPropertyOfExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getInverseObjectPropertyOf(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyChainEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyChainExpression* expression = (CObjectPropertyChainExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectPropertyChain(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseEquivalentObjectPropertiesEnd(CMergeExpressionStackObject* mergeStackObj) {
				CEquivalentObjectPropertiesExpression* expression = (CEquivalentObjectPropertiesExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getEquivalentObjectProperties(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseDisjointObjectPropertiesEnd(CMergeExpressionStackObject* mergeStackObj) {
				CDisjointObjectPropertiesExpression* expression = (CDisjointObjectPropertiesExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getDisjointObjectProperties(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseSymmetricObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CSymmetricObjectPropertyExpression* expression = (CSymmetricObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getSymmetricObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseAsymmetricObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CAsymmetricObjectPropertyExpression* expression = (CAsymmetricObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getAsymmetricObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseReflexiveObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CReflexiveObjectPropertyExpression* expression = (CReflexiveObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getReflexiveObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseIrreflexiveObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj) {
				CIrreflexiveObjectPropertyExpression* expression = (CIrreflexiveObjectPropertyExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getIrreflexiveObjectProperty(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseNamedIndividualEnd(CMergeExpressionStackObject* mergeStackObj) {
				CNamedIndividualExpression* expression = (CNamedIndividualExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getNamedIndividual(expression->getName());
			}

			void CConcreteOntologyMergingBuilder::parseAnonymousIndividualEnd(CMergeExpressionStackObject* mergeStackObj) {
				CAnonymousIndividualExpression* expression = (CAnonymousIndividualExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getAnonymousIndividual(expression->getOntologyName(),expression->getNodeName());
			}

			void CConcreteOntologyMergingBuilder::parseClassAssertionEnd(CMergeExpressionStackObject* mergeStackObj) {
				CClassAssertionExpression* expression = (CClassAssertionExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getClassAssertion(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseObjectPropertyAssertionEnd(CMergeExpressionStackObject* mergeStackObj) {
				CObjectPropertyAssertionExpression* expression = (CObjectPropertyAssertionExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getObjectPropertyAssertion(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseNegativeObjectPropertyAssertionEnd(CMergeExpressionStackObject* mergeStackObj) {
				CNegativeObjectPropertyAssertionExpression* expression = (CNegativeObjectPropertyAssertionExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getNegativeObjectPropertyAssertion(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseSameIndividualEnd(CMergeExpressionStackObject* mergeStackObj) {
				CSameIndividualExpression* expression = (CSameIndividualExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getSameIndividual(*mergeStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyMergingBuilder::parseDifferentIndividualsEnd(CMergeExpressionStackObject* mergeStackObj) {
				CDifferentIndividualsExpression* expression = (CDifferentIndividualsExpression*)mergeStackObj->mExpression;
				mergeStackObj->mReplaceExpression = getDifferentIndividuals(*mergeStackObj->getReplacedExpressions());
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude