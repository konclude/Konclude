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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREBUILDINGBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREBUILDINGBUILDER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>
#include <QStack>

// Namespace includes
#include "CConcreteOntologyUpdateBuilder.h"
#include "CExpressionSplitter.h"
#include "CExpressionHasher.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CTBox.h"
#include "Reasoner/Ontology/CABox.h"
#include "Reasoner/Ontology/CRBox.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"
#include "Reasoner/Ontology/CIRIName.h"


#include "Utilities/CTrible.hpp"
#include "Utilities/CNegLinker.hpp"
#include "Utilities/CHashableIntConvertedNTuble.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/Memory/CObjectAllocator.h"

#include "Parser/COntologyBuilder.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Parser;

	namespace Reasoner {

		using namespace Ontology;	

		namespace Generator {


			class CConcreteOntologyRebuildingBuilder;
			class CRebuildingExpressionStackObject;

			typedef void (CConcreteOntologyRebuildingBuilder::*FStartRebuildingExpressionFunction) (CRebuildingExpressionStackObject*  rebuildingStackObj);
			typedef void (CConcreteOntologyRebuildingBuilder::*FEndRebuildingExpressionFunction) (CRebuildingExpressionStackObject*  rebuildingStackObj);


			class CRebuildingExpressionStackObject {
				public:
					inline CRebuildingExpressionStackObject(CBuildExpression* expression) : mExpression(expression) {
					}

					inline CRebuildingExpressionStackObject* addProcessingBuildExpression(CBuildExpression* buildExpression) {
						mProcessingExpList.append(buildExpression);
						return this;
					}

					inline CRebuildingExpressionStackObject* addProcessingBuildExpressions(CQtList<CBuildExpression*>* expressions) {
						if (mProcessingExpList.isEmpty()) {
							mProcessingExpList = *expressions;
						} else {
							mProcessingExpList.append(*expressions);
						}
						return this;
					}

					inline CRebuildingExpressionStackObject* addReplacedBuildExpression(CBuildExpression* buildExpression) {
						mReplacedExpList.append(buildExpression);
						return this;
					}

					inline CRebuildingExpressionStackObject* addReplacedBuildExpressions(CQtList<CBuildExpression*>* expressions) {
						if (mReplacedExpList.isEmpty()) {
							mReplacedExpList = *expressions;
						} else {
							mReplacedExpList.append(*expressions);
						}
						return this;
					}

					inline bool hasProcessingExpressions() {
						return !mProcessingExpList.isEmpty();
					}

					inline bool hasReplacedExpressions() {
						return !mReplacedExpList.isEmpty();
					}

					inline CQtList<CBuildExpression*>* getProcessingExpressions() {
						return &mProcessingExpList;
					}

					inline CQtList<CBuildExpression*>* getReplacedExpressions() {
						return &mReplacedExpList;
					}
					
					FStartRebuildingExpressionFunction mStartFunction;
					FEndRebuildingExpressionFunction mEndFunction;
					CBuildExpression* mExpression;
					CBuildExpression* mReplaceExpression;
					CQtList<CBuildExpression*> mProcessingExpList;
					CQtList<CBuildExpression*> mReplacedExpList;
			};


			/*! 
			 *
			 *		\class		CConcreteOntologyRebuildingBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyRebuildingBuilder : public CConcreteOntologyUpdateBuilder {
				// public methods
				public:
					CConcreteOntologyRebuildingBuilder(CConcreteOntology* ontology);
					virtual ~CConcreteOntologyRebuildingBuilder();


					CBuildExpression* rebuildExpression(CBuildExpression* expression);

				// protected functions
				protected:
					virtual CRebuildingExpressionStackObject* createRebuildingExpressionStackObject(CBuildExpression* expression);
					virtual CBuildExpression* handleRebuildingEpxressionStack(CBuildExpression* expression);
					virtual CConcreteOntologyRebuildingBuilder* initJumpingHash();

					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CClassTermExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CObjectPropertyTermExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CBuildExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CIndividualTermExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CDataPropertyTermExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CDataRangeTermExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* expressionList);
					void addExpressions(CRebuildingExpressionStackObject* rebuildingStackObj, CBuildExpression* expression);


					void parseClassStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseIndividualVariableStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseEquivalentClassesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSubClassOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointClassesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointUnionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectComplementOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectIntersectionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectUnionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectMaxCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectMinCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectExactlyCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectAllValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectSomeValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectOneOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectHasValueStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectHasSelfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectProbertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSubObjectPropertyOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyDomainStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyRangeStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseInverseObjectPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseTransetiveObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseFunctionalObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseInverseFunctionalObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseInverseObjectPropertyOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyChainStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseEquivalentObjectPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointObjectPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSymmetricObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseAsymmetricObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseReflexiveObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseIrreflexiveObjectPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseNamedIndividualStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseAnonymousIndividualStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseClassAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseNegativeObjectPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSameIndividualStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDifferentIndividualsStart(CRebuildingExpressionStackObject* rebuildingStackObj);

					void parseDataComplementOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataIntersectionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataUnionOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataMaxCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataMinCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataExactCardinalityStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataAllValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataSomeValuesFromStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataOneOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataHasValueStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataProbertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSubDataPropertyOfStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataPropertyDomainStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataPropertyRangeStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseNegativeDataPropertyAssertionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDatatypeRestrictionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataLiteralStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDatatypeStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataLexicalValueStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataFacetStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataFacetRestrictionStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseEquivalentDataPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointDataPropertiesStart(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseFunctionalDataPropertyStart(CRebuildingExpressionStackObject* rebuildingStackObj);



					void parseClassEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseIndividualVariableEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseEquivalentClassesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSubClassOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointClassesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointUnionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectComplementOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectIntersectionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectUnionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectMaxCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectMinCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectExactlyCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectAllValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectSomeValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectOneOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectHasValueEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectHasSelfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectProbertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSubObjectPropertyOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyDomainEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyRangeEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseInverseObjectPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseTransetiveObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseFunctionalObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseInverseFunctionalObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseInverseObjectPropertyOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyChainEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseEquivalentObjectPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointObjectPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSymmetricObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseAsymmetricObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseReflexiveObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseIrreflexiveObjectPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseNamedIndividualEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseAnonymousIndividualEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseClassAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseObjectPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseNegativeObjectPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSameIndividualEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDifferentIndividualsEnd(CRebuildingExpressionStackObject* rebuildingStackObj);


					void parseDataComplementOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataIntersectionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataUnionOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataMaxCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataMinCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataExactCardinalityEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataAllValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataSomeValuesFromEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataOneOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataHasValueEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataProbertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseSubDataPropertyOfEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataPropertyDomainEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataPropertyRangeEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseNegativeDataPropertyAssertionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDatatypeRestrictionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataLiteralEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDatatypeEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataLexicalValueEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataFacetEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDataFacetRestrictionEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseEquivalentDataPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseDisjointDataPropertiesEnd(CRebuildingExpressionStackObject* rebuildingStackObj);
					void parseFunctionalDataPropertyEnd(CRebuildingExpressionStackObject* rebuildingStackObj);


					// protected variables
				protected:
					QStack<CRebuildingExpressionStackObject*> mRebuildingExpStack;
					QList<CRebuildingExpressionStackObject*> mRebuildingExpList;
					QList<CRebuildingExpressionStackObject*> mRebuildingExpContainer;

					class CRebuildingExpressionFunctionPair {
						public:
							CRebuildingExpressionFunctionPair() {
								mStartFunction = nullptr;
								mEndFunction = nullptr;
							}
							CRebuildingExpressionFunctionPair(FStartRebuildingExpressionFunction startFunction, FEndRebuildingExpressionFunction endFunction) {
								mStartFunction = startFunction;
								mEndFunction = endFunction;
							}
							FStartRebuildingExpressionFunction mStartFunction;
							FEndRebuildingExpressionFunction mEndFunction;
					};


					CQtHash<cint64,CRebuildingExpressionFunctionPair> mRebuildingExpFunctionJumpHash;



					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> > mRebuildingChangeAxiomList;
					CMAPPINGLIST<CNamePrefix*> mRebuildingPrefixList;


					// contains also previous ontology data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeAxiomList;

					// contains only updated data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellUpdatedAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractUpdatedAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeUpdatedAxiomList;

			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREBUILDINGBUILDER_H
