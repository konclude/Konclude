/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDISTINCTHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDISTINCTHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CDistinctIterator.h"
#include "CDistinctEdge.h"


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
				 *		\class		CDistinctHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDistinctHash : public CPROCESSHASH<cint64,CDistinctEdge*> {
					// public methods
					public:
						//! Constructor
						CDistinctHash(CContext* context = nullptr);
						CDistinctHash(const CDistinctHash& disHash, CContext* context = nullptr);

						//! Destructor
						virtual ~CDistinctHash();

						CDistinctHash* initDistinctHash(CDistinctHash* prevHash);

						bool isIndividualDistinct(cint64 indiID);
						CDistinctEdge* getIndividualDistinctEdge(cint64 indiID);

						CDistinctHash* insertDistinctIndividual(cint64 indiID, CDistinctEdge* disEdge = nullptr);
						CDistinctHash* removeDistinctIndividual(cint64 indiID);

						cint64 getDistinctCount();

						CDistinctIterator getDistinctIterator();


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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDISTINCTHASH_H
