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

#ifndef KONCLUDE_REASONER_GENERATOR_CSATURATIONCALCULATIONJOBGENERATOR_H
#define KONCLUDE_REASONER_GENERATOR_CSATURATIONCALCULATIONJOBGENERATOR_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Query/CSaturationCalculationJob.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Generator {

			/*! 
			 *
			 *		\class		CSaturationCalculationJobGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSaturationCalculationJobGenerator {
				// public methods
				public:
					//! Constructor
					CSaturationCalculationJobGenerator(CConcreteOntology *ontology);

					CSaturationCalculationJob* getSaturationCalculationJob(CConcept* concept, bool conceptNegation, CIndividual* individual = nullptr, CSaturationCalculationJob* satCalcJob = nullptr);

				// protected methods
				protected:

				// protected variables
				protected:		
					CConcreteOntology *mOntology;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CSATURATIONCALCULATIONJOBGENERATOR_H
