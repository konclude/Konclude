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

#include "CVariable.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CVariable::CVariable() {
				mNominalConcept = nullptr;
			}


			CVariable* CVariable::initVariable(CConcept* nominalConcept, cint64 pathVariableID) {
				mNominalConcept = nominalConcept;
				mPathVariableID = pathVariableID;
				return this;
			}


			bool CVariable::isNominalVariable() {
				return mNominalConcept != nullptr;
			}

			CConcept* CVariable::getNominalVariableConcept() {
				return mNominalConcept;
			}

			cint64 CVariable::getPathVariableID() {
				return mPathVariableID;
			}

			bool CVariable::operator<=(const CVariable& beforeData) {
				return this <= &beforeData;
			}

			bool CVariable::operator<=(const CVariable*& beforeData) {
				return this <= beforeData;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
