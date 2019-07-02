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

#include "CMarkerIndividualNodeData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CMarkerIndividualNodeData::CMarkerIndividualNodeData() {
					mLinker = nullptr;
					mLocIndiSet = nullptr;
					mUseIndiSet = nullptr;
				}

				CMarkerIndividualNodeData::CMarkerIndividualNodeData(const CMarkerIndividualNodeData& data) {
					mLinker = data.mLinker;
					mLocIndiSet = nullptr;
					mUseIndiSet = data.mUseIndiSet;
				}


				CMarkerIndividualNodeData* CMarkerIndividualNodeData::initMarkerIndividualNodeData(CMarkerIndividualNodeData* data) {
					mLinker = nullptr;
					mLocIndiSet = nullptr;
					mUseIndiSet = nullptr;
					if (data) {
						mLinker = data->mLinker;
						mLocIndiSet = nullptr;
						mUseIndiSet = data->mUseIndiSet;
					}
					return this;
				}

				CXNegLinker<CIndividualProcessNode*>* CMarkerIndividualNodeData::getMarkerIndividualNodeLinker() const {
					return mLinker;
				}

				CMarkerIndividualNodeData* CMarkerIndividualNodeData::addMarkerIndividualNodeLinker(CXNegLinker<CIndividualProcessNode*>* indiNodeLinker) {
					mLinker = indiNodeLinker->append(mLinker);
					return this;
				}


				CPROCESSSET< QPair<cint64, bool> >* CMarkerIndividualNodeData::getIndividualNondeterminismSet(bool localized) {
					if (localized) {
						return mLocIndiSet;
					} else {
						return mUseIndiSet;
					}
				}

				CMarkerIndividualNodeData* CMarkerIndividualNodeData::setIndividualNondeterminismSet(bool localized, CPROCESSSET< QPair<cint64, bool> >* set) {
					if (localized) {
						mLocIndiSet = set;
					} else {
						mUseIndiSet = set;
					}
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
