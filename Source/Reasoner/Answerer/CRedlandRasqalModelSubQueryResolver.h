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

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALMODELSUBQUERYRESOLVER_H
#define KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALMODELSUBQUERYRESOLVER_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "AnswererSettings.h"
#include "CRedlandRasqalSubQueryProcessingData.h"
#include "CRedlandRasqalSubQueryTripleResolvingData.h"
#include "CRedlandRasqalSubQueryBindingPositionResolvingHash.h"
#include "CRedlandRasqalSubQueryBoundNodeResolvingHash.h"


// Other includes
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CVariableBindingsListAnswerResult.h"
#include "Reasoner/Query/CRedlandRasqalBGPsCompositionQuery.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


#define KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
#ifndef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
	#define KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
#endif

			/*! 
			 *
			 *		\class		CRedlandRasqalModelSubQueryResolver
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandRasqalModelSubQueryResolver {
				// public methods
				public:
					//! Constructor
					CRedlandRasqalModelSubQueryResolver(QVector<CRedlandRasqalSubQueryProcessingData*>* subQueryProcDataVec, librdf_world* world, librdf_query* redlandQuery, rasqal_query* rasqalQuery, QHash<QString, TDependentOntologyPair>* queryKbNameDepOntPair);

					~CRedlandRasqalModelSubQueryResolver();

					
					librdf_model* getRedlandResolvingModel();
					librdf_world* getRedlandWorld();
					rasqal_query* getRasqalQuery();


					QVector<CRedlandRasqalSubQueryProcessingData*>* getSubQueryProcessingDataVector();

					QString getIterationStatusString();

					QHash<QString, TDependentOntologyPair>* getQueryKnowledgeBaseNameDependentOntologyPairHash();
					QHash<QString, TDependentOntologyPair>::const_iterator getQueryKnowledgeBaseNameDependentOntologyPairHashIterator();
					CRedlandRasqalModelSubQueryResolver* setQueryKnowledgeBaseNameDependentOntologyPairHashIterator(QHash<QString, TDependentOntologyPair>::const_iterator it);

					QVector<rasqal_literal*>* getTriplesContextLiteralVector();
					bool isTriplesContextLiteralsReset();
					CRedlandRasqalModelSubQueryResolver* setTriplesContextLiteralsReset(bool reset);


				// protected methods
				protected:
					void releaseRedlandResolvingModel();

				// protected variables
				protected:

					QVector<CRedlandRasqalSubQueryProcessingData*>* mSubQueryProcDataVec;
					librdf_world* mWorld;
					librdf_query* mRedlandQuery;
					rasqal_query* mRasqalQuery;
					librdf_model* mResolvingModel;

					QString* mRasqalErrorString;
					QHash<QString, TDependentOntologyPair>* mQueryKbNameDepOntPairHash;
					QHash<QString, TDependentOntologyPair>::const_iterator mQueryKbNameDepOntPairHashIt;

					QVector<rasqal_literal*> mTriplesContextLiteralVector;
					bool mTriplesContextLiteralsReset;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALMODELSUBQUERYRESOLVER_H

#endif // KONCLUDE_REDLAND_INTEGRATION
