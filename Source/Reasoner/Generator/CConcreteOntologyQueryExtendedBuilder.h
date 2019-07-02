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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYEXTENDEDBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYEXTENDEDBUILDER_H

// Libraries includes


// Namespace includes
#include "CQueryBuilderGenerator.h"
#include "CConcreteOntologyQueryBasicBuilder.h"
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
#include "Reasoner/Query/CAreAxiomsEntailedQuery.h"

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
			 *		\class		CConcreteOntologyQueryExtendedBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyQueryExtendedBuilder : public CConcreteOntologyQueryBasicBuilder {
				// public methods
				public:
					//! Constructor
					CConcreteOntologyQueryExtendedBuilder(CConcreteOntology* ontology, CConfigurationBase *configuration, COntologyBuilder* ontologyBuilder);

					//! Destructor
					virtual ~CConcreteOntologyQueryExtendedBuilder();

					virtual QList<CQuery*> generateQuerys();



					virtual CQueryIsEntailedExpression* getIsEntailedQuery(const CEXPRESSIONLIST<CAxiomExpression*>& axiomExpressions, const QString& queryName);


					virtual CQueryAreClassesEquivalentExpression* getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CClassTermExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryAreClassesDisjointExpression* getAreClassesDisjointQuery(const CEXPRESSIONLIST<CClassTermExpression*>& testClassExpressions, const QString& queryName);
					virtual CQueryIsClassSubsumedByExpression* getIsClassSubsumedByQuery(CClassTermExpression* subsumerClassExpression, CClassTermExpression* subsumedClassExpression, const QString& queryName);


				// protected methods
				protected:

				// protected variables
				protected:		

					COntologyBuilder* mOntologyBuilder;

					CEXPRESSIONLIST<CBuildExpression*> container;


					CEXPRESSIONLIST<CQueryIsClassSatisfiableExpression*> classSatExpList;
					CEXPRESSIONLIST<CQueryAreClassesEquivalentExpression*> classEqExpList;
					CEXPRESSIONLIST<CQueryIsClassSubsumedByExpression*> classSubsumedExpList;
					CEXPRESSIONLIST<CQueryAreClassesDisjointExpression*> classDisjointExpList;
					CEXPRESSIONLIST<CQueryIsInstanceOfExpression*> instanceExpList;
					CEXPRESSIONLIST<CQueryGetFlattenedTypesExpression*> flattenedTypesExpList;
					CEXPRESSIONLIST<CQueryGetFlattenedInstancesExpression*> flattenedInstancesExpList;
					CEXPRESSIONLIST<CQueryIsEntailedExpression*> isEntailedExpList;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYQUERYEXTENDEDBUILDER_H
