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

#ifndef KONCLUDE_RENDERER_CONTOLOGYEMPTYDEFAULTRENDERER_H
#define KONCLUDE_RENDERER_CONTOLOGYEMPTYDEFAULTRENDERER_H

// Libraries includes


// Namespace includes
#include "RendererSettings.h"
#include "CRenderedItemLinker.h"
#include "COntologyRenderer.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		COntologyEmptyDefaultRenderer
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyEmptyDefaultRenderer : public COntologyRenderer {
			// public methods
			public:
				//! Constructor
				COntologyEmptyDefaultRenderer();

				virtual bool beginRender();
				virtual bool endRender(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderSubClassOfAxiom();
				virtual CRenderedItemLinker* endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDeclarationAxiom();
				virtual CRenderedItemLinker* endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderObjectComplementOfExpression();
				virtual CRenderedItemLinker* endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectIntersectionOfExpression();
				virtual CRenderedItemLinker* endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectUnionOfExpression();
				virtual CRenderedItemLinker* endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectSomeValuesFromExpression();
				virtual CRenderedItemLinker* endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectAllValuesFromExpression();
				virtual CRenderedItemLinker* endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderClassExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name);

				virtual bool beginRenderObjectPropertyExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name);

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

#endif // KONCLUDE_RENDERER_CONTOLOGYEMPTYDEFAULTRENDERER_H
