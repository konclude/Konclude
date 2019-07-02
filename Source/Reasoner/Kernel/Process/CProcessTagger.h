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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSTAGGER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSTAGGER_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"

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
				 *		\class		CProcessTagger
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessTagger {
					// public methods
					public:
						//! Constructor
						CProcessTagger();

						//! Destructor
						virtual ~CProcessTagger();

						CProcessTagger* initProcessTagger(CProcessTagger* processTagger);


						CProcessTagger* setCurrentLocalizationTag(cint64 locTag);

						cint64 getCurrentLocalizationTag();
						cint64 getCurrentProcessingTag();
						cint64 getCurrentProcessingTagAndInc();
						cint64 getCurrentBranchingTag();
						cint64 getCurrentBlockingAddTag();
						cint64 getCurrentNodeSwitchTag();
						cint64 getCurrentConceptLabelSetModificationTag();
						cint64 getCurrentBlockingFollowTag();

						CProcessTagger* incLocalizationTag();
						CProcessTagger* incProcessingTag();
						CProcessTagger* incBranchingTag();
						CProcessTagger* incBlockingAddTag();
						CProcessTagger* incNodeSwitchTag();
						CProcessTagger* incConceptLabelSetModificationTag();
						CProcessTagger* incCurrentBlockingFollowTag();

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mLocalizationTag;
						cint64 mBranchingTag;
						cint64 mProcessingTag;
						cint64 mBlockingAddTag;
						cint64 mNodeSwitchTag;
						cint64 mConceptLabelSetModificationTag;
						cint64 mCurrentBlockingFollowTag;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSTAGGER_H
