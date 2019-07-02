/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTTEXTFORMATER_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTTEXTFORMATER_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CRole.h"
#include "CConcept.h"
#include "CIndividual.h"
#include "CTerminology.h"
#include "CIRIName.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CConceptTextFormater
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptTextFormater {
				// public methods
				public:
					//! Constructor
					CConceptTextFormater();

					//! Destructor
					virtual ~CConceptTextFormater();

					static QString getConceptString(CConcept *concept, bool negated, bool useTag = true, qint64 defSkipCount = 1);

					static QString getConceptViewString(CConcept *concept, bool negated, bool useTag = true, qint64 defSkipCount = 1, bool useNumberTag = true);

				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTTEXTFORMATER_H
