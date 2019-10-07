/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEENTRYEXPANDWRITEDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEENTRYEXPANDWRITEDATA_H

// Libraries includes


// Namespace includes


// Other includes
#include "CacheSettings.h"
#include "CSignatureSatisfiableExpanderCacheEntryWriteData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CSignatureSatisfiableExpanderCacheEntryExpandWriteData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureSatisfiableExpanderCacheEntryExpandWriteData : public CSignatureSatisfiableExpanderCacheEntryWriteData {
					// public methods
					public:
						//! Constructor
						CSignatureSatisfiableExpanderCacheEntryExpandWriteData();

						CSignatureSatisfiableExpanderCacheEntryExpandWriteData* initExpandWriteData(cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash);

						cint64 getPrevSignature();
						cint64 getNewSignature();
						CCACHINGLIST<CCacheValue>* getCacheValueList();
						CCACHINGHASH<cint64,cint64>* getDepHash();


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mPrevSignature;
						cint64 mNewSignature;
						CCACHINGLIST<CCacheValue>* mCacheValueList;
						CCACHINGHASH<cint64,cint64>* mDepHash;

					// private methods
					private:

					// private variables
					private:


				};


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEENTRYEXPANDWRITEDATA_H
