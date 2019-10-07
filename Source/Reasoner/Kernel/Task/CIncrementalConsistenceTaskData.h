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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CINCREMENTALCONSISTENCETASKDATA_H
#define KONCLUDE_REASONER_KERNEL_TASK_CINCREMENTALCONSISTENCETASKDATA_H

// Libraries includes


// Namespace includes
#include "CSatisfiableCalculationTask.h"
#include "CConsistenceTaskData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CIncrementalConsistenceTaskData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIncrementalConsistenceTaskData : public CConsistenceTaskData {
					// public methods
					public:
						//! Constructor
						CIncrementalConsistenceTaskData(CSatisfiableCalculationTask* detSatTask, CSatisfiableCalculationTask* graphCachedSatTask, CConcreteOntology* prevOntology, CConsistenceTaskData* prevConsData);

						//! Destructor
						virtual ~CIncrementalConsistenceTaskData();

						QSet<cint64>* getIndirectlyChangedNodeSet();
						QSet<cint64>* getDeterministicallyChangedNodeSet();
						QSet<cint64>* getChangedCompatibleNodeSet();

						CConcreteOntology* getPreviousOntology();
						CConsistenceTaskData* getPreviousConsistenceData();

						CIncrementalConsistenceTaskData* setPreviousOntology(CConcreteOntology* prevOntology);
						CIncrementalConsistenceTaskData* setPreviousConsistenceData(CConsistenceTaskData* prevConsData);


						cint64 getAddedNodeCount();
						cint64 getTotalNodeCount();
						cint64 getChangedCompatibleNodeCount();
						cint64 getDeterministicallyChangedNodeCount();
						cint64 getIndirectlyChangedCount();
						cint64 getPreviousNodeCount();


						CIncrementalConsistenceTaskData* setAddedNodeCount(cint64 addedNodeCount);
						CIncrementalConsistenceTaskData* setTotalNodeCount(cint64 totalNodeCount);
						CIncrementalConsistenceTaskData* setPreviousNodeCount(cint64 prevNodeCount);

					// protected methods
					protected:

					// protected variables
					protected:
						QSet<cint64> mIndirectlyChangedNodeSet;
						QSet<cint64> mDeterministicallyChangedNodeSet;
						QSet<cint64> mChangedCompatibleNodeSet;

						CConcreteOntology* mPrevOntology;
						CConsistenceTaskData* mPrevConsData;

						cint64 mAddedNodeCount;
						cint64 mTotalNodeCount;
						cint64 mPreviousNodeCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CINCREMENTALCONSISTENCETASKDATA_H
