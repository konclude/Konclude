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

#include "CRedlandRasqalSubQueryBindingPositionResolvingHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CRedlandRasqalSubQueryBindingPositionResolvingHash::CRedlandRasqalSubQueryBindingPositionResolvingHash(CVariableBindingsAnswersResult* varIndBindsResult) {
				mVarIndBindsResult = varIndBindsResult;
			}


			CRedlandRasqalSubQueryBindingPositionResolvingHash::~CRedlandRasqalSubQueryBindingPositionResolvingHash() {
				for (QHash<cint64, CRedlandRasqalSubQueryBindingPositionResolvingHashData*>::const_iterator it = QHash<cint64, CRedlandRasqalSubQueryBindingPositionResolvingHashData*>::constBegin(), itEnd = QHash<cint64, CRedlandRasqalSubQueryBindingPositionResolvingHashData*>::constEnd(); it != itEnd; ++it) {
					CRedlandRasqalSubQueryBindingPositionResolvingHashData* resolvingData = it.value();
					if (resolvingData) {
						delete resolvingData;
					}
				}
			}


			CRedlandRasqalSubQueryBindingPositionResolvingHashData* CRedlandRasqalSubQueryBindingPositionResolvingHash::getBindingPositionResolvingData(cint64 pos) {
				CRedlandRasqalSubQueryBindingPositionResolvingHashData*& resolveData = QHash<cint64, CRedlandRasqalSubQueryBindingPositionResolvingHashData*>::operator[](pos);
				if (!resolveData) {
					resolveData = new CRedlandRasqalSubQueryBindingPositionResolvingHashData(pos, mVarIndBindsResult);
				}
				return resolveData;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
