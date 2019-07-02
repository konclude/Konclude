/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLISTDATA_H
#define KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLISTDATA_H

// Namespace includes
#include "ContainerSettings.h"
#include "CQtManagedRestrictedModificationListBase.h"

#ifdef KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER

// Library includes
#include <QList>

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
			 *		\class		CQtManagedRestrictedModificationListData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			struct CQtManagedRestrictedModificationListData {
				struct Data {
					//QBasicAtomicInt ref;
					CQtManagedRestrictedModificationListBase* refList;
					int alloc, begin, end;
					uint sharable : 1;
					void *array[1];
				};
				enum { DataHeaderSize = sizeof(Data) - sizeof(void *) };

				Data *detach(int alloc, CMemoryAllocationManager* memMan, CQtManagedRestrictedModificationListBase* listBase);
				Data *detach_grow(int *i, int n, CMemoryAllocationManager* memMan, CQtManagedRestrictedModificationListBase* listBase);
				void realloc(int alloc, CMemoryAllocationManager* memMan);
				static Data shared_null;
				Data *d;
				void **erase(void **xi);
				void **append(int n, CMemoryAllocationManager* memMan);
				void **append(CMemoryAllocationManager* memMan);
				void **append(const CQtManagedRestrictedModificationListData &l, CMemoryAllocationManager* memMan);
				void **append2(const CQtManagedRestrictedModificationListData &l, CMemoryAllocationManager* memMan);
				void **prepend(CMemoryAllocationManager* memMan);
				void **insert(int i, CMemoryAllocationManager* memMan);
				void remove(int i);
				void remove(int i, int n);
				void move(int from, int to);
				inline int size() const { return d->end - d->begin; }
				inline bool isEmpty() const { return d->end  == d->begin; }
				inline void **at(int i) const { return d->array + d->begin + i; }
				inline void **begin() const { return d->array + d->begin; }
				inline void **end() const { return d->array + d->end; }
			};




		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER

#endif // KONCLUDE_UTILITIES_CONTAINER_CQTMANAGEDRESTRICTEDMODIFICATIONLISTDATA_H
