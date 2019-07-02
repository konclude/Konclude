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

#include "CClassificationCalculationSupport.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationCalculationSupport::CClassificationCalculationSupport() {
				subsumCalcSponsor = 0;
				satisCalcSponsor = 0;
				delSubsumCalcSponsor = 0;
				delSatisCalcSponsor = 0;
			}


			CClassificationCalculationSupport::~CClassificationCalculationSupport() {
				if (delSubsumCalcSponsor) {
					delete delSubsumCalcSponsor;
				}
				if (delSatisCalcSponsor) {
					delete delSatisCalcSponsor;
				}
			}


			CClassificationCalculationSupport *CClassificationCalculationSupport::useSubsumptionCalculationSponsor(CSubsumptionCalculationSponsor *subsumptionCalculationSponsor) {
				subsumCalcSponsor = subsumptionCalculationSponsor;
				return this;
			}

			CClassificationCalculationSupport *CClassificationCalculationSupport::setSubsumptionCalculationSponsor(CSubsumptionCalculationSponsor *takeSubsumptionCalculationSponsor) {
				subsumCalcSponsor = takeSubsumptionCalculationSponsor;
				if (delSubsumCalcSponsor) {
					delete delSubsumCalcSponsor;
				}
				delSubsumCalcSponsor = takeSubsumptionCalculationSponsor;
				return this;
			}

			CSubsumptionCalculationSponsor *CClassificationCalculationSupport::getSubsumptionCalculationSponsor() {
				return subsumCalcSponsor;
			}

			CClassificationCalculationSupport *CClassificationCalculationSupport::useSatisfiableCalculationSponsor(CSatisfiableCalculationSponsor *satisfiableCalculationSponsor) {
				satisCalcSponsor = satisfiableCalculationSponsor;
				return this;
			}

			CClassificationCalculationSupport *CClassificationCalculationSupport::setSatisfiableCalculationSponsor(CSatisfiableCalculationSponsor *takeSatisfiableCalculationSponsor) {
				satisCalcSponsor = takeSatisfiableCalculationSponsor;
				if (delSatisCalcSponsor) {
					delete delSatisCalcSponsor;
				}
				delSatisCalcSponsor = takeSatisfiableCalculationSponsor;
				return this;
			}

			CSatisfiableCalculationSponsor *CClassificationCalculationSupport::getSatisfiableCalculationSponsor() {
				return satisCalcSponsor;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
