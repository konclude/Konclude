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
 * 		This file incorporates work covered by the following copyright and  
 * 		permission notice:   
 *
 *
 * 			Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 * 			Contact: http://www.qt-project.org/legal
 *
 * 			This file is part of the QtCore module of the Qt Toolkit.
 *
 * 			$QT_BEGIN_LICENSE:LGPL$
 * 			Commercial License Usage
 * 			Licensees holding valid commercial Qt licenses may use this file in
 * 			accordance with the commercial license agreement provided with the
 * 			Software or, alternatively, in accordance with the terms contained in
 * 			a written agreement between you and Digia.  For licensing terms and
 * 			conditions see http://qt.digia.com/licensing.  For further information
 * 			use the contact form at http://qt.digia.com/contact-us.
 *
 * 			GNU Lesser General Public License Usage
 * 			Alternatively, this file may be used under the terms of the GNU Lesser
 * 			General Public License version 2.1 as published by the Free Software
 * 			Foundation and appearing in the file LICENSE.LGPL included in the
 *			packaging of this file.  Please review the following information to
 * 			ensure the GNU Lesser General Public License version 2.1 requirements
 * 			will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * 			In addition, as a special exception, Digia gives you certain additional
 * 			rights.  These rights are described in the Digia Qt LGPL Exception
 * 			version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 *
 * 			GNU General Public License Usage
 * 			Alternatively, this file may be used under the terms of the GNU
 * 			General Public License version 3.0 as published by the Free Software
 * 			Foundation and appearing in the file LICENSE.GPL included in the
 * 			packaging of this file.  Please review the following information to
 * 			ensure the GNU General Public License version 3.0 requirements will be
 * 			met: http://www.gnu.org/copyleft/gpl.html.
 *
 *
 * 			$QT_END_LICENSE$
 *
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
