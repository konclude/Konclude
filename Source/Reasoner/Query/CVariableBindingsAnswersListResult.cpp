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

#include "CVariableBindingsAnswersListResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingsAnswersListResult::CVariableBindingsAnswersListResult(const QStringList& varNames) {
				mVariableNames = varNames;
			}


			CVariableBindingsAnswersListResult::~CVariableBindingsAnswersListResult() {
				for (auto bindings : mBindingsList) {
					delete bindings;
				}
			}


			CVariableBindingsAnswersListResult* CVariableBindingsAnswersListResult::addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {
				mBindingsContainer.append(varBindings);
				for (cint64 i = 0; i < cardinality; ++i) {
					mBindingsList.append(varBindings);
				}
				return this;
			}


			CVariableBindingsAnswersListResult* CVariableBindingsAnswersListResult::setResultVariableBindingsList(const QList<CVariableBindingsAnswerResult*>& indiBindingList) {
				mBindingsList = indiBindingList;
				mBindingsContainer = mBindingsList.toSet().toList();
				return this;
			}



			QList<CVariableBindingsAnswerResult*>* CVariableBindingsAnswersListResult::getResult() {
				return &mBindingsList;
			}



			QString CVariableBindingsAnswersListResult::getQueryResultString() {
				return QString();
			}


			bool CVariableBindingsAnswersListResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					CVariableBindingsAnswersListResult *otherBoolQueryResult = dynamic_cast<CVariableBindingsAnswersListResult *>(otherQueryResult);
					if (otherBoolQueryResult) {
						return isResultEquivalentTo(otherBoolQueryResult);
					} 
				}
				return false;
			}


			bool CVariableBindingsAnswersListResult::isResultEquivalentTo(CVariableBindingsAnswersListResult *otherQueryResult) {
				if (otherQueryResult) {
					if (mBindingsList.count() != otherQueryResult->mBindingsList.count()) {
						return false;
					}
					for (QList<CVariableBindingsAnswerResult*>::const_iterator it1 = mBindingsList.constBegin(), it2 = otherQueryResult->mBindingsList.constBegin(), it1End = mBindingsList.constEnd(), it2End = otherQueryResult->mBindingsList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
						CVariableBindingsAnswerResult* bind1(*it1);
						CVariableBindingsAnswerResult* bind2(*it2);
						if (bind1 == nullptr && bind2 != nullptr) {
							return false;
						}
						if (bind1 && !bind1->isResultEquivalentTo(bind2)) {
							return false;
						}
					}
					return true;
				} else {
					return false;
				}
			}


			const QStringList& CVariableBindingsAnswersListResult::getVariableNames() {
				return mVariableNames;
			}



			CVariableBindingsAnswersResultIterator* CVariableBindingsAnswersListResult::getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt) {
				class CVariableBindingsListAnswerResultIterator : public CVariableBindingsAnswersResultIterator {
				public:
					CVariableBindingsListAnswerResultIterator(const QList<CVariableBindingsAnswerResult*>& list) {
						init(list);
					}

					CVariableBindingsListAnswerResultIterator* init(const QList<CVariableBindingsAnswerResult*>& list) {
						mIt = list.constBegin();
						mItEnd = list.constEnd();
						return this;
					}


					virtual CVariableBindingsAnswerResult* getNext(bool moveNext = true) {
						QList<CVariableBindingsAnswerResult*>::const_iterator it = mIt;
						if (it != mItEnd) {
							if (moveNext) {
								++mIt;
							}
							return *it;
						}
						return nullptr;
					}

					virtual bool hasNext() {
						return mIt != mItEnd;
					}

					QList<CVariableBindingsAnswerResult*>::const_iterator mIt, mItEnd;

				};
				if (reuseIt) {
					CVariableBindingsListAnswerResultIterator* listReuseIt = (CVariableBindingsListAnswerResultIterator*)reuseIt;
					return listReuseIt->init(mBindingsList);
				}
				return new CVariableBindingsListAnswerResultIterator(mBindingsList);

			}


			cint64 CVariableBindingsAnswersListResult::getResultCount() {
				return mBindingsList.count();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
