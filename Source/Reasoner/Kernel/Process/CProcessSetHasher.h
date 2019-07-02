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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSSETHASHER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSSETHASHER_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "ProcessSettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CProcessSetHasher
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				template<class T>
				class CProcessSetHasher {
					// public methods
					public:
						//! Constructor
						CProcessSetHasher(CPROCESSSET<T>* processSet) {
							mProcessSet = processSet;
							mHashValue = 0;
							if (mProcessSet) {
								for (typename CPROCESSSET<T>::const_iterator it = processSet->constBegin(), itEnd = processSet->constEnd(); it != itEnd; ++it) {
									mHashValue += qHash(*it);
								}
							}
						}

						cint64 getNodeHashValue() const {
							return mHashValue;
						}

						bool operator==(const CProcessSetHasher& processSetHasher) const {
							if (mHashValue != processSetHasher.getNodeHashValue()) {
								return false;
							}
							return mProcessSet->operator==(*processSetHasher.mProcessSet);
						}


					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSSET<T>* mProcessSet;
						cint64 mHashValue;

					// private methods
					private:

					// private variables
					private:

				};


				template<class T>
				inline uint qHash(const CProcessSetHasher<T>& processSetHasher) {
					qint64 key = (qint64)processSetHasher.getNodeHashValue();
					if (sizeof(quint64) > sizeof(uint)) {
						return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
					} else {
						return uint(key);
					}
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSSETHASHER_H
