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

#include "CPropagationBindingMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationBindingMap::CPropagationBindingMap(CProcessContext* processContext) : mProcessContext(processContext),CPROCESSMAP<cint64,CPropagationBindingMapData>(processContext) {
				}

				CPropagationBindingMap* CPropagationBindingMap::initPropagationBindingMap(CPropagationBindingMap* prevMap) {
					if (prevMap) {
						CPROCESSMAP<cint64, CPropagationBindingMapData>::operator=(*prevMap);
					} else {
						CPROCESSMAP<cint64,CPropagationBindingMapData>::clear();
					}
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
