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

#ifndef KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETRESULT_H
#define KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETRESULT_H

// Libraries includes
#include <QStringList>

// Namespace includes
#include "CQueryResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CIndividualSynsetResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualSynsetResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CIndividualSynsetResult();
					CIndividualSynsetResult(const QString &individualName);
					CIndividualSynsetResult(const QStringList &individualNameList);

					//! Destructor
					virtual ~CIndividualSynsetResult();

					QStringList getEquivalentIndividualNameList();
					CIndividualSynsetResult *addEquivalentIndividualName(const QString &individualName);
					CIndividualSynsetResult *addEquivalentIndividualNames(const QStringList &individualNameList);

					bool hasIndividualName(const QString &individualName);
					bool hasIndividualNames(const QStringList &individualNameList);

					qint64 getIndividualCount();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);


				// protected methods
				protected:

				// protected variables
				protected:
					QStringList eqIndividualNameList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETRESULT_H
