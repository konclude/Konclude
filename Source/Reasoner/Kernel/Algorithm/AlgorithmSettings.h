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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_ALGORITHMSETTINGS
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_ALGORITHMSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "KoncludeSettings.h"

#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"
#include "Utilities/Container/CQtManagedRestrictedModificationMap.h"
#include "Utilities/Container/CQtManagedRestrictedModificationSet.h"
#include "Utilities/Container/CQtManagedRestrictedModificationList.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				/*! 
				 *
				 *		\file		AlgorithmSettings
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */



#define CPROCESSINGLIST CQtManagedRestrictedModificationList
#define CPROCESSINGSET CQtManagedRestrictedModificationSet
#define CPROCESSINGHASH CQtManagedRestrictedModificationHash


				// forward declarations


				class CCalculationAlgorithmContext;
				class CUnsatisfiableCacheHandler;
				class CIndividualNodeManager;
				class CClashDescriptorFactory;
				class CDependencyFactory;
				class CSatisfiableExpanderCacheHandler;



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_KERNEL_ALGORITHM_ALGORITHMSETTINGS