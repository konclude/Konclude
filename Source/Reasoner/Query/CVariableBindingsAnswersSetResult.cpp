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

#include "CVariableBindingsAnswersSetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingsAnswersSetResult::CVariableBindingsAnswersSetResult(const QStringList& varNames) {
				mVariableNames = varNames;
			}


			CVariableBindingsAnswersSetResult::~CVariableBindingsAnswersSetResult() {
				qDeleteAll(mBindingsContainer);
			}


			CVariableBindingsAnswersSetResult* CVariableBindingsAnswersSetResult::addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {
				mBindingsSet.insert(varBindings);
				mBindingsContainer.append(varBindings);
				return this;
			}


			bool CVariableBindingsAnswersSetResult::hasResultVariableBindings(CVariableBindingsAnswerResult* varBindings) {
				return mBindingsSet.contains(varBindings);
			}


			CVariableBindingsAnswersSetResult* CVariableBindingsAnswersSetResult::setResultVariableBindingsList(const QSet<CVariableBindingsAnswerResultHasher>& indiBindingSet) {
				mBindingsSet = indiBindingSet;
				return this;
			}



			QSet<CVariableBindingsAnswerResultHasher>* CVariableBindingsAnswersSetResult::getResult() {
				return &mBindingsSet;
			}



			QString CVariableBindingsAnswersSetResult::getQueryResultString() {
				return QString();
			}


			bool CVariableBindingsAnswersSetResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					CVariableBindingsAnswersSetResult *otherBoolQueryResult = dynamic_cast<CVariableBindingsAnswersSetResult *>(otherQueryResult);
					if (otherBoolQueryResult) {
						return isResultEquivalentTo(otherBoolQueryResult);
					} 
				}
				return false;
			}


			bool CVariableBindingsAnswersSetResult::isResultEquivalentTo(CVariableBindingsAnswersSetResult *otherQueryResult) {
				if (otherQueryResult) {
					if (mBindingsSet.count() != otherQueryResult->mBindingsSet.count()) {
						return false;
					}
					return mBindingsSet == otherQueryResult->mBindingsSet;
				} else {
					return false;
				}
			}


			const QStringList& CVariableBindingsAnswersSetResult::getVariableNames() {
				return mVariableNames;
			}



			CVariableBindingsAnswersResultIterator* CVariableBindingsAnswersSetResult::getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt) {
				class CVariableBindingsListAnswerResultIterator : public CVariableBindingsAnswersResultIterator {
				public:
					CVariableBindingsListAnswerResultIterator(const QSet<CVariableBindingsAnswerResultHasher>& set) {
						init(set);
					}

					CVariableBindingsListAnswerResultIterator* init(const QSet<CVariableBindingsAnswerResultHasher>& set) {
						mIt = set.constBegin();
						mItEnd = set.constEnd();
						return this;
					}


					virtual CVariableBindingsAnswerResult* getNext(bool moveNext = true) {
						QSet<CVariableBindingsAnswerResultHasher>::const_iterator it = mIt;
						if (it != mItEnd) {
							if (moveNext) {
								++mIt;
							}
							return it->getBindingResult();
						}
						return nullptr;
					}

					virtual bool hasNext() {
						return mIt != mItEnd;
					}

					QSet<CVariableBindingsAnswerResultHasher>::const_iterator mIt, mItEnd;

				};
				if (reuseIt) {
					CVariableBindingsListAnswerResultIterator* listReuseIt = (CVariableBindingsListAnswerResultIterator*)reuseIt;
					return listReuseIt->init(mBindingsSet);
				}
				return new CVariableBindingsListAnswerResultIterator(mBindingsSet);

			}


			cint64 CVariableBindingsAnswersSetResult::getResultCount() {
				return mBindingsSet.count();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
