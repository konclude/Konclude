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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCRETEONTOLOGY_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCRETEONTOLOGY_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntology.h"
#include "COntologyStructureSummary.h"
#include "COntologyStringMapping.h"
#include "COntologyDataBoxes.h"
#include "COntologyContext.h"
#include "CConcreteOntologyContextBase.h"
#include "COntologyBuildData.h"
#include "COntologyCoreConceptCyclesData.h"
#include "CExpressionDataBoxMapping.h"
#include "COntologyProcessingSteps.h"
#include "COntologyIncrementalRevisionData.h"
#include "COntologyTriplesData.h"
#include "CIndividualNameResolver.h"
#include "COntologyLoadData.h"

// Other includes
#include "Parser/Expressions/CAxiomExpression.h"

#include "Config/CConfigurationBase.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"
#include "Reasoner/Consistence/CConsistence.h"
#include "Reasoner/Consistence/CPrecomputation.h"
#include "Reasoner/Classification/CClassification.h"
#include "Reasoner/Preprocess/CPreprocessing.h"
#include "Reasoner/Realization/CRealization.h"

#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;
	using namespace Config;

	namespace Reasoner {

		using namespace Taxonomy;
		using namespace Consistence;
		using namespace Classification;
		using namespace Preprocess;
		using namespace Realization;

		namespace Ontology {

			/*! 
			 *
			 *		\class		CConcreteOntology
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntology : public COntology {
				// public methods
				public:
					//! Constructor
					CConcreteOntology(CConfigurationBase* configuration);
					CConcreteOntology(CConcreteOntology* refOntology, CConfigurationBase* configuration);

					//! Destructor
					virtual ~CConcreteOntology();

					CConcreteOntology* referenceOntology(CConcreteOntology* ontology);

					CConfigurationBase* getConfiguration();


					CABox* getABox();
					CTBox* getTBox();
					CRBox* getRBox();

					CConcept* getConcept(const QString& className);
					CRole* getRole(const QString& propertyName);
					CIndividual* getIndividual(const QString& indiName);

					CConcreteOntology *setConceptTaxonomy(CTaxonomy *taxonomy);
					virtual CTaxonomy *getConceptTaxonomy();
					virtual bool hasConceptTaxonomy();


					CPrecomputation* getPrecomputation();
					bool hasPrecomputation();
					CConcreteOntology* setPrecomputation(CPrecomputation* precomputation);


					CPreprocessing* getPreprocessing();
					bool hasPreprocessing();
					CConcreteOntology* setPreprocessing(CPreprocessing* preprocessing);

					CRealization* getRealization();
					bool hasRealization();
					CConcreteOntology* setRealization(CRealization* realization);


					virtual CConsistence* getConsistence();
					virtual bool hasConsistence();
					CConcreteOntology *setConsistence(CConsistence *consistence);

					virtual CClassification* getClassification();
					virtual bool hasClassification();
					CConcreteOntology *setClassification(CClassification *classification);

					COntologyStructureSummary *getStructureSummary();
					CConcreteOntology *setStructureSummary(COntologyStructureSummary *structureSummary);

					COntologyDataBoxes *getDataBoxes();
					CConcreteOntology *setDataBoxes(COntologyDataBoxes *dataBoxes);

					COntologyStringMapping *getStringMapping();
					CConcreteOntology *setStringMapping(COntologyStringMapping *stringMapping);

					virtual COntologyContext* getOntologyContext();

					COntologyBuildData *getBuildData();
					CConcreteOntology *setBuildData(COntologyBuildData *buildData);

					COntologyCoreConceptCyclesData* getCoreConceptCyclesData();
					CConcreteOntology* setCoreConceptCyclesData(COntologyCoreConceptCyclesData* cyclesData);

					COntologyProcessingSteps* getProcessingSteps();
					CConcreteOntology* setProcessingSteps(COntologyProcessingSteps* stepDatas);


					COntologyTriplesData* getOntologyTriplesData();


					COntologyIncrementalRevisionData* getIncrementalRevisionData();
					CConcreteOntology* setIncrementalRevisionData(COntologyIncrementalRevisionData* incRevData);


					CIndividualNameResolver* getIndividualNameResolver();

					COntologyLoadData* getLoadData();
					CConcreteOntology *setLoadData(COntologyLoadData* loadData);


				// protected methods
				protected:

				// protected variables
				protected:
					CTaxonomy *mTax;
					CConsistence* mConsistence;
					CClassification* mClassification;
					CPreprocessing* mPreprocessing;
					CRealization* mRealization;
					CConfigurationBase* mConfiguration;

					COntologyStructureSummary* mStructSumm;
					COntologyDataBoxes* mDataBoxes;
					COntologyStringMapping* mStringMapping;
					COntologyBuildData* mBuildData;
					COntologyCoreConceptCyclesData* mConceptCyclesData;
					COntologyProcessingSteps* mProcessingSteps;
					COntologyIncrementalRevisionData* mIncRevisionData;
					COntologyLoadData* mLoadData;

					CPrecomputation* mPrecomputation;

					CConcreteOntologyContextBase* mOntConctext;

					COntologyTriplesData* mOntologyTriplesData;
					CIndividualNameResolver* mIndividualNameResolver;

					CONTOLOGYAXIOMSET<CAxiomExpression*>* mAxiomSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCRETEONTOLOGY_H
