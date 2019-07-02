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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGREQUIREMENTEXPANDER_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGREQUIREMENTEXPANDER_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntology.h"
#include "COntologyProcessingRequirement.h"
#include "COntologyProcessingStepRequirement.h"
#include "COntologyProcessingStep.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyProcessingRequirementExpander
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyProcessingRequirementExpander {
				// public methods
				public:
					//! Constructor
					COntologyProcessingRequirementExpander();
					~COntologyProcessingRequirementExpander();

					const QList<COntologyProcessingRequirement*> getUnsatisfiedRequirementsExpanded(const QList<COntologyProcessingRequirement*>& requirements, CConcreteOntology* ontology);

					COntologyProcessingStepRequirement* getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::PROCESSINGSTEPTYPE processingStep);
					COntologyProcessingStepRequirement* getRequiredConsistencyOntologyProcessingStepRequirement();
					COntologyProcessingStepRequirement* getRequiredIndividualPrecomputationOntologyProcessingStepRequirement();

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyProcessingStepRequirement* mTriplesMappingOntProStepReq;
					COntologyProcessingStepRequirement* mActiveCountOntProStepReq;
					COntologyProcessingStepRequirement* mBuildOntProStepReq;
					COntologyProcessingStepRequirement* mPreprocessOntProStepReq;
					COntologyProcessingStepRequirement* mTriplesIndexingOntProStepReq;
					COntologyProcessingStepRequirement* mConsistencyOntProStepReq;
					COntologyProcessingStepRequirement* mIndividualOntProStepReq;
					COntologyProcessingStepRequirement* mSaturationOntProStepReq;
					COntologyProcessingStepRequirement* mPrecomputeCyclesOntProStepReq;
					COntologyProcessingStepRequirement* mPrecomputeOccStatsOntProStepReq;
					COntologyProcessingStepRequirement* mClassClassifyOntProStepReq;
					COntologyProcessingStepRequirement* mObjPropClassifyOntProStepReq;
					COntologyProcessingStepRequirement* mDataPropClassifyOntProStepReq;
					COntologyProcessingStepRequirement* mInitRealizeOntProStepReq;
					COntologyProcessingStepRequirement* mConceptRealizeOntProStepReq;
					COntologyProcessingStepRequirement* mRoleRealizeOntProStepReq;
					COntologyProcessingStepRequirement* mSameIndividualsRealizeOntProStepReq;
					COntologyProcessingStepRequirement* mComplexQueryAnsweringOntProStepReq;

					COntologyProcessingStepRequirement* mRequiredConsistencyOntProStepReq;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGREQUIREMENTEXPANDER_H
