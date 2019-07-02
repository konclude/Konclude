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

#include "COntologyExtractionManager.h"


namespace Konclude {

	namespace Renderer {


		COntologyExtractionManager::COntologyExtractionManager() : CLogIdentifier("::Konclude::Renderer::OntologyExtractionManager",this) {
		}

		bool COntologyExtractionManager::realizeCommand(CCommand* command) {
			if (dynamic_cast<CExtractOntologyCommand*>(command)) {
				if (dynamic_cast<CExtractToldExpressionsOntologyCommand*>(command)) {
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					CExtractToldExpressionsOntologyCommand* extToldExpOntC = (CExtractToldExpressionsOntologyCommand*)command;
					COntologyRevision* ontoRev = extToldExpOntC->getOntologyRevision();
					COntologyRenderer* renderer = extToldExpOntC->getRenderer();
					QStringList extEntityList = extToldExpOntC->getExtractionEntityList();

					COntologyParsedBuildExpressionRenderVisitor extractor;
					extractor.visitAndRender(extEntityList,ontoRev->getOntology(),renderer);

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
					return true;
				}
			}
			return false;
		}

	}; // end namespace Renderer

}; // end namespace Konclude
