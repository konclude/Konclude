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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CConceptSaturationDescriptorReapplyData.h"
#include "CImplicationReapplyConceptSaturationDescriptor.h"
#include "CReapplyConceptSaturationLabelSetIterator.h"
#include "CSaturationModifiedProcessUpdateLinker.h"
#include "CConceptSetFlags.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReapplyConceptSaturationLabelSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyConceptSaturationLabelSet {
					// public methods
					public:
						//! Constructor
						CReapplyConceptSaturationLabelSet(CProcessContext* processContext = nullptr);


						CReapplyConceptSaturationLabelSet* initReapplyConceptSaturationLabelSet();
						CReapplyConceptSaturationLabelSet* copyReapplyConceptSaturationLabelSet(CReapplyConceptSaturationLabelSet* copyConceptSaturationLabelSet, bool tryFlatLabelCopy);

						cint64 getConceptCount();
						cint64 getTotalCount();

						bool getConceptSaturationDescriptor(cint64 conTag, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes);
						bool getConceptSaturationDescriptor(CConcept* concept, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes);


						bool getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes);
						bool getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes);

						bool hasConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes);

						bool hasConcept(CConcept* concept, bool negated);
						bool hasConcept(cint64 conTag, bool negated);

						bool hasConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool hasConcept(cint64 conTag, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool* containsNegated = nullptr);
						bool containsConcept(CConcept* concept, bool negated);

						bool containsConceptOrReapllyQueue(cint64 conTag);


						bool containsConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes);


						bool hasModifiedUpdateLinkers();
						CSaturationModifiedProcessUpdateLinker* getModifiedUpdateLinker();
						CReapplyConceptSaturationLabelSet* addModifiedUpdateLinker(CSaturationModifiedProcessUpdateLinker* modUpdateLinker);
						CReapplyConceptSaturationLabelSet* setModifiedUpdateLinker(CSaturationModifiedProcessUpdateLinker* modUpdateLinker);


						bool insertConceptReturnClashed(CConceptSaturationDescriptor* conSatDes, bool* newInsertion = nullptr, CImplicationReapplyConceptSaturationDescriptor*** impReapplyConSatDesPtr = nullptr);
						
						bool insertConceptReapplicationReturnTriggered(cint64 conTag, CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDes, CConceptSaturationDescriptor** conSatDes);


						CConceptSaturationDescriptor* getConceptSaturationDescriptionLinker();


						CReapplyConceptSaturationLabelSetIterator getIterator(bool iterateConSatDes = true, bool iterateReapplies = true);


						CConceptSetFlags* getConceptFlags();

						bool areAllConceptsInAdditionalHash();

						CConceptSaturationDescriptor* getLastNominalIndependentConceptSaturationDescriptorLinker();
						CReapplyConceptSaturationLabelSet* setLastNominalIndependentConceptSaturationDescriptorLinker(CConceptSaturationDescriptor* conSatDes);

					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>* mConceptDesDepHash;
						CPROCESSHASH<cint64,CConceptSaturationDescriptorReapplyData>* mAdditionalConceptDesDepHash;
						CConceptSaturationDescriptor* mConceptSatDesLinker;
						CConceptSaturationDescriptor* mLastNominalIndepConSatDes;
						CConceptSetFlags mConceptFlags;

						cint64 mConceptCount;
						cint64 mTotelCount;

						const static cint64 ADDITIONALCOPYSIZE = 300;

						CSaturationModifiedProcessUpdateLinker* mModifiedUpdateLinker;

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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELSET_H
