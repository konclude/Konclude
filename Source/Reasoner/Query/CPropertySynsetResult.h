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

#ifndef KONCLUDE_REASONER_QUERY_CPROPERTYSYNSETRESULT_H
#define KONCLUDE_REASONER_QUERY_CPROPERTYSYNSETRESULT_H

// Libraries includes
#include <QStringList>

// Namespace includes
#include "CQueryResult.h"
#include "CStringSynsetResult.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CPropertySynsetResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPropertySynsetResult : public CStringSynsetResult<CPropertySynsetResult> {
				// public methods
				public:
					//! Constructor
					CPropertySynsetResult(bool dataProperties = false);
					CPropertySynsetResult(const QString &propertyName, bool dataProperties = false);
					CPropertySynsetResult(const QStringList &propertyNameList, bool dataProperties = false);


					QStringList getEquivalentPropertyNameList();
					CPropertySynsetResult *addEquivalentPropertyName(const QString &propertyName);
					CPropertySynsetResult *addEquivalentPropertyNames(const QStringList &propertyNameList);

					bool hasPropertyName(const QString &propertyName);
					bool hasPropertyNames(const QStringList &propertyNameList);

					qint64 getPropertyCount();

					virtual QString getQueryResultString();

					bool hasDataProperties();
					bool hasObjectProperties();

					CPropertySynsetResult* setDataProperties(bool dataProperties);


				// protected methods
				protected:
					bool mDataProperties;

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

#endif // KONCLUDE_REASONER_QUERY_CPROPERTYSYNSETRESULT_H
