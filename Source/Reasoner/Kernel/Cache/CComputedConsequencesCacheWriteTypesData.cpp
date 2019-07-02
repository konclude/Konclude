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

#include "CComputedConsequencesCacheWriteTypesData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CComputedConsequencesCacheWriteTypesData::CComputedConsequencesCacheWriteTypesData() {
					mWriteDataType = CCWT_TYPE;
				}

				CComputedConsequencesCacheWriteTypesData* CComputedConsequencesCacheWriteTypesData::initTypesCacheWriteData(CIndividual* individual, CConcept* concept, bool negation) {
					mIndividual = individual;
					mConceptNegation = negation;
					mConceptType = concept;
					return this;
				}

				CIndividual* CComputedConsequencesCacheWriteTypesData::getIndividual() {
					return mIndividual;
				}

				CConcept* CComputedConsequencesCacheWriteTypesData::getConcept() {
					return mConceptType;
				}

				bool CComputedConsequencesCacheWriteTypesData::getNegation() {
					return mConceptNegation;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
