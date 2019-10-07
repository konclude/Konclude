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

#include "CMarkerIndividualNodeHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CMarkerIndividualNodeHash::CMarkerIndividualNodeHash(CProcessContext* context) : mContext(context) {
				}


				CMarkerIndividualNodeHash* CMarkerIndividualNodeHash::initMarkerIndividualNodeHash(CMarkerIndividualNodeHash* prevMarkerIndiNodeHash) {
					if (prevMarkerIndiNodeHash) {
						CPROCESSHASH< CConcept*, CMarkerIndividualNodeData >::operator=(*prevMarkerIndiNodeHash);
					} else {
						CPROCESSHASH< CConcept*, CMarkerIndividualNodeData >::clear();
					}
					return this;
				}

				CXNegLinker<CIndividualProcessNode*>* CMarkerIndividualNodeHash::getMarkerIndividualNodeLinker(CConcept* markerConcept) {
					CMarkerIndividualNodeData data = value(markerConcept);
					return data.getMarkerIndividualNodeLinker();
				}

				CMarkerIndividualNodeHash* CMarkerIndividualNodeHash::addMarkerIndividualNodeLinker(CConcept* markerConcept, CXNegLinker<CIndividualProcessNode*>* indiNodeLinker) {
					CMarkerIndividualNodeData& data = this->operator [](markerConcept);
					CPROCESSSET< QPair<cint64, bool> >* indiNondetSet = data.getIndividualNondeterminismSet(false);
					cint64 indiId = indiNodeLinker->getData()->getIndividualNodeID();
					bool nonDet = indiNodeLinker->isNegated();
					if (!indiNondetSet || !indiNondetSet->contains(QPair<cint64, bool>(indiId, nonDet))) {						
						CPROCESSSET< QPair<cint64, bool> >* localNondetSet = data.getIndividualNondeterminismSet(true);
						if (!localNondetSet) {
							localNondetSet = CObjectParameterizingAllocator< CPROCESSSET< QPair<cint64, bool> >, CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(), mContext);
							if (indiNondetSet) {
								*localNondetSet = *indiNondetSet;
							}
							data.setIndividualNondeterminismSet(true, localNondetSet);
							data.setIndividualNondeterminismSet(false, localNondetSet);
						}
						localNondetSet->insert(QPair<cint64, bool>(indiId, nonDet));
						data.addMarkerIndividualNodeLinker(indiNodeLinker);
					}
					return this;
				}

				CMarkerIndividualNodeHash* CMarkerIndividualNodeHash::addMarkerIndividualNode(CConcept* markerConcept, CIndividualProcessNode* indiNode, bool nondeterministicallyAdded) {
					CXNegLinker<CIndividualProcessNode*>* indiNodeLinker = CObjectAllocator< CXNegLinker<CIndividualProcessNode*> >::allocateAndConstruct(mContext->getMemoryAllocationManager());
					indiNodeLinker->initNegLinker(indiNode,nondeterministicallyAdded);
					addMarkerIndividualNodeLinker(markerConcept,indiNodeLinker);
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
