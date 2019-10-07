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

#include "CClassification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {


			CClassification::CClassification() {
				mClassified = false;
				mClassConceptClassification = nullptr;
				mObjectPropertyRoleClassification = nullptr;
				mDataPropertyRoleClassification = nullptr;
			}


			CClassification::~CClassification() {
			}


			bool CClassification::isOntologyClassified() {
				return mClassified;
			}

			CClassification* CClassification::setOntologyClassified(bool classified) {
				mClassified = classified;
				return this;
			}

			bool CClassification::hasClassConceptClassification() {
				return mClassConceptClassification != nullptr;
			}

			CClassConceptClassification* CClassification::getClassConceptClassification() {
				return mClassConceptClassification;
			}

			CClassification* CClassification::setClassConceptClassification(CClassConceptClassification* classConceptClassificaton) {
				mClassConceptClassification = classConceptClassificaton;
				return this;
			}



			bool CClassification::hasObjectPropertyRoleClassification() {
				return mObjectPropertyRoleClassification != nullptr;
			}

			CPropertyRoleClassification* CClassification::getObjectPropertyRoleClassification() {
				return mObjectPropertyRoleClassification;
			}

			CClassification* CClassification::setObjectPropertyRoleClassification(CPropertyRoleClassification* objectPropertyRoleClassificaton) {
				mObjectPropertyRoleClassification = objectPropertyRoleClassificaton;
				return this;
			}



			bool CClassification::hasDataPropertyRoleClassification() {
				return mDataPropertyRoleClassification != nullptr;
			}

			CPropertyRoleClassification* CClassification::getDataPropertyRoleClassification() {
				return mDataPropertyRoleClassification;
			}

			CClassification* CClassification::setDataPropertyRoleClassification(CPropertyRoleClassification* dataPropertyRoleClassificaton) {
				mDataPropertyRoleClassification = dataPropertyRoleClassificaton;
				return this;
			}


			bool CClassification::hasPropertyRoleClassification(bool dataProperties) {
				if (dataProperties) {
					return hasDataPropertyRoleClassification();
				} else {
					return hasObjectPropertyRoleClassification();
				}
			}

			CPropertyRoleClassification* CClassification::getPropertyRoleClassification(bool dataProperties) {
				if (dataProperties) {
					return getDataPropertyRoleClassification();
				} else {
					return getObjectPropertyRoleClassification();
				}
			}

			CClassification* CClassification::setPropertyRoleClassification(CPropertyRoleClassification* propertyRoleClassificaton, bool dataProperties) {
				if (dataProperties) {
					return setDataPropertyRoleClassification(propertyRoleClassificaton);
				} else {
					return setObjectPropertyRoleClassification(propertyRoleClassificaton);
				}
			}


		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude
