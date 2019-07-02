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

#ifndef KONCLUDE_REASONER_QUERY_QUERYSETTINGS
#define KONCLUDE_REASONER_QUERY_QUERYSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QList>
#include <QHash>

// Namespace includes


// Other includes
#include "Utilities/Memory/CObjectAllocator.h"

#include "Utilities/Container/CLinker.h"


// Logger includes


namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\file		ProcessSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations

			// mappings

#define CQUERYLIST QList
#define CQUERYHASH QHash

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_QUERY_QUERYSETTINGS