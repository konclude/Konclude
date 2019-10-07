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

#include "CClassSynsetsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSynsetsResult::CClassSynsetsResult() {
			}

			CClassSynsetResult *CClassSynsetsResult::getClassSynset(const QString &className, bool create) {
				return getStringSynset(className,create);
			}

			CClassSynsetResult *CClassSynsetsResult::getClassSynset(const QStringList &classNames, bool create) {
				return getStringSynset(classNames,create);
			}

			CClassSynsetResult *CClassSynsetsResult::addClassSynset(CClassSynsetResult *takeClassSynset) {
				return addStringSynset(takeClassSynset);
			} 
 
			QList<CClassSynsetResult*>* CClassSynsetsResult::getClassSynsetList() {
				return getStringSynsetList();
			}

 

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
