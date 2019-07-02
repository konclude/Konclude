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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEVARIABLEBINDINGPATHSETDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEVARIABLEBINDINGPATHSETDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CLocalizationTag.h"
#include "CRepresentativeVariableBindingPathSetMigrateData.h"
#include "CRepresentativeVariableBindingPathSetDataSignature.h"
#include "CRepresentativeVariableBindingPathSetJoiningHash.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CRepresentativeVariableBindingPathSetData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CRepresentativeVariableBindingPathSetData : public CLinkerBase<cint64,CRepresentativeVariableBindingPathSetData>, public CLocalizationTag {
					// public methods
					public:
						//! Constructor
						CRepresentativeVariableBindingPathSetData(CProcessContext* processContext);

						CRepresentativeVariableBindingPathSetData* initRepresentativeVariableBindingPathData(CRepresentativeVariableBindingPathSetData* data);

						cint64 getRepresentativeKey();

						cint64 getRepresentativeID();
						CRepresentativeVariableBindingPathSetData* setRepresentativeID(cint64 repID);

						cint64 getUseCount();
						CRepresentativeVariableBindingPathSetData* setUseCount(cint64 useCount);
						CRepresentativeVariableBindingPathSetData* incUseCount(cint64 incCount = 1);

						cint64 getShareCount();
						CRepresentativeVariableBindingPathSetData* setShareCount(cint64 shareCount);
						CRepresentativeVariableBindingPathSetData* incShareCount(cint64 incCount = 1);
						CRepresentativeVariableBindingPathSetData* decShareCount(cint64 decCount = 1);

						bool isMigratable();
						CRepresentativeVariableBindingPathSetData* setMigratable(bool migratable);

						bool hasMigrateData();
						CRepresentativeVariableBindingPathSetMigrateData* getMigrateData(bool localizeOrCreate = true);

						CRepresentativeVariableBindingPathSetMigrateData* takeMigrateDataFrom(CRepresentativeVariableBindingPathSetData* repData);
						CRepresentativeVariableBindingPathSetMigrateData* copyMigrateDataFrom(CRepresentativeVariableBindingPathSetData* repData);

						CRepresentativeVariableBindingPathSetData* addKeySignatureValue(cint64 keySignatureValue);
						CRepresentativeVariableBindingPathSetDataSignature* getKeySignature();

						CRepresentativeVariableBindingPathSetJoiningHash* getJoiningHash(bool create);
						bool hasJoiningData(CConcept* joinConcept);

						cint64 getRepresentatedVariableCount();

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						bool mMigratable;
						cint64 mUseCount;
						cint64 mShareCount;

						CRepresentativeVariableBindingPathSetDataSignature mSigKey;

						CRepresentativeVariableBindingPathSetMigrateData* mLocMigrateData;
						CRepresentativeVariableBindingPathSetMigrateData* mUseMigrateData;

						CRepresentativeVariableBindingPathSetJoiningHash* mUseJoiningHash;
						CRepresentativeVariableBindingPathSetJoiningHash* mLocJoiningHash;


					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEVARIABLEBINDINGPATHSETDATA_H
