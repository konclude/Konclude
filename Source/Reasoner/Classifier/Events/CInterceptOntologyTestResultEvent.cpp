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

#include "CInterceptOntologyTestResultEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {

			namespace Events {


				CInterceptOntologyTestResultEvent::CInterceptOntologyTestResultEvent(CConcreteOntology *ontology, CConcept *satisfiableTestedConcept, CCallbackData *callbackData) 
						: CCustomEvent(EVENTINTERCEPTONTOLOGYTESTRESULT) {
					onto = ontology;
					conceptSatisfiableTest = true;
					conceptSubsumptionTest = false;
					con0 = satisfiableTestedConcept;
					con1 = 0;
					callback = callbackData;
				}

				CInterceptOntologyTestResultEvent::CInterceptOntologyTestResultEvent(CConcreteOntology *ontology, CConcept *subsumerTestedConcept, CConcept *subsumedTestedConcept, CCallbackData *callbackData) 
						: CCustomEvent(EVENTINTERCEPTONTOLOGYTESTRESULT) {
					onto = ontology;
					conceptSatisfiableTest = false;
					conceptSubsumptionTest = true;
					con0 = subsumerTestedConcept;
					con1 = subsumedTestedConcept;
					callback = callbackData;
				}


				CInterceptOntologyTestResultEvent::~CInterceptOntologyTestResultEvent() {
				}


				CConcreteOntology *CInterceptOntologyTestResultEvent::getOntology() {
					return onto;
				}

				bool CInterceptOntologyTestResultEvent::isConceptSatisfiableTest() {
					return conceptSatisfiableTest;
				}

				bool CInterceptOntologyTestResultEvent::isConceptSubsumptionTest() {
					return conceptSubsumptionTest;
				}


				CConcept *CInterceptOntologyTestResultEvent::getSatisfiableTestedConcept() {
					if (conceptSatisfiableTest) {
						return con0;
					} 
					return 0;
				}

				CConcept *CInterceptOntologyTestResultEvent::getSubsumerTestedConcept() {
					if (conceptSubsumptionTest) {
						return con0;
					} 
					return 0;
				}

				CConcept *CInterceptOntologyTestResultEvent::getSubsumedTestedConcept() {
					if (conceptSubsumptionTest) {
						return con1;
					} 
					return 0;
				}

				CCallbackData *CInterceptOntologyTestResultEvent::getCallbackData() {
					return callback;
				}


			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
