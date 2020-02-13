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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H

// Libraries includes
#include <QtConcurrent>

// Namespace includes
#include "CompositionSettings.h"
#include "CAbstractVariableMappingsCompositionJoinComputator.h"
#include "CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker.h"
#include "CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker.h"
#include "CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker.h"
#include "CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector.h"

// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappingsMultiHash.h"



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"







namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CQtConcurrentVariableMappingsCompositionJoinComputator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CQtConcurrentVariableMappingsCompositionJoinComputator : public CAbstractVariableMappingsCompositionJoinComputator {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionJoinComputator();

						virtual CAbstractVariableMappingsCompositionComputator* configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAnswererContext* answererContext);



					// protected methods
					protected:

					// protected variables
					protected:
						virtual bool computeVariableMappingsJoinComposition(COptimizedComplexVariableJoiningItem* joiningItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing);

					// private methods
					private:
						cint64 mConfBatchSizeLimit = 200;
						cint64 mConfBatchSizeCount = 200;
						cint64 mConfConcurrentJoiningHashCount = 50;
						cint64 mConfConcurrentVariableMappingPartCount = 50;

						cint64 mConcurrentJoinComputationTaskCount = 50;


						bool mConfPartitionizedMemoryManagement = false;
						bool mConfCheckingSideDirectJoining = false;

						bool mConfPerformanceLogging = false;

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H
