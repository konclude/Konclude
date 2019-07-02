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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSETITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSETITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptorDependencyReapplyData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReapplyConceptLabelSetIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyConceptLabelSetIterator {
					// public methods
					public:
						//! Constructor
						CReapplyConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& beginIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator& endIt, bool skipEmptyConceptDescriptors = true);

						cint64 getRemainingConceptCount();

						CConceptDescriptor* getConceptDescriptor();
						cint64 getDataTag();
						CDependencyTrackPoint* getDependencyTrackPoint();		
						const CCondensedReapplyQueue *getPosNegReapplyQueue();


						CReapplyConceptLabelSetIterator& operator++();
						CConceptDescriptor* operator*();
						CReapplyConceptLabelSetIterator* moveNext();
						bool hasValue();
						bool hasNext();
						CConceptDescriptor* next(bool moveToNext = true);

						bool operator!=(CReapplyConceptLabelSetIterator& iterator);
						bool operator==(CReapplyConceptLabelSetIterator& iterator);


					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator mConDesDepBeginIt;
						CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator mConDesDepEndIt;
						CConceptDescriptor* mConceptDesLinkerIt;
						cint64 mConceptCount;
						bool mSkipEmptyConceptDescriptors;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSETITERATOR_H
