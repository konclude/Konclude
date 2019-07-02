/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "COntologyOWL2QtXMLRenderer.h"


namespace Konclude {

	namespace Renderer {


		COntologyOWL2QtXMLRenderer::COntologyOWL2QtXMLRenderer() {
			mAxiomRenderedItemLinker = nullptr;
			mTmpRenderedItemLinker = nullptr;
		}

		COntologyOWL2QtXMLRenderer::~COntologyOWL2QtXMLRenderer() {
			qDeleteAll(mRenderedItemContainer);
		}


		bool COntologyOWL2QtXMLRenderer::addRenderedAxiomsToDomElement(QDomElement& domElement) {
			bool added = false;
			for (CRenderedQtXMLItemLinker* linkerIt = mAxiomRenderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				domElement.appendChild(linkerIt->getDomElement());
				added = true;
			}
			return added;
		}


		bool COntologyOWL2QtXMLRenderer::beginRender() {
			return true;
		}

		bool COntologyOWL2QtXMLRenderer::endRender(CRenderedItemLinker* renderedItemLinker) {
			mAxiomRenderedItemLinker = (CRenderedQtXMLItemLinker*)renderedItemLinker;
			return true;
		}


		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:SubClassOf");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:Declaration");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}



		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:ObjectComplementOf");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:ObjectIntersectionOf");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:ObjectUnionOf");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:ObjectSomeValuesFrom");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			QDomElement node = mDocument.createElement("owl:ObjectAllValuesFrom");
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}



		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderClassExpression(const QString& name) {
			QDomElement node = mDocument.createElement("owl:Class");
			node.setAttribute("IRI",name);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectPropertyExpression(const QString& name) {
			QDomElement node = mDocument.createElement("owl:ObjectProperty");
			node.setAttribute("IRI",name);
			return createRenderedItemLinker()->setDomElement(node);
		}

		CRenderedQtXMLItemLinker* COntologyOWL2QtXMLRenderer::createRenderedItemLinker() {
			CRenderedQtXMLItemLinker* linker = mTmpRenderedItemLinker;
			if (linker) {
				mTmpRenderedItemLinker = mTmpRenderedItemLinker->getNextItemLinker();
				linker->clearNext();
			} else {
				linker = new CRenderedQtXMLItemLinker();
				mRenderedItemContainer.append(linker);
			}
			return linker;
		}

		COntologyOWL2QtXMLRenderer* COntologyOWL2QtXMLRenderer::releaseRenderedItemLinkers(CRenderedItemLinker* linkers) {
			if (linkers) {
				for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)linkers; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
					linkerIt->clearDomElement();
				}
				mTmpRenderedItemLinker = (CRenderedQtXMLItemLinker*)linkers->append(mTmpRenderedItemLinker);
			}
			return this;
		}



	}; // end namespace Renderer

}; // end namespace Konclude
