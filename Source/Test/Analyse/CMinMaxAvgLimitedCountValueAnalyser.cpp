/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CMinMaxAvgLimitedCountValueAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CMinMaxAvgLimitedCountValueAnalyser::CMinMaxAvgLimitedCountValueAnalyser(qint64 valueLimit) {
				valLimit = valueLimit;
			}



			CMinMaxAvgLimitedCountValueAnalyser::~CMinMaxAvgLimitedCountValueAnalyser() {
			}



			CMinMaxAvgLimitedCountValueAnalyser *CMinMaxAvgLimitedCountValueAnalyser::setValueLimit(qint64 valueLimit) {
				valLimit = valueLimit;
				return this;
			}

			qint64 CMinMaxAvgLimitedCountValueAnalyser::getValueLimit() {
				return valLimit;
			}




			CValueAnalyser *CMinMaxAvgLimitedCountValueAnalyser::analyseValue(double douVal) {
				if (valCount > 0) {
					valCount++;
					maxVal = qMax(douVal,maxVal);
					minVal = qMin(douVal,minVal);
					avgVal = avgVal + (1./(valCount+1.)) * (douVal - avgVal);
				} else {
					avgVal = douVal;
					minVal = douVal;
					maxVal = douVal;
					valCount++;
				}
				values.prepend(douVal);

				if (valCount > valLimit) {
					double lastVal = values.takeLast();
					avgVal = avgVal - (1./(valCount-1.)) * (lastVal - avgVal);
					valCount--;
					if (lastVal == minVal || lastVal == maxVal) {
						// search new min and max
						double newMin = 1.7976931348623158e+308;
						double newMax = -1.7976931348623158e+307;
						for (QLinkedList<double>::const_iterator it = values.begin(); it != values.end(); ++it) {
							double val = (*it);
							newMax = qMax(val,newMax);
							newMin = qMin(val,newMin);
						}	
						minVal = newMin;
						maxVal = newMax;
					}
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




		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
