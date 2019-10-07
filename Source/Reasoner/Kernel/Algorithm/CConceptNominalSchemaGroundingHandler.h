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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCONCEPTNOMINALSCHEMAGROUNDINGHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCONCEPTNOMINALSCHEMAGROUNDINGHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CConceptNominalSchemaGroundingHasher.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Algorithm {


				typedef QPair<CConcept*,CPropagationBindingDescriptor*> TConceptPropagationBindingPair;

				/*! 
				 *
				 *		\class		CConceptNominalSchemaGroundingHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConceptNominalSchemaGroundingHandler {
					// public methods
					public:
						//! Constructor
						CConceptNominalSchemaGroundingHandler();

						CSortedNegLinker<CConcept*>* getGroundingConceptLinker(CIndividualProcessNode* processNode, CPropagationBindingSet* propBindSet, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>*& groundedConPropBindDesHash, CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>*& additionalPropBindDesHash, CCalculationAlgorithmContext* calcAlgContext);
						CSortedNegLinker<CConcept*>* getGroundingConceptLinker(CIndividualProcessNode* processNode, CVariableBindingPathSet* varBindPathSet, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,CVariableBindingPathDescriptor*>*& groundedConVarBindPathDesHash, CCalculationAlgorithmContext* calcAlgContext);
						CSortedNegLinker<CConcept*>* getGroundingConceptLinker(CIndividualProcessNode* processNode, CRepresentativeVariableBindingPathMap* repVarBindPathSetMap, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,CVariableBindingPath*>*& groundedConVarBindPathHash, CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:
						CConcept* getNominalConcept(CIndividualProcessNode* processNode, bool forceNotPruned, CCalculationAlgorithmContext* calcAlgContext);
						void collectAllNominalConcepts(CPROCESSINGSET<CConcept*>*& allNominalConceptSet, CCalculationAlgorithmContext* calcAlgContext);
						void addConceptOperand(CConcept* concept, CConcept* opConcept, bool negated);
						CConcept* createNominalSchemaConceptCopy(CConcept* concept);
						
						CConcept* createGroundedNominalSchemaConcept(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash, QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >* nomSchConIndividualItHash, CCalculationAlgorithmContext* calcAlgContext);
						CSortedNegLinker<CConcept*>* createGroundedNominalSchemaConcept(CConcept* concept, bool negated, CReapplyConceptLabelSet* conLabelSet, CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash, QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >* nomSchConIndividualItHash, CCalculationAlgorithmContext* calcAlgContext);
						
						CSortedNegLinker<CConcept*>* createNominalSchemaGroundingConcepts(CIndividualProcessNode* indiProcessNode, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*, TConceptPropagationBindingPair >* nominalSchemaVarBindedNominalHash, CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>*& groundedConPropBindDesHash, CNominalSchemaTemplate* nsTemplate, CCalculationAlgorithmContext* calcAlgContext);
						CSortedNegLinker<CConcept*>* createNominalSchemaGroundingConcepts(CIndividualProcessNode* indiProcessNode, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair >* nominalSchemaVarBindedNominalHash, CNominalSchemaTemplate* nsTemplate, CCalculationAlgorithmContext* calcAlgContext);
						

						bool forceExtensionLocalisation(CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:
						CMemoryAllocationManager* mMemMan;
						CConceptVector* mConceptVec;
						CTBox* mTBox;
						CConceptNominalSchemaGroundingHash* mReplaceHash;
						CProcessingDataBox* mDataBox;

						bool mLocalizedExtensions;

						bool mConfReuseGroundedNominalSchemaConcepts;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCONCEPTNOMINALSCHEMAGROUNDINGHANDLER_H
