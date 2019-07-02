/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
