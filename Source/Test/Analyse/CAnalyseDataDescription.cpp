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

#include "CAnalyseDataDescription.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseDataDescription::CAnalyseDataDescription() {
			}


			CAnalyseDataDescription::~CAnalyseDataDescription() {
				qDeleteAll(valueUpdateDesVec);
			}


			QString CAnalyseDataDescription::getDataName() {
				return dataName;
			}

			CAnalyseDataDescription *CAnalyseDataDescription::setDataName(const QString &name) {
				dataName = name;
				return this;
			}


			CAnalyseValueDescription *CAnalyseDataDescription::getValueDescription(const QString &valueName) {
				qint64 index = desNameIndexHash.value(valueName,-1);
				return getValueDescription(index);
			}


			CAnalyseValueDescription *CAnalyseDataDescription::getValueDescription(qint64 index) {
				if (index < 0 || index >= valueUpdateDesVec.count()) {
					return 0;
				}
				CAnalyseValueDescription *valueDes = 0;
				CAnalyseUpdateDescription *analyseUpdateDes = valueUpdateDesVec[index];
				if (analyseUpdateDes) {
					valueDes = analyseUpdateDes->getValueDescription();
				}
				return valueDes;
			}

			CAnalyseUpdateDescription *CAnalyseDataDescription::getUpdateDescription(qint64 index) {
				if (index < 0 || index >= valueUpdateDesVec.count()) {
					return 0;
				}
				CAnalyseUpdateDescription *analyseUpdateDes = valueUpdateDesVec[index];
				return analyseUpdateDes;
			}


			CAnalyseUpdateDescription *CAnalyseDataDescription::installValueDescription(CAnalyseValueDescription *valueDescription, CAnalyseValueUpdateReader *updateReader) {
				qint64 nextIdx = valueUpdateDesVec.count();
				QString name = valueDescription->getValueName();
				desNameIndexHash.insert(name,nextIdx);
				CAnalyseUpdateDescription *updDes = new CAnalyseUpdateDescription();
				updDes->setValueDescription(valueDescription);
				updDes->setValueIndex(nextIdx);
				updDes->setUpdateReader(updateReader);
				valueUpdateDesVec.append(updDes);
				return updDes;
			}


			qint64 CAnalyseDataDescription::getAnalyseValueCount() {
				return valueUpdateDesVec.count();
			}


			CAnalyseData *CAnalyseDataDescription::createData() {
				CAnalyseData *data = new CAnalyseData(getAnalyseValueCount(),this);
				return data;
			}



		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
