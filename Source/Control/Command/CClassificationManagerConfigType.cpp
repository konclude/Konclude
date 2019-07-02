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

#include "CClassificationManagerConfigType.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			CClassificationManagerConfigType::CClassificationManagerConfigType(CClassificationManager *classificationManager) {
				cManager = classificationManager;
			}


			CClassificationManagerConfigType::~CClassificationManagerConfigType() {
			}


			CClassificationManagerConfigType *CClassificationManagerConfigType::getTypeCopy() {
				return new CClassificationManagerConfigType(cManager);
			}


			CClassificationManager *CClassificationManagerConfigType::getClassificationManager() {
				return cManager;
			}

			CClassificationManagerConfigType *CClassificationManagerConfigType::setClassificationManager(CClassificationManager *classificationManager) {
				cManager = classificationManager;
				return this;
			}




			bool CClassificationManagerConfigType::equals(CConfigType *dataType) {
				CClassificationManagerConfigType *rCommanderType = dynamic_cast<CClassificationManagerConfigType *>(dataType);
				if (rCommanderType) {
					return (cManager == rCommanderType->cManager);
				}
				return false;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
