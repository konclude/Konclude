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

#ifndef KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETSRESULT_H
#define KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETSRESULT_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CQueryResult.h"
#include "CIndividualSynsetResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CIndividualSynsetsResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualSynsetsResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CIndividualSynsetsResult();

					//! Destructor
					virtual ~CIndividualSynsetsResult();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					CIndividualSynsetResult* getIndividualSynset(const QString& individualName, bool create = true);
					CIndividualSynsetResult* getIndividualSynset(const QStringList& individualNames, bool create = true);
					CIndividualSynsetResult* addIndividualSynset(CIndividualSynsetResult* takeIndividualSynset);
				
					QList<CIndividualSynsetResult*>* getIndividualSynsetList();

				// protected methods
				protected:

				// protected variables
				protected:
					QList<CIndividualSynsetResult*> mIndividualSynsetContainer;
					QHash<QString,CIndividualSynsetResult*> mIndividualIndividualSynsetHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETSRESULT_H
