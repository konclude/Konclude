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
 *		Note, this is a modified version of the QMap from the Qt framework.
 *
 */

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAPDATA_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAPDATA_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationMapBase.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER

// Library includes
#include <QMap>

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
			 *		\class		CQtManagedRestrictedModificationMapData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			struct CQtManagedRestrictedModificationMapData
			{
				struct Node {
					Node *backward;
					Node *forward[1];
				};
				enum { LastLevel = 11, Sparseness = 3 };

				CQtManagedRestrictedModificationMapData *backward;
				CQtManagedRestrictedModificationMapData *forward[CQtManagedRestrictedModificationMapData::LastLevel + 1];
				//QBasicAtomicInt ref;
				CQtManagedRestrictedModificationMapBase *refMap;
				int topLevel;
				int size;
				uint randomBits;
				uint insertInOrder : 1;
				uint sharable : 1;
				uint strictAlignment : 1;
				uint reserved : 29;

				static CQtManagedRestrictedModificationMapData *createData(CQtManagedRestrictedModificationMapBase* referenceMap, CMemoryAllocationManager* memMan); // ### Qt5 remove me
				static CQtManagedRestrictedModificationMapData *createData(int alignment, CQtManagedRestrictedModificationMapBase* referenceMap, CMemoryAllocationManager* memMan);
				void continueFreeData(int offset, CMemoryAllocationManager* memMan);
				Node *node_create(Node *update[], int offset, CMemoryAllocationManager* memMan); // ### Qt5 remove me
				Node *node_create(Node *update[], int offset, int alignment, CMemoryAllocationManager* memMan);
				void node_delete(Node *update[], int offset, Node *node, CMemoryAllocationManager* memMan);
#ifdef KONCLUDE_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_DEBUG
				uint adjust_ptr(Node *node);
				void dump();
#endif

				static CQtManagedRestrictedModificationMapData shared_null;
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONMAPDATA_H
