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

#include "CAnalyseUpdateDescription.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseUpdateDescription::CAnalyseUpdateDescription() {
				valIdx = 0;
				valueDes = 0;
				updateReader = 0;
			}


			CAnalyseUpdateDescription::~CAnalyseUpdateDescription() {
			}


			CAnalyseValueUpdateReader *CAnalyseUpdateDescription::getUpdateReader() {
				return updateReader;
			}


			CAnalyseValueDescription *CAnalyseUpdateDescription::getValueDescription() {
				return valueDes;
			}

			CAnalyseUpdateDescription *CAnalyseUpdateDescription::setValueDescription(CAnalyseValueDescription *valDes) {
				valueDes = valDes;
				return this;
			}



			CAnalyseUpdateDescription *CAnalyseUpdateDescription::setUpdateReader(CAnalyseValueUpdateReader *analyseValueUpdateReader) {
				updateReader = analyseValueUpdateReader;
				return this;
			}


			qint64 CAnalyseUpdateDescription::getValueIndex() {
				return valIdx;
			}

			CAnalyseUpdateDescription *CAnalyseUpdateDescription::setValueIndex(qint64 index) {
				valIdx = index;
				return this;
			}


			CAnalyseValue *CAnalyseUpdateDescription::createValue() {
				CAnalyseValue *genValue = 0;
				if (valueDes) {
					genValue = valueDes->createValue();
				}
				return genValue;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
