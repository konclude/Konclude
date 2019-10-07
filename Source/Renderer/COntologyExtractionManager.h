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

#ifndef KONCLUDE_RENDERER_CONTOLOGYEXTRACTIONMANAGER_H
#define KONCLUDE_RENDERER_CONTOLOGYEXTRACTIONMANAGER_H

// Libraries includes


// Namespace includes
#include "RendererSettings.h"
#include "CRenderedItemLinker.h"
#include "COntologyParsedBuildExpressionRenderVisitor.h"


// Other includes
#include "Control/Command/CCommand.h"
#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Instructions/CExtractToldExpressionsOntologyCommand.h"
#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CFinishProcessCommandRecord.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

using namespace Konclude::Logger;

namespace Konclude {

	using namespace Control;
	using namespace Command;
	using namespace Records;
	using namespace Instructions;

	namespace Renderer {



		/*! 
		*
		*		\class		COntologyExtractionManager
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyExtractionManager : public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				COntologyExtractionManager();

                                virtual bool realizeCommand(CCommand* command);

			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CONTOLOGYEXTRACTIONMANAGER_H
