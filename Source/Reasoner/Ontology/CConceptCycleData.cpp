/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CConceptCycleData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CConceptCycleData::CConceptCycleData(CContext* context) : mContext(context),mCycleConceptSet(context) {
			}

			cint64 CConceptCycleData::getCycleID() {
				return mCycleID;
			}

			CConceptCycleData* CConceptCycleData::setCycleID(cint64 cycleID) {
				mCycleID = cycleID;
				return this;
			}

			CConceptCycleData* CConceptCycleData::initConceptCycleData(CConceptCycleData* prevData) {
				if (prevData) {
					mCycleID = prevData->mCycleID;
					mCycleConceptSet = prevData->mCycleConceptSet;
					mRepCycleClass = prevData->mRepCycleClass;
					mNominalConnection = prevData->mNominalConnection;
				} else {
					mCycleID = 0;
					mCycleConceptSet.clear();
					mRepCycleClass = nullptr;
					mNominalConnection = false;
				}
				return this;
			}

			cint64 CConceptCycleData::getConceptCount() {
				return mCycleConceptSet.count();
			}

			CBOXSET<TConceptNegPair>* CConceptCycleData::getCycleConceptSet() {
				return &mCycleConceptSet;
			}

			CConceptCycleData* CConceptCycleData::addCycleConcept(CConcept* concept, bool negation) {
				addCycleConcept(TConceptNegPair(concept,negation));
				return this;
			}

			CConceptCycleData* CConceptCycleData::addCycleConcept(const TConceptNegPair& conNegPair) {
				mCycleConceptSet.insert(conNegPair);
				CConcept* concept = conNegPair.first;
				bool conNeg = conNegPair.second;
				if (!conNeg && concept->hasClassName()) {
					mCycleClassSet.insert(concept);
					if (!mRepCycleClass) {
						mRepCycleClass = concept;
					}
				}
				return this;
			}

			CConceptCycleData* CConceptCycleData::addCycleConcepts(const QSet<TConceptNegPair>& conNegPairSet) {
				for (QSet<TConceptNegPair>::const_iterator it = conNegPairSet.constBegin(), itEnd = conNegPairSet.constEnd(); it != itEnd; ++it) {
					addCycleConcept(*it);
				}
				return this;
			}


			CBOXSET<CConcept*>* CConceptCycleData::getCycleClassSet() {
				return &mCycleClassSet;
			}

			CConcept* CConceptCycleData::getRepresentativeCycleClass() {
				return mRepCycleClass;
			}

			bool CConceptCycleData::hasNominalConnection() {
				return mNominalConnection;
			}

			CConceptCycleData* CConceptCycleData::setNominalConnection(bool nominalConnection) {
				mNominalConnection = nominalConnection;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
