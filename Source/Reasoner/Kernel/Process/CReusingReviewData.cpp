/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CReusingReviewData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CReusingReviewData::CReusingReviewData(CProcessContext* context) : mIndividualSet(context),mDepthIndividualMap(context) {
					mContext = context;
				}


				CReusingReviewData* CReusingReviewData::initReviewData(CReusingReviewData* sigBlockRevData) {
					if (sigBlockRevData) {
						mIndividualSet = sigBlockRevData->mIndividualSet;
						mDepthIndividualMap = sigBlockRevData->mDepthIndividualMap;
					} else {
						mIndividualSet.clear();
						mDepthIndividualMap.clear();
					}
					return this;
				}

				CReusingReviewData* CReusingReviewData::insert(cint64 depth, cint64 indiID) {
					if (!mIndividualSet.contains(indiID)) {
						mIndividualSet.insert(indiID);
						mDepthIndividualMap.insertMulti(depth,indiID);
					}
					return this;
				}


				bool CReusingReviewData::contains(cint64 indiID) {
					return mIndividualSet.contains(indiID);
				}

				CReusingReviewData* CReusingReviewData::remove(cint64 indiID) {
					mIndividualSet.remove(indiID);
					return this;
				}

				bool CReusingReviewData::isEmpty() {
					return mIndividualSet.isEmpty();
				}

				bool CReusingReviewData::hasNextIndividualID() {
					return mIndividualSet.isEmpty();
				}

				cint64 CReusingReviewData::takeNextIndividualID() {
					cint64 indiID = 0;
					CPROCESSMAP<cint64,cint64>::iterator depthIndividualMapIt = mDepthIndividualMap.begin(), depthIndividualMapItEnd = mDepthIndividualMap.end();
					while (depthIndividualMapIt != depthIndividualMapItEnd) {
						indiID = depthIndividualMapIt.value();
						depthIndividualMapIt = mDepthIndividualMap.erase(depthIndividualMapIt);
						if (mIndividualSet.contains(indiID)) {
							mIndividualSet.remove(indiID);
							break;
						}
					}
					return indiID;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
