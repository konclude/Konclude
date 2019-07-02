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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTPILINGLABELSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTPILINGLABELSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CConceptPilingDescriptorReapplyData.h"
#include "CConceptLabelSetModificationTag.h"
#include "CImplicationReapplyConceptPilingDescriptor.h"
#include "CReapplyConceptPilingLabelSetIterator.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReapplyConceptPilingLabelSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyConceptPilingLabelSet : public CConceptLabelSetModificationTag {
					// public methods
					public:
						//! Constructor
						CReapplyConceptPilingLabelSet(CProcessContext* processContext = nullptr);


						CReapplyConceptPilingLabelSet* initReapplyConceptPilingLabelSet();
						CReapplyConceptPilingLabelSet* copyReapplyConceptPilingLabelSet(CReapplyConceptPilingLabelSet* copyConceptPilingLabelSet, cint64 maxCopyPilingDepth);

						cint64 getConceptCount();

						bool getConceptPilingDescriptor(cint64 conTag, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth);
						bool getConceptPilingDescriptor(CConcept* concept, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth);


						bool getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth);
						bool getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth);

						bool hasConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes);
						bool hasConcept(CConcept* concept, bool negated);
						bool hasConcept(cint64 conTag, bool negated);


						bool hasConcept(cint64 maxPilingDepth, CConcept* concept, bool negated);
						bool hasConcept(cint64 maxPilingDepth, cint64 conTag, bool negated);


						bool hasConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool hasConcept(cint64 conTag, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool negated);

						bool containsConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes);



						bool insertConceptReturnClashed(CConceptPilingDescriptor* conPilDes, cint64 pilingDepth, bool* newInsertion = nullptr, CImplicationReapplyConceptPilingDescriptor** reapplyImpReapplyConPilDes = nullptr, bool* insertionUpdate = nullptr, cint64* prevPilingDepth = nullptr, CImplicationReapplyConceptPilingDescriptor*** impReapplyConPilDesPtr = nullptr);
						
						bool insertConceptReapplicationReturnTriggered(cint64 conTag, CImplicationReapplyConceptPilingDescriptor* reapplyImpReapplyConPilDes, CConceptPilingDescriptor** conPilDes, cint64* pilingDepth);


						CConceptPilingDescriptor* getConceptPilingDescriptionLinker();


						CReapplyConceptPilingLabelSetIterator getIterator(bool iterateConPilDes = true, bool iterateReapplies = true, cint64 maxPilingDepthIterate = -1);


					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSHASH<cint64,CConceptPilingDescriptorReapplyData> mConceptDesDepMap;
						CConceptPilingDescriptor* mConceptPilDesLinker;
						cint64 mConceptCount;
						cint64 mTotelCount;

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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTPILINGLABELSET_H
