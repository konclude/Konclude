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

#include "CRepresentativeVariableBindingPathSetDataSignature.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathSetDataSignature::CRepresentativeVariableBindingPathSetDataSignature() {
				}

				CRepresentativeVariableBindingPathSetDataSignature* CRepresentativeVariableBindingPathSetDataSignature::initSignature(CRepresentativeVariableBindingPathSetDataSignature* prevSignature) {
					if (prevSignature) {
						mIncomingRepPropKey1 = prevSignature->mIncomingRepPropKey1;
						mIncomingRepPropKey2 = prevSignature->mIncomingRepPropKey2;
						mIncomingRepPropKey = prevSignature->mIncomingRepPropKey;
					} else {
						mIncomingRepPropKey1 = 13;
						mIncomingRepPropKey2 = 13;
						mIncomingRepPropKey = 0;
					}
					return this;
				}


				CRepresentativeVariableBindingPathSetDataSignature* CRepresentativeVariableBindingPathSetDataSignature::addKey(cint64 key) {
					mIncomingRepPropKey1 += key;
					mIncomingRepPropKey2 *= key;
					mIncomingRepPropKey = mIncomingRepPropKey1 + (mIncomingRepPropKey2*17);
					return this;
				}

				cint64 CRepresentativeVariableBindingPathSetDataSignature::getSignatureValue() {
					return mIncomingRepPropKey;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
