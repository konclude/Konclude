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

#include "CReasonerEvaluationAnalyseContext.h"
#include "CReasonerEvaluationGNUPlotPlotter.h"
#include "CReasonerEvaluationHighchartPlotter.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationAnalyseContext::CReasonerEvaluationAnalyseContext(CConfiguration* configuration, const QString& additionalTitleString, const QString& additionalfileString) {
				mConfiguration = nullptr;
				mGNUPlotPlotter = new CReasonerEvaluationGNUPlotPlotter(this);
				mHighchartPlotter = new CReasonerEvaluationHighchartPlotter(this);
				mAdditionalTitleString = additionalTitleString;
				mAdditionalFileString = additionalfileString;

				mGNUPlot = false;
				mHighchartPlot = true;

				if (mConfiguration) {
					mGNUPlot = CConfigDataReader::readConfigBoolean(mConfiguration,"Konclude.Evaluation.Analyser.CreateGNUPlots");
					mHighchartPlot = CConfigDataReader::readConfigBoolean(mConfiguration,"Konclude.Evaluation.Analyser.CreateGNUPlots");
				}
			}


			CReasonerEvaluationAnalyseContext::~CReasonerEvaluationAnalyseContext() {
				delete mGNUPlotPlotter;
			}

			CMemoryAllocationManager* CReasonerEvaluationAnalyseContext::getMemoryAllocationManager() {
				return nullptr;
			}

			CConfiguration* CReasonerEvaluationAnalyseContext::getConfiguration() {
				return mConfiguration;
			}

			CReasonerEvaluationGNUPlotPlotter* CReasonerEvaluationAnalyseContext::getGNUPlotPlotter() {
				return mGNUPlotPlotter;
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationAnalyseContext::getHighchartPlotter() {
				return mHighchartPlotter;
			}

			QString CReasonerEvaluationAnalyseContext::getAdditionalTitleString() {
				return mAdditionalTitleString;
			}


			QString CReasonerEvaluationAnalyseContext::getAdditionalFileString() {
				return mAdditionalFileString;
			}

			bool CReasonerEvaluationAnalyseContext::isGNUPlottingActivated() {
				return mGNUPlot;
			}

			bool CReasonerEvaluationAnalyseContext::isHighchartPlottingActivated() {
				return mHighchartPlot;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
