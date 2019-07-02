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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CReapplyConceptLabelSetIterator.h"
#include "CClashedConceptDescriptor.h"
#include "CConceptSetSignature.h"
#include "CConceptDescriptorDependencyReapplyData.h"
#include "CConceptLabelSetModificationTag.h"
#include "CCoreConceptDescriptor.h"
#include "CConceptSetStructure.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReapplyConceptLabelSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyConceptLabelSet : public CConceptLabelSetModificationTag {
					// public methods
					public:
						//! Constructor
						CReapplyConceptLabelSet(CProcessContext* processContext = nullptr);

						//! Destructor
						virtual ~CReapplyConceptLabelSet();

						CReapplyConceptLabelSet* initConceptLabelSet(CReapplyConceptLabelSet* prevConceptLabelSet);

						cint64 getConceptCount();

						bool getConceptDescriptor(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint);
						bool getConceptDescriptor(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint);

						bool hasConceptDescriptor(CConceptDescriptor* conceptDescriptor);
						bool hasConcept(CConcept* concept, bool negated);
						bool hasConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool negated);

						bool containsConceptDescriptor(CConceptDescriptor* conceptDescriptor);
						bool insertConceptIgnoreClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CCondensedReapplyQueueIterator* reapplyQueueIt = nullptr);
						bool insertConceptGetClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CCondensedReapplyQueueIterator* reapplyQueueIt = nullptr, CConceptDescriptor** clashedConDes = nullptr, CDependencyTrackPoint** clashedDepTrackPoint = nullptr);

						CClashedConceptDescriptor* insertConceptReturnClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, bool* hasContained = nullptr, CCondensedReapplyQueueIterator* reapplyQueueIt = nullptr);
						bool insertConceptThrowClashReturnContained(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CCondensedReapplyQueueIterator* reapplyQueueIt = nullptr);

						CReapplyConceptLabelSetIterator getConceptLabelSetIterator(bool getSorted = false, bool getDependencies = false, bool getAllStructure = false);

						CConceptSetSignature* getConceptSignature();
						CConceptSetStructure* getConceptStructure();
						cint64 getConceptSignatureValue();

						bool getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue);
						bool getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue);
						bool getConceptDescriptorOrReapplyQueue(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue);
						bool getConceptDescriptorOrReapplyQueue(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue);
						CCondensedReapplyQueue* getConceptDescriptorAndReapplyQueue(CConcept*& concept, CConceptDescriptor*& conDes, bool create = false);
						CCondensedReapplyQueue* getConceptReapplyQueue(CConcept*& concept, bool& conceptNegation, bool create = false);
						CCondensedReapplyQueue* getConceptReapplyQueue(cint64 conTag, bool create = false);
						bool containsConceptReapplyQueue(CConcept *&concept, bool &conceptNegation);

						CCondensedReapplyQueueIterator getConceptReapplyIterator(CConcept* concept, bool conceptNegation, bool clearDynamicReapplyQueue = true);
						CCondensedReapplyQueueIterator getConceptReapplyIterator(CConceptDescriptor* conDes, bool clearDynamicReapplyQueue = true);

						CConceptDescriptor* getAddingSortedConceptDescriptionLinker();

						CConceptDescriptorDependencyReapplyData* getConceptDescriptorDependencyReapplyData(cint64 dataTag);


						CCoreConceptDescriptor* getCoreConceptDescriptorLinker();
						CReapplyConceptLabelSet* addCoreConceptDescriptor(CCoreConceptDescriptor* coreConDes);

					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData> mConceptDesDepMap;
						CCoreConceptDescriptor* mCoreConDesLinker;
						CConceptDescriptor* mConceptDesLinker;
						CConceptDescriptor* mPrevConceptDesLinker;
						CConceptSetSignature mConceptSignature;
						CConceptSetStructure mConceptStructure;
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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSET_H
