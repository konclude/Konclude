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

#ifndef KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATION_H
#define KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATION_H

// Libraries includes


// Namespace includes
#include "ClassificationSettings.h"
#include "CClassConceptClassification.h"
#include "CPropertyRoleClassification.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {

			/*! 
			 *
			 *		\class		CClassification
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassification {
				// public methods
				public:
					//! Constructor
					CClassification();

					//! Destructor
					virtual ~CClassification();


					bool isOntologyClassified();
					CClassification* setOntologyClassified(bool classified);

					bool hasClassConceptClassification();
					CClassConceptClassification* getClassConceptClassification();
					CClassification* setClassConceptClassification(CClassConceptClassification* classConceptClassificaton);

					bool hasObjectPropertyRoleClassification();
					CPropertyRoleClassification* getObjectPropertyRoleClassification();
					CClassification* setObjectPropertyRoleClassification(CPropertyRoleClassification* objectPropertyRoleClassificaton);

					bool hasDataPropertyRoleClassification();
					CPropertyRoleClassification* getDataPropertyRoleClassification();
					CClassification* setDataPropertyRoleClassification(CPropertyRoleClassification* dataPropertyRoleClassificaton);


					bool hasPropertyRoleClassification(bool dataProperties = false);
					CPropertyRoleClassification* getPropertyRoleClassification(bool dataProperties = false);
					CClassification* setPropertyRoleClassification(CPropertyRoleClassification* propertyRoleClassificaton, bool dataProperties = false);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mClassified;
					CClassConceptClassification* mClassConceptClassification;
					CPropertyRoleClassification* mObjectPropertyRoleClassification;
					CPropertyRoleClassification* mDataPropertyRoleClassification;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATION_H
