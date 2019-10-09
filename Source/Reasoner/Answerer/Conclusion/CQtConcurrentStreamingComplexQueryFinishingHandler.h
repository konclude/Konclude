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

#ifndef KONCLUDE_REASONER_ANSWERER_CONCLUSION_CQTCONCURRENTSTREAMINGCOMPLEXQUERYFINISHINGHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_CONCLUSION_CQTCONCURRENTSTREAMINGCOMPLEXQUERYFINISHINGHANDLER_H

// Libraries includes
#include <QtConcurrent>

// Namespace includes
#include "ConclusionSettings.h"
#include "CSequentialStreamingComplexQueryFinishingHandler.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Conclusion {







				/*! 
				 *
				 *		\class		CQtConcurrentStreamingComplexQueryFinishingHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CQtConcurrentStreamingComplexQueryFinishingHandler : public CSequentialStreamingComplexQueryFinishingHandler {
					// public methods
					public:
						//! Constructor
						CQtConcurrentStreamingComplexQueryFinishingHandler(CComplexQueryProcessingData* queryProcessingData, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, bool confExtendedLogging, const QString& logDomain, cint64 confMaximumBatchMappingsComputationSize, cint64 confFirstBatchMappingsComputationSize, double confBatchMappingsComputationSizeIncreasingFactor, bool confContinueMappingsCompuationWhenResultsNotWriteable, cint64 confMappingsComputationUnlimitedInterpretationSize);

						virtual ~CQtConcurrentStreamingComplexQueryFinishingHandler();


					// protected methods
					protected:

						virtual void processUpdatedBindingsCardinalityLinkers();

						void handleBindingCardinalityLinkerBatches(const QVector<COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker*>& bindingsCardLinkerBatchVec, cint64 bindingCardLinkerCount);

					// protected variables
					protected:

						bool mAllowConcurrentAnswerGeneration;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CONCLUSION_CQTCONCURRENTSTREAMINGCOMPLEXQUERYFINISHINGHANDLER_H