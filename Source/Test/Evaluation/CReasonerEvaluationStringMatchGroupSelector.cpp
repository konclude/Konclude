/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CReasonerEvaluationStringMatchGroupSelector.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationStringMatchGroupSelector::CReasonerEvaluationStringMatchGroupSelector(const QSet<QString>& groupStringSet) {
				mGroupStringSet = groupStringSet;
			}


			CReasonerEvaluationStringMatchGroupSelector::CReasonerEvaluationStringMatchGroupSelector() {
			}


			bool CReasonerEvaluationStringMatchGroupSelector::isGroupSelected(const QString& groupString) {
				return mGroupStringSet.contains(groupString);
			}

			bool CReasonerEvaluationStringMatchGroupSelector::isRequestSelected(const QString& groupString, const QString& requestString) {
				return true;
			}


			CReasonerEvaluationStringMatchGroupSelector* CReasonerEvaluationStringMatchGroupSelector::addGroupString(const QString& groupString) {
				mGroupStringSet.insert(groupString);
				return this;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
