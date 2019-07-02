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

#ifndef KONCLUDE_UTILITIES_MEMORY_CCENTRALIZEDALLOCATIONCONFIGPROVIDEDDEPENDENDLIMITATION_H
#define KONCLUDE_UTILITIES_MEMORY_CCENTRALIZEDALLOCATIONCONFIGPROVIDEDDEPENDENDLIMITATION_H

// Library includes
#include <qglobal.h>
#include <qmath.h>
#include <QMutex>
#include <QMutexLocker>

// Namespace includes
#include "MemorySettings.h"
#include "CCentralizedAllocationLimitation.h"
#include "CCentralizedAllocationLimitator.h"


// Other includes
#include "Config/CConfigurationProvider.h"
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CCentralizedAllocationConfigProvidedDependendLimitation
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCentralizedAllocationConfigProvidedDependendLimitation : public CCentralizedAllocationLimitation {
				// public methods
				public:
					//! Constructor
					CCentralizedAllocationConfigProvidedDependendLimitation(CConfigurationProvider* configProvider, const QString& configString);

					//! Destructor
					virtual ~CCentralizedAllocationConfigProvidedDependendLimitation();

					CCentralizedAllocationLimitator* getLimitator();

					virtual cint64 getUpdatedMemoryAllocationLimit(CAllocationLimitator* limitator);

					virtual cint64 getAllocatedMemorySize();
					virtual cint64 getAllocatedMemorySizeMaximum();
					virtual cint64 getReservedMemorySizeMaximum();

				// protected methods
				protected:
					bool updateMaxMemoryAllocationSize();

					bool requestLimitatorMemoryAllocationSizeUpdate();


				// protected variables
				protected:
					CConfigurationProvider* mConfigProvider;
					QString mConfigString;

					bool mMemAllocLimited;
					cint64 mMaxMemAllocSize;
					cint64 mAddMemAllocSize;
					cint64 mCurrMemAllocSize;
					cint64 mCriticalMemAllocSize;

					CConfiguration* mLastConfig;
					QMutex mSyncMutex;

					CCentralizedAllocationLimitator* mLimitatorLinker;


					cint64 mStatMaxAllocatedMemSize;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CCENTRALIZEDALLOCATIONCONFIGPROVIDEDDEPENDENDLIMITATION_H
