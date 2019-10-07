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

#include "CIndividualSynsetsResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualSynsetsResult::CIndividualSynsetsResult() {
			}



			CIndividualSynsetsResult::~CIndividualSynsetsResult() {
				qDeleteAll(mIndividualSynsetContainer);
			}


			CIndividualSynsetResult *CIndividualSynsetsResult::getIndividualSynset(const QString &individualName, bool create) {
				CIndividualSynsetResult *individualSynset = mIndividualIndividualSynsetHash.value(individualName,0);
				if (!individualSynset && create) {
					individualSynset = new CIndividualSynsetResult(individualName);
					mIndividualIndividualSynsetHash.insert(individualName,individualSynset);
					mIndividualSynsetContainer.append(individualSynset);
				}
				return individualSynset;
			}

			CIndividualSynsetResult *CIndividualSynsetsResult::getIndividualSynset(const QStringList &individualNames, bool create) {
				CIndividualSynsetResult *individualSynset = 0;
				if (!individualNames.isEmpty()) {
					individualSynset = mIndividualIndividualSynsetHash.value(individualNames.first(),0);
					if (!individualSynset && create) {
						individualSynset = new CIndividualSynsetResult(individualNames);
						foreach (QString individualName, individualNames) {
							mIndividualIndividualSynsetHash.insert(individualName,individualSynset);
						}
						mIndividualSynsetContainer.append(individualSynset);
					}
				}
				return individualSynset;
			}

			CIndividualSynsetResult *CIndividualSynsetsResult::addIndividualSynset(CIndividualSynsetResult *takeIndividualSynset) {
				foreach (QString individualName, takeIndividualSynset->getEquivalentIndividualNameList()) {
					mIndividualIndividualSynsetHash.insert(individualName,takeIndividualSynset);
				}
				mIndividualSynsetContainer.append(takeIndividualSynset);
				return takeIndividualSynset;
			} 
 
			QList<CIndividualSynsetResult*>* CIndividualSynsetsResult::getIndividualSynsetList() {
				return &mIndividualSynsetContainer;
			}


			QString CIndividualSynsetsResult::getQueryResultString() {
				QString string;
				return string;
			}
 

			bool CIndividualSynsetsResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				CIndividualSynsetsResult *otherIndividualSynsets = dynamic_cast<CIndividualSynsetsResult *>(otherQueryResult);
				if (!otherIndividualSynsets) {
					return false;
				}

				if (mIndividualIndividualSynsetHash.count() != otherIndividualSynsets->mIndividualIndividualSynsetHash.count()) {
					return false;
				}

				for (QList<CIndividualSynsetResult*>::const_iterator it = mIndividualSynsetContainer.constBegin(), itEnd = mIndividualSynsetContainer.constEnd(); it != itEnd; ++it) {
					CIndividualSynsetResult* individualSynRes(*it);
					CIndividualSynsetResult* otherIndividualSynRes = nullptr;
					QStringList individualNameList(individualSynRes->getEquivalentIndividualNameList());
					if (!individualNameList.isEmpty()) {
						const QString& individualName = individualNameList.first();
						otherIndividualSynRes = otherIndividualSynsets->mIndividualIndividualSynsetHash.value(individualName);
					}
					if (!otherIndividualSynRes && !individualNameList.isEmpty()) {
						return false;
					}
					if (!individualSynRes->isResultEquivalentTo(otherIndividualSynRes)) {
						return false;
					}
				}
				return true;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
