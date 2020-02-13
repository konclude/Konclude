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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYTRIPLERESOLVINGDATA_H
#define KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYTRIPLERESOLVINGDATA_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComplexQueryCompositionProcessingData.h"



// Other includes
#include "Reasoner/Query/CComplexAnsweringExpressionQuery.h"
#include "Reasoner/Query/CVariableBindingsAnswersResultIterator.h"

#include "Concurrent/Callback/CCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CRedlandRasqalSubQueryTripleResolvingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandRasqalSubQueryTripleResolvingData {
				// public methods
			public:
				//! Constructor
				CRedlandRasqalSubQueryTripleResolvingData(CRedlandRasqalSubQueryProcessingData* subQueryData, cint64 triplePos, cint64 subjectPos, cint64 predicatePos, cint64 objectPos);

				~CRedlandRasqalSubQueryTripleResolvingData();

				cint64 getTriplePosition();
				CRedlandRasqalSubQueryProcessingData* getSubQueryProcessingData();


				bool isResolved();
				CRedlandRasqalSubQueryTripleResolvingData* setResolved(bool resolved);

				cint64 getBindingsVectorResolvingPosition(cint64 pos);

				cint64 getSubjectBindingsVectorResolvingPosition();
				cint64 getPredicateBindingsVectorResolvingPosition();
				cint64 getObjectBindingsVectorResolvingPosition();


				bool getVariableBound(cint64 pos);
				bool getSubjectVariableBound();
				bool getPredicateVariableBound();
				bool getObjectVariableBound();

				CRedlandRasqalSubQueryTripleResolvingData* setSubjectVariableBound(bool bound);
				CRedlandRasqalSubQueryTripleResolvingData* setPredicateVariableBound(bool bound);
				CRedlandRasqalSubQueryTripleResolvingData* setObjectVariableBound(bool bound);

				rasqal_variable* getVariable(cint64 pos);
				CRedlandRasqalSubQueryTripleResolvingData* setVariable(cint64 pos, rasqal_variable* var);

				rasqal_variable* getSubjectVariable();
				rasqal_variable* getPredicateVariable();
				rasqal_variable* getObjectVariable();

				CRedlandRasqalSubQueryTripleResolvingData* setSubjectVariable(rasqal_variable* var);
				CRedlandRasqalSubQueryTripleResolvingData* setPredicateVariable(rasqal_variable* var);
				CRedlandRasqalSubQueryTripleResolvingData* setObjectVariable(rasqal_variable* var);


				librdf_node* getBoundNode(cint64 pos);
				CRedlandRasqalSubQueryTripleResolvingData* setBoundNode(cint64 pos, librdf_node* node);

				CRedlandRasqalSubQueryTripleResolvingData* setSubjectBoundNode(librdf_node* node);
				CRedlandRasqalSubQueryTripleResolvingData* setPredicateBoundNode(librdf_node* node);
				CRedlandRasqalSubQueryTripleResolvingData* setObjectBoundNode(librdf_node* node);

				rasqal_literal* getBoundLiteral(cint64 pos);
				CRedlandRasqalSubQueryTripleResolvingData* setBoundLiteral(cint64 pos, rasqal_literal* node);

				CRedlandRasqalSubQueryTripleResolvingData* setSubjectBoundLiteral(rasqal_literal* node);
				CRedlandRasqalSubQueryTripleResolvingData* setPredicateBoundLiteral(rasqal_literal* node);
				CRedlandRasqalSubQueryTripleResolvingData* setObjectBoundLiteral(rasqal_literal* node);

				librdf_statement* getNextTransferringStatement();


				// protected methods
			protected:

				// protected variables
			protected:
				bool mResolved;
				cint64 mTriplePos;
				CRedlandRasqalSubQueryProcessingData* mSubQueryData;
				cint64 mPos[3];
				bool mVariableBound[3];
				rasqal_variable* mVariable[3];
				librdf_node* mBoundNode[3];
				rasqal_literal* mBoundLiteral[3];

				librdf_statement* mTransferringStatement[3];
				cint64 mNextTransferrringStatementPos = 0;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYTRIPLERESOLVINGDATA_H

#endif // KONCLUDE_REDLAND_INTEGRATION
