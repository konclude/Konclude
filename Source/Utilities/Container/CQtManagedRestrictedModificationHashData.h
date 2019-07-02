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
 *		Note, this is a modified version of the QHash from the Qt framework.
 *
 */

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASHDATA_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASHDATA_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationHashBase.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER

// Library includes
#include <QHash>

// Other includes
#include "Utilities/Memory/CMemoryAllocationManager.h"
#include "Utilities/Memory/CObjectAllocator.h"
#include "Utilities/Memory/CMemoryAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		using namespace Memory;

		namespace Container {


			/*! 
			 *
			 *		\class		CQtManagedRestrictedModificationHashData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */

			struct CQtManagedRestrictedModificationHashData
			{
				struct Node {
					Node *next;
					uint h;
				};

				Node *fakeNext;
				Node **buckets;
				//QBasicAtomicInt ref;
				CQtManagedRestrictedModificationHashBase *refHash;
				int size;
				int nodeSize;
				short userNumBits;
				short numBits;
				int numBuckets;
				uint sharable : 1;
				uint strictAlignment : 1;
				uint reserved : 30;

				void *allocateNode(CMemoryAllocationManager* memMan); // ### Qt5 remove me
				void *allocateNode(int nodeAlign, CMemoryAllocationManager* memMan);
				void freeNode(void *node, CMemoryAllocationManager* memMan);
				CQtManagedRestrictedModificationHashData *detach_helper(void (*node_duplicate)(Node *, void *), int nodeSize); // ### Qt5 remove me
				CQtManagedRestrictedModificationHashData *detach_helper2(void (*node_duplicate)(Node *, void *), void (*node_delete)(Node *),
					int nodeSize, int nodeAlign, CQtManagedRestrictedModificationHashBase* referencedHash, CMemoryAllocationManager* memMan);
				CQtManagedRestrictedModificationHashData *restricted_detach_helper2(bool (*copy_restricted)(Node *, void *), void* assistValue, void (*node_duplicate)(Node *, void *), void (*node_delete)(Node *),
					int nodeSize, int nodeAlign, CQtManagedRestrictedModificationHashBase* referencedHash, CMemoryAllocationManager* memMan);
				void mightGrow(CMemoryAllocationManager* memMan);
				bool willGrow(CMemoryAllocationManager* memMan) {
					if (size >= numBuckets) {
						rehash(numBits + 1,memMan);
						return true;
					} else {
						return false;
					}
				}
				void hasShrunk(CMemoryAllocationManager* memMan);
				void rehash(int hint, CMemoryAllocationManager* memMan);
				void free_helper(void (*node_delete)(Node *), CMemoryAllocationManager* memMan);
				void destroyAndFree(CMemoryAllocationManager* memMan); // ### Qt5 remove me
				Node *firstNode();
#ifdef KONCLUDE_CQTMANAGEDRESTRICTEDMODIFICATIONHASH_DEBUG
				void dump();
				void checkSanity();
#endif
				static Node *nextNode(Node *node);
				static Node *previousNode(Node *node);

				static CQtManagedRestrictedModificationHashData shared_null;
			};



		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONHASHDATA_H
