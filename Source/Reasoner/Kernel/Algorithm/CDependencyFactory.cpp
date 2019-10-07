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

#include "CDependencyFactory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CDependencyFactory::CDependencyFactory() {
					mConfBuildDependencies = true;
				}

				CREPRESENTATIVEGROUNDINGDependencyNode* CDependencyFactory::createREPRESENTATIVEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CVariableBindingPath* selectVarBindPath, CCalculationAlgorithmContext* calcAlgContext) {
					CREPRESENTATIVEGROUNDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREPRESENTATIVEGROUNDINGDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREPRESENTATIVEGROUNDINGDependencyNode(conDes,prevDepTrackPoint,selectVarBindPath);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CREPRESENTATIVEJOINDependencyNode* CDependencyFactory::createREPRESENTATIVEJOINDependency(CDependencyTrackPoint*& joinContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREPRESENTATIVEJOINDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREPRESENTATIVEJOINDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREPRESENTATIVEJOINDependencyNode(conDes,prevDepTrackPoint,otherDepTrackPoint);
						joinContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CREPRESENTATIVEBINDVARIABLEDependencyNode* CDependencyFactory::createREPRESENTATIVEBINDVARIABLEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREPRESENTATIVEBINDVARIABLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREPRESENTATIVEBINDVARIABLEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREPRESENTATIVEBINDVARIABLEDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CREPRESENTATIVEIMPLICATIONDependencyNode* CDependencyFactory::createREPRESENTATIVEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CREPRESENTATIVEIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREPRESENTATIVEIMPLICATIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREPRESENTATIVEIMPLICATIONDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CREPRESENTATIVEALLDependencyNode* CDependencyFactory::createREPRESENTATIVEALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREPRESENTATIVEALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREPRESENTATIVEALLDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREPRESENTATIVEALLDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CREPRESENTATIVEANDDependencyNode* CDependencyFactory::createREPRESENTATIVEANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREPRESENTATIVEANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREPRESENTATIVEANDDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREPRESENTATIVEANDDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CRESOLVEREPRESENTATIVEDependencyNode* CDependencyFactory::createRESOLVEREPRESENTATIVEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CRepresentativeVariableBindingPathMap* resolveVarBindPathMap, CRepresentativePropagationMap* resolveRepPropMap, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* additionalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CRESOLVEREPRESENTATIVEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CRESOLVEREPRESENTATIVEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initRESOLVEREPRESENTATIVEDependencyNode(conDes,resolveVarBindPathMap,resolveRepPropMap,prevDepTrackPoint,additionalDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}








				CVARBINDPROPAGATEIMPLICATIONDependencyNode* CDependencyFactory::createVARBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CVARBINDPROPAGATEIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVARBINDPROPAGATEIMPLICATIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVARBINDPROPAGATEIMPLICATIONDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CPROPAGATEVARIABLECONNECTIONDependencyNode* CDependencyFactory::createPROPAGATEVARIABLECONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATEVARIABLECONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATEVARIABLECONNECTIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATEVARIABLECONNECTIONDependencyNode(conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CVARBINDPROPAGATEGROUNDINGDependencyNode* CDependencyFactory::createVARBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CVARBINDPROPAGATEGROUNDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVARBINDPROPAGATEGROUNDINGDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVARBINDPROPAGATEGROUNDINGDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CVARBINDPROPAGATEANDDependencyNode* CDependencyFactory::createVARBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CVARBINDPROPAGATEANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVARBINDPROPAGATEANDDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVARBINDPROPAGATEANDDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CPROPAGATEVARIABLEBINDINGDependencyNode* CDependencyFactory::createPROPAGATEVARIABLEBINDINGDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATEVARIABLEBINDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATEVARIABLEBINDINGDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATEVARIABLEBINDINGDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CVARBINDPROPAGATEALLDependencyNode* CDependencyFactory::createVARBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CVARBINDPROPAGATEALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVARBINDPROPAGATEALLDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVARBINDPROPAGATEALLDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* CDependencyFactory::createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CVARBINDVARIABLEDependencyNode* CDependencyFactory::createVARBINDVARIABLEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CVARBINDVARIABLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVARBINDVARIABLEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVARBINDVARIABLEDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CVARBINDPROPAGATEJOINDependencyNode* CDependencyFactory::createVARBINDPROPAGATEJOINDependency(CDependencyTrackPoint*& mergeDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CVARBINDPROPAGATEJOINDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVARBINDPROPAGATEJOINDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVARBINDPROPAGATEJOINDependencyNode(conDes,prevDepTrackPoint,otherDepTrackPoint);
						mergeDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}









				CBINDPROPAGATEGROUNDINGDependencyNode* CDependencyFactory::createBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CBINDPROPAGATEGROUNDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CBINDPROPAGATEGROUNDINGDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initBINDPROPAGATEGROUNDINGDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CPROPAGATECONNECTIONAWAYDependencyNode* CDependencyFactory::createPROPAGATECONNECTIONAWAYDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATECONNECTIONAWAYDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATECONNECTIONAWAYDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATECONNECTIONAWAYDependencyNode(conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CPROPAGATECONNECTIONDependencyNode* CDependencyFactory::createPROPAGATECONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATECONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATECONNECTIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATECONNECTIONDependencyNode(conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CBINDPROPAGATECYCLEDependencyNode* CDependencyFactory::createBINDPROPAGATECYCLEDependency(CDependencyTrackPoint*& depTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* triggDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CBINDPROPAGATECYCLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CBINDPROPAGATECYCLEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initBINDPROPAGATECYCLEDependencyNode(conDes,prevDepTrackPoint,triggDepTrackPoint);
						depTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CBINDPROPAGATEALLDependencyNode* CDependencyFactory::createBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CBINDPROPAGATEALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CBINDPROPAGATEALLDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initBINDPROPAGATEALLDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CPROPAGATEBINDINGSSUCCESSORDependencyNode* CDependencyFactory::createPROPAGATEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATEBINDINGSSUCCESSORDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATEBINDINGSSUCCESSORDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATEBINDINGSSUCCESSORDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CBINDPROPAGATEIMPLICATIONDependencyNode* CDependencyFactory::createBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CBINDPROPAGATEIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CBINDPROPAGATEIMPLICATIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initBINDPROPAGATEIMPLICATIONDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}


				CBINDPROPAGATEANDDependencyNode* CDependencyFactory::createBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CBINDPROPAGATEANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CBINDPROPAGATEANDDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initBINDPROPAGATEANDDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}


				CPROPAGATEBINDINGDependencyNode* CDependencyFactory::createPROPAGATEBINDINGDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CPROPAGATEBINDINGDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CPROPAGATEBINDINGDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initPROPAGATEBINDINGDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CBINDVARIABLEDependencyNode* CDependencyFactory::createBINDVARIABLEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CBINDVARIABLEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CBINDVARIABLEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initBINDVARIABLEDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CANDDependencyNode* CDependencyFactory::createANDDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CANDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CANDDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initANDDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CNOMINALDependencyNode* CDependencyFactory::createNOMINALDependency(CDependencyTrackPoint*& nominalContDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CNOMINALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CNOMINALDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initNOMINALDependencyNode(conDes,processIndi,prevDepTrackPoint,nominalDepTrackPoint);
						nominalContDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CAUTOMATCHOOSEDependencyNode* CDependencyFactory::createAUTOMATCHOOSEDependency(CDependencyTrackPoint*& andDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CAUTOMATCHOOSEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CAUTOMATCHOOSEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initAUTOMATCHOOSEDependencyNode(conDes,prevDepTrackPoint);
						andDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CSOMEDependencyNode* CDependencyFactory::createSOMEDependency(CDependencyTrackPoint*& someDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CSOMEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CSOMEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initSOMEDependencyNode(conDes,processIndi,prevDepTrackPoint);
						someDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CSELFDependencyNode* CDependencyFactory::createSELFDependency(CDependencyTrackPoint*& someDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CSELFDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CSELFDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initSELFDependencyNode(conDes,processIndi,prevDepTrackPoint);
						someDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CVALUEDependencyNode* CDependencyFactory::createVALUEDependency(CDependencyTrackPoint*& valueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CVALUEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CVALUEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initVALUEDependencyNode(conDes,processIndi,prevDepTrackPoint,nominalDepTrackPoint);
						valueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CROLEASSERTIONDependencyNode* CDependencyFactory::createROLEASSERTIONDependency(CDependencyTrackPoint*& roleAssDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CRole* baseAssertionRole, CIndividual* baseAssertionIndi, CCalculationAlgorithmContext* calcAlgContext) {
					CROLEASSERTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CROLEASSERTIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initROLEASSERTIONDependencyNode(processIndi,prevDepTrackPoint,nominalDepTrackPoint, baseAssertionRole, baseAssertionIndi);
						roleAssDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CDATAASSERTIONDependencyNode* CDependencyFactory::createDATAASSERTIONDependency(CDependencyTrackPoint*& roleAssDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDATAASSERTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CDATAASSERTIONDependencyNode, CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(), calcAlgContext->getUsedProcessContext());
						depNode->initDATAASSERTIONDependencyNode(processIndi, prevDepTrackPoint);
						roleAssDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CNEGVALUEDependencyNode* CDependencyFactory::createNEGVALUEDependency(CDependencyTrackPoint*& negValueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CNEGVALUEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CNEGVALUEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initNEGVALUEDependencyNode(conDes,processIndi,prevDepTrackPoint,nominalDepTrackPoint);
						negValueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CALLDependencyNode* CDependencyFactory::createALLDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CALLDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CALLDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initALLDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CMERGEDCONCEPTDependencyNode* CDependencyFactory::createMERGEDCONCEPTDependency(CDependencyTrackPoint*& mergedConceptContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* conceptPrevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CMERGEDCONCEPTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CMERGEDCONCEPTDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initMERGEDCONCEPTDependencyNode(conDes,mergePrevDepTrackPoint,conceptPrevDepTrackPoint);
						mergedConceptContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CMERGEDLINKDependencyNode* CDependencyFactory::createMERGEDLINKDependency(CDependencyTrackPoint*& mergedLinkContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* linkPrevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CMERGEDLINKDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CMERGEDLINKDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initMERGEDLINKDependencyNode(nullptr,processIndi,mergePrevDepTrackPoint,linkPrevDepTrackPoint);
						mergedLinkContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CFUNCTIONALDependencyNode* CDependencyFactory::createFUNCTIONALDependency(CDependencyTrackPoint*& functionalContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* prevLink1DependencyTrackPoint, CDependencyTrackPoint* prevLink2DependencyTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CFUNCTIONALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CFUNCTIONALDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initFUNCTIONALDependencyNode(conDes,processIndi,prevDepTrackPoint,prevLink1DependencyTrackPoint,prevLink2DependencyTrackPoint);
						functionalContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CDISTINCTDependencyNode* CDependencyFactory::createDISTINCTDependency(CDependencyTrackPoint*& distinctDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDISTINCTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CDISTINCTDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initDISTINCTDependencyNode(conDes,processIndi,prevDepTrackPoint);
						distinctDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CAUTOMATTRANSACTIONDependencyNode* CDependencyFactory::createAUTOMATTRANSACTIONDependency(CDependencyTrackPoint*& allDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CAUTOMATTRANSACTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CAUTOMATTRANSACTIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initAUTOMATTRANSACTIONDependencyNode(conDes,processIndi,prevDepTrackPoint,linkDepTrackPoint);
						allDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CATLEASTDependencyNode* CDependencyFactory::createATLEASTDependency(CDependencyTrackPoint*& atleastDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CATLEASTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CATLEASTDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initATLEASTDependencyNode(conDes,processIndi,prevDepTrackPoint);
						atleastDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CORDependencyNode* CDependencyFactory::createORDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CORDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CORDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initORDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,prevDepTrackPoint);
					}
					return depNode;
				}

				CATMOSTDependencyNode* CDependencyFactory::createATMOSTDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CATMOSTDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CATMOSTDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initATMOSTDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CMERGEDependencyNode* CDependencyFactory::createMERGEDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CMERGEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CMERGEDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initMERGEDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CREUSEINDIVIDUALDependencyNode* CDependencyFactory::createREUSEINDIVIDUALDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREUSEINDIVIDUALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREUSEINDIVIDUALDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREUSEINDIVIDUALDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CREUSECOMPLETIONGRAPHDependencyNode* CDependencyFactory::createREUSECOMPLETIONGRAPHDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREUSECOMPLETIONGRAPHDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREUSECOMPLETIONGRAPHDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREUSECOMPLETIONGRAPHDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode* CDependencyFactory::createMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode(CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CIndividualProcessNode* mergingIndi, CCalculationAlgorithmContext* calcAlgContext) {
					CMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode, CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(), calcAlgContext->getUsedProcessContext());
						depNode->initMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode(calcAlgContext->getUsedBranchTreeNode(), processIndi, mergingIndi, prevDepTrackPoint);
					}
					return depNode;
				}

				CREUSECONCEPTSDependencyNode* CDependencyFactory::createREUSECONCEPTSDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CREUSECONCEPTSDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CREUSECONCEPTSDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initREUSECONCEPTSDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}

				CQUALIFYDependencyNode* CDependencyFactory::createQUALIFYDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CQUALIFYDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CQUALIFYDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initQUALIFYDependencyNode(calcAlgContext->getUsedBranchTreeNode(),conDes,processIndi,prevDepTrackPoint);
					}
					return depNode;
				}


				CIMPLICATIONDependencyNode* CDependencyFactory::createIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CIMPLICATIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CIMPLICATIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initIMPLICATIONDependencyNode(conDes,prevDepTrackPoint,prevOtherDependencies);
						implContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

				CORONLYOPTIONDependencyNode* CDependencyFactory::createORONLYOPTIONDependency(CDependencyTrackPoint*& orContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CORONLYOPTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CORONLYOPTIONDependencyNode, CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(), calcAlgContext->getUsedProcessContext());
						depNode->initORONLYOPTIONDependencyNode(conDes, prevDepTrackPoint, prevOtherDependencies);
						orContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}


				CEXPANDEDDependencyNode* CDependencyFactory::createEXPANDEDDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CEXPANDEDDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CEXPANDEDDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initEXPANDEDDependencyNode(prevDepTrackPoint,prevOtherDependencies);
						expContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}


				CCONNECTIONDependencyNode* CDependencyFactory::createCONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CCONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CCONNECTIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initCONNECTIONDependencyNode(conDes,prevDepTrackPoint);
					}
					return depNode;
				}


				CDATATYPETRIGGERDependencyNode* CDependencyFactory::createDATATYPETRIGGERDependency(CDependencyTrackPoint*& triggerContinueDepTrackPoint, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CDATATYPETRIGGERDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CDATATYPETRIGGERDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initDATATYPETRIGGERDependencyNode(prevDepTrackPoint,prevOtherDependencies);
						triggerContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}


				CDATATYPECONNECTIONDependencyNode* CDependencyFactory::createDATATYPECONNECTIONDependency(CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDATATYPECONNECTIONDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CDATATYPECONNECTIONDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initDATATYPECONNECTIONDependencyNode(prevDepTrackPoint);
					}
					return depNode;
				}


				CMERGEDIndividualDependencyNode* CDependencyFactory::createMERGEDINDIVIDUALDependency(CDependencyTrackPoint*& mergedIndividualContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* individualPrevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CMERGEDIndividualDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CMERGEDIndividualDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						depNode->initMERGEDIndividualDependencyNode(processIndi,mergePrevDepTrackPoint,individualPrevDepTrackPoint);
						mergedIndividualContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}




				CSAMEINDIVIDUALSMERGEDependencyNode* CDependencyFactory::createSAMEINDIVIDUALMERGEDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* prevOtherDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CSAMEINDIVIDUALSMERGEDependencyNode* depNode = nullptr;
					if (mConfBuildDependencies) {
						depNode = CObjectParameterizingAllocator< CSAMEINDIVIDUALSMERGEDependencyNode, CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(), calcAlgContext->getUsedProcessContext());
						depNode->initSAMEINDIVIDUALSMERGEDependencyNode(processIndi, prevDepTrackPoint, prevOtherDepTrackPoint);
						expContinueDepTrackPoint = depNode->getContinueDependencyTrackPoint();
					}
					return depNode;
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
