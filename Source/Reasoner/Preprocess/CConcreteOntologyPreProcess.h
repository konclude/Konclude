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

#ifndef KONCLUDE_REASONER_PREPROCESS_CCONCRETEONTOLOGYPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CCONCRETEONTOLOGYPREPROCESS_H

// Libraries includes


// Namespace includes
#include "COntologyPreProcess.h"


// Other includes
#include "Reasoner/Ontology/COntology.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CConcreteOntologyPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyPreProcess : public COntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CConcreteOntologyPreProcess();

					//! Destructor
					virtual ~CConcreteOntologyPreProcess();

					virtual COntology *preprocess(COntology *ontology);
					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context) = 0;


				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CCONCRETEONTOLOGYPREPROCESS_H
