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

#include "CRetrieveIncompletelyAssociationCachedEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				namespace Events {


					CRetrieveIncompletelyAssociationCachedEvent::CRetrieveIncompletelyAssociationCachedEvent(CCallbackData* callbackData, cint64 ontologyIdentifier, CBackendIndividualRetrievalComputationUpdateCoordinationHash* lastRetrievalHash, CBackendIndividualRetrievalComputationUpdateCoordinationHash* newRetrievalHash, bool allIndividualsAdded, bool refillRetrievalCoordHash, cint64 limit)
							: CCustomEvent(EVENTTYPE),mCallback(callbackData), mLastRetrievalHash(lastRetrievalHash), mNewRetrievalHash(newRetrievalHash), mLimit(limit), mOntologyIdentifier(ontologyIdentifier), mRefillRetrievalCoordHash(refillRetrievalCoordHash) {

						mAllIndividualsAdded = allIndividualsAdded;
					}


					CCallbackData* CRetrieveIncompletelyAssociationCachedEvent::getCallback() {
						return mCallback;
					}


					bool CRetrieveIncompletelyAssociationCachedEvent::hasRefillRetrievalCoordHashOrdered() {
						return mRefillRetrievalCoordHash;
					}

					cint64 CRetrieveIncompletelyAssociationCachedEvent::getRetrievalLimit() {
						return mLimit;
					}

					cint64 CRetrieveIncompletelyAssociationCachedEvent::getOntologyIdentifier() {
						return mOntologyIdentifier;
					}

					CBackendIndividualRetrievalComputationUpdateCoordinationHash* CRetrieveIncompletelyAssociationCachedEvent::getLastIndividualsRetrievalCoordinationHash() {
						return mLastRetrievalHash;
					}

					CBackendIndividualRetrievalComputationUpdateCoordinationHash* CRetrieveIncompletelyAssociationCachedEvent::getNewIndividualsRetrievalCoordinationHash() {
						return mNewRetrievalHash;
					}


					bool CRetrieveIncompletelyAssociationCachedEvent::hasAllIndividualsAdded() {
						return mAllIndividualsAdded;
					}


				}; // end namespace Events

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
