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

#include "CVariableBindingsAnswersOrderedMapResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingsAnswersOrderedMapResult::CVariableBindingsAnswersOrderedMapResult(const QStringList& varNames, CVariableBindingsAnswerResultMapOrdering* mapOrdering) {
				mVariableNames = varNames;
				mMapOrdering = mapOrdering;
			}


			CVariableBindingsAnswersOrderedMapResult::~CVariableBindingsAnswersOrderedMapResult() {
				qDeleteAll(mBindingsContainer);
				delete mMapOrdering;
			}


			CVariableBindingsAnswersOrderedMapResult* CVariableBindingsAnswersOrderedMapResult::addResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {
				CVariableBindingsListAnswerResult* listVarBindings = dynamic_cast<CVariableBindingsListAnswerResult*>(varBindings);
				if (listVarBindings) {
					return addResultVariableBindings(listVarBindings);
				} else {
					mBindingsContainer.append(varBindings);
					return this;
				}
			}

			CVariableBindingsAnswersOrderedMapResult* CVariableBindingsAnswersOrderedMapResult::addResultVariableBindings(CVariableBindingsListAnswerResult* varBindings, cint64 cardinality) {
				for (cint64 i = 0; i < cardinality; ++i) {
					mOrderedBindingsMap.insertMulti(CVariableBindingsAnswerResultMapOrderer(varBindings, mMapOrdering), varBindings);
				}
				mBindingsContainer.append(varBindings);
				return this;
			}

			CVariableBindingsAnswersOrderedMapResult* CVariableBindingsAnswersOrderedMapResult::setResultVariableBindingsList(const QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>& indiBindingSet) {
				mOrderedBindingsMap = indiBindingSet;
				return this;
			}



			QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>* CVariableBindingsAnswersOrderedMapResult::getResult() {
				return &mOrderedBindingsMap;
			}



			QString CVariableBindingsAnswersOrderedMapResult::getQueryResultString() {
				return QString();
			}


			bool CVariableBindingsAnswersOrderedMapResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					CVariableBindingsAnswersOrderedMapResult *otherBoolQueryResult = dynamic_cast<CVariableBindingsAnswersOrderedMapResult *>(otherQueryResult);
					if (otherBoolQueryResult) {
						return isResultEquivalentTo(otherBoolQueryResult);
					} 
				}
				return false;
			}


			bool CVariableBindingsAnswersOrderedMapResult::isResultEquivalentTo(CVariableBindingsAnswersOrderedMapResult *otherQueryResult) {
				if (otherQueryResult) {
					if (mOrderedBindingsMap.count() != otherQueryResult->mOrderedBindingsMap.count()) {
						return false;
					}
					return mOrderedBindingsMap == otherQueryResult->mOrderedBindingsMap;
				} else {
					return false;
				}
			}


			const QStringList& CVariableBindingsAnswersOrderedMapResult::getVariableNames() {
				return mVariableNames;
			}



			CVariableBindingsAnswersResultIterator* CVariableBindingsAnswersOrderedMapResult::getVariableBindingsAnswersIterator(CVariableBindingsAnswersResultIterator* reuseIt) {
				class CVariableBindingsListAnswerResultIterator : public CVariableBindingsAnswersResultIterator {
				public:
					CVariableBindingsListAnswerResultIterator(const QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>& set) {
						init(set);
					}

					CVariableBindingsListAnswerResultIterator* init(const QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>& set) {
						mIt = set.constBegin();
						mItEnd = set.constEnd();
						return this;
					}


					virtual CVariableBindingsAnswerResult* getNext(bool moveNext = true) {
						QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>::const_iterator it = mIt;
						if (it != mItEnd) {
							if (moveNext) {
								++mIt;
							}
							return it.value();
						}
						return nullptr;
					}

					virtual bool hasNext() {
						return mIt != mItEnd;
					}

					QMap<CVariableBindingsAnswerResultMapOrderer, CVariableBindingsAnswerResult*>::const_iterator mIt, mItEnd;

				};
				if (reuseIt) {
					CVariableBindingsListAnswerResultIterator* listReuseIt = (CVariableBindingsListAnswerResultIterator*)reuseIt;
					return listReuseIt->init(mOrderedBindingsMap);
				}
				return new CVariableBindingsListAnswerResultIterator(mOrderedBindingsMap);

			}


			cint64 CVariableBindingsAnswersOrderedMapResult::getResultCount() {
				return mOrderedBindingsMap.count();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
