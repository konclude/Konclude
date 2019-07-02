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

#ifndef KONCLUDE_RENDERER_CONTOLOGYRENDERER_H
#define KONCLUDE_RENDERER_CONTOLOGYRENDERER_H

// Libraries includes


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
		*		\class		COntologyRenderer
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyRenderer {
			// public methods
			public:
				//! Constructor
				COntologyRenderer();

				virtual bool beginRender() = 0;
				virtual bool endRender(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderSubClassOfAxiom() = 0;
				virtual CRenderedItemLinker* endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDeclarationAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderObjectComplementOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectIntersectionOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectUnionOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectSomeValuesFromExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectAllValuesFromExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderClassExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name) = 0;

				virtual bool beginRenderObjectPropertyExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name) = 0;

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

#endif // KONCLUDE_RENDERER_CONTOLOGYRENDERER_H
