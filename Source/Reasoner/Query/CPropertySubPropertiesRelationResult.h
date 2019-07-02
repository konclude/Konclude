/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CPROPERTYSUBPROPERTIESRELATIONRESULT_H
#define KONCLUDE_REASONER_QUERY_CPROPERTYSUBPROPERTIESRELATIONRESULT_H

// Libraries includes

// Namespace includes
#include "CStringSubStringsRelationResult.h"
#include "CPropertySynsetResult.h"
#include "CQueryResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CPropertySubPropertiesRelationResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPropertySubPropertiesRelationResult : public CStringSubStringsRelationResult<CPropertySubPropertiesRelationResult,CPropertySynsetResult> {
				// public methods
				public:
					//! Constructor
					CPropertySubPropertiesRelationResult();
					CPropertySubPropertiesRelationResult(CPropertySynsetResult *superPropertySynset);

					CPropertySynsetResult *getSuperPropertySynset();
					CPropertySubPropertiesRelationResult *setSuperPropertySynset(CPropertySynsetResult *superPropertySynset);

					QSet<CPropertySynsetResult *>* getSubPropertiesSynsetSet();
					QList<CPropertySynsetResult *> getSubPropertiesSynsetList();
					CPropertySubPropertiesRelationResult *addSubPropertiesSynset(CPropertySynsetResult *subPropertySynset);
					CPropertySubPropertiesRelationResult *addSubPropertiesSynsets(const QList<CPropertySynsetResult *> &subPropertiesSynsets);

					bool hasSubPropertiesSynset(CPropertySynsetResult *subPropertySynset);


					virtual QString getQueryResultString();

					qint64 getSubPropertiesSynsetCount();


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CPROPERTYSUBPROPERTIESRELATIONRESULT_H
