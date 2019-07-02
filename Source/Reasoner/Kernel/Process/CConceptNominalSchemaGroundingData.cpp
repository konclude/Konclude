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

#include "CConceptNominalSchemaGroundingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptNominalSchemaGroundingData::CConceptNominalSchemaGroundingData(CContext* context) : mContext(context),mBindedNomSchConList(context) {
					mGroundedConcept = nullptr;
					mGroundingConcept = nullptr;
				}

				CConceptNominalSchemaGroundingData* CConceptNominalSchemaGroundingData::setGroundingConcept(CConcept* concept) {
					mGroundingConcept = concept;
					return this;
				}

				CConceptNominalSchemaGroundingData* CConceptNominalSchemaGroundingData::setGroundedConcept(CConcept* concept) {
					mGroundedConcept = concept;
					return this;
				}

				CConceptNominalSchemaGroundingData* CConceptNominalSchemaGroundingData::addBindedNominalSchemaConcept(CConcept* nsConcept) {
					mBindedNomSchConList.append(nsConcept);
					return this;
				}

				CPROCESSLIST<CConcept*>* CConceptNominalSchemaGroundingData::getBindedNominalSchemaConceptList() {
					return &mBindedNomSchConList;
				}

				CConcept* CConceptNominalSchemaGroundingData::getGroundedConcept() {
					return mGroundedConcept;
				}

				CConcept* CConceptNominalSchemaGroundingData::getGroundingConcept() {
					return mGroundingConcept;
				}

				cint64 CConceptNominalSchemaGroundingData::calculateHashValue() {
					cint64 hashValue = (cint64)mGroundingConcept;
					cint64 multiplier = 13;
					for (CPROCESSLIST<CConcept*>::const_iterator it = mBindedNomSchConList.constBegin(), itEnd = mBindedNomSchConList.constEnd(); it != itEnd; ++it) {
						CConcept* concept = *it;
						hashValue += multiplier*(cint64)concept;
						multiplier = multiplier*2+1;
					}
					return hashValue;
				}

				bool CConceptNominalSchemaGroundingData::isEquivalentTo(const CConceptNominalSchemaGroundingData& data) const {
					if (mGroundingConcept != data.mGroundingConcept) {
						return false;
					}
					CPROCESSLIST<CConcept*>::const_iterator it1 = mBindedNomSchConList.constBegin(), it1End = mBindedNomSchConList.constEnd();
					CPROCESSLIST<CConcept*>::const_iterator it2 = data.mBindedNomSchConList.constBegin(), it2End = data.mBindedNomSchConList.constEnd();
					while (it1 != it1End) {
						if (it2 == it2End) {
							return false;
						} else {
							CConcept* con1 = *it1;
							CConcept* con2 = *it2;
							if (con1 != con2) {
								return false;
							}
						}
						++it1;
						++it2;
					}
					return true;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
