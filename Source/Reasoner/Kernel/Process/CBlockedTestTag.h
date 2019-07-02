/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKEDTESTTAG_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKEDTESTTAG_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessTag.h"
#include "CProcessTagger.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBlockedTestTag
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBlockedTestTag : public CProcessTag {
					// public methods
					public:
						//! Constructor
						CBlockedTestTag(cint64 blockingAddTag = 0);

						cint64 getBlockedTestTag();
						CBlockedTestTag* setBlockedTestTag(cint64 blockingAddTag);
						CBlockedTestTag* setBlockedTestTag(CProcessTagger* processTagger);
						CBlockedTestTag* initBlockedTestTag(cint64 blockingAddTag);
						CBlockedTestTag* initBlockedTestTag(CProcessTagger* processTagger);
						bool updateBlockedTestTag(cint64 blockingAddTag);
						bool updateBlockedTestTag(CProcessTagger* processTagger);

						
						bool isBlockedTestTagUpdated(cint64 blockingAddTag);
						bool isBlockedTestTagUpToDate(cint64 blockingAddTag);
						bool isBlockedTestTagUpdated(CProcessTagger* processTagger);
						bool isBlockedTestTagUpToDate(CProcessTagger* processTagger);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKEDTESTTAG_H
