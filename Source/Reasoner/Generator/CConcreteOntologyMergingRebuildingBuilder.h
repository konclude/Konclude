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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYMERGINGREBUILDINGBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYMERGINGREBUILDINGBUILDER_H


// Libraries includes


// Namespace includes
#include "CConcreteOntologyRebuildingBuilder.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	namespace Reasoner {

		namespace Generator {



			/*! 
			 *
			 *		\class		CConcreteOntologyMergingRebuildingBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyMergingRebuildingBuilder : public CConcreteOntologyRebuildingBuilder {
				// public methods
				public:
					CConcreteOntologyMergingRebuildingBuilder(CConcreteOntology* baseConcreteOntology);
					virtual ~CConcreteOntologyMergingRebuildingBuilder();


					virtual bool mergeOntology(CConcreteOntology* mergingConcreteOntology, CConcreteOntology* prevConcreteOntology);


				// protected functions
				protected:


					// protected variables
				protected:

			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYMERGINGREBUILDINGBUILDER_H
