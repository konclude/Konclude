/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_RENDERER_CONTOLOGYOWL2QTXMLRENDERER_H
#define KONCLUDE_RENDERER_CONTOLOGYOWL2QTXMLRENDERER_H

// Libraries includes


// Namespace includes
#include "RendererSettings.h"
#include "COntologyRenderer.h"
#include "COntologyEmptyDefaultRenderer.h"
#include "CRenderedQtXMLItemLinker.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		COntologyOWL2QtXMLRenderer
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyOWL2QtXMLRenderer : public COntologyEmptyDefaultRenderer {
			// public methods
			public:
				//! Constructor
				COntologyOWL2QtXMLRenderer();

				//! Destructor
				virtual ~COntologyOWL2QtXMLRenderer();


				virtual bool beginRender();
				virtual bool endRender(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name);

				CRenderedQtXMLItemLinker* createRenderedItemLinker();
				COntologyOWL2QtXMLRenderer* releaseRenderedItemLinkers(CRenderedItemLinker* linkers);


				bool addRenderedAxiomsToDomElement(QDomElement& domElement);


			// protected methods
			protected:

			// protected variables
			protected:
				QList<CRenderedQtXMLItemLinker*> mRenderedItemContainer;
				CRenderedQtXMLItemLinker* mTmpRenderedItemLinker;

				QDomDocument mDocument;

				CRenderedQtXMLItemLinker* mAxiomRenderedItemLinker;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CONTOLOGYOWL2QTXMLRENDERER_H
