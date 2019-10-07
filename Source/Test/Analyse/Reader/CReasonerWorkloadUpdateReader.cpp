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

#include "CReasonerWorkloadUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CReasonerWorkloadUpdateReader::CReasonerWorkloadUpdateReader(CAnalyseReasonerManager  *reasonerManager) {
					reasoner = reasonerManager;

					queryProgressDescription = 0;
					queryTotalWorkDescription = 0;
					queryClosedWorkDescription = 0;

					if (reasoner) {
						queryProgressDescription = new CAnalyseValueDescription();
						queryProgressDescription->setValueName(QString("ReasonerManager::QueryProgress"));
						queryProgressDescription->setTemplateValue(new CDoubleAnalyseValue(0.));

						queryTotalWorkDescription = new CAnalyseValueDescription();
						queryTotalWorkDescription->setValueName(QString("ReasonerManager::QueryTotalWorkCount"));
						queryTotalWorkDescription->setTemplateValue(new CDoubleAnalyseValue(0.));

						queryClosedWorkDescription = new CAnalyseValueDescription();
						queryClosedWorkDescription->setValueName(QString("ReasonerManager::QueryCalculatedWorkCount"));
						queryClosedWorkDescription->setTemplateValue(new CDoubleAnalyseValue(0.));
					}
				}

				CReasonerWorkloadUpdateReader::~CReasonerWorkloadUpdateReader() {
					if (queryProgressDescription) {
						delete queryProgressDescription;
					}
					if (queryTotalWorkDescription) {
						delete queryTotalWorkDescription;
					}
					if (queryClosedWorkDescription) {
						delete queryClosedWorkDescription;
					}
				}



				CAnalyseValueDescription *CReasonerWorkloadUpdateReader::getQueryTotalWorkCountValueDescription() {
					return queryTotalWorkDescription;
				}


				CAnalyseValueDescription *CReasonerWorkloadUpdateReader::getQueryCalcedWorkCountValueDescription() {
					return queryClosedWorkDescription;
				}


				CAnalyseValueDescription *CReasonerWorkloadUpdateReader::getQueryProgressValueDescription() {
					return queryProgressDescription;
				}




				CAnalyseValue *CReasonerWorkloadUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == queryProgressDescription && reasoner) {
						val = new CDoubleAnalyseValue(reasoner->getQueryProgress());
					} else if (valueDes == queryTotalWorkDescription && reasoner) {
						val = new CDoubleAnalyseValue(reasoner->getQueryTotalWorkCount());
					} else if (valueDes == queryClosedWorkDescription && reasoner) {
						val = new CDoubleAnalyseValue(reasoner->getQueryCalculatedWorkCount());
					} 

					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
