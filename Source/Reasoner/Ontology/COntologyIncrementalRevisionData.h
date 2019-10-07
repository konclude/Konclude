/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINCREMENTALREVISIONDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINCREMENTALREVISIONDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"
#include "COntologyIncrementalAxiomChangeData.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyIncrementalRevisionData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyIncrementalRevisionData {
				// public methods
				public:
					//! Constructor
					COntologyIncrementalRevisionData(COntologyContext* ontoContext = nullptr);

					//! Destructor
					virtual ~COntologyIncrementalRevisionData();


					COntologyIncrementalRevisionData* referenceIncrementalRevision(COntologyIncrementalRevisionData* incRevData);


					CConcreteOntology* getBasementOntology();
					CConcreteOntology* getPreviousOntologyVersion();

					COntologyIncrementalRevisionData* setBasementOntology(CConcreteOntology* ontology);
					COntologyIncrementalRevisionData* setPreviousOntologyVersion(CConcreteOntology* ontology);


					COntologyIncrementalAxiomChangeData* getCummulativeAxiomChangeData();
					COntologyIncrementalAxiomChangeData* getAxiomChangeData();
					cint64 getLastChangeCountedAxiom();
					COntologyIncrementalRevisionData* setLastChangeCountedAxiom(cint64 lastAxiom);


					bool isBasementOntology();
					bool isInitialOntology();
					bool isIncrementalOntology();



					bool isTrivialExpansionToPreviousOntology(CConcreteOntology* ontology);
					bool isTrivialExpansionToPreviousBuiltOntology();
					bool isTrivialExpansionToPreviousConsistentOntology();


					COntologyIncrementalRevisionData* setPreviousBuiltOntology(CConcreteOntology* ontology);
					CConcreteOntology* getPreviousBuiltOntology();


					COntologyIncrementalRevisionData* setPreviousConsistentOntology(CConcreteOntology* ontology);
					CConcreteOntology* getPreviousConsistentOntology();

					COntologyIncrementalRevisionData* setPreviousClassesClassifiedOntology(CConcreteOntology* ontology);
					CConcreteOntology* getPreviousClassesClassifiedOntology();

					COntologyIncrementalRevisionData* setPreviousSameIndividualsRealizedOntology(CConcreteOntology* ontology);
					CConcreteOntology* getPreviousSameIndividualsRealizedOntology();


					COntologyIncrementalRevisionData* setPreviousClassTypesRealizedOntology(CConcreteOntology* ontology);
					CConcreteOntology* getPreviousClassTypesRealizedOntology();


					COntologyIncrementalRevisionData* setPreviousRoleTypesRealizedOntology(CConcreteOntology* ontology);
					CConcreteOntology* getPreviousRoleTypesRealizedOntology();

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;


					CConcreteOntology* mBasementOntology;
					CConcreteOntology* mPreviousVersionOntology;

					CConcreteOntology* mPreviousBuiltOntology;
					CConcreteOntology* mPreviousConsistentOntology;
					CConcreteOntology* mPreviousClassesClassifiedOntology;
					CConcreteOntology* mPreviousSameIndividualsRealizedOntology;
					CConcreteOntology* mPreviousClassTypesRealizedOntology;
					CConcreteOntology* mPreviousRoleTypesRealizedOntology;


					COntologyIncrementalAxiomChangeData mAxiomChangeData;
					COntologyIncrementalAxiomChangeData mCummulativeAxiomChangeData;
					cint64 mLastChangeCountedAxiom;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINCREMENTALREVISIONDATA_H
