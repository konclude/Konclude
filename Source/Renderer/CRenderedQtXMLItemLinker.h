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

#ifndef KONCLUDE_RENDERER_CRENDEREDQTXMLITEMLINKER_H
#define KONCLUDE_RENDERER_CRENDEREDQTXMLITEMLINKER_H

// Libraries includes
#include <QDomElement>

// Namespace includes
#include "RendererSettings.h"
#include "CRenderedItemLinker.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		CRenderedQtXMLItemLinker
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CRenderedQtXMLItemLinker : public CRenderedItemLinker {
			// public methods
			public:
				//! Constructor
				CRenderedQtXMLItemLinker();

				QDomElement getDomElement();
				QDomElement takeDomElement();
				CRenderedQtXMLItemLinker* setDomElement(const QDomElement& domElement);
				CRenderedQtXMLItemLinker* clearDomElement();

				CRenderedQtXMLItemLinker* getNextItemLinker();

			// protected methods
			protected:

			// protected variables
			protected:
				QDomElement mDomElement;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CRENDEREDQTXMLITEMLINKER_H
