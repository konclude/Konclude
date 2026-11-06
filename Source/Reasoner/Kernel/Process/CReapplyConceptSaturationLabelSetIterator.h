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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELSETITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELSETITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptorDependencyReapplyData.h"
#include "CConceptSaturationDescriptorReapplyData.h"
#include "CReapplyConceptSaturationLabelHash.h"
#include "CConceptSaturationDescriptor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReapplyConceptSaturationLabelSetIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyConceptSaturationLabelSetIterator {
					// public methods
					public:
						//! Constructor
						CReapplyConceptSaturationLabelSetIterator(const CReapplyConceptSaturationLabelHash::const_iterator& itBegin, const CReapplyConceptSaturationLabelHash::const_iterator& itEnd, const CReapplyConceptSaturationLabelHash::const_iterator& itAdditionalBegin, const CReapplyConceptSaturationLabelHash::const_iterator& itAdditionalEnd, bool iterateConSatDes = true, bool iterateReapplies = true);


						cint64 getDataTag();
						CConceptSaturationDescriptor* getConceptSaturationDescriptor();
						CImplicationReapplyConceptSaturationDescriptor* getImplicationReapplyConceptSaturationDescriptor();


						bool hasNext();
						cint64 next(bool moveToNext = true);
						CReapplyConceptSaturationLabelSetIterator* moveNext();

						bool operator!=(CReapplyConceptSaturationLabelSetIterator& const_iterator);
						bool operator==(CReapplyConceptSaturationLabelSetIterator& const_iterator);


					// protected methods
					protected:
						bool isIteratorValid();
						bool isIteratorValid(bool allowConSatDes, bool allowReapplies);

					// protected variables
					protected:
						CReapplyConceptSaturationLabelHash::const_iterator mItBegin;
						CReapplyConceptSaturationLabelHash::const_iterator mItEnd;
						CReapplyConceptSaturationLabelHash::const_iterator mItAdditionalBegin;
						CReapplyConceptSaturationLabelHash::const_iterator mItAdditionalEnd;

						bool mIterateConSatDes;
						bool mIterateReapplies;
						cint64 mMaxSaturationDepthIterate;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTSATURATIONLABELSETITERATOR_H
