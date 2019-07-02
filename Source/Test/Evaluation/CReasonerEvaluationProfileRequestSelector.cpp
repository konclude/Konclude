/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CReasonerEvaluationProfileRequestSelector.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {


			CReasonerEvaluationProfileRequestSelector::CReasonerEvaluationProfileRequestSelector(cint64 profile, const QString& baseExpressivityDir) {
				mProfile = profile;
				mBaseExpressivityDir = baseExpressivityDir;
			}



			bool CReasonerEvaluationProfileRequestSelector::isGroupSelected(const QString& groupString) {
				return true;
			}


			QString CReasonerEvaluationProfileRequestSelector::getProfileKeyString(cint64 profile) {
				if (profile == REPRS_PROFILE_DL) {
					return "DLProfile:";
				} else if (profile == REPRS_PROFILE_EL) {
					return "ELProfile:";
				} else if (profile == REPRS_PROFILE_QL) {
					return "QLProfile:";
				} else if (profile == REPRS_PROFILE_RL) {
					return "RLProfile:";
				}
				return "";
			}

			bool CReasonerEvaluationProfileRequestSelector::isRequestSelected(const QString& groupString, const QString& requestString) {
				QString profileString;
				bool foundProfile = false;
				QString infoFileString = mBaseExpressivityDir+groupString+requestString+".txt";
				QFile infoFile(infoFileString);
				if (infoFile.open(QIODevice::ReadOnly)) {
					while (!foundProfile && !infoFile.atEnd()) {
						QString line = infoFile.readLine();
						QStringList keyValueLists = line.split("\t");
						if (keyValueLists.count() >= 2) {
							QString profileKeyString = getProfileKeyString(mProfile);
							if (keyValueLists.first().trimmed() == profileKeyString) {
								foundProfile = true;
								profileString = keyValueLists.last().trimmed();
							}
						}
					}
					infoFile.close();
				}
				if (!foundProfile) {
					return false;
				} else {
					if (profileString == "1") {
						return true;
					} else if (profileString == "0") {
						return false;
					}
				}
				return true;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
