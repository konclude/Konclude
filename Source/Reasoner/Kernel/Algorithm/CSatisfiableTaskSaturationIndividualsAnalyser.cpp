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

#include "CSatisfiableTaskSaturationIndividualsAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSatisfiableTaskSaturationIndividualsAnalyser::CSatisfiableTaskSaturationIndividualsAnalyser() {
				}


				CSatisfiableTaskSaturationIndividualsAnalyser::~CSatisfiableTaskSaturationIndividualsAnalyser() {
				}


				bool CSatisfiableTaskSaturationIndividualsAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CSaturationIndividualsAnalysingAdapter* satIndiAnAdapter = statCalcTask->getSaturationIndividualsAnalysationObserver();
					if (satIndiAnAdapter) {
						bool foundInsufficientHandledNode = false;
						bool foundClashedNode = false;
						CProcessingDataBox* procDataBox = statCalcTask->getProcessingDataBox();
						CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinker = procDataBox->getIndividualSaturationAnalysationNodeLinker();
						for (CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinker; indiSaturationAnalysingNodeLinkerIt; indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* satIndiNode = indiSaturationAnalysingNodeLinkerIt->getProcessingIndividual();
							CIndividualSaturationProcessNodeStatusFlags* indStatFlags = satIndiNode->getIndirectStatusFlags();
							CIndividualSaturationProcessNodeStatusFlags* dirFlags = satIndiNode->getDirectStatusFlags();
							if (indStatFlags->hasInsufficientFlag() || !indStatFlags->hasCompletedFlag() || !dirFlags->hasCompletedFlag()) {
								foundInsufficientHandledNode = true;
							}
							if (indStatFlags->hasClashedFlag()) {
								foundClashedNode = true;
							}
						}

						CSaturationIndividualAnalysationObserver* satIndAnalObs = satIndiAnAdapter->getIndividualsAnalysationObserver();
						if (satIndAnalObs) {
							if (foundInsufficientHandledNode) {
								satIndAnalObs->notifyInsufficientIndividual();
							}
							if (foundClashedNode) {
								satIndAnalObs->notifyClashedIndividual();
							}
						}
						return true;
					}
					return false;
				}



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
