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

#ifndef KONCLUDE_REASONER_CONSISTENCE_CPRECOMPUTATION_H
#define KONCLUDE_REASONER_CONSISTENCE_CPRECOMPUTATION_H

// Libraries includes
#include <atomic>


// Namespace includes
#include "ConsistenceSettings.h"
#include "CSaturationData.h"
#include "CPrecomputationStatistics.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistence {

			/*! 
			 *
			 *		\class		CPrecomputation
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPrecomputation {
				// public methods
				public:
					//! Constructor
					CPrecomputation();
					virtual ~CPrecomputation();


					bool isPrecomputed();
					CPrecomputation* setPrecomputed(bool precomputed);

					CSaturationData* getSaturationModelData();
					bool hasSaturationModelData();
					CPrecomputation* setSaturationModelData(CSaturationData* saturationData);

					CPrecomputationStatistics* getPrecomputationStatistics();
					CPrecomputation* setPrecomputationStatistics(CPrecomputationStatistics* statColl);

					/**
					 * The progress of the saturation of this ontology, for a progress report from
					 * another thread while the saturation runs, see CJNIQueryProcessor::queryReasoningProgress:
					 * the saturation nodes that have been initialised, counted by the saturation
					 * algorithm once per node when it initialises it, and the nodes that exist, the
					 * largest node count seen, which grows while the saturation creates successors.
					 * Nothing else in the precomputation is counted, the saturation is what takes the time.
					 */
					CPrecomputation* incInitializedSaturationNodeCount(cint64 count = 1);
					CPrecomputation* updateSaturationNodeCount(cint64 nodeCount);
					cint64 getInitializedSaturationNodeCount();
					cint64 getSaturationNodeCount();

				// protected methods
				protected:

				// protected variables
				protected:
					bool mPrecomputed;
					CSaturationData* mSaturData;

					CPrecomputationStatistics* mStatsColl;

					std::atomic<cint64> mInitializedSaturationNodeCount;
					std::atomic<cint64> mSaturationNodeCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTENCE_CPRECOMPUTATION_H
