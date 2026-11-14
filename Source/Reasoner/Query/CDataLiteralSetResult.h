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
 */#ifndef KONCLUDE_REASONER_QUERY_CDATALITERALSETRESULT_H
#define KONCLUDE_REASONER_QUERY_CDATALITERALSETRESULT_H

// Libraries includes
#include <QList>

// Namespace includes
#include "CQueryResult.h"


// Other includes
#include "Reasoner/Ontology/CDataLiteral.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CDataLiteralSetResult
			 *
			 *		\brief		Result of an OWLlink data property target request: a set of literals.
			 *
			 */
			class CDataLiteralSetResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CDataLiteralSetResult();

					virtual ~CDataLiteralSetResult();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					CDataLiteralSetResult* addDataLiteral(CDataLiteral* dataLiteral);

					QList<CDataLiteral*>* getDataLiteralList();

				// protected methods
				protected:

				// protected variables
				protected:
					QList<CDataLiteral*> mDataLiteralList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CDATALITERALSETRESULT_H