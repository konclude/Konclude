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

#ifndef KONCLUDE_REASONER_QUERY_CCOMPLEXASSERTIONSINDIVIDUALVARIABLESANSWERINGQUERY_H
#define KONCLUDE_REASONER_QUERY_CCOMPLEXASSERTIONSINDIVIDUALVARIABLESANSWERINGQUERY_H

// Libraries includes


// Namespace includes
#include "CComplexAnsweringQuery.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CComplexAssertionsIndividualVariablesAnsweringQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexAssertionsIndividualVariablesAnsweringQuery : public CComplexAnsweringQuery {
				// public methods
				public:
					//! Constructor
					CComplexAssertionsIndividualVariablesAnsweringQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName);

					//! Destructor
					virtual ~CComplexAssertionsIndividualVariablesAnsweringQuery();


					QList<CAxiomExpression*>* getAxiomExpressions();
					QList<CExpressionVariable*>* getDistinguishedVariableExpressions();

					bool isDistinctRequired();
					CComplexAssertionsIndividualVariablesAnsweringQuery* setDistinctRequired(bool distinctRequired);

					bool isBooleanEntailmentResultRequired();
					CComplexAssertionsIndividualVariablesAnsweringQuery* setBooleanEntailmentResultRequired(bool booleanEntailmentResultRequired);

					cint64 getResultLimit();
					CComplexAssertionsIndividualVariablesAnsweringQuery* setResultLimit(cint64 resultLimit);

					cint64 getResultOffset();
					CComplexAssertionsIndividualVariablesAnsweringQuery* setResultOffset(cint64 resultOffset);

					cint64 getResultLimitIncludingOffset();



				// protected methods
				protected:

				// protected variables
				protected:
					QList<CAxiomExpression*> mAxiomExpressions;
					QList<CExpressionVariable*> mDisVariables;

					bool mDistinct;
					bool mBooleanEntailmentResult;

					cint64 mResultLimit;
					cint64 mResultOffset;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CCOMPLEXASSERTIONSINDIVIDUALVARIABLESANSWERINGQUERY_H
