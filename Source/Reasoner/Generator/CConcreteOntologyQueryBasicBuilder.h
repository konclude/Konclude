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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYBASICBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYBASICBUILDER_H

// Libraries includes


// Namespace includes
#include "CQueryBuilderGenerator.h"
#include "CSatisfiableCalculationJobGenerator.h"
#include "CExpressionSplitter.h"


// Other includes
#include "Reasoner/Query/CIsClassSatisfiableQuery.h"
#include "Reasoner/Query/CAreClassesEquivalentQuery.h"
#include "Reasoner/Query/CIsClassSubsumedByQuery.h"
#include "Reasoner/Query/CAreClassesDisjointQuery.h"
#include "Reasoner/Query/CIsClassSubsumedByQuery.h"
#include "Reasoner/Query/CIsInstanceOfQuery.h"
#include "Reasoner/Query/CQueryStatisticsCollectionStrings.h"
#include "Reasoner/Query/CFlattenedTypesQuery.h"
#include "Reasoner/Query/CFlattenedInstancesQuery.h"
#include "Reasoner/Query/CTypesQuery.h"
#include "Reasoner/Query/CInstancesQuery.h"
#include "Reasoner/Query/CSubClassesQuery.h"
#include "Reasoner/Query/CSuperClassesQuery.h"
#include "Reasoner/Query/CEquivalentClassesQuery.h"
#include "Reasoner/Query/CAreAxiomsEntailedQuery.h"
#include "Reasoner/Query/CObjectPropertyTargetsQuery.h"
#include "Reasoner/Query/CFlattenedObjectPropertyTargetsQuery.h"
#include "Reasoner/Query/CSameIndividualsQuery.h"

#include "Reasoner/Query/CNondeterministicIndividualsQuery.h"
#include "Reasoner/Query/CDeterministicIndividualsQuery.h"
#include "Reasoner/Query/CDeterministicClassAssertionQuery.h"
#include "Reasoner/Query/CNondeterministicClassAssertionQuery.h"


#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/CIRIName.h"

#include "Parser/COntologyBuilder.h"

#include "Config/CConfigurationBase.h"
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Parser;
	using namespace Config;

	namespace Reasoner {

		using namespace Query;
		using namespace Ontology;

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyQueryBasicBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyQueryBasicBuilder : public CQueryBuilderGenerator {
				// public methods
				public:
					//! Constructor
					CConcreteOntologyQueryBasicBuilder(CConcreteOntology* ontology, CConfigurationBase *configuration);

					//! Destructor
					virtual ~CConcreteOntologyQueryBasicBuilder();

					virtual QList<CQuery*> generateQuerys();


					virtual CQueryIsClassSatisfiableExpression* getIsClassSatisfiableQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryAreClassesEquivalentExpression* getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryAreClassesDisjointExpression* getAreClassesDisjointQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryIsClassSubsumedByExpression* getIsClassSubsumedByQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryIsInstanceOfExpression* getIsInstanceOfQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryGetFlattenedTypesExpression* getGetFlattenedTypesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName);
					virtual CQueryGetFlattenedInstancesExpression* getGetFlattenedInstancesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName);
					virtual CQueryGetTypesExpression* getGetTypesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName);
					virtual CQueryGetSameIndividualsExpression* getGetSameIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryGetInstancesExpression* getGetInstancesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName);
					virtual CQueryGetSubClassesExpression* getGetSubClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName);
					virtual CQueryGetSuperClassesExpression* getGetSuperClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName);
					virtual CQueryGetEquivalentClassesExpression* getGetEquivalentClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryIsEntailedExpression* getIsEntailedQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryGetObjectPropertyTargetsExpression* getGetObjectPropertyTargetsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName);
					virtual CQueryGetFlattenedObjectPropertyTargetsExpression* getGetFlattenedObjectPropertyTargetsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName);


					virtual CQueryGetDeterministicIndividualsExpression* getGetDeterministicIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName);
					virtual CQueryGetNondeterministicIndividualsExpression* getGetNondeterministicIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName);
					virtual CQueryGetDeterministicClassAssertionsExpression* getGetDeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName);
					virtual CQueryGetNondeterministicClassAssertionsExpression* getGetNondeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName);


					virtual CQueryIsClassSatisfiableExpression* getIsClassSatisfiableQuery(CClassTermExpression* testClassExpressions, const QString& queryName);
					virtual CQueryIsInstanceOfExpression* getIsInstanceOfQuery(CIndividualTermExpression* individualTermExpression, CClassTermExpression* classExpression, const QString& queryName);
					virtual CQueryGetFlattenedTypesExpression* getGetFlattenedTypesQuery(CIndividualTermExpression* individualTermExpression, bool direct, const QString& queryName);
					virtual CQueryGetFlattenedInstancesExpression* getGetFlattenedInstancesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName);
					virtual CQueryGetTypesExpression* getGetTypesQuery(CIndividualTermExpression* individualTermExpression, bool direct, const QString& queryName);
					virtual CQueryGetSameIndividualsExpression* getGetSameIndividualsQuery(CIndividualTermExpression* individualTermExpression, const QString& queryName);
					virtual CQueryGetInstancesExpression* getGetInstancesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName);
					virtual CQueryGetSubClassesExpression* getGetSubClassesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName);
					virtual CQueryGetSuperClassesExpression* getGetSuperClassesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName);
					virtual CQueryGetEquivalentClassesExpression* getGetEquivalentClassesQuery(CClassTermExpression* classTermExpression, const QString& queryName);

					virtual CQueryGetObjectPropertyTargetsExpression* getGetObjectPropertyTargetsQuery(CObjectPropertyTermExpression* objectPropertyExpression, CIndividualTermExpression* individualExpression, const QString& queryName);
					virtual CQueryGetFlattenedObjectPropertyTargetsExpression* getGetFlattenedObjectPropertyTargetsQuery(CObjectPropertyTermExpression* objectPropertyExpression, CIndividualTermExpression* individualExpression, const QString& queryName);


					virtual CQueryGetDeterministicIndividualsExpression* getGetDeterministicIndividualsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName);
					virtual CQueryGetNondeterministicIndividualsExpression* getGetNondeterministicIndividualsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName);
					virtual CQueryGetDeterministicClassAssertionsExpression* getGetDeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName);
					virtual CQueryGetNondeterministicClassAssertionsExpression* getGetNondeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName);



					virtual CQueryAreClassesEquivalentExpression* getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CClassTermExpression*>& testClassExpressions, const QString& queryName) = 0;
					virtual CQueryAreClassesDisjointExpression* getAreClassesDisjointQuery(const CEXPRESSIONLIST<CClassTermExpression*>& testClassExpressions, const QString& queryName) = 0;
					virtual CQueryIsClassSubsumedByExpression* getIsClassSubsumedByQuery(CClassTermExpression* subsumerClassExpression, CClassTermExpression* subsumedClassExpression, const QString& queryName) = 0;
					virtual CQueryIsEntailedExpression* getIsEntailedQuery(const CEXPRESSIONLIST<CAxiomExpression*>& axiomExpressions, const QString& queryName) = 0;



				// protected methods
				protected:
					virtual QString getStringFromConcept(CConcept *concept);
					virtual CConcept *getConceptFromBuildExpression(CClassTermExpression *buildExp);
					virtual CRole *getRoleFromBuildExpression(CObjectPropertyTermExpression *buildExp);
					virtual CIndividual *getIndividualFromBuildExpression(CIndividualTermExpression *buildExp);

				// protected variables
				protected:		

					CConcreteOntology* mOntology;
					CConfigurationBase* config;

					CEXPRESSIONLIST<CBuildExpression*> container;


					CEXPRESSIONLIST<CQueryIsClassSatisfiableExpression*> classSatExpList;
					CEXPRESSIONLIST<CQueryAreClassesEquivalentExpression*> classEqExpList;
					CEXPRESSIONLIST<CQueryIsClassSubsumedByExpression*> classSubsumedExpList;
					CEXPRESSIONLIST<CQueryAreClassesDisjointExpression*> classDisjointExpList;
					CEXPRESSIONLIST<CQueryIsInstanceOfExpression*> instanceExpList;
					CEXPRESSIONLIST<CQueryGetFlattenedTypesExpression*> flattenedTypesExpList;
					CEXPRESSIONLIST<CQueryGetFlattenedInstancesExpression*> flattenedInstancesExpList;
					CEXPRESSIONLIST<CQueryGetTypesExpression*> typesExpList;
					CEXPRESSIONLIST<CQueryGetSameIndividualsExpression*> sameIndiExpList;
					CEXPRESSIONLIST<CQueryGetInstancesExpression*> instancesExpList;
					CEXPRESSIONLIST<CQueryGetSubClassesExpression*> mSubClassesExpList;
					CEXPRESSIONLIST<CQueryGetSuperClassesExpression*> mSuperClassesExpList;
					CEXPRESSIONLIST<CQueryGetEquivalentClassesExpression*> mEquivClassesExpList;
					CEXPRESSIONLIST<CQueryGetObjectPropertyTargetsExpression*> mObjectPropTargetsExpList;
					CEXPRESSIONLIST<CQueryGetFlattenedObjectPropertyTargetsExpression*> mObjectFlattenedPropTargetsExpList;


					CEXPRESSIONLIST<CQueryGetDeterministicIndividualsExpression*> mDetIndisExpList;
					CEXPRESSIONLIST<CQueryGetNondeterministicIndividualsExpression*> mNdetIndisExpList;
					CEXPRESSIONLIST<CQueryGetDeterministicClassAssertionsExpression*> mDetClassAssExpList;
					CEXPRESSIONLIST<CQueryGetNondeterministicClassAssertionsExpression*> mNdetClassAssExpList;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYBASICBUILDER_H
