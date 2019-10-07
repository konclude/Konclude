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

#include "CCallbackData.h"



namespace Konclude {

	namespace Concurrent {

		namespace Callback {


			CCallbackData::CCallbackData(CCallbackDataContext* callbackContext) : CLinkerBase<CCallbackData*,CCallbackData>(this,nullptr) {
				mContext = callbackContext;
			}


			CCallbackData::~CCallbackData() {
				if (mContext) {
					delete mContext;
				}
			}

			void CCallbackData::setCallbackDataContext(CCallbackDataContext* callbackContext) {
				if (mContext && mContext != callbackContext) {
					delete mContext;
				}
				mContext = callbackContext;
			}

			CCallbackDataContext* CCallbackData::getCallbackDataContext() {
				return mContext;
			}

			CCallbackDataContext* CCallbackData::takeCallbackDataContext() {
				CCallbackDataContext* context = mContext;
				mContext = nullptr;
				return context;
			}

		}; // end namespace Callback

	}; // end namespace Concurrent

}; // end namespace Konclude