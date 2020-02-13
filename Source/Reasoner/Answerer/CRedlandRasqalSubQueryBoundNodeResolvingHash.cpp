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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CRedlandRasqalSubQueryBoundNodeResolvingHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CRedlandRasqalSubQueryBoundNodeResolvingHash::CRedlandRasqalSubQueryBoundNodeResolvingHash(CVariableBindingsAnswersResult* varIndBindsResult) {
				mVarIndBindsResult = varIndBindsResult;
				mResolved = false;
			}


			CRedlandRasqalSubQueryBoundNodeResolvingHash::~CRedlandRasqalSubQueryBoundNodeResolvingHash() {
				for (QHash<CRedlandRasqalLiteralHasher, CRedlandRasqalSubQueryBoundNodeResolvingHashData*>::const_iterator it = QHash<CRedlandRasqalLiteralHasher, CRedlandRasqalSubQueryBoundNodeResolvingHashData*>::constBegin(), itEnd = QHash<CRedlandRasqalLiteralHasher, CRedlandRasqalSubQueryBoundNodeResolvingHashData*>::constEnd(); it != itEnd; ++it) {
					CRedlandRasqalSubQueryBoundNodeResolvingHashData* resolvingData = it.value();
					if (resolvingData) {
						delete resolvingData;
					}
				}
			}

			CVariableBindingsAnswersResult* CRedlandRasqalSubQueryBoundNodeResolvingHash::getResolvingVariableBindingsResult() {
				return mVarIndBindsResult;
			}

			
			CRedlandRasqalSubQueryBoundNodeResolvingHashData* CRedlandRasqalSubQueryBoundNodeResolvingHash::getBoundNodeResolvingData(rasqal_literal* node) {
				CRedlandRasqalSubQueryBoundNodeResolvingHashData*& data = QHash<CRedlandRasqalLiteralHasher, CRedlandRasqalSubQueryBoundNodeResolvingHashData*>::operator[](CRedlandRasqalLiteralHasher(node));
				if (!data) {
					data = new CRedlandRasqalSubQueryBoundNodeResolvingHashData(new CVariableBindingsAnswersListResult(mVarIndBindsResult->getVariableNames()));
				}
				return data;
			}

			bool CRedlandRasqalSubQueryBoundNodeResolvingHash::isResolved() {
				return mResolved;
			}

			CRedlandRasqalSubQueryBoundNodeResolvingHash* CRedlandRasqalSubQueryBoundNodeResolvingHash::setResolved(bool resolved) {
				mResolved = resolved;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
