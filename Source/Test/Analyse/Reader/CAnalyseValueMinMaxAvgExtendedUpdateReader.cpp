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

#include "CAnalyseValueMinMaxAvgExtendedUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CAnalyseValueMinMaxAvgExtendedUpdateReader::CAnalyseValueMinMaxAvgExtendedUpdateReader(CAnalyseValueDescription *valueDes, CAnalyseValueUpdateReader *updateReader) {
					reader = updateReader;
					valueDescription = valueDes;
					hasMinMaxAvg = false;
					valCount = 0;

					minValueDescription = 0;
					maxValueDescription = 0;
					avgValueDescription = 0;

					if (valueDescription) {
						minValueDescription = new CAnalyseValueDescription();
						minValueDescription->setValueName(valueDescription->getValueName()+QString("-Minimum"));
						minValueDescription->setTemplateValue(new CDoubleAnalyseValue(0.));

						maxValueDescription = new CAnalyseValueDescription();
						maxValueDescription->setValueName(valueDescription->getValueName()+QString("-Maximum"));
						maxValueDescription->setTemplateValue(new CDoubleAnalyseValue(0.));

						avgValueDescription = new CAnalyseValueDescription();
						avgValueDescription->setValueName(valueDescription->getValueName()+QString("-Average"));
						avgValueDescription->setTemplateValue(new CDoubleAnalyseValue(0.));

					}
				}

				CAnalyseValueMinMaxAvgExtendedUpdateReader::~CAnalyseValueMinMaxAvgExtendedUpdateReader() {
					if (avgValueDescription) {
						delete avgValueDescription;
					}
					if (maxValueDescription) {
						delete maxValueDescription;
					}
					if (minValueDescription) {
						delete minValueDescription;
					}
				}



				CAnalyseValueDescription *CAnalyseValueMinMaxAvgExtendedUpdateReader::getMinValueDescription() {
					return minValueDescription;
				}

				CAnalyseValueDescription *CAnalyseValueMinMaxAvgExtendedUpdateReader::getMaxValueDescription() {
					return maxValueDescription;
				}

				CAnalyseValueDescription *CAnalyseValueMinMaxAvgExtendedUpdateReader::getAvgValueDescription() {
					return avgValueDescription;
				}



				CAnalyseValue *CAnalyseValueMinMaxAvgExtendedUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;
					if (valueDes == valueDescription) {
						if (reader) {
							val = reader->getAnalyseValueUpdate(valueDes);
						}
						CConvertDoubleAnalyseValue *douVal = dynamic_cast<CConvertDoubleAnalyseValue *>(val);
						if (douVal) {
							double mDouVal = douVal->getDouble();
							if (!hasMinMaxAvg) {
								avgVal = mDouVal;
								maxVal = mDouVal;
								minVal = mDouVal;
								valCount++;
							} else {
								maxVal = qMax(maxVal,mDouVal);
								minVal = qMin(minVal,mDouVal);
								avgVal = avgVal + 1./(valCount+1.) * (mDouVal - avgVal);
								valCount++;
							}
						}
					} else if (valueDes == minValueDescription) {
						if (hasMinMaxAvg) {
							val = new CDoubleAnalyseValue(minVal);
						}
					} else if (valueDes == maxValueDescription) {
						if (hasMinMaxAvg) {
							val = new CDoubleAnalyseValue(maxVal);
						}
					} else if (valueDes == avgValueDescription) {
						if (hasMinMaxAvg) {
							val = new CDoubleAnalyseValue(avgVal);
						}
					}
					return val;
				}

			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
