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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptorDependencyPair.h"
#include "CProcessContext.h"
#include "CConceptLabelSetIterator.h"
#include "CClashedConceptDescriptor.h"
#include "CConceptSetSignature.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CConceptLabelSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConceptLabelSet {
					// public methods
					public:
						//! Constructor
						CConceptLabelSet(CProcessContext* processContext = nullptr);

						//! Destructor
						virtual ~CConceptLabelSet();

						CConceptLabelSet* initConceptLabelSet(CConceptLabelSet* prevConceptLabelSet);

						cint64 getConceptCount();

						bool hasConceptDescriptor(CConceptDescriptor* conceptDescriptor);
						bool hasConcept(CConcept* concept, bool negated);
						bool hasConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool negated);
						bool containsConceptDescriptor(CConceptDescriptor* conceptDescriptor);
						CClashedConceptDescriptor* insertConceptReturnClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint);
						CConceptLabelSet* insertConceptThrowClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint);
						CConceptLabelSet* updateConceptDependencyTrackPoint(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* updateDepTrackPoint);

						CConceptLabelSetIterator getConceptLabelSetIterator(bool getSorted = false, bool getDependencies = false);

						CConceptSetSignature* getConceptSignature();
						cint64 getConceptSignatureValue();

					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSMAP<cint64,CConceptDescriptorDependencyPair> mConceptDesDepMap;
						CConceptDescriptor* mConceptDesLinker;
						CConceptDescriptor* mPrevConceptDesLinker;
						CConceptSetSignature mConceptSignature;
						cint64 mConceptCount;

						CProcessContext* mProcessContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSET_H
