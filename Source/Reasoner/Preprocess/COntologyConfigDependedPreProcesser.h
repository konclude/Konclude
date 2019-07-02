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

#ifndef KONCLUDE_REASONER_PREPROCESS_CONTOLOGYCONFIGDEPENDEDPREPROCESSER_H
#define KONCLUDE_REASONER_PREPROCESS_CONTOLOGYCONFIGDEPENDEDPREPROCESSER_H

// Libraries includes


// Namespace includes
#include "COntologyPreProcesser.h"
#include "CLexicalNormalisationPreProcess.h"
#include "CNegationTransformationPreProcess.h"
#include "CSubroleTransformationPreProcess.h"
#include "COntologyAssignmentTransformationPreProcess.h"
#include "COntologyStructureInspectionPreProcess.h"
#include "CRoleChainAutomataTransformationPreProcess.h"
#include "CMappingUpdaterPreProcess.h"
#include "CBranchTriggerPreProcess.h"
#include "COntologyPreProcess.h"
#include "CDataExtenderPreProcess.h"
#include "CConsistenceCheckerPreProcess.h"
#include "CTriggeredImplicationBinaryAbsorberPreProcess.h"
#include "CCommonDisjunctConceptExtractionPreProcess.h"
#include "CCoreOntologyCyclesExtractorPreProcess.h"
#include "CCoreBlockingConceptMarkerPreProcess.h"
#include "CFullNominalSchemaGroundingPreProcess.h"
#include "CNominalSchemaTemplateExtractionPreProcess.h"
#include "CPrecomputationPreProcess.h"
#include "CPreProcessContextBase.h"
#include "CExtractPropagationIntoCreationDirectionPreProcess.h"
#include "CExtractRelevantConceptRolePreProcess.h"
#include "CDisjunctSortingPreProcess.h"
#include "CDataLiteralNormalizerPreProcess.h"
#include "CDatatypeNormalizerPreProcess.h"
#include "CReverseRoleAssertionGeneratorPreProcess.h"
#include "CBranchStatisticsExtenderPreProcess.h"

// Other includes
#include "Config/CConfigDebugDataReader.h"

#include "Reasoner/Ontology/COntologyTextFormater.h"

#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CUnspecifiedMessageInformationRecord.h"

#include "Logger/CLogIdentifier.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Control::Command;
	using namespace Control::Command::Records;
	using namespace Logger;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		COntologyConfigDependedPreProcesser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyConfigDependedPreProcesser : public COntologyPreProcesser, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COntologyConfigDependedPreProcesser(CCommandRecordRouter &commandRecRouter);
					COntologyConfigDependedPreProcesser();

					//! Destructor
					virtual ~COntologyConfigDependedPreProcesser();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CConfiguration *config);

				// protected methods
				protected:
					virtual COntologyConfigDependedPreProcesser* reportLog(const QString& string);

				// protected variables
				private:
					CCommandRecordRouter* commandRecordRouter;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CONTOLOGYCONFIGDEPENDEDPREPROCESSER_H
