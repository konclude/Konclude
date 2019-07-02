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

#ifndef KONCLUDE_REASONER_PREPROCESS_CFULLNOMINALSCHEMAGROUNDINGPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CFULLNOMINALSCHEMAGROUNDINGPREPROCESS_H

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
			 *		\class		CFullNominalSchemaGroundingPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CFullNominalSchemaGroundingPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CFullNominalSchemaGroundingPreProcess();

					//! Destructor
					virtual ~CFullNominalSchemaGroundingPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);


				// protected methods
				protected:
					CConcept* createNominalSchemaAxiomCollectionConcept(CConcept* concept, bool negated, CNominalSchemaTemplate* nsTemplate);
					void addConceptOperand(CConcept* concept, CConcept* opConcept, bool negated);

					CConcept* createGroundedNominalSchemaConcept(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash, QHash<CConcept*, QSet<CConcept*>::const_iterator >* nomSchConIndividualItHash);
					
					CConcept* createNominalSchemaConceptCopy(CConcept* concept);

					bool requiresGCIGrounding(CNominalSchemaTemplate* nsTemplate, bool negated);
					bool findAbsorbableNominalSchemas(CConcept* concept, bool negation, QSet<CConcept*>* absorbableNominalVariableSet);

				// protected variables
				protected:
					CMemoryAllocationManager* mMemMan;

					CConceptVector* mConceptVec;
					CRoleVector* mRolesVec;
					CConcept* mBottomConcept;
					CConcept* mTopConcept;
					cint64 mConceptCount;
					cint64 mRoleCount;
					CIndividualVector* mIndividualVector;
					cint64 mIndividualCount;
					CConcreteOntology* mOnto;
					CMBox* mMBox;

					CNominalSchemaTemplateVector* mNomSchTemplVec;

					QSet<CConcept*> mNominalConceptSet;


					QHash< QPair<CConcept*,bool>, CConcept*> mCollConHash;
					QHash< QPair<CConcept*,bool>, bool> mReqGroundConHash;

					QHash<QByteArray,CConcept*> mConceptNomSchemIndiReplaceHash;

					bool mConfReuseGroundedNominalSchemaConcepts;
					bool mConfGroundNominalSchemaAbsorbableConcepts;
					bool mConfFullyGroundNominalSchemaConcepts;

					cint64 mStatGroundedTemplateCount;
					cint64 mStatCreatedConceptCount;
					cint64 mStatReplacedConceptCount;

					cint64 mStatInstantiatedAxiomsCount;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CFULLNOMINALSCHEMAGROUNDINGPREPROCESS_H
