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

#include "CJNIInstanceManagerConfigType.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIInstanceManagerConfigType::CJNIInstanceManagerConfigType(CJNIInstanceManager *jniInstanceMananger) {
					mJNIInstanceManager = jniInstanceMananger;
				}


				CJNIInstanceManagerConfigType::~CJNIInstanceManagerConfigType() {
				}


				CJNIInstanceManagerConfigType *CJNIInstanceManagerConfigType::getTypeCopy() {
					return new CJNIInstanceManagerConfigType(mJNIInstanceManager);
				}


				CJNIInstanceManager *CJNIInstanceManagerConfigType::getJNIInstanceManager() {
					return mJNIInstanceManager;
				}

				CJNIInstanceManagerConfigType *CJNIInstanceManagerConfigType::setJNIInstanceManager(CJNIInstanceManager *jniInstanceMananger) {
					mJNIInstanceManager = jniInstanceMananger;
					return this;
				}


				bool CJNIInstanceManagerConfigType::equals(CConfigType *dataType) {
					CJNIInstanceManagerConfigType *rCommanderType = dynamic_cast<CJNIInstanceManagerConfigType *>(dataType);
					if (rCommanderType) {
						return (mJNIInstanceManager == rCommanderType->mJNIInstanceManager);
					}
					return false;
				}

			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
