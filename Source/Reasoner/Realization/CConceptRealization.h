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

#ifndef KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATION_H
#define KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATION_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CConceptRealizationInstanceVisitor.h"
#include "CConceptRealizationInstantiatedVisitor.h"
#include "CConceptRealizationConceptVisitor.h"
#include "CConceptRealizationIndividualVisitor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CConceptRealization
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptRealization {
				// public methods
				public:
					//! Constructor
					CConceptRealization();

					virtual bool visitDirectInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitInstances(CConcept* concept, bool direct, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitDirectTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitTypes(CIndividual* individual, bool direct, CConceptRealizationInstantiatedVisitor* visitor);

					virtual bool visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) = 0;
					virtual bool visitInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) = 0;
					virtual bool visitInstances(CConceptInstantiatedItem* item, bool direct, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitDirectTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor) = 0;
					virtual bool visitTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor) = 0;
					virtual bool visitTypes(CConceptInstanceItem* item, bool direct, CConceptRealizationInstantiatedVisitor* visitor);

					virtual bool visitIndividuals(CConceptInstanceItem* item, CConceptRealizationIndividualVisitor* visitor) = 0;
					virtual bool visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor) = 0;

					virtual CConceptInstantiatedItem* getInstantiatedItem(CConcept* concept) = 0;
					virtual CConceptInstanceItem* getInstanceItem(CIndividual* individual) = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATION_H
