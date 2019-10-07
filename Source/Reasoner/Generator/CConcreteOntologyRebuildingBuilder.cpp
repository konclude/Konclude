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

#include "CConcreteOntologyRebuildingBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyRebuildingBuilder::CConcreteOntologyRebuildingBuilder(CConcreteOntology* ontology) : CConcreteOntologyUpdateBuilder(ontology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcretOntologyRebuildingBuilder",logTr("ConcretOntologyRebuildingBuilder initialized."),this);

				initJumpingHash();
			}

			CConcreteOntologyRebuildingBuilder::~CConcreteOntologyRebuildingBuilder() {
			}





			CRebuildingExpressionStackObject* CConcreteOntologyRebuildingBuilder::createRebuildingExpressionStackObject(CBuildExpression* expression) {
				CRebuildingExpressionStackObject* rebuildingExpStackObj = new CRebuildingExpressionStackObject(expression);
				mRebuildingExpContainer.append(rebuildingExpStackObj);
				const CRebuildingExpressionFunctionPair& rebuildingExpFuncPair(mRebuildingExpFunctionJumpHash.value((cint64)expression->getType()));
				rebuildingExpStackObj->mEndFunction = rebuildingExpFuncPair.mEndFunction;
				rebuildingExpStackObj->mStartFunction = rebuildingExpFuncPair.mStartFunction;
				if (rebuildingExpStackObj->mStartFunction) {
					FStartRebuildingExpressionFunction startFunction = rebuildingExpStackObj->mStartFunction;
					(this->*startFunction)(rebuildingExpStackObj);							
				}
				return rebuildingExpStackObj;
			}


			CBuildExpression* CConcreteOntologyRebuildingBuilder::handleRebuildingEpxressionStack(CBuildExpression* expression) {
				CBuildExpression* newMergeExpStackObj = nullptr;


				CRebuildingExpressionStackObject* nextRebuildingExpStackObj = createRebuildingExpressionStackObject(expression);
				mRebuildingExpStack.push(nextRebuildingExpStackObj);

				while (!mRebuildingExpStack.isEmpty()) {
					nextRebuildingExpStackObj = mRebuildingExpStack.top();


					if (nextRebuildingExpStackObj->hasProcessingExpressions()) {
						CBuildExpression* nextExpression = nextRebuildingExpStackObj->mProcessingExpList.takeFirst();
						mRebuildingExpStack.push(createRebuildingExpressionStackObject(nextExpression));
					} else {
						nextRebuildingExpStackObj = mRebuildingExpStack.pop();
						if (nextRebuildingExpStackObj->mEndFunction) {
							FEndRebuildingExpressionFunction endFunction = nextRebuildingExpStackObj->mEndFunction;
							(this->*endFunction)(nextRebuildingExpStackObj);		
						}
						CBuildExpression* replacedExpression = nextRebuildingExpStackObj->mReplaceExpression;
						if (!mRebuildingExpStack.isEmpty()) {
							nextRebuildingExpStackObj = mRebuildingExpStack.top();
							nextRebuildingExpStackObj->addReplacedBuildExpression(replacedExpression);
						} else {
							newMergeExpStackObj = replacedExpression;
						}
					}
				}

				return newMergeExpStackObj;
			}

			



			CBuildExpression* CConcreteOntologyRebuildingBuilder::rebuildExpression(CBuildExpression* expression) {
				return handleRebuildingEpxressionStack(expression);
			}




			CConcreteOntologyRebuildingBuilder* CConcreteOntologyRebuildingBuilder::initJumpingHash() {
				mRebuildingExpFunctionJumpHash.insert((cint64)CClassExpression::BETCLASS,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseClassStart,&CConcreteOntologyRebuildingBuilder::parseClassEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectIndividualVariableExpression::BETNOMINALINDIVIDUALVARIABLE,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseIndividualVariableStart,&CConcreteOntologyRebuildingBuilder::parseIndividualVariableEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CEquivalentClassesExpression::BETEQUIVALENTCLASSES,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseEquivalentClassesStart,&CConcreteOntologyRebuildingBuilder::parseEquivalentClassesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CSubClassOfExpression::BETESUBCLASSOF,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseSubClassOfStart,&CConcreteOntologyRebuildingBuilder::parseSubClassOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDisjointClassesExpression::BETDISJOINTCLASSES,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDisjointClassesStart,&CConcreteOntologyRebuildingBuilder::parseDisjointClassesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDisjointUnionExpression::BETDISJOINTUNION,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDisjointUnionStart,&CConcreteOntologyRebuildingBuilder::parseDisjointUnionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectComplementOfExpression::BETOBJECTCOMPLEMENTOF,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectComplementOfStart,&CConcreteOntologyRebuildingBuilder::parseObjectComplementOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectIntersectionOfExpression::BETOBJECTINTERSECTION,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectIntersectionOfStart,&CConcreteOntologyRebuildingBuilder::parseObjectIntersectionOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectUnionOfExpression::BETOBJECTUNION,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectUnionOfStart,&CConcreteOntologyRebuildingBuilder::parseObjectUnionOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectMaxCardinalityExpression::BETOBJECTMAXCARDINALITY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectMaxCardinalityStart,&CConcreteOntologyRebuildingBuilder::parseObjectMaxCardinalityEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectMinCardinalityExpression::BETOBJECTMINCARDINALITY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectMinCardinalityStart,&CConcreteOntologyRebuildingBuilder::parseObjectMinCardinalityEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectExactlyCardinalityExpression::BETOBJECTEXACTLYCARDINALITY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectExactlyCardinalityStart,&CConcreteOntologyRebuildingBuilder::parseObjectExactlyCardinalityEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectAllValuesFromExpression::BETOBJECTALLVALUEFROM,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectAllValuesFromStart,&CConcreteOntologyRebuildingBuilder::parseObjectAllValuesFromEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectSomeValuesFromExpression::BETOBJECTSOMEVALUEFROM,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectSomeValuesFromStart,&CConcreteOntologyRebuildingBuilder::parseObjectSomeValuesFromEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectOneOfExpression::BETOBJECTONEOF,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectOneOfStart,&CConcreteOntologyRebuildingBuilder::parseObjectOneOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectHasValueExpression::BETOBJECTHASVALUE,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectHasValueStart,&CConcreteOntologyRebuildingBuilder::parseObjectHasValueEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectHasSelfExpression::BETOBJECTHASSELF,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectHasSelfStart,&CConcreteOntologyRebuildingBuilder::parseObjectHasSelfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectPropertyExpression::BETOBJECTPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectProbertyStart,&CConcreteOntologyRebuildingBuilder::parseObjectProbertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CSubObjectPropertyOfExpression::BETSUBOBJECTPROPERTYOF,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseSubObjectPropertyOfStart,&CConcreteOntologyRebuildingBuilder::parseSubObjectPropertyOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectPropertyDomainExpression::BETOBJECTPROPERTYDOMAIN,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectPropertyDomainStart,&CConcreteOntologyRebuildingBuilder::parseObjectPropertyDomainEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectPropertyRangeExpression::BETOBJECTPROPERTYRANGE,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectPropertyRangeStart,&CConcreteOntologyRebuildingBuilder::parseObjectPropertyRangeEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CInverseObjectPropertiesExpression::BETINVERSEOBJECTPROPERTIES,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertiesStart,&CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertiesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CTransetiveObjectPropertyExpression::BETTRANSITIVEOBJECTPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseTransetiveObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseTransetiveObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CFunctionalObjectPropertyExpression::BETFUNCTIONALOBJECTPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseFunctionalObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseFunctionalObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CInverseFunctionalObjectPropertyExpression::BETINVERSEFUNCTIONALPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseInverseFunctionalObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseInverseFunctionalObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CInverseObjectPropertyOfExpression::BETINVERSEOBJECTPROPERTYOF,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertyOfStart,&CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertyOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectPropertyChainExpression::BETOBJECTPROPERTYCHAIN,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectPropertyChainStart,&CConcreteOntologyRebuildingBuilder::parseObjectPropertyChainEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CEquivalentObjectPropertiesExpression::BETEQUIVALENTOBJECTPROPERTIES,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseEquivalentObjectPropertiesStart,&CConcreteOntologyRebuildingBuilder::parseEquivalentObjectPropertiesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDisjointObjectPropertiesExpression::BETDISJOINTOBJECTPROPERTIES,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDisjointObjectPropertiesStart,&CConcreteOntologyRebuildingBuilder::parseDisjointObjectPropertiesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CSymmetricObjectPropertyExpression::BETSYMMETRICPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseSymmetricObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseSymmetricObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CAsymmetricObjectPropertyExpression::BETASYMMETRICPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseAsymmetricObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseAsymmetricObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CReflexiveObjectPropertyExpression::BETREFLEXIVEPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseReflexiveObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseReflexiveObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CIrreflexiveObjectPropertyExpression::BETIRREFLEXIVEPROPERTY,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseIrreflexiveObjectPropertyStart,&CConcreteOntologyRebuildingBuilder::parseIrreflexiveObjectPropertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CNamedIndividualExpression::BETNAMEDINDIVIDUAL,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseNamedIndividualStart,&CConcreteOntologyRebuildingBuilder::parseNamedIndividualEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CAnonymousIndividualExpression::BETANONYMOUSINDIVIDUAL,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseAnonymousIndividualStart,&CConcreteOntologyRebuildingBuilder::parseAnonymousIndividualEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CClassAssertionExpression::BETCLASSASSERTION,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseClassAssertionStart,&CConcreteOntologyRebuildingBuilder::parseClassAssertionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CObjectPropertyAssertionExpression::BETOBJECTPROPERTYASSERTION,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseObjectPropertyAssertionStart,&CConcreteOntologyRebuildingBuilder::parseObjectPropertyAssertionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CNegativeObjectPropertyAssertionExpression::BETNEGATIVEOBJECTPROPERTYASSERTION,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseNegativeObjectPropertyAssertionStart,&CConcreteOntologyRebuildingBuilder::parseNegativeObjectPropertyAssertionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CSameIndividualExpression::BETSAMEINDIVIDUAL,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseSameIndividualStart,&CConcreteOntologyRebuildingBuilder::parseSameIndividualEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDifferentIndividualsExpression::BETDIFFERENTINDIVIDUAL,CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDifferentIndividualsStart,&CConcreteOntologyRebuildingBuilder::parseDifferentIndividualsEnd));


				mRebuildingExpFunctionJumpHash.insert((cint64)CDataComplementOfExpression::BETDATACOMPLEMENTOF, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataComplementOfStart, &CConcreteOntologyRebuildingBuilder::parseDataComplementOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataIntersectionOfExpression::BETDATAINTERSECTIONOF, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataIntersectionOfStart, &CConcreteOntologyRebuildingBuilder::parseDataIntersectionOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataUnionOfExpression::BETDATAUNIONOF, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataUnionOfStart, &CConcreteOntologyRebuildingBuilder::parseDataUnionOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataMaxCardinalityExpression::BETDATAMAXCARDINALITY, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataMaxCardinalityStart, &CConcreteOntologyRebuildingBuilder::parseDataMaxCardinalityEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataMinCardinalityExpression::BETDATAMINCARDINALITY, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataMinCardinalityStart, &CConcreteOntologyRebuildingBuilder::parseDataMinCardinalityEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataExactCardinalityExpression::BETDATAEXACTCARDINALITY, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataExactCardinalityStart, &CConcreteOntologyRebuildingBuilder::parseDataExactCardinalityEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataAllValuesFromExpression::BETDATAALLVALUEFROM, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataAllValuesFromStart, &CConcreteOntologyRebuildingBuilder::parseDataAllValuesFromEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataSomeValuesFromExpression::BETDATASOMEVALUEFROM, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataSomeValuesFromStart, &CConcreteOntologyRebuildingBuilder::parseDataSomeValuesFromEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataOneOfExpression::BETDATAONEOF, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataOneOfStart, &CConcreteOntologyRebuildingBuilder::parseDataOneOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataHasValueExpression::BETDATAHASVALUE, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataHasValueStart, &CConcreteOntologyRebuildingBuilder::parseDataHasValueEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataPropertyExpression::BETDATAPROPERTY, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataProbertyStart, &CConcreteOntologyRebuildingBuilder::parseDataProbertyEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CSubDataPropertyOfExpression::BETSUBDATAPROPERTYOF, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseSubDataPropertyOfStart, &CConcreteOntologyRebuildingBuilder::parseSubDataPropertyOfEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataPropertyDomainExpression::BETDATAPROPERTYDOMAIN, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataPropertyDomainStart, &CConcreteOntologyRebuildingBuilder::parseDataPropertyDomainEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataPropertyRangeExpression::BETDATAPROPERTYRANGE, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataPropertyRangeStart, &CConcreteOntologyRebuildingBuilder::parseDataPropertyRangeEnd));

				mRebuildingExpFunctionJumpHash.insert((cint64)CDataPropertyAssertionExpression::BETDATAPROPERTYASSERTION, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataPropertyAssertionStart, &CConcreteOntologyRebuildingBuilder::parseDataPropertyAssertionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CNegativeDataPropertyAssertionExpression::BETNEGATIVEDATAPROPERTYASSERTION, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseNegativeDataPropertyAssertionStart, &CConcreteOntologyRebuildingBuilder::parseNegativeDataPropertyAssertionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDatatypeRestrictionExpression::BETDATATYPERESTRICTION, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDatatypeRestrictionStart, &CConcreteOntologyRebuildingBuilder::parseDatatypeRestrictionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataLiteralExpression::BETDATALITERAL, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataLiteralStart, &CConcreteOntologyRebuildingBuilder::parseDataLiteralEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDatatypeExpression::BETDATATYPE, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDatatypeStart, &CConcreteOntologyRebuildingBuilder::parseDatatypeEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataLexicalValueExpression::BETDATALEXICALVALUE, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataLexicalValueStart, &CConcreteOntologyRebuildingBuilder::parseDataLexicalValueEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataFacetExpression::BETDATAFACETIRI, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataFacetStart, &CConcreteOntologyRebuildingBuilder::parseDataFacetEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDataFacetRestrictionExpression::BETDATAFACETRESTRICION, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDataFacetRestrictionStart, &CConcreteOntologyRebuildingBuilder::parseDataFacetRestrictionEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CEquivalentDataPropertiesExpression::BETEQUIVALENTDATAPROPERTIES, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseEquivalentDataPropertiesStart, &CConcreteOntologyRebuildingBuilder::parseEquivalentDataPropertiesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CDisjointDataPropertiesExpression::BETDISJOINTDATAPROPERTIES, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseDisjointDataPropertiesStart, &CConcreteOntologyRebuildingBuilder::parseDisjointDataPropertiesEnd));
				mRebuildingExpFunctionJumpHash.insert((cint64)CFunctionalDataPropertyExpression::BETFUNCTIONALDATAPROPERTY, CRebuildingExpressionFunctionPair(&CConcreteOntologyRebuildingBuilder::parseFunctionalDataPropertyStart, &CConcreteOntologyRebuildingBuilder::parseFunctionalDataPropertyEnd));


				return this;
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CClassTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CClassTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CBuildExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CObjectPropertyTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CObjectPropertyTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CIndividualTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CIndividualTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CDataPropertyTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CDataPropertyTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CDataRangeTermExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CDataRangeTermExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* expressionList) {
				for (CEXPRESSIONLIST<CDataFacetRestrictionExpression*>::const_iterator it = expressionList->constBegin(), itEnd = expressionList->constEnd(); it != itEnd; ++it) {
					rebuildingStackObj->addProcessingBuildExpression(*it);
				}
			}

			void CConcreteOntologyRebuildingBuilder::addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CBuildExpression* expression) {
				rebuildingStackObj->addProcessingBuildExpression(expression);
			}


			void CConcreteOntologyRebuildingBuilder::parseClassStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseIndividualVariableStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseEquivalentClassesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentClassesExpression* expression = (CEquivalentClassesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseSubClassOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSubClassOfExpression* expression = (CSubClassOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getSubClassTermExpression());
				addExpressions(rebuildingStackObj,expression->getSuperClassTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointClassesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointClassesExpression* expression = (CDisjointClassesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointUnionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointUnionExpression* expression = (CDisjointUnionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassExpression());
				addExpressions(rebuildingStackObj,expression->getClassTermExpressionList());
		}

			void CConcreteOntologyRebuildingBuilder::parseObjectComplementOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectComplementOfExpression* expression = (CObjectComplementOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectIntersectionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectIntersectionOfExpression* expression = (CObjectIntersectionOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectUnionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectUnionOfExpression* expression = (CObjectUnionOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectMaxCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectMaxCardinalityExpression* expression = (CObjectMaxCardinalityExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectMinCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectMinCardinalityExpression* expression = (CObjectMinCardinalityExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectExactlyCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectExactlyCardinalityExpression* expression = (CObjectExactlyCardinalityExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectAllValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectAllValuesFromExpression* expression = (CObjectAllValuesFromExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectSomeValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectSomeValuesFromExpression* expression = (CObjectSomeValuesFromExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectOneOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectOneOfExpression* expression = (CObjectOneOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getIndividualTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectHasValueStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectHasValueExpression* expression = (CObjectHasValueExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getIndividualTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectHasSelfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectHasSelfExpression* expression = (CObjectHasSelfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectProbertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseSubObjectPropertyOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSubObjectPropertyOfExpression* expression = (CSubObjectPropertyOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getSubObjectPropertyTermExpressionList());
				addExpressions(rebuildingStackObj,expression->getSuperObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyDomainStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyDomainExpression* expression = (CObjectPropertyDomainExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyRangeStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyRangeExpression* expression = (CObjectPropertyRangeExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CInverseObjectPropertiesExpression* expression = (CInverseObjectPropertiesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getFirstObjectPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getSecondObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseTransetiveObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CTransetiveObjectPropertyExpression* expression = (CTransetiveObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseFunctionalObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CFunctionalObjectPropertyExpression* expression = (CFunctionalObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseInverseFunctionalObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CInverseFunctionalObjectPropertyExpression* expression = (CInverseFunctionalObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertyOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CInverseObjectPropertyOfExpression* expression = (CInverseObjectPropertyOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyChainStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyChainExpression* expression = (CObjectPropertyChainExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getChainObjectPropertyTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseEquivalentObjectPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentObjectPropertiesExpression* expression = (CEquivalentObjectPropertiesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointObjectPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointObjectPropertiesExpression* expression = (CDisjointObjectPropertiesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseSymmetricObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSymmetricObjectPropertyExpression* expression = (CSymmetricObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseAsymmetricObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CAsymmetricObjectPropertyExpression* expression = (CAsymmetricObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseReflexiveObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CReflexiveObjectPropertyExpression* expression = (CReflexiveObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseIrreflexiveObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CIrreflexiveObjectPropertyExpression* expression = (CIrreflexiveObjectPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseNamedIndividualStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseAnonymousIndividualStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseClassAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CClassAssertionExpression* expression = (CClassAssertionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getIndividualTermExpression());
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyAssertionExpression* expression = (CObjectPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getFirstIndividualTermExpression());
				addExpressions(rebuildingStackObj,expression->getSecondIndividualTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseNegativeObjectPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CNegativeObjectPropertyAssertionExpression* expression = (CNegativeObjectPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getFirstIndividualTermExpression());
				addExpressions(rebuildingStackObj,expression->getSecondIndividualTermExpression());
				addExpressions(rebuildingStackObj,expression->getObjectPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseSameIndividualStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSameIndividualExpression* expression = (CSameIndividualExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getIndividualTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseDifferentIndividualsStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDifferentIndividualsExpression* expression = (CDifferentIndividualsExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getIndividualTermExpressionList());
			}







			void CConcreteOntologyRebuildingBuilder::parseDataComplementOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataComplementOfExpression* expression = (CDataComplementOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataIntersectionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataIntersectionOfExpression* expression = (CDataIntersectionOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataUnionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataUnionOfExpression* expression = (CDataUnionOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataMaxCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataMaxCardinalityExpression* expression = (CDataMaxCardinalityExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataMinCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataMinCardinalityExpression* expression = (CDataMinCardinalityExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataExactCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataExactCardinalityExpression* expression = (CDataExactCardinalityExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataAllValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataAllValuesFromExpression* expression = (CDataAllValuesFromExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataSomeValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataSomeValuesFromExpression* expression = (CDataSomeValuesFromExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataOneOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataOneOfExpression* expression = (CDataOneOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpressionList());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataHasValueStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataHasValueExpression* expression = (CDataHasValueExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
			}


			void CConcreteOntologyRebuildingBuilder::parseDataProbertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseSubDataPropertyOfStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSubDataPropertyOfExpression* expression = (CSubDataPropertyOfExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getSubDataPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getSuperDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataPropertyDomainStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyDomainExpression* expression = (CDataPropertyDomainExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getClassTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataPropertyRangeStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyRangeExpression* expression = (CDataPropertyRangeExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj,expression->getDataPropertyTermExpression());
				addExpressions(rebuildingStackObj,expression->getDataRangeTermExpression());
			}


			void CConcreteOntologyRebuildingBuilder::parseDataPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyAssertionExpression* expression = (CDataPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataLiteralTermExpression());
				addExpressions(rebuildingStackObj, expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseNegativeDataPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CNegativeDataPropertyAssertionExpression* expression = (CNegativeDataPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataLiteralTermExpression());
				addExpressions(rebuildingStackObj, expression->getDataPropertyTermExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDatatypeRestrictionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDatatypeRestrictionExpression* expression = (CDatatypeRestrictionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataFacetRestrictionExpressionList());
				addExpressions(rebuildingStackObj, expression->getDatatypeExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataLiteralStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataLiteralExpression* expression = (CDataLiteralExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataLexicalValueExpression());
				addExpressions(rebuildingStackObj, expression->getDatatypeExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseDatatypeStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseDataLexicalValueStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}

			void CConcreteOntologyRebuildingBuilder::parseDataFacetStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
			}


			void CConcreteOntologyRebuildingBuilder::parseDataFacetRestrictionStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataFacetRestrictionExpression* expression = (CDataFacetRestrictionExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataFacetExpression());
				addExpressions(rebuildingStackObj, expression->getDataLiteralExpression());
			}

			void CConcreteOntologyRebuildingBuilder::parseEquivalentDataPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentDataPropertiesExpression* expression = (CEquivalentDataPropertiesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataPropertyTermExpressionList());
			}


			void CConcreteOntologyRebuildingBuilder::parseDisjointDataPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointDataPropertiesExpression* expression = (CDisjointDataPropertiesExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataPropertyTermExpressionList());
			}


			void CConcreteOntologyRebuildingBuilder::parseFunctionalDataPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CFunctionalDataPropertyExpression* expression = (CFunctionalDataPropertyExpression*)rebuildingStackObj->mExpression;
				addExpressions(rebuildingStackObj, expression->getDataPropertyTermExpression());
			}


















			void CConcreteOntologyRebuildingBuilder::parseClassEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CClassExpression* expression = (CClassExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getClass(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseIndividualVariableEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectIndividualVariableExpression* expression = (CObjectIndividualVariableExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getNominalIndividualVariable(expression->getName(),mNextAxiomNumber);
			}

			void CConcreteOntologyRebuildingBuilder::parseEquivalentClassesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentClassesExpression* expression = (CEquivalentClassesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getEquivalentClasses(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseSubClassOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentClassesExpression* expression = (CEquivalentClassesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getSubClassOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointClassesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointClassesExpression* expression = (CDisjointClassesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDisjointClasses(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointUnionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointUnionExpression* expression = (CDisjointUnionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDisjointUnion(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectComplementOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectComplementOfExpression* expression = (CObjectComplementOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectComplementOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectIntersectionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectIntersectionOfExpression* expression = (CObjectIntersectionOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectIntersectionOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectUnionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectUnionOfExpression* expression = (CObjectUnionOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectUnionOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectMaxCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectMaxCardinalityExpression* expression = (CObjectMaxCardinalityExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectMaxCardinality(*rebuildingStackObj->getReplacedExpressions(),expression->getCardinality());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectMinCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectMinCardinalityExpression* expression = (CObjectMinCardinalityExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectMinCardinality(*rebuildingStackObj->getReplacedExpressions(),expression->getCardinality());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectExactlyCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectExactlyCardinalityExpression* expression = (CObjectExactlyCardinalityExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectExactlyCardinality(*rebuildingStackObj->getReplacedExpressions(),expression->getCardinality());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectAllValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectAllValuesFromExpression* expression = (CObjectAllValuesFromExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectAllValuesFrom(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectSomeValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectSomeValuesFromExpression* expression = (CObjectSomeValuesFromExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectSomeValuesFrom(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectOneOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectOneOfExpression* expression = (CObjectOneOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectOneOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectHasValueEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectHasValueExpression* expression = (CObjectHasValueExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectHasValue(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectHasSelfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectHasSelfExpression* expression = (CObjectHasSelfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectHasSelf(*rebuildingStackObj->getReplacedExpressions());
			}


			void CConcreteOntologyRebuildingBuilder::parseObjectProbertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyExpression* expression = (CObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectProberty(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseSubObjectPropertyOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSubObjectPropertyOfExpression* expression = (CSubObjectPropertyOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getSubObjectPropertyOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyDomainEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyDomainExpression* expression = (CObjectPropertyDomainExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectPropertyDomainExpression(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyRangeEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyRangeExpression* expression = (CObjectPropertyRangeExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectPropertyRangeExpression(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CInverseObjectPropertiesExpression* expression = (CInverseObjectPropertiesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getInverseObjectProperties(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseTransetiveObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CTransetiveObjectPropertyExpression* expression = (CTransetiveObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getTransetiveObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseFunctionalObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CFunctionalObjectPropertyExpression* expression = (CFunctionalObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getFunctionalObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseInverseFunctionalObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CInverseFunctionalObjectPropertyExpression* expression = (CInverseFunctionalObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getInverseFunctionalObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseInverseObjectPropertyOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CInverseObjectPropertyOfExpression* expression = (CInverseObjectPropertyOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getInverseObjectPropertyOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyChainEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyChainExpression* expression = (CObjectPropertyChainExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectPropertyChain(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseEquivalentObjectPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentObjectPropertiesExpression* expression = (CEquivalentObjectPropertiesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getEquivalentObjectProperties(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointObjectPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointObjectPropertiesExpression* expression = (CDisjointObjectPropertiesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDisjointObjectProperties(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseSymmetricObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSymmetricObjectPropertyExpression* expression = (CSymmetricObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getSymmetricObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseAsymmetricObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CAsymmetricObjectPropertyExpression* expression = (CAsymmetricObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getAsymmetricObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseReflexiveObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CReflexiveObjectPropertyExpression* expression = (CReflexiveObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getReflexiveObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseIrreflexiveObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CIrreflexiveObjectPropertyExpression* expression = (CIrreflexiveObjectPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getIrreflexiveObjectProperty(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseNamedIndividualEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CNamedIndividualExpression* expression = (CNamedIndividualExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getNamedIndividual(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseAnonymousIndividualEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CAnonymousIndividualExpression* expression = (CAnonymousIndividualExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getAnonymousIndividual(expression->getOntologyName(),expression->getNodeName());
			}

			void CConcreteOntologyRebuildingBuilder::parseClassAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CClassAssertionExpression* expression = (CClassAssertionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getClassAssertion(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseObjectPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CObjectPropertyAssertionExpression* expression = (CObjectPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getObjectPropertyAssertion(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseNegativeObjectPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CNegativeObjectPropertyAssertionExpression* expression = (CNegativeObjectPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getNegativeObjectPropertyAssertion(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseSameIndividualEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSameIndividualExpression* expression = (CSameIndividualExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getSameIndividual(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDifferentIndividualsEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDifferentIndividualsExpression* expression = (CDifferentIndividualsExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDifferentIndividuals(*rebuildingStackObj->getReplacedExpressions());
			}







			void CConcreteOntologyRebuildingBuilder::parseDataComplementOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataComplementOfExpression* expression = (CDataComplementOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataComplementOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataIntersectionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataIntersectionOfExpression* expression = (CDataIntersectionOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataIntersectionOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataUnionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataUnionOfExpression* expression = (CDataUnionOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataUnionOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataMaxCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataMaxCardinalityExpression* expression = (CDataMaxCardinalityExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataMaxCardinality(*rebuildingStackObj->getReplacedExpressions(), expression->getCardinality());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataMinCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataMinCardinalityExpression* expression = (CDataMinCardinalityExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataMinCardinality(*rebuildingStackObj->getReplacedExpressions(), expression->getCardinality());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataExactCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataExactCardinalityExpression* expression = (CDataExactCardinalityExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataExactCardinality(*rebuildingStackObj->getReplacedExpressions(), expression->getCardinality());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataAllValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataAllValuesFromExpression* expression = (CDataAllValuesFromExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataAllValuesFrom(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataSomeValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataSomeValuesFromExpression* expression = (CDataSomeValuesFromExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataSomeValuesFrom(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataOneOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataOneOfExpression* expression = (CDataOneOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataOneOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataHasValueEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataHasValueExpression* expression = (CDataHasValueExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataHasValue(*rebuildingStackObj->getReplacedExpressions());
			}
			
			void CConcreteOntologyRebuildingBuilder::parseDataProbertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyExpression* expression = (CDataPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataProberty(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseSubDataPropertyOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CSubDataPropertyOfExpression* expression = (CSubDataPropertyOfExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getSubDataPropertyOf(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataPropertyDomainEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyDomainExpression* expression = (CDataPropertyDomainExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataPropertyDomainExpression(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataPropertyRangeEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyRangeExpression* expression = (CDataPropertyRangeExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataPropertyRangeExpression(*rebuildingStackObj->getReplacedExpressions());
			}





			void CConcreteOntologyRebuildingBuilder::parseDataPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataPropertyAssertionExpression* expression = (CDataPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataPropertyAssertion(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseNegativeDataPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CNegativeDataPropertyAssertionExpression* expression = (CNegativeDataPropertyAssertionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getNegativeDataPropertyAssertion(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDatatypeRestrictionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDatatypeRestrictionExpression* expression = (CDatatypeRestrictionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDatatypeRestriction(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataLiteralEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataLiteralExpression* expression = (CDataLiteralExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataLiteral(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDatatypeEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDatatypeExpression* expression = (CDatatypeExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDatatype(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataLexicalValueEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataLexicalValueExpression* expression = (CDataLexicalValueExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataLexicalValue(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataFacetEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataFacetExpression* expression = (CDataFacetExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataFacet(expression->getName());
			}

			void CConcreteOntologyRebuildingBuilder::parseDataFacetRestrictionEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDataFacetRestrictionExpression* expression = (CDataFacetRestrictionExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDataFacetRestriction(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseEquivalentDataPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CEquivalentDataPropertiesExpression* expression = (CEquivalentDataPropertiesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getEquivalentDataProperties(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseDisjointDataPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CDisjointDataPropertiesExpression* expression = (CDisjointDataPropertiesExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getDisjointDataProperties(*rebuildingStackObj->getReplacedExpressions());
			}

			void CConcreteOntologyRebuildingBuilder::parseFunctionalDataPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj) {
				CFunctionalDataPropertyExpression* expression = (CFunctionalDataPropertyExpression*)rebuildingStackObj->mExpression;
				rebuildingStackObj->mReplaceExpression = getFunctionalDataProperty(*rebuildingStackObj->getReplacedExpressions());
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude