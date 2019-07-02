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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTDESCRIPTOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTDESCRIPTOR_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "ProcessSettings.h"
#include "CDependencyTracker.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"

#include "Utilities/Container/CNegLinker.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CConceptDescriptor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConceptDescriptor : public CNegLinkerBase<CConcept*,CConceptDescriptor>, public CDependencyTracker {
					// public methods
					public:
						//! Constructor
						CConceptDescriptor();

						CConceptDescriptor* initConceptDescriptor(CConcept* concept, bool negated, CDependencyTrackPoint* depTrackPoint);

						bool isEqualsToBOTTOM(bool negated = false);
						bool isEqualsToTOP(bool negated = false);

						CConcept *getConcept();
						qint64 getConceptTag();
						qint64 getTerminologyTag();
						QPair<qint64,qint64> getTerminologyConceptTagPair();

						bool isClashWith(CConceptDescriptor *conDesNegLinker);

						CConceptDescriptor *getNextConceptDesciptor();



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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTDESCRIPTOR_H
