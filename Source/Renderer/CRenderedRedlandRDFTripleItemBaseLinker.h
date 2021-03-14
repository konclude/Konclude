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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_RENDERER_CRENDEREDREDLANDRDFTRIPLEITEMBASELINKER_H
#define KONCLUDE_RENDERER_CRENDEREDREDLANDRDFTRIPLEITEMBASELINKER_H

// Libraries includes
#include <QDomElement>

// Namespace includes
#include "RendererSettings.h"
#include "CRenderedRDFTripleItemBaseLinker.h"


// Other includes
#include <redland.h>


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		CRenderedRedlandRDFTripleItemBaseLinker
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CRenderedRedlandRDFTripleItemBaseLinker : public CRenderedRDFTripleItemBaseLinker {
			// public methods
			public:
				//! Constructor
				CRenderedRedlandRDFTripleItemBaseLinker(librdf_statement* tripleStatement = nullptr);

				librdf_statement* getRedlandTripleStatement();
				CRenderedRedlandRDFTripleItemBaseLinker* setRedlandTripleStatement(librdf_statement* tripleStatement);


				CRenderedRedlandRDFTripleItemBaseLinker* clearRedlandTripleStatement();


			// protected methods
			protected:

			// protected variables
			protected:
				librdf_statement* mTripleStatement;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CRENDEREDREDLANDRDFTRIPLEITEMBASELINKER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
