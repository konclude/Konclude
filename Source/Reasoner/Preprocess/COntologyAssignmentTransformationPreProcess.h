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

#ifndef KONCLUDE_REASONER_PREPROCESS_CONTOLOGYASSIGNMENTTRANSFORMATIONPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CONTOLOGYASSIGNMENTTRANSFORMATIONPREPROCESS_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"


// Other includes
#include "Reasoner/Ontology/CBoxContext.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CMBox.h"

#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CSortedNegLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		COntologyAssignmentTransformationPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyAssignmentTransformationPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					COntologyAssignmentTransformationPreProcess(CConcreteOntology *ontologyAssignment);

					//! Destructor
					virtual ~COntologyAssignmentTransformationPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology *ontology;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CONTOLOGYASSIGNMENTTRANSFORMATIONPREPROCESS_H
