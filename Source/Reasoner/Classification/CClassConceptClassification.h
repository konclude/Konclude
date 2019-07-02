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

#ifndef KONCLUDE_REASONER_CLASSIFICATION_CCLASSCONCEPTCLASSIFICATION_H
#define KONCLUDE_REASONER_CLASSIFICATION_CCLASSCONCEPTCLASSIFICATION_H

// Libraries includes


// Namespace includes
#include "ClassificationSettings.h"
#include "CClassificationStatistics.h"

// Other includes
#include "Reasoner/Taxonomy/CTaxonomy.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Taxonomy;

		namespace Classification {

			/*! 
			 *
			 *		\class		CClassConceptClassification
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassConceptClassification {
				// public methods
				public:
					//! Constructor
					CClassConceptClassification();

					bool hasClassConceptTaxonomy();
					CTaxonomy* getClassConceptTaxonomy();
					CClassConceptClassification* setClassConceptTaxonomy(CTaxonomy* taxonomy);

					bool hasClassificationStatistics();
					CClassificationStatistics* getClassificationStatistics();
					CClassConceptClassification* setClassificationStatistics(CClassificationStatistics* statColl);

				// protected methods
				protected:

				// protected variables
				protected:
					CTaxonomy* mClassConceptTaxonomy;
					CClassificationStatistics* mStatsColl;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFICATION_CCLASSCONCEPTCLASSIFICATION_H
