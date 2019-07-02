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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSUCCESSORROLEITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSUCCESSORROLEITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualLinkEdge.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CSuccessorRoleIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSuccessorRoleIterator {
					// public methods
					public:
						//! Constructor
						CSuccessorRoleIterator();

						CSuccessorRoleIterator(cint64 indi, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& endIt);

						bool hasNext();
						CIndividualLinkEdge* next(bool moveNext = true);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mIndi;
						CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator mBeginIt;
						CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator mEndIt;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSUCCESSORROLEITERATOR_H
