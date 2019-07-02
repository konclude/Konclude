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

#ifndef KONCLUDE_REASONER_PREPROCESS_CNominalSchemaTemplateExtractionPreProcess_H
#define KONCLUDE_REASONER_PREPROCESS_CNominalSchemaTemplateExtractionPreProcess_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"
#include "CConceptRoleIndividualLocator.h"
#include "CConceptTriggerLinker.h"


// Other includes
#include "Utilities/Container/CNegLinker.h"

#include "Config/CConfigDataReader.h"

#include "Reasoner/Generator/CConcreteOntologyInverseRoleBuilder.h"

#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/COntologyTextFormater.h"
#include "Reasoner/Ontology/CNominalSchemaTemplateVector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Generator;
		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CNominalSchemaTemplateExtractionPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNominalSchemaTemplateExtractionPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CNominalSchemaTemplateExtractionPreProcess();

					//! Destructor
					virtual ~CNominalSchemaTemplateExtractionPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);


				// protected methods
				protected:
					bool findAbsorbableNominalSchemas(CConcept* concept, bool negation, QSet<CConcept*>* absorbableNominalVariableSet);
					void countNominalSchemaConcepts(CConcept* concept, QHash<CConcept*,cint64>* nominalSchemaConceptCountHash);
					void replaceNominalSchemasConcepts(CConcept* concept, CConcept* replacingConcept, const QSet<CConcept*>& removeNominalVariableSet);
					bool removeSingleAbsorbableNominalSchemaConcepts(CConcept* concept, bool negation);

					bool hasNominalSchemaConcept(CConcept* concept);
					bool hasNominalSchemaConcept(CConcept* concept, QSet<CConcept*>* testedConceptSet);

					bool hasNominalSchemaConceptFillHash(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet);
					bool hasNominalSchemaAbsorbableConceptFillHash(CConcept* concept, bool negation, CBOXHASH<CConcept*,CConcept*>* absorbableConceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet);

					void collectNominalSchemaConcepts(CConcept* concept, CBOXSET<CConcept*>* nominalSchemaConceptSet);
					void createTemplateConceptNominalSchemaConceptMap(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept);

					CNominalSchemaTemplate* getNominalSchemaTemplate(CConcept* concept, bool negation);

					CConcept* createNominalSchemaTemplateReferenceConcept(CNominalSchemaTemplate* nsTemplate);
					CConcept* transformEquivalentConceptToGCI(CConcept* eqConcept);

					void addConceptOperand(CConcept* concept, CConcept* opConcept, bool negated);


				// protected variables
				protected:
					CMemoryAllocationManager* mMemMan;

					CConceptVector* mConceptVec;
					CRoleVector* mRolesVec;
					CConcept* mBottomConcept;
					CConcept* mTopConcept;
					cint64 mConceptCount;
					cint64 mRoleCount;
					CConcreteOntology* mOnto;
					CMBox* mMBox;
					CTBox* mTBox;
					CIndividualVector* mIndividualVector;
					cint64 mIndividualCount;

					CNominalSchemaTemplateVector* mNomSchTemplVec;


					QHash<CConcept*,bool> mConceptNominalSchemaHash;
					QHash<CConcept*,CNominalSchemaTemplate*> mConceptNominalSchemaTemplateHash;


					cint64 mStatCreatedTemplateCount;
					cint64 mStatReplacedConceptCount;


				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CNominalSchemaTemplateExtractionPreProcess_H
