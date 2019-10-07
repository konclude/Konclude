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

#include "CMinMaxAvgCountValueAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CMinMaxAvgCountValueAnalyser::CMinMaxAvgCountValueAnalyser() {
				valCount = 0;
				minVal = 1.7976931348623158e+308;
				maxVal = -1.7976931348623158e+307;
				avgVal = 0;

				valCountAnaVal = 0;
				maxValAnaVal = 0;
				minValAnaVal = 0;
				avgValAnaVal = 0;
			}



			CMinMaxAvgCountValueAnalyser::~CMinMaxAvgCountValueAnalyser() {
				if (valCountAnaVal) {
					delete valCountAnaVal;
					delete maxValAnaVal;
					delete minValAnaVal;
					delete avgValAnaVal;
				} 
			}


			double CMinMaxAvgCountValueAnalyser::getMaxValue() {
				return maxVal;
			}

			double CMinMaxAvgCountValueAnalyser::getAvgValue() {
				return avgVal;
			}

			double CMinMaxAvgCountValueAnalyser::getMinValue() {
				return minVal;
			}

			qint64 CMinMaxAvgCountValueAnalyser::getValueCount() {
				return valCount;
			}



			CValueAnalyser *CMinMaxAvgCountValueAnalyser::analyseValue(double douVal) {
				if (valCount > 0) {
					valCount++;
					maxVal = qMax(douVal,maxVal);
					minVal = qMin(douVal,minVal);
					avgVal = avgVal + (1./valCount) * (douVal - avgVal);
				} else {
					avgVal = douVal;
					minVal = douVal;
					maxVal = douVal;
					valCount++;
				}
				if (!valCountAnaVal) {
					valCountAnaVal = new CIntegerAnalyseValue(valCount);
					maxValAnaVal = new CDoubleAnalyseValue(maxVal);
					minValAnaVal = new CDoubleAnalyseValue(minVal);
					avgValAnaVal = new CDoubleAnalyseValue(avgVal);
				} else {
					valCountAnaVal->parseFromInteger(valCount);
					maxValAnaVal->parseFromDouble(maxVal);
					minValAnaVal->parseFromDouble(minVal);
					avgValAnaVal->parseFromDouble(avgVal);
				}
				return this;
			}


			CValueAnalyser *CMinMaxAvgCountValueAnalyser::analyseValue(CAnalyseValue *value) {
				if (value) {
					CConvertDoubleAnalyseValue *douAnData = dynamic_cast<CConvertDoubleAnalyseValue *>(value);
					if (douAnData) {
						double douVal = douAnData->getDouble();
						analyseValue(douVal);
					}
				}
				return this;
			}


			CDoubleAnalyseValue *CMinMaxAvgCountValueAnalyser::getMaxAnalyseValue() {
				return maxValAnaVal;
			}

			CDoubleAnalyseValue *CMinMaxAvgCountValueAnalyser::getAvgAnalyseValue() {
				return avgValAnaVal;
			}

			CDoubleAnalyseValue *CMinMaxAvgCountValueAnalyser::getMinAnalyseValue() {
				return minValAnaVal;
			}


			CIntegerAnalyseValue *CMinMaxAvgCountValueAnalyser::getAnalyseValueCount() {
				return valCountAnaVal;
			}





		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
