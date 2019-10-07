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

#ifndef KONCLUDE_UTILITIES_CDYNAMICEXPANDINGMEMORYMANAGER_H
#define KONCLUDE_UTILITIES_CDYNAMICEXPANDINGMEMORYMANAGER_H

// Libraries includes
#include <QLinkedList>


// Namespace includes
#include "CAllocationObject.h"
#include "UtilitiesSettings.h"
#include "CMemoryReleaser.h"
#include "CMemoryManager.hpp"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CDynamicExpandingMemoryManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CDynamicExpandingMemoryManager : public CMemoryManager<T> {
			// public methods
			public:
				//! Constructor
				CDynamicExpandingMemoryManager(qreal expansionFactor = 10., qint64 beginExtensionSize = 8, qint64 maxExtensionSize = 2040, qint64 beginAllocationSize = 0);

				//! Destructor
				virtual ~CDynamicExpandingMemoryManager();

				virtual T *allocate();
				virtual void release(T *&mem);
				virtual void release(CAllocationObject *object);

				void clear();

			// protected methods
			protected:

			// private methods
			private:

				void extendMemory();

			// private variables
			private:
				qint64 totalElements;
				qint64 freeElements;

				qint64 nextMemVecIndex;
				qint64 releaseMemVecIndex;

				qint64 vecSize;

				qreal extendFac;
				qint64 minExtSize;
				qint64 maxExtSize;

				qint64 currExtSizeInt;
				qreal currExtSizeReal;

				QList<T *> memory;
				QList<T **> free;
				QList<T **> memoryPtr;
				QList<T **> freeMemPtr;

				T **releaseMemVec;
				T **nextMemVec;

				bool hasAllocationContextChecked;
				bool hasAllocationContext;
		};


		template<class T>
		CDynamicExpandingMemoryManager<T>::CDynamicExpandingMemoryManager(qreal expansionFactor, qint64 beginExtensionSize, qint64 maxExtensionSize, qint64 beginAllocationSize) {
			vecSize = maxExtensionSize;

			extendFac = expansionFactor;
			minExtSize = beginExtensionSize;
			maxExtSize = maxExtensionSize;

			currExtSizeInt = minExtSize;
			currExtSizeReal = minExtSize;

			releaseMemVec = 0;
			nextMemVec = 0;
			nextMemVecIndex = 0;
			releaseMemVecIndex = 0;

			freeElements = 0;
			totalElements = 0;

			hasAllocationContextChecked = false;
			hasAllocationContext = false;

			for (qint64 i = 0; i < beginAllocationSize; i += vecSize) {
				extendMemory();
			}

		}

		template<class T>
		CDynamicExpandingMemoryManager<T>::~CDynamicExpandingMemoryManager() {
			clear();
		}

		template<class T>
		void CDynamicExpandingMemoryManager<T>::clear() {
			releaseMemVec = 0;
			nextMemVec = 0;
			nextMemVecIndex = 0;
			releaseMemVecIndex = 0;

			freeElements = 0;
			totalElements = 0;

			foreach (T *mem, memory) {
				delete [] mem;
			}
			foreach (T **memPtr, memoryPtr) {
				delete [] memPtr;
			}


			memory.clear();
			free.clear();
			memoryPtr.clear();
			freeMemPtr.clear();
		}

		template<class T>
		void CDynamicExpandingMemoryManager<T>::extendMemory() {
			T *newMem = new T[currExtSizeInt];

			if (!hasAllocationContextChecked) {
				CAllocationObject *allCon = dynamic_cast<CAllocationObject *>(&newMem[0]);
				if (allCon == 0) {
					hasAllocationContext = false;
				} else {
					hasAllocationContext = true;
				}
				hasAllocationContextChecked = true;
			}

			freeElements += currExtSizeInt;
			totalElements += currExtSizeInt;
			memory.append(newMem);
			T **newMemPtr = new T *[vecSize];
			memoryPtr.append(newMemPtr);
			for (int i = 0; i < currExtSizeInt; ++i) {
				newMemPtr[i] = &(newMem[i]);
			}
			for (int i = currExtSizeInt; i < vecSize; ++i) {
				newMemPtr[i] = 0;
			}
			free.append(newMemPtr);

			currExtSizeReal *= extendFac;
			if (currExtSizeReal > maxExtSize) {
				currExtSizeReal = maxExtSize;
			}
			currExtSizeInt = (qint64)currExtSizeReal;
		}


		template<class T>
		T *CDynamicExpandingMemoryManager<T>::allocate() {

			T *memElement = 0;
			while (!memElement) {
				if (nextMemVec) {
					memElement = nextMemVec[nextMemVecIndex++];
					if (nextMemVecIndex >= vecSize) {
						freeMemPtr.append(nextMemVec);
						nextMemVec = 0;
					}
				}
				if (memElement == 0) {
					if (free.isEmpty()) {
						extendMemory();
					}
					nextMemVec = free.takeFirst();
					nextMemVecIndex = 0;
				}
			}

			freeElements--;
			if (hasAllocationContext) {
				CAllocationObject *allCon = (CAllocationObject *)(memElement);
				allCon->setMemoryReleaser(this);
			}
			return memElement;
		}


		template<class T>
		void CDynamicExpandingMemoryManager<T>::release(T *&mem) {

			if (releaseMemVec == 0) {
				if (!freeMemPtr.isEmpty()) {
					releaseMemVec = freeMemPtr.takeFirst();
				} else {
					releaseMemVec = new T *[vecSize];
					memoryPtr.append(releaseMemVec);
				}
			}

			freeElements++;
			releaseMemVec[releaseMemVecIndex++] = mem;
			mem = 0;

			if (releaseMemVecIndex >= vecSize) {
				releaseMemVecIndex = 0;
				if (releaseMemVec != 0) {
					free.append(releaseMemVec);
					releaseMemVec = 0;
				}
			}
		}

		template<class T>
		void CDynamicExpandingMemoryManager<T>::release(CAllocationObject *object) {
			T *mem = (T*)object;
			release(mem);
		}

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CDYNAMICEXPANDINGMEMORYMANAGER_H
