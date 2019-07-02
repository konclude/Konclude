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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDEXPRESSIONCOUNTER_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDEXPRESSIONCOUNTER_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"


// Other includes
#include "Parser/Expressions/CBuildExpression.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyBuildExpressionCounter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyBuildExpressionCounter {
				// public methods
				public:
					//! Constructor
					COntologyBuildExpressionCounter(COntologyContext* ontoContext = nullptr);


					COntologyBuildExpressionCounter* referenceBuildExpressionCounter(COntologyBuildExpressionCounter* buildExpCounter);

					cint64& getBuildExpressionCount(CBuildExpression::ExpressionType expType);
					COntologyBuildExpressionCounter* incBuildExpressionCount(CBuildExpression::ExpressionType expType, cint64 incCount = 1);
					COntologyBuildExpressionCounter* resetBuildExpressionCount();
					bool hasBuildExpressions(CBuildExpression::ExpressionType expType);

					cint64 getMinimumMaxObjectCardinality();
					cint64 getMaximumMaxObjectCardinality();

					cint64 getMinimumMinObjectCardinality();
					cint64 getMaximumMinObjectCardinality();

					COntologyBuildExpressionCounter* setMinObjectCardinalityMinimumMaximumCandidate(cint64 cardinality);
					COntologyBuildExpressionCounter* setMaxObjectCardinalityMinimumMaximumCandidate(cint64 cardinality);


					cint64 getMinimumMaxDataCardinality();
					cint64 getMaximumMaxDataCardinality();

					cint64 getMinimumMinDataCardinality();
					cint64 getMaximumMinDataCardinality();

					COntologyBuildExpressionCounter* setMinDataCardinalityMinimumMaximumCandidate(cint64 cardinality);
					COntologyBuildExpressionCounter* setMaxDataCardinalityMinimumMaximumCandidate(cint64 cardinality);



					COntologyBuildExpressionCounter* incRefferedBottomClassCount(cint64 incCount = 1);
					cint64 getRefferedBottomClassCount();

					COntologyBuildExpressionCounter* incRefferedBottomObjectPropertyCount(cint64 incCount = 1);
					cint64 getRefferedBottomObjectPropertyCount();

					COntologyBuildExpressionCounter* incRefferedBottomDataPropertyCount(cint64 incCount = 1);
					cint64 getRefferedBottomDataPropertyCount();


					bool hasConsistencyProblematicExpressions();


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;


					const static cint64 mExpTypeCount = 200;
					cint64* mExpCountVec;


					cint64 mMinimumMaxObjectCardinality;
					cint64 mMaximumMaxObjectCardinality;

					cint64 mMinimumMinObjectCardinality;
					cint64 mMaximumMinObjectCardinality;



					cint64 mMinimumMinDataCardinality;
					cint64 mMaximumMinDataCardinality;

					cint64 mMinimumMaxDataCardinality;
					cint64 mMaximumMaxDataCardinality;


					cint64 mRefferedBottomClassCount;
					cint64 mRefferedBottomObjectPropertyCount;
					cint64 mRefferedBottomDataPropertyCount;

					bool mConsistencyProblematicExpressions;
					bool mConsistencyProblematicExpressionsChecked;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDEXPRESSIONCOUNTER_H
