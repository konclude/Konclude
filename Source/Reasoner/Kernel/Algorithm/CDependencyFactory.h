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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CDEPENDENCYFACTORY_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CDEPENDENCYFACTORY_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"


// Other includes
#include "Reasoner/Kernel/Process/Dependency/CANDDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CSOMEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVALUEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CNEGVALUEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CALLDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CATLEASTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CORDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CAUTOMATTRANSACTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CAUTOMATCHOOSEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CSELFDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CDISTINCTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CNOMINALDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CFUNCTIONALDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CATMOSTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CQUALIFYDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEDCONCEPTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEDLINKDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CIMPLICATIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CEXPANDEDDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CCONNECTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREUSEINDIVIDUALDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREUSECOMPLETIONGRAPHDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREUSECONCEPTSDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CBINDVARIABLEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATEBINDINGDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CBINDPROPAGATEANDDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CBINDPROPAGATEIMPLICATIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATEBINDINGSSUCCESSORDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CBINDPROPAGATEALLDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CBINDPROPAGATECYCLEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATECONNECTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATECONNECTIONAWAYDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CBINDPROPAGATEGROUNDINGDependencyNode.h"

#include "Reasoner/Kernel/Process/Dependency/CVARBINDPROPAGATEANDDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVARBINDPROPAGATEALLDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATEVARIABLEBINDINGDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVARBINDVARIABLEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVARBINDPROPAGATEJOINDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVARBINDPROPAGATEGROUNDINGDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CPROPAGATEVARIABLECONNECTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVARBINDPROPAGATEIMPLICATIONDependencyNode.h"

#include "Reasoner/Kernel/Process/Dependency/CREPRESENTATIVEANDDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CRESOLVEREPRESENTATIVEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREPRESENTATIVEIMPLICATIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREPRESENTATIVEALLDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREPRESENTATIVEBINDVARIABLEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREPRESENTATIVEJOINDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREPRESENTATIVEGROUNDINGDependencyNode.h"

#include "Reasoner/Kernel/Process/Dependency/CDATATYPETRIGGERDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CDATATYPECONNECTIONDependencyNode.h"

#include "Utilities/Memory/CObjectParameterizingAllocator.h"


// Logger includes
#include "Logger/CLogger.h"




namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CDependencyFactory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDependencyFactory {
					// public methods
					public:
						//! Constructor
						CDependencyFactory();

						CREPRESENTATIVEGROUNDINGDependencyNode* createREPRESENTATIVEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CVariableBindingPath* selectVarBindPath, CCalculationAlgorithmContext* calcAlgContext);
						CREPRESENTATIVEJOINDependencyNode* createREPRESENTATIVEJOINDependency(CDependencyTrackPoint*& joinContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* otherDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CREPRESENTATIVEBINDVARIABLEDependencyNode* createREPRESENTATIVEBINDVARIABLEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CREPRESENTATIVEIMPLICATIONDependencyNode* createREPRESENTATIVEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CREPRESENTATIVEALLDependencyNode* createREPRESENTATIVEALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CREPRESENTATIVEANDDependencyNode* createREPRESENTATIVEANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CRESOLVEREPRESENTATIVEDependencyNode* createRESOLVEREPRESENTATIVEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CRepresentativeVariableBindingPathMap* resolveVarBindPathMap, CRepresentativePropagationMap* resolveRepPropMap, CDependencyTrackPoint* additionalDepTrackPoint, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						CVARBINDPROPAGATEALLDependencyNode* createVARBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CVARBINDPROPAGATEANDDependencyNode* createVARBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATEVARIABLEBINDINGDependencyNode* createPROPAGATEVARIABLEBINDINGDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CVARBINDVARIABLEDependencyNode* createVARBINDVARIABLEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CVARBINDPROPAGATEJOINDependencyNode* createVARBINDPROPAGATEJOINDependency(CDependencyTrackPoint*& mergeDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CVARBINDPROPAGATEGROUNDINGDependencyNode* createVARBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATEVARIABLECONNECTIONDependencyNode* createPROPAGATEVARIABLECONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CVARBINDPROPAGATEIMPLICATIONDependencyNode* createVARBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);


						CBINDPROPAGATEGROUNDINGDependencyNode* createBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATECONNECTIONAWAYDependencyNode* createPROPAGATECONNECTIONAWAYDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATECONNECTIONDependencyNode* createPROPAGATECONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CBINDPROPAGATECYCLEDependencyNode* createBINDPROPAGATECYCLEDependency(CDependencyTrackPoint*& continueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* triggDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CBINDPROPAGATEALLDependencyNode* createBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATEBINDINGSSUCCESSORDependencyNode* createPROPAGATEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CBINDPROPAGATEIMPLICATIONDependencyNode* createBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CBINDPROPAGATEANDDependencyNode* createBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CPROPAGATEBINDINGDependencyNode* createPROPAGATEBINDINGDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CBINDVARIABLEDependencyNode* createBINDVARIABLEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CANDDependencyNode* createANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CSOMEDependencyNode* createSOMEDependency(CDependencyTrackPoint*& someContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CSELFDependencyNode* createSELFDependency(CDependencyTrackPoint*& selfContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CVALUEDependencyNode* createVALUEDependency(CDependencyTrackPoint*& valueContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CALLDependencyNode* createALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CATLEASTDependencyNode* createATLEASTDependency(CDependencyTrackPoint*& atleastContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CAUTOMATTRANSACTIONDependencyNode* createAUTOMATTRANSACTIONDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CAUTOMATCHOOSEDependencyNode* createAUTOMATCHOOSEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CDISTINCTDependencyNode* createDISTINCTDependency(CDependencyTrackPoint*& distinctContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CNEGVALUEDependencyNode* createNEGVALUEDependency(CDependencyTrackPoint*& negValueContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CNOMINALDependencyNode* createNOMINALDependency(CDependencyTrackPoint*& nominalContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CFUNCTIONALDependencyNode* createFUNCTIONALDependency(CDependencyTrackPoint*& functionalContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* prevLink1DependencyTrackPoint, CDependencyTrackPoint* prevLink2DependencyTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CORDependencyNode* createORDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CATMOSTDependencyNode* createATMOSTDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CMERGEDependencyNode* createMERGEDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CREUSEINDIVIDUALDependencyNode* createREUSEINDIVIDUALDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CREUSECOMPLETIONGRAPHDependencyNode* createREUSECOMPLETIONGRAPHDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CREUSECONCEPTSDependencyNode* createREUSECONCEPTSDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CQUALIFYDependencyNode* createQUALIFYDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CMERGEDCONCEPTDependencyNode* createMERGEDCONCEPTDependency(CDependencyTrackPoint*& mergedConceptContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* conceptPrevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CMERGEDLINKDependencyNode* createMERGEDLINKDependency(CDependencyTrackPoint*& mergedLinkContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* linkPrevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CIMPLICATIONDependencyNode* createIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CEXPANDEDDependencyNode* createEXPANDEDDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CCONNECTIONDependencyNode* createCONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CDATATYPETRIGGERDependencyNode* createDATATYPETRIGGERDependency(CDependencyTrackPoint*& triggerContinueDepTrackPoint, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CDATATYPECONNECTIONDependencyNode* createDATATYPECONNECTIONDependency(CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);



					// protected methods
					protected:

					// protected variables
					protected:
						bool mConfBuildDependencies;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CDEPENDENCYFACTORY_H
