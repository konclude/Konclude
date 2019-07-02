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

#include "CJNIOntologyRevisionData.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIOntologyRevisionData::CJNIOntologyRevisionData(CJNIOntologyRevisionData* ontRevData) {
					mOntRev = nullptr;
					if (ontRevData) {
						mMappingHash = ontRevData->mMappingHash;
					}
					mQueryingPrepared = false;
				}

				CJNIOntologyRevisionData::~CJNIOntologyRevisionData() {
				}

				COntologyRevision* CJNIOntologyRevisionData::getOntologyRevision() {
					return mOntRev;
				}

				CJNIOntologyRevisionData* CJNIOntologyRevisionData::setOntologyRevision(COntologyRevision* ontRev) {
					mOntRev = ontRev;
					return this;
				}

				CJNIEntityMappingHash* CJNIOntologyRevisionData::getEntityMappingHash() {
					return &mMappingHash;
				}

				bool CJNIOntologyRevisionData::isQueryingPrepared() {
					return mQueryingPrepared;
				}

				CJNIOntologyRevisionData* CJNIOntologyRevisionData::setQueryingPrepared(bool prepared) {
					mQueryingPrepared = prepared;
					return this;
				}


			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
