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

#include "CAnalyseData.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseData::CAnalyseData(qint64 valueCount, CAnalyseDataDescription *dataDes) {
				valCount = valueCount;
				values = new CAnalyseValue *[valueCount];
				for (qint64 i = 0; i < valCount; ++i) {
					values[i] = 0;
				}

				description = dataDes;
			}



			CAnalyseData::~CAnalyseData() {
				for (qint64 i = 0; i < valCount; ++i) {
					if (values[i]) {
						delete values[i];
					}
				}
				if (values) {
					delete[] values;
				}
			}


			CAnalyseData *CAnalyseData::setValue(qint64 index, CAnalyseValue *takeValue) {
				if (index < 0 || index >= valCount) {
					// can not set value
				} else {
					if (values[index]) {
						delete values[index];
					}
					values[index] = takeValue;
				}
				return this;
			}



			CAnalyseDataDescription *CAnalyseData::getDataDescription() {
				return description;
			}

			CAnalyseData *CAnalyseData::setDataDescription(CAnalyseDataDescription *dataDes) {
				description = dataDes;
				return this;
			}

			CAnalyseValue *CAnalyseData::getValue(qint64 index) {
				if (index < 0 || index >= valCount) {
					return 0;
				}
				return values[index];
			}



			CAnalyseValue *CAnalyseData::createInstalledValue(CAnalyseUpdateDescription *valueDes) {
				CAnalyseValue *value = 0;
				qint64 index = valueDes->getValueIndex();
				value = getValue(index);
				if (!value) {					
					value = valueDes->createValue();
					setValue(index,value);
				}
				return value;
			}


			CAnalyseValue *CAnalyseData::createInstalledValue(qint64 valueDesIndex) {
				CAnalyseValue *value = 0;
				qint64 index = valueDesIndex;
				value = getValue(valueDesIndex);
				if (!value) {				
					CAnalyseUpdateDescription *valueDes = description->getUpdateDescription(valueDesIndex);
					if (valueDes) {
						value = valueDes->createValue();
						setValue(index,value);
					}
				}
				return value;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
