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

#include "CDifferenceValueAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CDifferenceValueAnalyser::CDifferenceValueAnalyser() {
				lastValue = 0;
				difference = 0;
				hasDifference = false;
				hasLastValue = false;

				diffAnaVal = 0;
			}



			CDifferenceValueAnalyser::~CDifferenceValueAnalyser() {
				if (diffAnaVal) {
					delete diffAnaVal;
				}
			}


			double CDifferenceValueAnalyser::getDifferenceValue() {
				return difference;
			}

			bool CDifferenceValueAnalyser::hasDiferenceValue() {
				return hasDifference;
			}


			CDoubleAnalyseValue *CDifferenceValueAnalyser::getDifferenceAnalyseValue() {
				return diffAnaVal;
			}



			CValueAnalyser *CDifferenceValueAnalyser::analyseValue(double douVal) {
				if (hasLastValue) {
					difference = douVal - lastValue;
					hasDifference = true;
					if (!diffAnaVal) {
						diffAnaVal = new CDoubleAnalyseValue(difference);
					} else {
						diffAnaVal->parseFromDouble(difference);
					}
				}
				lastValue = douVal;
				hasLastValue = true;
				return this;
			}


			CValueAnalyser *CDifferenceValueAnalyser::analyseValue(CAnalyseValue *value) {
				if (value) {
					CConvertDoubleAnalyseValue *douAnData = dynamic_cast<CConvertDoubleAnalyseValue *>(value);
					if (douAnData) {
						double douVal = douAnData->getDouble();
						analyseValue(douVal);
					}
				}
				return this;
			}



		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
