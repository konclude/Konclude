/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CIndividualSaturationProcessNodeStatusFlags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualSaturationProcessNodeStatusFlags::CIndividualSaturationProcessNodeStatusFlags() {
					initStatusFlags();
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::initStatusFlags() {
					mStatusFlags = 0;
					return this;
				}

				bool CIndividualSaturationProcessNodeStatusFlags::hasClashedFlag() {
					return hasFlags(INDSATFLAGCLASHED,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setClashedFlag(bool value) {
					setFlags(INDSATFLAGCLASHED,value);
					return this;
				}

				bool CIndividualSaturationProcessNodeStatusFlags::hasInsufficientFlag() {
					return hasFlags(INDSATFLAGINSUFFICIENT,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setInsufficientFlag(bool value) {
					setFlags(INDSATFLAGINSUFFICIENT,value);
					return this;
				}

				bool CIndividualSaturationProcessNodeStatusFlags::hasCriticalFlag() {
					return hasFlags(INDSATFLAGCRITICAL,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setCriticalFlag(bool value) {
					setFlags(INDSATFLAGCRITICAL,value);
					return this;
				}

				bool CIndividualSaturationProcessNodeStatusFlags::hasNominalConnectionFlag() {
					return hasFlags(INDSATFLAGNOMINALCONNECTION,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setNominalConnectionFlag(bool value) {
					setFlags(INDSATFLAGNOMINALCONNECTION,value);
					return this;
				}


				bool CIndividualSaturationProcessNodeStatusFlags::hasEQCandidateProplematicFlag() {
					return hasFlags(INDSATFLAGEQCANDPROPLEMATIC,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setEQCandidateProplematicFlag(bool value) {
					setFlags(INDSATFLAGEQCANDPROPLEMATIC,value);
					return this;
				}

				bool CIndividualSaturationProcessNodeStatusFlags::hasCardinalityProplematicFlag() {
					return hasFlags(INDSATFLAGCARDINALITYPROPLEMATIC,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setCardinalityProplematicFlag(bool value) {
					setFlags(INDSATFLAGCARDINALITYPROPLEMATIC,value);
					return this;
				}

				bool CIndividualSaturationProcessNodeStatusFlags::hasCardinalityRestrictedFlag() {
					return hasFlags(INDSATFLAGCARDINALITYRESTRICTED,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setCardinalityRestrictedFlag(bool value) {
					setFlags(INDSATFLAGCARDINALITYRESTRICTED,value);
					return this;
				}


				bool CIndividualSaturationProcessNodeStatusFlags::hasInitializedFlag() {
					return hasFlags(INDSATFLAGINITIALIZED,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setInitializedFlag(bool value) {
					setFlags(INDSATFLAGINITIALIZED,value);
					return this;
				}




				bool CIndividualSaturationProcessNodeStatusFlags::hasCompletedFlag() {
					return hasFlags(INDSATFLAGCOMPLETED,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setCompletedFlag(bool value) {
					setFlags(INDSATFLAGCOMPLETED,value);
					return this;
				}



				bool CIndividualSaturationProcessNodeStatusFlags::hasMissedABoxConsistencyFlag() {
					return hasFlags(INDMISSEDABOXCONSISTENCYDATA,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setMissedABoxConsistencyFlag(bool value) {
					setFlags(INDMISSEDABOXCONSISTENCYDATA,value);
					return this;
				}



				bool CIndividualSaturationProcessNodeStatusFlags::hasSuccessorNodeExtensionsFlag() {
					return hasFlags(INDSUCCESSORNODEEXTENSIONS,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setSuccessorNodeExtensionsFlag(bool value) {
					setFlags(INDSUCCESSORNODEEXTENSIONS,value);
					return this;
				}



				bool CIndividualSaturationProcessNodeStatusFlags::hasPropagationIncompleteFlag() {
					return hasFlags(INDSATFLAGPROPAGATIONINCOMPLETE,false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setPropagationIncompleteFlag(bool value) {
					setFlags(INDSATFLAGPROPAGATIONINCOMPLETE,value);
					return this;
				}


				bool CIndividualSaturationProcessNodeStatusFlags::hasUnregisteredPropagationFlag() {
					return hasFlags(INDSATFLAGUNREGISTEREDPROPAGATION, false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setUnregisteredPropagationFlag(bool value) {
					setFlags(INDSATFLAGUNREGISTEREDPROPAGATION, value);
					return this;
				}


				bool CIndividualSaturationProcessNodeStatusFlags::hasUnprocessedFlag() {
					return hasFlags(INDSATFLAGUNPROCESSED, false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setUnprocessedFlag(bool value) {
					setFlags(INDSATFLAGUNPROCESSED, value);
					return this;
				}


				bool CIndividualSaturationProcessNodeStatusFlags::hasUnmarkedRoleAssertionFlag() {
					return hasFlags(INDSATFLAGUNMARKEDROLEASSERTION, false);
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setUnmarkedRoleAssertionFlag(bool value) {
					setFlags(INDSATFLAGUNMARKEDROLEASSERTION, value);
					return this;
				}



				bool CIndividualSaturationProcessNodeStatusFlags::hasFlags(cint64 flags, bool checkAllFlags) {
					if (flags == 0) {
						return true;
					}
					if (checkAllFlags) {
						return (~mStatusFlags & flags) == 0;
					} else {
						return (mStatusFlags & flags) != 0;
					}
				}


				bool CIndividualSaturationProcessNodeStatusFlags::hasFlags(CIndividualSaturationProcessNodeStatusFlags* flags, bool checkAllFlags) {
					return hasFlags(flags->getFlags(),checkAllFlags);
				}


				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::setFlags(cint64 flags, bool value) {
					if (value) {
						mStatusFlags |= flags;
					} else {
						mStatusFlags = (mStatusFlags & ~flags);
					}
					return this;
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::addFlags(CIndividualSaturationProcessNodeStatusFlags* flags) {
					addFlags(flags->getFlags());
					return this;
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::addFlags(cint64 flags) {
					mStatusFlags |= flags;
					return this;
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNodeStatusFlags::clearFlags(cint64 flags) {
					mStatusFlags = (mStatusFlags & ~flags);
					return this;
				}


				cint64 CIndividualSaturationProcessNodeStatusFlags::getFlags() {
					return mStatusFlags;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
