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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGTAG_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGTAG_H

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
				 *		\class		CProcessingTag
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessingTag : public CProcessTag {
					// public methods
					public:
						//! Constructor
						CProcessingTag(cint64 processingTag = 0);

						//! Destructor
						virtual ~CProcessingTag();

						cint64 getProcessingTag();
						CProcessingTag* setProcessingTag(cint64 processingTag);
						CProcessingTag* setProcessingTag(CProcessTagger* processTagger);
						CProcessingTag* initProcessingTag(cint64 processingTag);
						CProcessingTag* initProcessingTag(CProcessTagger* processTagger);
						bool updateProcessingTag(cint64 processingTag);
						bool updateProcessingTag(CProcessTagger* processTagger);

						
						bool isProcessingTagUpdated(cint64 processingTag);
						bool isProcessingTagUpToDate(cint64 processingTag);
						bool isProcessingTagUpdated(CProcessTagger* processTagger);
						bool isProcessingTagUpToDate(CProcessTagger* processTagger);


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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGTAG_H
