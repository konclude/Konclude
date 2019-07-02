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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSETMODIFICATIONTAG_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSETMODIFICATIONTAG_H

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
				 *		\class		CConceptLabelSetModificationTag
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConceptLabelSetModificationTag : public CProcessTag {
					// public methods
					public:
						//! Constructor
						CConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag = 0);

						//! Destructor
						virtual ~CConceptLabelSetModificationTag();

						cint64 getConceptLabelSetModificationTag();
						CConceptLabelSetModificationTag* setConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag);
						CConceptLabelSetModificationTag* setConceptLabelSetModificationTag(CProcessTagger* processTagger);
						CConceptLabelSetModificationTag* initConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag);
						CConceptLabelSetModificationTag* initConceptLabelSetModificationTag(CProcessTagger* processTagger);
						bool updateConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag);
						bool updateConceptLabelSetModificationTag(CProcessTagger* processTagger);

						
						bool isConceptLabelSetModificationTagUpdated(cint64 conceptLabelSetModificationTag);
						bool isConceptLabelSetModificationTagUpToDate(cint64 conceptLabelSetModificationTag);
						bool isConceptLabelSetModificationTagUpdated(CProcessTagger* processTagger);
						bool isConceptLabelSetModificationTagUpToDate(CProcessTagger* processTagger);


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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSETMODIFICATIONTAG_H
