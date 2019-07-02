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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDESCRIPTORHASHER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDESCRIPTORHASHER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CTrackedClashedDescriptor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CTrackedClashedDescriptorHasher
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTrackedClashedDescriptorHasher {
					// public methods
					public:
						//! Constructor
						CTrackedClashedDescriptorHasher(CTrackedClashedDescriptor* trackedClashedDescriptor);
						CTrackedClashedDescriptorHasher(const CTrackedClashedDescriptorHasher& hasher);

						cint64 getDescriptorHashValue() const;

						bool operator==(const CTrackedClashedDescriptorHasher& clashedDesHasher) const;

					// protected methods
					protected:
						cint64 calculateDescriptorHashValue(CTrackedClashedDescriptor* des);

					// protected variables
					protected:
						CTrackedClashedDescriptor* mTrackedClashedDes;
						cint64 mHashValue;


					// private methods
					private:

					// private variables
					private:

				};

				inline uint qHash(const CTrackedClashedDescriptorHasher& desHasher) {
					qint64 key = (qint64)desHasher.getDescriptorHashValue();
					if (sizeof(quint64) > sizeof(uint)) {
						return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
					} else {
						return uint(key);
					}
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDESCRIPTORHASHER_H
