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

#ifndef KONCLUDE_REASONER_REALIZATION_CINDIVIDUALCONCEPTASSERTIONSETCOLLECTOR_H
#define KONCLUDE_REASONER_REALIZATION_CINDIVIDUALCONCEPTASSERTIONSETCOLLECTOR_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CConceptRealizationConceptVisitor.h"
#include "CConceptRealizationInstantiatedVisitor.h"
#include "CConceptRealization.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CIndividual.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realization {

			/*! 
			 *
			 *		\class		CIndividualConceptAssertionSetCollector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualConceptAssertionSetCollector : public CConceptRealizationConceptVisitor, public CConceptRealizationInstantiatedVisitor {
				// public methods
				public:
					//! Constructor
					CIndividualConceptAssertionSetCollector(CIndividual* indi, QSet< QPair<CIndividual*,CConcept*> >* assSet);

					virtual bool visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization);
					virtual bool visitConcept(CConcept* concept, CConceptRealization* conRealization);

				// protected methods
				protected:

				// protected variables
				protected:
					CIndividual* mIndi;
					QSet< QPair<CIndividual*,CConcept*> >* mAssSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CINDIVIDUALCONCEPTASSERTIONSETCOLLECTOR_H
