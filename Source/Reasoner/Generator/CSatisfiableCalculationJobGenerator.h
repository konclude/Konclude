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

#ifndef KONCLUDE_REASONER_GENERATOR_CSATISFIABLECALCULATIONJOBGENERATOR_H
#define KONCLUDE_REASONER_GENERATOR_CSATISFIABLECALCULATIONJOBGENERATOR_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Query/CSatisfiableCalculationJob.h"
#include "Reasoner/Query/CSatisfiableCalculationConceptConstruct.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Generator {

			/*! 
			 *
			 *		\class		CSatisfiableCalculationJobGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSatisfiableCalculationJobGenerator {
				// public methods
				public:
					//! Constructor
					CSatisfiableCalculationJobGenerator(CConcreteOntology *ontology);

					//! Destructor
					virtual ~CSatisfiableCalculationJobGenerator();


					CSatisfiableCalculationJob* getSatisfiableCalculationJob(CConcept* concept, bool negation = false, CIndividual* individual = nullptr, CSatisfiableCalculationJob* satCalcJob = nullptr);
					CSatisfiableCalculationJob* getSatisfiableCalculationJob(CConcept* concept1, bool negation1, CConcept* concept2, bool negation2, CIndividual* individual = nullptr, CSatisfiableCalculationJob* satCalcJob = nullptr);
					CSatisfiableCalculationJob* getSatisfiableCalculationJob(const QList<CConcept*>& conceptList, CIndividual* individual = nullptr, CSatisfiableCalculationJob* satCalcJob = nullptr);
					CSatisfiableCalculationJob* getSatisfiableCalculationJob(const QList< QPair<CConcept*,bool> >& conceptNegationList, CIndividual* individual = nullptr, CSatisfiableCalculationJob* satCalcJob = nullptr);

					CSatisfiableCalculationJob* getSatisfiableCalculationJob(const QList< CIndividual* >& indiList, CSatisfiableCalculationJob* satCalcJob = nullptr);

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

#endif // KONCLUDE_REASONER_GENERATOR_CSATISFIABLECALCULATIONJOBGENERATOR_H
