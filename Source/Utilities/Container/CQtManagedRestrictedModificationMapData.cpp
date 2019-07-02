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

#include "CQtManagedRestrictedModificationMapData.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {


#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER



			CQtManagedRestrictedModificationMapData CQtManagedRestrictedModificationMapData::shared_null = {
				&shared_null,
				{ &shared_null, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				/*Q_BASIC_ATOMIC_INITIALIZER(1)*/nullptr, 0, 0, 0, false, true, false, 0
			};

			CQtManagedRestrictedModificationMapData *CQtManagedRestrictedModificationMapData::createData(CQtManagedRestrictedModificationMapBase* referenceMap, CMemoryAllocationManager* memMan)
			{
				return createData(0,referenceMap,memMan);
			}

			CQtManagedRestrictedModificationMapData *CQtManagedRestrictedModificationMapData::createData(int alignment, CQtManagedRestrictedModificationMapBase* referenceMap, CMemoryAllocationManager* memMan)
			{

				CQtManagedRestrictedModificationMapData *d = CObjectAllocator<CQtManagedRestrictedModificationMapData>::allocateAndConstruct(memMan);
				
				//CMemoryAllocator::allocateMemory(memMan,sizeof(CQtManagedRestrictedModificationMapData));
				//CQtManagedRestrictedModificationMapData *d = CMemoryAllocator::allocateMemory(memMan,sizeof(CQtManagedRestrictedModificationMapData));
				//CQtManagedRestrictedModificationMapData *d = new CQtManagedRestrictedModificationMapData;
				Q_CHECK_PTR(d);
				Node *e = reinterpret_cast<Node *>(d);
				e->backward = e;
				e->forward[0] = e;
				//d->ref = 1;
				d->refMap = referenceMap;
				d->topLevel = 0;
				d->size = 0;
				d->randomBits = 0;
				d->insertInOrder = false;
				d->sharable = true;
				d->strictAlignment = alignment > 8;
				d->reserved = 0;
				return d;
			}

			void CQtManagedRestrictedModificationMapData::continueFreeData(int offset, CMemoryAllocationManager* memMan)
			{
				Node *e = reinterpret_cast<Node *>(this);
				Node *cur = e->forward[0];
				Node *prev;
				while (cur != e) {
					prev = cur;
					cur = cur->forward[0];
					if (strictAlignment)
						//qFreeAligned(reinterpret_cast<char *>(prev) - offset);
						CMemoryAllocator::releaseAlignedMemory(reinterpret_cast<char *>(prev) - offset,memMan);
					else
						//qFree(reinterpret_cast<char *>(prev) - offset);
						CMemoryAllocator::releaseMemory(reinterpret_cast<char *>(prev) - offset,memMan);
				}
				CObjectAllocator<CQtManagedRestrictedModificationMapData>::release(this,memMan);
				//delete this;
			}

			CQtManagedRestrictedModificationMapData::Node *CQtManagedRestrictedModificationMapData::node_create(Node *update[], int offset, CMemoryAllocationManager* memMan)
			{
				return node_create(update, offset, 0, memMan);
			}

			/*!
				Creates a new node inside the data structure.

				\a update is an array with pointers to the node after which the new node
				should be inserted. Because of the strange skip list data structure there
				could be several pointers to this node on different levels.
				\a offset is an amount of bytes that needs to reserved just before the
				CQtManagedRestrictedModificationMapData::Node structure.

				\a alignment dictates the alignment for the data.

				\internal
				\since 4.6
			*/
			CQtManagedRestrictedModificationMapData::Node *CQtManagedRestrictedModificationMapData::node_create(Node *update[], int offset, int alignment, CMemoryAllocationManager* memMan)
			{
				int level = 0;
				uint mask = (1 << Sparseness) - 1;

				while ((randomBits & mask) == mask && level < LastLevel) {
					++level;
					mask <<= Sparseness;
				}

				if (level > topLevel) {
					Node *e = reinterpret_cast<Node *>(this);
					level = ++topLevel;
					e->forward[level] = e;
					update[level] = e;
				}

				++randomBits;
				if (level == 3 && !insertInOrder)
					randomBits = qrand();

				//void *concreteNode = strictAlignment ?
				//					 qMallocAligned(offset + sizeof(Node) + level * sizeof(Node *), alignment) :
				//					 qMalloc(offset + sizeof(Node) + level * sizeof(Node *));
				void *concreteNode = strictAlignment ?
					CMemoryAllocator::allocateAlignedMemory(memMan,offset + sizeof(Node) + level * sizeof(Node *), alignment) :
					CMemoryAllocator::allocateMemory(memMan,offset + sizeof(Node) + level * sizeof(Node *));
				Q_CHECK_PTR(concreteNode);

				Node *abstractNode = reinterpret_cast<Node *>(reinterpret_cast<char *>(concreteNode) + offset);

				abstractNode->backward = update[0];
				update[0]->forward[0]->backward = abstractNode;

				for (int i = level; i >= 0; i--) {
					abstractNode->forward[i] = update[i]->forward[i];
					update[i]->forward[i] = abstractNode;
					update[i] = abstractNode;
				}
				++size;
				return abstractNode;
			}

			void CQtManagedRestrictedModificationMapData::node_delete(Node *update[], int offset, Node *node, CMemoryAllocationManager* memMan)
			{
				node->forward[0]->backward = node->backward;

				for (int i = 0; i <= topLevel; ++i) {
					if (update[i]->forward[i] != node)
						break;
					update[i]->forward[i] = node->forward[i];
				}
				--size;
				if (strictAlignment)
					//qFreeAligned(reinterpret_cast<char *>(node) - offset);
					CMemoryAllocator::releaseAlignedMemory(reinterpret_cast<char *>(node) - offset,memMan);
				else
					//qFree(reinterpret_cast<char *>(node) - offset);
					CMemoryAllocator::releaseMemory(reinterpret_cast<char *>(node) - offset,memMan);
			}

#ifdef KONCLUDE_CQTMANAGEDRESTRICTEDMODIFICATIONMAP_DEBUG

			uint CQtManagedRestrictedModificationMapData::adjust_ptr(Node *node)
			{
				if (node == reinterpret_cast<Node *>(this)) {
				   return (uint)0xDEADBEEF;
				} else {
					return (uint)node;
				}
			}

			void CQtManagedRestrictedModificationMapData::dump()
			{
				qDebug("Map data (ref = %d, size = %d, randomBits = %#.8x)", int(ref), size, randomBits);

				QString preOutput;
				QVector<QString> output(topLevel + 1);
				Node *e = reinterpret_cast<Node *>(this);

				QString str;
				str.sprintf("    %.8x", adjust_ptr(reinterpret_cast<Node *>(this)));
				preOutput += str;

				Node *update[LastLevel + 1];
				for (int i = 0; i <= topLevel; ++i) {
					str.sprintf("%d: [%.8x] -", i, adjust_ptr(reinterpret_cast<Node *>(forward[i])));
					output[i] += str;
					update[i] = reinterpret_cast<Node *>(forward[i]);
				}

				Node *node = reinterpret_cast<Node *>(forward[0]);
				while (node != e) {
					int level = 0;
					while (level < topLevel && update[level + 1] == node)
						++level;

					str.sprintf("       %.8x", adjust_ptr(node));
					preOutput += str;

					for (int i = 0; i <= level; ++i) {
						str.sprintf("-> [%.8x] -", adjust_ptr(node->forward[i]));
						output[i] += str;
						update[i] = node->forward[i];
					}
					for (int j = level + 1; j <= topLevel; ++j)
						output[j] += QLatin1String("---------------");
					node = node->forward[0];
				}

				qDebug("%s", preOutput.ascii());
				for (int i = 0; i <= topLevel; ++i)
					qDebug("%s", output[i].ascii());
			}
#endif


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

