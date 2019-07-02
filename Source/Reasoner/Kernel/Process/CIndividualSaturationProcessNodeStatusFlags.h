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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODESTATUSFLAGS_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODESTATUSFLAGS_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualSaturationProcessNodeStatusFlags
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualSaturationProcessNodeStatusFlags {
					// public methods
					public:
						//! Constructor
						CIndividualSaturationProcessNodeStatusFlags();

						CIndividualSaturationProcessNodeStatusFlags* initStatusFlags();


						bool hasClashedFlag();
						CIndividualSaturationProcessNodeStatusFlags* setClashedFlag(bool value);

						bool hasInsufficientFlag();
						CIndividualSaturationProcessNodeStatusFlags* setInsufficientFlag(bool value);

						bool hasCriticalFlag();
						CIndividualSaturationProcessNodeStatusFlags* setCriticalFlag(bool value);

						bool hasNominalConnectionFlag();
						CIndividualSaturationProcessNodeStatusFlags* setNominalConnectionFlag(bool value);
						
						bool hasEQCandidateProplematicFlag();
						CIndividualSaturationProcessNodeStatusFlags* setEQCandidateProplematicFlag(bool value);

						bool hasCardinalityProplematicFlag();
						CIndividualSaturationProcessNodeStatusFlags* setCardinalityProplematicFlag(bool value);

						bool hasInitializedFlag();
						CIndividualSaturationProcessNodeStatusFlags* setInitializedFlag(bool value);

						bool hasCompletedFlag();
						CIndividualSaturationProcessNodeStatusFlags* setCompletedFlag(bool value);

						bool hasMissedABoxConsistencyFlag();
						CIndividualSaturationProcessNodeStatusFlags* setMissedABoxConsistencyFlag(bool value);

						bool hasFlags(cint64 flags, bool checkAllFlags);
						bool hasFlags(CIndividualSaturationProcessNodeStatusFlags* flags, bool checkAllFlags);
						CIndividualSaturationProcessNodeStatusFlags* setFlags(cint64 flags, bool value);
						CIndividualSaturationProcessNodeStatusFlags* addFlags(cint64 flags);
						CIndividualSaturationProcessNodeStatusFlags* addFlags(CIndividualSaturationProcessNodeStatusFlags* flags);
						CIndividualSaturationProcessNodeStatusFlags* clearFlags(cint64 flags);

						cint64 getFlags();

						const static cint64 INDSATFLAGCLASHED					= 0x0001;
						const static cint64 INDSATFLAGCRITICAL					= 0x0002;
						const static cint64 INDSATFLAGINSUFFICIENT				= 0x0004;
						const static cint64 INDSATFLAGNOMINALCONNECTION			= 0x0008;

						const static cint64 INDSATFLAGEQCANDPROPLEMATIC			= 0x0010;
						const static cint64 INDSATFLAGCARDINALITYPROPLEMATIC	= 0x0020;
						const static cint64 INDMISSEDABOXCONSISTENCYDATA		= 0x0040;


						const static cint64 INDSATFLAGINITIALIZED				= 0x0100;
						const static cint64 INDSATFLAGCOMPLETED					= 0x0200;

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mStatusFlags;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODESTATUSFLAGS_H
