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

#include "CPropertyHierarchyResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CPropertyHierarchyResult::CPropertyHierarchyResult(bool dataProperties) {
				mDataProperties = dataProperties;
			}

			CPropertyHierarchyResult::CPropertyHierarchyResult(const QString &bottomHierarchyProperty, const QString &topHierarchyProperty, bool dataProperties) : CStringHierarchyResult<CPropertyHierarchyResult,CPropertySubPropertiesRelationResult,CPropertySynsetResult>(bottomHierarchyProperty,topHierarchyProperty) {
				mDataProperties = dataProperties;
			}

			CPropertyHierarchyResult::CPropertyHierarchyResult(const QStringList &bottomHierarchyProperties, const QStringList &topHierarchyProperties, bool dataProperties) : CStringHierarchyResult<CPropertyHierarchyResult,CPropertySubPropertiesRelationResult,CPropertySynsetResult>(bottomHierarchyProperties,topHierarchyProperties) {
				mDataProperties = dataProperties;
			}

			CPropertyHierarchyResult::CPropertyHierarchyResult(CPropertySynsetResult *takeBottomPropertySynset, CPropertySynsetResult *takeTopPropertySynset, bool dataProperties) : CStringHierarchyResult<CPropertyHierarchyResult,CPropertySubPropertiesRelationResult,CPropertySynsetResult>(takeBottomPropertySynset,takeTopPropertySynset) {
				mDataProperties = dataProperties;
			}

			CPropertyHierarchyResult::CPropertyHierarchyResult(CPropertySynsetResult *takeBottomPropertySynset, bool dataProperties) : CStringHierarchyResult<CPropertyHierarchyResult,CPropertySubPropertiesRelationResult,CPropertySynsetResult>(takeBottomPropertySynset) {
				mDataProperties = dataProperties;
			}


			CPropertySynsetResult *CPropertyHierarchyResult::getBottomPropertySynset() {
				return getBottomStringSynset();
			}

			CPropertySynsetResult *CPropertyHierarchyResult::getTopPropertySynset() {
				return getTopStringSynset();
			}

			CPropertySynsetResult *CPropertyHierarchyResult::getPropertySynset(const QString &propertyName, bool create) {
				CPropertySynsetResult* propertySynset = getStringSynset(propertyName, create);
				if (propertySynset) {
					propertySynset->setDataProperties(mDataProperties);
				}
				return propertySynset;
			}

			CPropertySynsetResult *CPropertyHierarchyResult::getPropertySynset(const QStringList &propertyNames, bool create) {
				CPropertySynsetResult* propertySynset = getStringSynset(propertyNames,create);
				if (propertySynset) {
					propertySynset->setDataProperties(mDataProperties);
				}
				return propertySynset;
			}
			
			
			CPropertySynsetResult *CPropertyHierarchyResult::setTopPropertySynset(CPropertySynsetResult *propertySynset) {
				propertySynset->setDataProperties(mDataProperties);
				return setTopStringSynset(propertySynset);
			}


			CPropertySynsetResult *CPropertyHierarchyResult::addPropertySynset(CPropertySynsetResult *takePropertySynset) {
				takePropertySynset->setDataProperties(mDataProperties);
				return addStringSynset(takePropertySynset);
			} 
 

			CPropertySubPropertiesRelationResult *CPropertyHierarchyResult::addSubPropertyRelation(CPropertySynsetResult *superPropertySynset, CPropertySynsetResult *anySubPropertySynset) {
				return addSubStringRelation(superPropertySynset,anySubPropertySynset);
			}

			CPropertySubPropertiesRelationResult *CPropertyHierarchyResult::getSubPropertyRelation(CPropertySynsetResult *superPropertySynset, bool create) {
				return getSubStringRelation(superPropertySynset,create);
			}

			bool CPropertyHierarchyResult::hasDataProperties() {
				return mDataProperties;
			}

			bool CPropertyHierarchyResult::hasObjectProperties() {
				return !mDataProperties;
			}

			CPropertyHierarchyResult* CPropertyHierarchyResult::setDataProperties(bool dataProperties) {
				mDataProperties = dataProperties;
				return this;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
