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

#ifndef KONCLUDE_REASONER_QUERY_CPROPERTYHIERARCHYRESULT_H
#define KONCLUDE_REASONER_QUERY_CPROPERTYHIERARCHYRESULT_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CQueryResult.h"
#include "CStringHierarchyResult.h"
#include "CPropertySubPropertiesRelationResult.h"
#include "CPropertySynsetResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CPropertyHierarchyResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPropertyHierarchyResult : public CStringHierarchyResult<CPropertyHierarchyResult,CPropertySubPropertiesRelationResult,CPropertySynsetResult> {
				// public methods
				public:
					//! Constructor
					CPropertyHierarchyResult(bool dataProperties = false);
					CPropertyHierarchyResult(const QString &bottomHierarchyProperty, const QString &topHierarchyProperty, bool dataProperties = false);
					CPropertyHierarchyResult(const QStringList &bottomHierarchyProperties, const QStringList &topHierarchyProperties, bool dataProperties = false);
					CPropertyHierarchyResult(CPropertySynsetResult *takeBottomPropertySynset, CPropertySynsetResult *takeTopPropertySynset, bool dataProperties = false);
					CPropertyHierarchyResult(CPropertySynsetResult *takeBottomPropertySynset, bool dataProperties = false);



					CPropertySynsetResult* getPropertySynset(const QString &propertyName, bool create = true);
					CPropertySynsetResult* getPropertySynset(const QStringList &propertyNames, bool create = true);
					CPropertySynsetResult* addPropertySynset(CPropertySynsetResult *takePropertySynset);
									   
					CPropertySynsetResult* setTopPropertySynset(CPropertySynsetResult *propertySynset);
									   
									   
					CPropertySynsetResult* getBottomPropertySynset();
					CPropertySynsetResult* getTopPropertySynset();

					CPropertySubPropertiesRelationResult* addSubPropertyRelation(CPropertySynsetResult *superPropertySynset, CPropertySynsetResult *anySubPropertySynset);
					CPropertySubPropertiesRelationResult* getSubPropertyRelation(CPropertySynsetResult *superPropertySynset, bool create = true);


					bool hasDataProperties();
					bool hasObjectProperties();

					CPropertyHierarchyResult* setDataProperties(bool dataProperties);


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

#endif // KONCLUDE_REASONER_QUERY_CPROPERTYHIERARCHYRESULT_H
