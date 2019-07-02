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

#include "CApproximatedSaturationCalculationConstructionConstruct.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CApproximatedSaturationCalculationConstructionConstruct::CApproximatedSaturationCalculationConstructionConstruct() : CLinkerBase<CApproximatedSaturationCalculationConstructionConstruct*,CApproximatedSaturationCalculationConstructionConstruct>(this) {
				mIndividual = nullptr;
				mConstructRole = nullptr;
				mConstructConcept = nullptr;
				mIndiID = -1;
				mConstructConceptNegation = false;
				mQueueProcessing = false;
				mSatConRefLinking = nullptr;
				mSatIndiRefLinking = nullptr;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::getNextConstruct() {
				return getNext();
			}

			CConcept* CApproximatedSaturationCalculationConstructionConstruct::getConstructConcept() {
				return mConstructConcept;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setConstructConcept(CConcept* concept) {
				mConstructConcept = concept;
				return this;
			}

			CRole* CApproximatedSaturationCalculationConstructionConstruct::getConstructRole() {
				return mConstructRole;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setConstructRole(CRole* role) {
				mConstructRole = role;
				return this;
			}

			bool CApproximatedSaturationCalculationConstructionConstruct::getConstructConceptNegation() {
				return mConstructConceptNegation;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setConstructConceptNegation(bool negation) {
				mConstructConceptNegation = negation;
				return this;
			}


			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setIndividual(CIndividual* individual) {
				mIndividual = individual;
				return this;
			}

			CIndividual* CApproximatedSaturationCalculationConstructionConstruct::getIndividual() {
				return mIndividual;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setIndividualID(cint64 individualID) {
				mIndiID = individualID;
				return this;
			}

			cint64 CApproximatedSaturationCalculationConstructionConstruct::getIndividualID() {
				return mIndiID;
			}

			bool CApproximatedSaturationCalculationConstructionConstruct::getQueueProcessing() {
				return mQueueProcessing;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setQueueProcessing(bool queueProcessing) {
				mQueueProcessing = queueProcessing;
				return this;
			}

			CSaturationConceptReferenceLinking* CApproximatedSaturationCalculationConstructionConstruct::getSaturationConceptReferenceLinking() {
				return mSatConRefLinking;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setSaturationConceptReferenceLinking(CSaturationConceptReferenceLinking* satConRefLinking) {
				mSatConRefLinking = satConRefLinking;
				return this;
			}

			CSaturationIndividualReferenceLinking* CApproximatedSaturationCalculationConstructionConstruct::getSaturationIndividualReferenceLinking() {
				return mSatIndiRefLinking;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationConstructionConstruct::setSaturationIndividualReferenceLinking(CSaturationIndividualReferenceLinking* satIndiRefLinking) {
				mSatIndiRefLinking = satIndiRefLinking;
				return this;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
