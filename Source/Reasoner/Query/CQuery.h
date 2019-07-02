/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_QUERY_CQUERY_H
#define KONCLUDE_REASONER_QUERY_CQUERY_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CQueryResult.h"
#include "CQuerySupport.h"
#include "CQueryStatistics.h"
#include "CQueryError.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CQuery {
				// public methods
				public:
					//! Constructor
					CQuery();

					//! Destructor
					virtual ~CQuery();

					virtual QString getQueryName() = 0;
					virtual QString getQueryString() = 0;
					virtual QString getAnswerString() = 0;
					virtual bool hasAnswer() = 0;
					virtual bool hasError();

					virtual CQueryResult* getQueryResult() = 0;
					virtual CQuerySupport* getQuerySupport();
					virtual CQueryStatistics* getQueryStatistics();

					virtual QStringList getErrorStringList();
					virtual CQuery* addErrorString(const QString& errorString);
					virtual CQuery* addQueryError(CQueryError* queryError);
					virtual CQueryError* getQueryErrorLinker();


				// protected methods
				protected:

				// protected variables
				protected:
					CQueryError* mErrorLinker;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CQUERY_H
