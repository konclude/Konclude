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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_PROCESSSETTINGS
#define KONCLUDE_REASONER_KERNEL_PROCESS_PROCESSSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"
#include "Utilities/Container/CQtManagedRestrictedModificationMap.h"
#include "Utilities/Container/CQtManagedRestrictedModificationSet.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\file		ProcessSettings
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */



				// forward declarations

				class CIndividualProcessNodeReference;
				class CConceptLabelSet;
				class CIndividualProcessNode;
				class CIndividualLinkEdge;
				class CProcessingStatisticDescription;
				class CImplicationReapplyConceptPilingDescriptor;
				class CConceptPilingDescriptor;
				class CIndividualPilingProcessNode;
				class CImplicationReapplyConceptSaturationDescriptor;
				class CConceptSaturationDescriptor;
				class CIndividualSaturationProcessNode;
				class CIndividualSaturationProcessNodeLinker;

				class CRepresentativeVariableBindingPathSetData;

				class CSaturationIndividualNodeExtensionResolveData;
				class CSaturationIndividualNodeExtensionResolveHash;
				class CSaturationIndividualNodeExtensionResolveHashData;

#define CPROCESSHASH CQtManagedRestrictedModificationHash
#define CPROCESSMAP CQtManagedRestrictedModificationMap
#define CPROCESSSET CQtManagedRestrictedModificationSet
#define CPROCESSLIST CQtManagedRestrictedModificationList

#define KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING



				const cint64 DEFAULTVECTOR2POTSIZE = 9;

				const cint64 CINDIVIDUALPROCESSNODEVECTOR2POTSIZE				= DEFAULTVECTOR2POTSIZE;
				const cint64 CINDIVIDUALPILINGPROCESSNODEVECTOR2POTSIZE			= DEFAULTVECTOR2POTSIZE;
				const cint64 CINDIVIDUALSATURATIONPROCESSNODEVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_KERNEL_PROCESS_PROCESSSETTINGS