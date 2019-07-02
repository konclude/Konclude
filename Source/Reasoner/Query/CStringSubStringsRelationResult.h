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

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSUBSTRINGSRELATIONRESULT_H
#define KONCLUDE_REASONER_QUERY_CSTRINGSUBSTRINGSRELATIONRESULT_H

// Libraries includes

// Namespace includes
#include "CStringSynsetResult.h"
#include "CQueryResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CStringSubStringsRelationResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class E>
			class CStringSubStringsRelationResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CStringSubStringsRelationResult();
					CStringSubStringsRelationResult(E *superStringSynset);

					//! Destructor
					virtual ~CStringSubStringsRelationResult();

					E *getSuperStringSynset();
					T *setSuperStringSynset(E *superStringSynset);

					QSet<E *>* getSubStringsSynsetSet();
					QList<E *> getSubStringsSynsetList();
					T *addSubStringsSynset(E *subStringSynset);
					T *addSubStringsSynsets(const QList<E *> &subStringsSynsets);

					bool hasSubStringsSynset(E *subStringSynset);


					virtual QString getQueryResultString(const QString& typeStringName);
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					qint64 getSubStringsSynsetCount();


				// protected methods
				protected:

				// protected variables
				protected:
					E* mSuperString;

					QSet<E *> mSubStringSynsetSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CStringSubStringsRelationResult.cpp"

#endif // KONCLUDE_REASONER_QUERY_CSTRINGSUBSTRINGSRELATIONRESULT_H
