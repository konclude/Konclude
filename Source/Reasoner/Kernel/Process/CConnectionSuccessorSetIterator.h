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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONNECTIONSUCCESSORSETITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONNECTIONSUCCESSORSETITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CConnectionSuccessorSetIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConnectionSuccessorSetIterator {
					// public methods
					public:
						//! Constructor
						CConnectionSuccessorSetIterator(cint64 connID = CINT64_MIN);
						CConnectionSuccessorSetIterator(const CPROCESSSET<cint64>::iterator& beginIt, const CPROCESSSET<cint64>::iterator& endIt);

						bool hasNext();
						cint64 nextSuccessorConnectionID(bool moveNext = true);
						cint64 next(bool moveNext = true);

					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSSET<cint64>::iterator mBeginIt;
						CPROCESSSET<cint64>::iterator mEndIt;
						cint64 mConnID;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONNECTIONSUCCESSORSETITERATOR_H
