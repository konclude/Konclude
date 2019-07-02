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

#include "CDatatypeDependencyTrackPointCollection.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeDependencyTrackPointCollection::CDatatypeDependencyTrackPointCollection(CProcessContext* processContext) : mDepTrackPointSet(processContext) {
					mProcessContext = processContext;
					mClashDepLinker = nullptr;
				}


				CDatatypeDependencyTrackPointCollection* CDatatypeDependencyTrackPointCollection::initDependencyTrackPointCollection(CDatatypeDependencyTrackPointCollection* collection) {
					if (collection) {
						mClashDepLinker = collection->mClashDepLinker;
						mDepTrackPointSet = collection->mDepTrackPointSet;
					} else {
						mClashDepLinker = nullptr;
						mDepTrackPointSet.clear();
					}
					return this;
				}



				CClashedDependencyDescriptor* CDatatypeDependencyTrackPointCollection::getClashedDependencyDescriptors() {
					return mClashDepLinker;
				}


				CClashedDependencyDescriptor* CDatatypeDependencyTrackPointCollection::createClashedDependencyDescriptor(CClashedDependencyDescriptor* appendDepDesLinker, CDependencyTrackPoint* depTrackPoint) {
					if (depTrackPoint) {
						CClashedDependencyDescriptor* clashDes = CObjectAllocator<CClashedDependencyDescriptor>::allocateAndConstruct(mProcessContext->getUsedMemoryAllocationManager());
						clashDes->initClashedDependencyDescriptor(depTrackPoint);
						clashDes->append(appendDepDesLinker);
						return clashDes;
					} else {
						return appendDepDesLinker;
					}
				}


				bool CDatatypeDependencyTrackPointCollection::addDependency(CDependencyTrackPoint* depTrackPoint) {
					if (!mDepTrackPointSet.contains(depTrackPoint)) {
						mDepTrackPointSet.insert(depTrackPoint);
						mClashDepLinker = createClashedDependencyDescriptor(mClashDepLinker,depTrackPoint);
						return true;
					}
					return false;
				}

				bool CDatatypeDependencyTrackPointCollection::addCollectionDependencies(CDatatypeValueSpaceDependencyCollector* depCollection) {
					bool depAdded = false;
					for (CClashedDependencyDescriptor* clashDesIt = mClashDepLinker; clashDesIt; clashDesIt = clashDesIt->getNext()) {
						depAdded |= depCollection->addDependency(clashDesIt->getDependencyTrackPoint());
					}
					return depAdded;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
