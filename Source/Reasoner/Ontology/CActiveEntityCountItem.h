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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CACTIVEENTITYCOUNTITEM_H
#define KONCLUDE_REASONER_ONTOLOGY_CACTIVEENTITYCOUNTITEM_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Parser/Expressions/CExpressionEntity.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CActiveEntityCountItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CActiveEntityCountItem {
				// public methods
				public:
					//! Constructor
					CActiveEntityCountItem();

					cint64 getActiveCount();
					CActiveEntityCountItem* setActiveCount(cint64 activeCount);
					CActiveEntityCountItem* incActiveCount(cint64 incCount = 1);
					CActiveEntityCountItem* decActiveCount(cint64 decCount = 1);

					CActiveEntityCountItem* setEntity(CExpressionEntity* entity);
					CExpressionEntity* getEntity();

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mActiveCount;
					CExpressionEntity* mEntity;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CACTIVEENTITYCOUNTITEM_H
