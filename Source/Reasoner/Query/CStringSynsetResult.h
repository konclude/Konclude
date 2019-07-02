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

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGSYNSETRESULT_H
#define KONCLUDE_REASONER_QUERY_CSTRINGSYNSETRESULT_H

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
			 *		\class		CStringSynsetResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CStringSynsetResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CStringSynsetResult();
					CStringSynsetResult(const QString &stringName);
					CStringSynsetResult(const QStringList &stringNameList);

					//! Destructor
					virtual ~CStringSynsetResult();

					QStringList getEquivalentStringNameList();
					T* addEquivalentStringName(const QString &stringName);
					T* addEquivalentStringNames(const QStringList &stringNameList);

					bool hasStringName(const QString &stringName);
					bool hasStringNames(const QStringList &stringNameList);

					qint64 getStringCount();

					virtual QString getQueryResultString(const QString& stringTypeName);
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);


				// protected methods
				protected:

				// protected variables
				protected:
					QStringList mStringNameList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude


#include "CStringSynsetResult.cpp"


#endif // KONCLUDE_REASONER_QUERY_CSTRINGSYNSETRESULT_H
