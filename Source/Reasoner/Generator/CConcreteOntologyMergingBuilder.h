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

#ifndef KONCLUDE_REASONER_GENERATOR_CConcreteOntologyMergingBuilder_H
#define KONCLUDE_REASONER_GENERATOR_CConcreteOntologyMergingBuilder_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>
#include <QStack>

// Namespace includes
#include "CConcreteOntologyBuildDataUpdater.h"
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


			class CConcreteOntologyMergingBuilder;
			class CMergeExpressionStackObject;

			typedef void (CConcreteOntologyMergingBuilder::*FStartMergeExpressionFunction) (CMergeExpressionStackObject*  mergeStackObj);
			typedef void (CConcreteOntologyMergingBuilder::*FEndMergeExpressionFunction) (CMergeExpressionStackObject*  mergeStackObj);


			class CMergeExpressionStackObject {
				public:
					inline CMergeExpressionStackObject(CBuildExpression* expression) : mExpression(expression) {
					}

					inline CMergeExpressionStackObject* addProcessingBuildExpression(CBuildExpression* buildExpression) {
						mProcessingExpList.append(buildExpression);
						return this;
					}

					inline CMergeExpressionStackObject* addProcessingBuildExpressions(CQtList<CBuildExpression*>* expressions) {
						if (mProcessingExpList.isEmpty()) {
							mProcessingExpList = *expressions;
						} else {
							mProcessingExpList.append(*expressions);
						}
						return this;
					}

					inline CMergeExpressionStackObject* addReplacedBuildExpression(CBuildExpression* buildExpression) {
						mReplacedExpList.append(buildExpression);
						return this;
					}

					inline CMergeExpressionStackObject* addReplacedBuildExpressions(CQtList<CBuildExpression*>* expressions) {
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
					
					FStartMergeExpressionFunction mStartFunction;
					FEndMergeExpressionFunction mEndFunction;
					CBuildExpression* mExpression;
					CBuildExpression* mReplaceExpression;
					CQtList<CBuildExpression*> mProcessingExpList;
					CQtList<CBuildExpression*> mReplacedExpList;
			};


			/*! 
			 *
			 *		\class		CConcreteOntologyMergingBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyMergingBuilder : public CConcreteOntologyBuildDataUpdater {
				// public methods
				public:
					CConcreteOntologyMergingBuilder(CConcreteOntology* baseConcreteOntology, CConcreteOntology* mergingConcreteOntology, CConcreteOntology* prevConcreteOntology);
					virtual ~CConcreteOntologyMergingBuilder();

					virtual bool initializeBuilding();
					virtual bool completeBuilding();
					virtual bool mergingBuilding();

					virtual bool tellOntologyAxiom(CAxiomExpression* axiom);
					virtual bool retractOntologyAxiom(CAxiomExpression* axiom);

					void addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CClassTermExpression*>* expressionList);
					void addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CObjectPropertyTermExpression*>* expressionList);
					void addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CBuildExpression*>* expressionList);
					void addExpressions(CMergeExpressionStackObject* mergeStackObj, CEXPRESSIONLIST<CIndividualTermExpression*>* expressionList);
					void addExpressions(CMergeExpressionStackObject* mergeStackObj, CBuildExpression* expression);


					void parseClassStart(CMergeExpressionStackObject* mergeStackObj);
					void parseIndividualVariableStart(CMergeExpressionStackObject* mergeStackObj);
					void parseEquivalentClassesStart(CMergeExpressionStackObject* mergeStackObj);
					void parseSubClassOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseDisjointClassesStart(CMergeExpressionStackObject* mergeStackObj);
					void parseDisjointUnionStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectComplementOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectIntersectionOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectUnionOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectMaxCardinalityStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectMinCardinalityStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectExactlyCardinalityStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectAllValuesFromStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectSomeValuesFromStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectOneOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectHasValueStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectHasSelfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectProbertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseSubObjectPropertyOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyDomainStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyRangeStart(CMergeExpressionStackObject* mergeStackObj);
					void parseInverseObjectPropertiesStart(CMergeExpressionStackObject* mergeStackObj);
					void parseTransetiveObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseFunctionalObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseInverseFunctionalObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseInverseObjectPropertyOfStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyChainStart(CMergeExpressionStackObject* mergeStackObj);
					void parseEquivalentObjectPropertiesStart(CMergeExpressionStackObject* mergeStackObj);
					void parseDisjointObjectPropertiesStart(CMergeExpressionStackObject* mergeStackObj);
					void parseSymmetricObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseAsymmetricObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseReflexiveObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseIrreflexiveObjectPropertyStart(CMergeExpressionStackObject* mergeStackObj);
					void parseNamedIndividualStart(CMergeExpressionStackObject* mergeStackObj);
					void parseAnonymousIndividualStart(CMergeExpressionStackObject* mergeStackObj);
					void parseClassAssertionStart(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyAssertionStart(CMergeExpressionStackObject* mergeStackObj);
					void parseNegativeObjectPropertyAssertionStart(CMergeExpressionStackObject* mergeStackObj);
					void parseSameIndividualStart(CMergeExpressionStackObject* mergeStackObj);
					void parseDifferentIndividualsStart(CMergeExpressionStackObject* mergeStackObj);


					void parseClassEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseIndividualVariableEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseEquivalentClassesEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseSubClassOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseDisjointClassesEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseDisjointUnionEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectComplementOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectIntersectionOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectUnionOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectMaxCardinalityEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectMinCardinalityEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectExactlyCardinalityEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectAllValuesFromEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectSomeValuesFromEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectOneOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectHasValueEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectHasSelfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectProbertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseSubObjectPropertyOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyDomainEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyRangeEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseInverseObjectPropertiesEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseTransetiveObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseFunctionalObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseInverseFunctionalObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseInverseObjectPropertyOfEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyChainEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseEquivalentObjectPropertiesEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseDisjointObjectPropertiesEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseSymmetricObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseAsymmetricObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseReflexiveObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseIrreflexiveObjectPropertyEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseNamedIndividualEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseAnonymousIndividualEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseClassAssertionEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseObjectPropertyAssertionEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseNegativeObjectPropertyAssertionEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseSameIndividualEnd(CMergeExpressionStackObject* mergeStackObj);
					void parseDifferentIndividualsEnd(CMergeExpressionStackObject* mergeStackObj);

				// protected functions
				protected:
					virtual CMergeExpressionStackObject* createMergeExpressionStackObject(CBuildExpression* expression);
					virtual CBuildExpression* handleMergeEpxressionStack(CBuildExpression* expression);
					virtual CConcreteOntologyMergingBuilder* initJumpingHash();

					// protected variables
				protected:
					QStack<CMergeExpressionStackObject*> mMergeExpStack;
					QList<CMergeExpressionStackObject*> mMergeExpList;
					QList<CMergeExpressionStackObject*> mMergeExpContainer;

					class CMergeExpressionFunctionPair {
						public:
							CMergeExpressionFunctionPair() {
								mStartFunction = nullptr;
								mEndFunction = nullptr;
							}
							CMergeExpressionFunctionPair(FStartMergeExpressionFunction startFunction, FEndMergeExpressionFunction endFunction) {
								mStartFunction = startFunction;
								mEndFunction = endFunction;
							}
							FStartMergeExpressionFunction mStartFunction;
							FEndMergeExpressionFunction mEndFunction;
					};


					CQtHash<cint64,CMergeExpressionFunctionPair> mMergeExpFunctionJumpHash;


					CConcreteOntology* mBaseConcreteOntology;
					CConcreteOntology* mMergingConcreteOntology;
					CConcreteOntology* mPrevConcreteOntology;


					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> > mMergeChangeAxiomList;
					CMAPPINGLIST<CNamePrefix*> mMergePrefixList;


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

#endif // end KONCLUDE_REASONER_GENERATOR_CConcreteOntologyMergingBuilder_H
