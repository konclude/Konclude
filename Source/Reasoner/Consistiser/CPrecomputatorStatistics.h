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

#ifndef KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATORSTATISTICS_H
#define KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATORSTATISTICS_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			/*! 
			 *
			 *		\class		CPrecomputatorStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPrecomputatorStatistics {
				// public methods
				public:
					//! Constructor
					CPrecomputatorStatistics(CPrecomputatorStatistics *nextPrecompuationStatistics = 0);



					qint64 getTotalSaturationTaskCount();
					qint64 getIndividualSaturationTaskCount();
					qint64 getIndividualPrecomputationTaskCount();



					CPrecomputatorStatistics *incTotalSaturationTaskCount(qint64 incCount = 1);
					CPrecomputatorStatistics *incIndividualSaturationTaskCount(qint64 incCount = 1);
					CPrecomputatorStatistics *incIndividualPrecomputationTaskCount(qint64 incCount = 1);

					CPrecomputatorStatistics *resetValues();
					CPrecomputatorStatistics *appendStatistics(CPrecomputatorStatistics *statistics);

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 mTotalSaturationTasks;
					qint64 mIndividualSaturationTasks;
					qint64 mIndividualPrecomputationTasks;


					CPrecomputatorStatistics *nextCompStat;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATORSTATISTICS_H
