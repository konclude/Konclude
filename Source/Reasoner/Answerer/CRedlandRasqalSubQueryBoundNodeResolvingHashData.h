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

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYBOUNDNODERESOLVINGHASHDATA_H
#define KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYBOUNDNODERESOLVINGHASHDATA_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "AnswererSettings.h"


// Other includes
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersListResult.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CRedlandRasqalSubQueryBoundNodeResolvingHashData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandRasqalSubQueryBoundNodeResolvingHashData {
				// public methods
				public:
					//! Constructor
					CRedlandRasqalSubQueryBoundNodeResolvingHashData(CVariableBindingsAnswersResult* varIndBindsResult, bool takeOwnership = true);

					~CRedlandRasqalSubQueryBoundNodeResolvingHashData();

					CVariableBindingsAnswersResult* getResolvedVariableBindingsResult();

					CRedlandRasqalSubQueryBindingPositionResolvingHash* getBindingPositionResolvingHash();



				// protected methods
				protected:

				// protected variables
				protected:
					CVariableBindingsAnswersResult* mVarIndBindsResult;
					bool mVarBindResultOwnership;

					CRedlandRasqalSubQueryBindingPositionResolvingHash* mBindingPositionResolvingHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYBOUNDNODERESOLVINGHASHDATA_H

#endif // KONCLUDE_REDLAND_INTEGRATION
