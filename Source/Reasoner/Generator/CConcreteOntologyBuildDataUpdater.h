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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYBUILDDATAUPDATER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYBUILDDATAUPDATER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>

// Namespace includes
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

			/*! 
			 *
			 *		\class		CConcreteOntologyBuildDataUpdater
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyBuildDataUpdater : public COntologyBuilder {
				// public methods
				public:
					CConcreteOntologyBuildDataUpdater(CConcreteOntology* updateConcreteOntology);

					virtual bool initializeBuilding();
					virtual bool completeBuilding();

					virtual cint64 getAxiomNumber();

					virtual bool addNameAbbreviation(const QString &name, const QString &abbreviatedName);

					virtual QString resolveAbbreviationName(const QString& name);
					virtual QString resolveAbbreviationName(const QStringRef& name);
					virtual QString getOntologyName();

					virtual CDeclarationAxiomExpression* getDeclaration(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDeclarationAxiomExpression* getDeclaration(CBuildExpression* buildExpression);
					virtual CDeclarationAxiomExpression* getDeclaration(CExpressionEntity* entity);


					// virtual methods from CTBoxAxiomBuilder
					virtual CClassExpression* getClass(const QString& className);
					virtual CClassExpression* getClass(const QStringRef& className);
					virtual CClassTermExpression* getTopClass();
					virtual CClassTermExpression* getBottomClass();

					virtual CObjectIndividualVariableExpression* getIndividualVariable(const QString &individualVariableName, cint64 axiomNumber);
					virtual CObjectIndividualVariableExpression* getIndividualVariable(const QStringRef &individualVariableName, cint64 axiomNumber);


					virtual CEquivalentClassesExpression* getEquivalentClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CEquivalentClassesExpression* getEquivalentClasses(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CSubClassOfExpression* getSubClassOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSubClassOfExpression* getSubClassOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDisjointClassesExpression* getDisjointClasses(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDisjointUnionExpression *getDisjointUnion(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectComplementOfExpression* getObjectComplementOf(CBuildExpression* expression);
					virtual CObjectComplementOfExpression* getObjectComplementOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectIntersectionOfExpression* getObjectIntersectionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectUnionOfExpression* getObjectUnionOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);
					virtual CObjectMinCardinalityExpression* getObjectMinCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CObjectMinCardinalityExpression* getObjectMinCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);
					virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(const CEXPRESSIONLIST<CBuildExpression*>& expressions, int cardinality);
					virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(CBuildExpression* expression1, CBuildExpression* expression2, int cardinality);
					virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectOneOfExpression *getObjectOneOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectOneOfExpression *getObjectOneOf(CBuildExpression *expression);
					virtual CObjectHasValueExpression *getObjectHasValue(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectHasValueExpression *getObjectHasValue(CBuildExpression *expression1, CBuildExpression *expression2);
					virtual CObjectHasSelfExpression *getObjectHasSelf(const CEXPRESSIONLIST<CBuildExpression *> &expressions);
					virtual CObjectHasSelfExpression *getObjectHasSelf(CBuildExpression *expression);


					virtual CEquivalentClassesExpression* getEquivalentClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CSubClassOfExpression* getSubClassOf(CClassTermExpression* expression1, CClassTermExpression* expression2);
					virtual CDisjointClassesExpression* getDisjointClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CDisjointUnionExpression *getDisjointUnion(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CObjectComplementOfExpression* getObjectComplementOf(CClassTermExpression* expression);
					virtual CObjectIntersectionOfExpression* getObjectIntersectionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CObjectUnionOfExpression* getObjectUnionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions);
					virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality);
					virtual CObjectMinCardinalityExpression* getObjectMinCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality);
					virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality);
					virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectOneOfExpression *getObjectOneOf(const CEXPRESSIONLIST<CIndividualTermExpression*> &expressions);
					virtual CObjectOneOfExpression *getObjectOneOf(CIndividualTermExpression* expression);
					virtual CObjectHasValueExpression *getObjectHasValue(CObjectPropertyTermExpression *expression1, CIndividualTermExpression *expression2);
					virtual CObjectHasSelfExpression *getObjectHasSelf(CObjectPropertyTermExpression *expression);





					// virtual methods from CRBoxAxiomBuilder
					virtual CObjectPropertyExpression* getObjectProberty(const QString& probertyName);
					virtual CObjectPropertyExpression* getObjectProberty(const QStringRef& probertyName);
					virtual CObjectPropertyTermExpression* getTopObjectProberty();

					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(CBuildExpression* expression);
					virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(CBuildExpression* expression);
					virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(CBuildExpression* expression);
					virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(CBuildExpression* expression);
					virtual CObjectPropertyChainExpression* getObjectPropertyChain(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectPropertyChainExpression* getObjectPropertyChain(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(CBuildExpression* expression);
					virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(CBuildExpression* expression);
					virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(CBuildExpression* expression);
					virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(CBuildExpression* expression);



					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions, CObjectPropertyTermExpression* expression);
					virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2);
					virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(CObjectPropertyTermExpression* expression1, CObjectPropertyTermExpression* expression2);
					virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(CObjectPropertyTermExpression* expression);
					virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(CObjectPropertyChainExpression* expressions, CObjectPropertyTermExpression* expression);
					virtual CObjectPropertyChainExpression* getObjectPropertyChain(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions);
					virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions);
					virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions);
					virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(CObjectPropertyTermExpression* expression);
					virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(CObjectPropertyTermExpression* expression);

					CObjectPropertyTermExpression* getCorrectedInverseObjectPropertyOf(CObjectPropertyTermExpression* expression);


					// virtual methods from CABoxAxiomBuilder
					virtual CNamedIndividualExpression* getNamedIndividual(const QString& individualName);
					virtual CNamedIndividualExpression* getNamedIndividual(const QStringRef& individualName);
					virtual CAnonymousIndividualExpression* getAnonymousIndividual(const QString& ontologyName, const QString& individualName);
					virtual CAnonymousIndividualExpression* getAnonymousIndividual(const QStringRef& ontologyName, const QStringRef& individualName);

					virtual CClassAssertionExpression* getClassAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CClassAssertionExpression* getClassAssertion(CBuildExpression* expression1, CBuildExpression* expression2);
					virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3);
					virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3);
					virtual CSameIndividualExpression* getSameIndividual(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					virtual CDifferentIndividualsExpression* getDifferentIndividuals(const CEXPRESSIONLIST<CBuildExpression*>& expressions);


					virtual CClassAssertionExpression* getClassAssertion(CIndividualTermExpression* expression1, CClassTermExpression* expression2);
					virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3);
					virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3);
					virtual CSameIndividualExpression* getSameIndividual(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions);
					virtual CDifferentIndividualsExpression* getDifferentIndividuals(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions);

				// protected functions
				protected:

					// protected variables
				protected:
					CConcreteOntology* mOnto;
					COntologyStringMapping* mOntoStrings;
					COntologyBuildData* mOntoBuild;


					CMemoryAllocationManager* mMemManager;


					CBUILDSET<CDeclarationAxiomExpression*>* mDeclarationAxiomSet;

					cint64 mNextAxiomNumber;
					cint64 mNextEntityNumber;
					cint64 mNextMaxAxiomNumberOffset;


					CClassTermExpression* mTopClassExpression;
					CClassTermExpression* mBottomClassExpression;

					CObjectPropertyTermExpression* mTopObjPropExpression;
					CObjectPropertyTermExpression* mBottomObjPropExpression;



					CBUILDLIST<CBuildExpression*>* mExpressionBuildContainerList;




					CBUILDHASH<CObjectIndividualVariableExpression*,cint64>* mIndividualVariableIDHash;

					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mInverseObjectPropertyHash;
					CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >* mInverseObjectPropertyList;


					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mExpressionBuildHash;

					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* mClassBuildHash;
					CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>* mIndividualVariableBuildHash;
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* mObjectPropertyBuildHash;
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* mIndividualBuildHash;
					CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>* mAnoIndividualBuildHash;


					CMAPPINGHASH<CStringRefStringHasher,CConcept*>* mClassNameConceptMapHash;
					CMAPPINGHASH<CStringRefStringHasher,CRole*>* mPropertyNameRoleMapHash;
					CMAPPINGHASH<CStringRefStringHasher,CIndividual*>* mIndividualNameIndividualMapHash;

					CMAPPINGHASH<QString,CNamePrefix*>* mAbbreviatedNamePrefixMapHash;
					CMAPPINGHASH<QString,CNamePrefix*>* mNamePrefixMapHash;
					CMAPPINGLIST<CNamePrefix*>* mPrefixList;


					CBUILDSET<CClassTermExpression*>* mBuildConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mBuildRoleSet;
					CBUILDSET<CIndividualTermExpression*>* mBuildIndividualSet;

					CBUILDLIST<CIndividualTermExpression*>* mBuildIndividualList;
					CBUILDLIST<CObjectPropertyTermExpression*>* mBuildRoleList;
					CBUILDLIST<CClassTermExpression*>* mBuildConceptList;









					CObjectOneOfExpression* mTmpObjectOneOfExpression;


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYBUILDDATAUPDATER_H
