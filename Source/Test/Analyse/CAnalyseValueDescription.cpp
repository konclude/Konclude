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

#include "CAnalyseValueDescription.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseValueDescription::CAnalyseValueDescription() {
				templateValue = 0;
			}


			CAnalyseValueDescription::~CAnalyseValueDescription() {
				if (templateValue) {
					delete templateValue;
				}
			}



			QString CAnalyseValueDescription::getValueName() {
				return valName;
			}

			CAnalyseValueDescription *CAnalyseValueDescription::setValueName(const QString &valueName) {
				valName = valueName;
				return this;
			}



			CAnalyseValue *CAnalyseValueDescription::getTemplateValue() {
				return templateValue;
			}

			CAnalyseValueDescription *CAnalyseValueDescription::setTemplateValue(CAnalyseValue *takeTemplateValue) {
				templateValue = takeTemplateValue;
				return this;
			}



			CAnalyseValue *CAnalyseValueDescription::createValue() {
				CAnalyseValue *genValue = 0;
				if (templateValue) {
					genValue = templateValue->createCopy();
				}
				return genValue;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
