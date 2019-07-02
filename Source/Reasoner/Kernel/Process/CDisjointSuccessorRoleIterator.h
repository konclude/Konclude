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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDISJOINTSUCCESSORROLEITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDISJOINTSUCCESSORROLEITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CNegationDisjointEdge.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDisjointSuccessorRoleIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDisjointSuccessorRoleIterator {
					// public methods
					public:
						//! Constructor
						CDisjointSuccessorRoleIterator();
						CDisjointSuccessorRoleIterator(cint64 succIndi, const CPROCESSHASH<CRole*,CNegationDisjointEdge*>::iterator& beginIt, const CPROCESSHASH<CRole*,CNegationDisjointEdge*>::iterator& endIt);

						bool hasNext();
						CNegationDisjointEdge* next(bool moveNext = true);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mSuccIndi;
						CPROCESSHASH<CRole*,CNegationDisjointEdge*>::iterator mBeginIt;
						CPROCESSHASH<CRole*,CNegationDisjointEdge*>::iterator mEndIt;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDISJOINTSUCCESSORROLEITERATOR_H
