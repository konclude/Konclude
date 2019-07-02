/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CClashDescriptorFactory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CClashDescriptorFactory::CClashDescriptorFactory() {
				}


				CClashDescriptorFactory::~CClashDescriptorFactory() {
				}


				CClashedDependencyDescriptor* CClashDescriptorFactory::createClashedConceptDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedConceptDescriptor* clashDes = CObjectAllocator<CClashedConceptDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					clashDes->initClashedConceptDescriptor(conDes,prevDepTrackPoint,processIndi);
					if (prevClashes) {
						clashDes->append(prevClashes);
					}
					return clashDes;
				}

				CClashedDependencyDescriptor* CClashDescriptorFactory::createClashedIndividualLinkDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualLinkEdge* link, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedIndividualLinkDescriptor* clashDes = CObjectAllocator<CClashedIndividualLinkDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					clashDes->initClashedLinkDescriptor(link,prevDepTrackPoint);
					if (prevClashes) {
						clashDes->append(prevClashes);
					}
					return clashDes;
				}

				CClashedDependencyDescriptor* CClashDescriptorFactory::createClashedIndividualDistinctDescriptor(CClashedDependencyDescriptor* prevClashes, CDistinctEdge* distinct, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedIndividualDistinctDescriptor* clashDes = CObjectAllocator<CClashedIndividualDistinctDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					clashDes->initClashedDistinctDescriptor(distinct,prevDepTrackPoint);
					if (prevClashes) {
						clashDes->append(prevClashes);
					}
					return clashDes;
				}

				CClashedDependencyDescriptor* CClashDescriptorFactory::createClashedNegationDisjointDescriptor(CClashedDependencyDescriptor* prevClashes, CNegationDisjointEdge* disjointNegLink, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedNegationDisjointLinkDescriptor* clashDes = CObjectAllocator<CClashedNegationDisjointLinkDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					clashDes->initClashedLinkDescriptor(disjointNegLink,prevDepTrackPoint);
					if (prevClashes) {
						clashDes->append(prevClashes);
					}
					return clashDes;
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
