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

#include "CPropertySynsetsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CPropertySynsetsResult::CPropertySynsetsResult(bool dataProperties) {
				mDataProperties = dataProperties;
			}

			CPropertySynsetResult *CPropertySynsetsResult::getPropertySynset(const QString &propertyName, bool create) {
				CPropertySynsetResult* propertySynset = getStringSynset(propertyName, create);
				if (propertySynset) {
					return propertySynset->setDataProperties(mDataProperties);
				} else {
					return nullptr;
				}
			}

			CPropertySynsetResult *CPropertySynsetsResult::getPropertySynset(const QStringList &propertyNames, bool create) {
				CPropertySynsetResult* propertySynset = getStringSynset(propertyNames,create);
				if (propertySynset) {
					return propertySynset->setDataProperties(mDataProperties);
				} else {
					return nullptr;
				}
			}

			CPropertySynsetResult *CPropertySynsetsResult::addPropertySynset(CPropertySynsetResult *takePropertySynset) {
				CPropertySynsetResult* propertySynset = addStringSynset(takePropertySynset);
				return propertySynset->setDataProperties(mDataProperties);
			} 
 
			QList<CPropertySynsetResult*>* CPropertySynsetsResult::getPropertySynsetList() {
				return getStringSynsetList();
			}

			bool CPropertySynsetsResult::hasDataProperties() {
				return mDataProperties;
			}

			bool CPropertySynsetsResult::hasObjectProperties() {
				return !mDataProperties;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
