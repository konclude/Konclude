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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CLOCALIZATIONTAG_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CLOCALIZATIONTAG_H

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
				 *		\class		CLocalizationTag
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CLocalizationTag : public CProcessTag {
					// public methods
					public:
						//! Constructor
						CLocalizationTag(cint64 processTag = 0);

						cint64 getLocalizationTag();
						CLocalizationTag* setLocalizationTag(cint64 localizationTag);
						CLocalizationTag* setLocalizationTag(CProcessTagger* processTagger);
						CLocalizationTag* initLocalizationTag(cint64 localizationTag);
						CLocalizationTag* initLocalizationTag(CProcessTagger* processTagger);
						bool updateLocalizationTag(cint64 localizationTag);
						bool updateLocalizationTag(CProcessTagger* processTagger);

						
						bool isLocalizationTagUpdated(cint64 localizationTag);
						bool isLocalizationTagUpToDate(cint64 localizationTag);
						bool isLocalizationTagUpdated(CProcessTagger* processTagger);
						bool isLocalizationTagUpToDate(CProcessTagger* processTagger);

						bool isRelocalized();
						CLocalizationTag* setRelocalized(bool relocalized = true);

					// protected methods
					protected:

					// protected variables
					protected:
						bool mRelocalized;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CLOCALIZATIONTAG_H
