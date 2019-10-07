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

#include "CPropertySynsetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CPropertySynsetResult::CPropertySynsetResult(bool dataProperties) {
				mDataProperties = dataProperties;
			}

			CPropertySynsetResult::CPropertySynsetResult(const QString &propertyName, bool dataProperties) {
				addEquivalentPropertyName(propertyName);
				mDataProperties = dataProperties;
			}

			CPropertySynsetResult::CPropertySynsetResult(const QStringList &propertyNameList, bool dataProperties) {
				addEquivalentPropertyNames(propertyNameList);
				mDataProperties = dataProperties;
			}


			QStringList CPropertySynsetResult::getEquivalentPropertyNameList() {
				return getEquivalentStringNameList();
			}

			CPropertySynsetResult *CPropertySynsetResult::addEquivalentPropertyName(const QString &propertyName) {
				return addEquivalentStringName(propertyName);
			}

			CPropertySynsetResult *CPropertySynsetResult::addEquivalentPropertyNames(const QStringList &propertyNameList) {
				return addEquivalentStringNames(propertyNameList);
			}

			qint64 CPropertySynsetResult::getPropertyCount() {
				return getStringCount();
			}


			bool CPropertySynsetResult::hasPropertyName(const QString &propertyName) {
				return hasStringName(propertyName);
			}

			bool CPropertySynsetResult::hasPropertyNames(const QStringList &propertyNameList) {
				return hasStringNames(propertyNameList);
			}

			QString CPropertySynsetResult::getQueryResultString() {
				return CStringSynsetResult<CPropertySynsetResult>::getQueryResultString("Property");
			}

			bool CPropertySynsetResult::hasDataProperties() {
				return mDataProperties;
			}

			bool CPropertySynsetResult::hasObjectProperties() {
				return !mDataProperties;
			}

			CPropertySynsetResult* CPropertySynsetResult::setDataProperties(bool dataProperties) {
				mDataProperties = dataProperties;
				return this;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
