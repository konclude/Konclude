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

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYBINDINGPOSITIONRESOLVINGHASH_H
#define KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYBINDINGPOSITIONRESOLVINGHASH_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "AnswererSettings.h"
#include "CRedlandRasqalSubQueryBindingPositionResolvingHashData.h"



// Other includes
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CRedlandRasqalSubQueryBindingPositionResolvingHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandRasqalSubQueryBindingPositionResolvingHash : public QHash<cint64, CRedlandRasqalSubQueryBindingPositionResolvingHashData*> {
				// public methods
				public:
					//! Constructor
					CRedlandRasqalSubQueryBindingPositionResolvingHash(CVariableBindingsAnswersResult* varIndBindsResult);

					~CRedlandRasqalSubQueryBindingPositionResolvingHash();

					CRedlandRasqalSubQueryBindingPositionResolvingHashData* getBindingPositionResolvingData(cint64 pos);




				// protected methods
				protected:

				// protected variables
				protected:
					CVariableBindingsAnswersResult* mVarIndBindsResult;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDRASQALSUBQUERYBINDINGPOSITIONRESOLVINGHASH_H

#endif // KONCLUDE_REDLAND_INTEGRATION
