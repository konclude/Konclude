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

#include "CVariableBindingsListAnswerResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingsListAnswerResult::CVariableBindingsListAnswerResult() {
			}

			CVariableBindingsListAnswerResult::CVariableBindingsListAnswerResult(cint64 count) {
				for (cint64 i = 0; i < count; ++i) {
					mBindingsList.push_back(nullptr);
				}
			}


			CVariableBindingsListAnswerResult::~CVariableBindingsListAnswerResult() {
				for (auto binding : mBindingsList) {
					delete binding;
				}
			}


			CVariableBindingsListAnswerResult::CVariableBindingsListAnswerResult(const CVariableBindingsListAnswerResult& varListResult) {
				for (auto binding : varListResult.mBindingsList) {
					mBindingsList.append(binding->createCopy());
				}
			}

			CVariableBindingsListAnswerResult* CVariableBindingsListAnswerResult::clearResultVariableBindings() {
				mBindingsList.clear();
				return this;
			}



			CVariableBindingsListAnswerResult* CVariableBindingsListAnswerResult::addResultVariableBinding(CVariableBindingResult* varBinding) {
				mBindingsList.append(varBinding);
				return this;
			}

			CVariableBindingsListAnswerResult* CVariableBindingsListAnswerResult::setResultVariableBindings(const QList<CVariableBindingResult*>& bindingsList) {
				mBindingsList = bindingsList;
				return this;
			}

			CVariableBindingResult* CVariableBindingsListAnswerResult::getResult(cint64 index) {
				return mBindingsList.at(index);
			}

			cint64 CVariableBindingsListAnswerResult::getResultCount() {
				return mBindingsList.count();
			}


			CVariableBindingsListAnswerResult* CVariableBindingsListAnswerResult::setResult(cint64 index, CVariableBindingResult* varBinding) {
				mBindingsList[index] = varBinding;
				return this;
			}


			QList<CVariableBindingResult*>* CVariableBindingsListAnswerResult::getResult() {
				return &mBindingsList;
			}



			QString CVariableBindingsListAnswerResult::getQueryResultString() {
				QStringList bindingsStringList;
				for (auto binding : mBindingsList) {
					bindingsStringList.append(binding->getBindingString());
				}
				return bindingsStringList.join(", ");
			}


			bool CVariableBindingsListAnswerResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					CVariableBindingsListAnswerResult *otherBoolQueryResult = dynamic_cast<CVariableBindingsListAnswerResult *>(otherQueryResult);
					if (otherBoolQueryResult) {
						return isResultEquivalentTo(otherBoolQueryResult);
					} 
				}
				return false;
			}


			bool CVariableBindingsListAnswerResult::isResultEquivalentTo(CVariableBindingsListAnswerResult *otherQueryResult) {
				if (otherQueryResult) {
					if (mBindingsList.count() != otherQueryResult->mBindingsList.count()) {
						return false;
					}
					for (QList<CVariableBindingResult*>::const_iterator it1 = mBindingsList.constBegin(), it2 = otherQueryResult->mBindingsList.constBegin(), it1End = mBindingsList.constEnd(), it2ENd = otherQueryResult->mBindingsList.constEnd(); it1 != it1End && it2 != it2ENd; ++it1, ++it2) {
						CVariableBindingResult* bind1(*it1);
						CVariableBindingResult* bind2(*it2);
						if ((bind1 == nullptr || bind2 == nullptr) && bind2 != bind1) {
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


			CVariableBindingsResultIterator* CVariableBindingsListAnswerResult::getVariableBindingsIterator(CVariableBindingsResultIterator* reuseIt) {
				class CVariableBindingsListAnswerResultIterator : public CVariableBindingsResultIterator {
				public: 
					CVariableBindingsListAnswerResultIterator(const QList<CVariableBindingResult*>& list) {
						init(list);
					}

					CVariableBindingsListAnswerResultIterator* init(const QList<CVariableBindingResult*>& list) {
						mIt = list.constBegin();
						mItEnd = list.constEnd();
						return this;
					}


					virtual CVariableBindingResult* getNext(bool moveNext = true) {
						QList<CVariableBindingResult*>::const_iterator it = mIt;
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

					QList<CVariableBindingResult*>::const_iterator mIt, mItEnd;

				};
				if (reuseIt) {
					CVariableBindingsListAnswerResultIterator* listReuseIt = (CVariableBindingsListAnswerResultIterator*)reuseIt;
					return listReuseIt->init(mBindingsList);
				}
				return new CVariableBindingsListAnswerResultIterator(mBindingsList);
			}


			CVariableBindingsAnswerResult* CVariableBindingsListAnswerResult::getCopy() {
				return new CVariableBindingsListAnswerResult(*this);
			}


			cint64 CVariableBindingsListAnswerResult::getHashCode() {
				cint64 hashCode = mBindingsList.count();
				for (CVariableBindingResult* binding : mBindingsList) {
					cint64 bindingType = 0;
					if (binding) {
						bindingType = (cint64)binding->getVariableBindingType();
						bindingType += qHash(binding->getQueryResultString());
					}
					hashCode = bindingType * 13 + hashCode;
				}
				return hashCode;
			}



		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
