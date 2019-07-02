/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_UTILITIES_CSTATICEXPANDINGMEMORYMANAGER_H
#define KONCLUDE_UTILITIES_CSTATICEXPANDINGMEMORYMANAGER_H

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
		 *		\class		CStaticExpandingMemoryManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CStaticExpandingMemoryManager : public CMemoryManager<T> {
			// public methods
			public:
				//! Constructor
				CStaticExpandingMemoryManager(qint64 extensionSize = 100, qint64 beginAllocationSize = 0);

				//! Destructor
				virtual ~CStaticExpandingMemoryManager();

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

				QLinkedList<T *> memory;
				QLinkedList<T **> free;
				QLinkedList<T **> memoryPtr;
				QLinkedList<T **> freeMemPtr;

				T **releaseMemVec;
				T **nextMemVec;

				bool hasAllocationContextChecked;
				bool hasAllocationContext;
		};


		template<class T>
		CStaticExpandingMemoryManager<T>::CStaticExpandingMemoryManager(qint64 extensionSize, qint64 beginAllocationSize) {
			vecSize = extensionSize;

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
		CStaticExpandingMemoryManager<T>::~CStaticExpandingMemoryManager() {
			clear();
		}

		template<class T>
		void CStaticExpandingMemoryManager<T>::clear() {
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
		void CStaticExpandingMemoryManager<T>::extendMemory() {
			T *newMem = new T[vecSize];

			if (!hasAllocationContextChecked) {
				CAllocationObject *allCon = dynamic_cast<CAllocationObject *>(&newMem[0]);
				if (allCon == 0) {
					hasAllocationContext = false;
				} else {
					hasAllocationContext = true;
				}
				hasAllocationContextChecked = true;
			}

			freeElements += vecSize;
			totalElements += vecSize;
			memory.append(newMem);
			T **newMemPtr = new T *[vecSize];
			memoryPtr.append(newMemPtr);
			for (int i = 0; i < vecSize; ++i) {
				newMemPtr[i] = &(newMem[i]);
			}
			free.append(newMemPtr);
		}


		template<class T>
		T *CStaticExpandingMemoryManager<T>::allocate() {
			if (nextMemVec == 0) {
				if (free.count() <= 0) {
					extendMemory();
				}
				nextMemVec = free.first();
				free.removeFirst();
				nextMemVecIndex = 0;
			}

			T *memElement = nextMemVec[nextMemVecIndex++];
			if (nextMemVecIndex >= vecSize) {
				freeMemPtr.append(nextMemVec);
				nextMemVec = 0;
			}

			freeElements--;
			if (hasAllocationContext) {
				CAllocationObject *allCon = (CAllocationObject *)(memElement);
				allCon->setContainered();
				allCon->setMemoryReleaser(this);
			}
			return memElement;
		}


		template<class T>
		void CStaticExpandingMemoryManager<T>::release(T *&mem) {

			if (releaseMemVec == 0) {
				if (freeMemPtr.count() > 0) {
					releaseMemVec = freeMemPtr.first();
					freeMemPtr.removeFirst();
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
		void CStaticExpandingMemoryManager<T>::release(CAllocationObject *object) {
			T *mem = (T*)object;
			release(mem);
		}

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CSTATICEXPANDINGMEMORYMANAGER_H
