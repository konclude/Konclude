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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCACHETAGGINGPOOL_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCACHETAGGINGPOOL_H

// Libraries includes


// Namespace includes


// Other includes
#include <qglobal.h>
#include <QTime>

#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CCacheTaggingPool
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCacheTaggingPool {
					// public methods
					public:
						//! Constructor
						CCacheTaggingPool(cint64 poolSize = 1024);

						//! Destructor
						virtual ~CCacheTaggingPool();
						
						cint64 takeNextTag();


					// protected methods
					protected:
						bool hasMoreTags();
						CCacheTaggingPool* createNextTaggingPool();

					// protected variables
					protected:
						cint64 mPoolSize;
						cint64 mPoolSizeMask;
						cint64 mNextPoolStartTag;
						cint64* mTagPool;
						cint64 mPoolIndex;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCACHETAGGINGPOOL_H
