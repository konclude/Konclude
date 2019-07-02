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

#include "COntologyOWL2QtXMLRenderer.h"


namespace Konclude {

	namespace Renderer {


		COntologyOWL2QtXMLRenderer::COntologyOWL2QtXMLRenderer(const QString& prefixNamespace) {
			mPrefixNamespace = prefixNamespace;
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


		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::createXMLNodeAppendChildren(const QString& nodeName, CRenderedItemLinker* renderedChildrenItemLinker) {
			QDomElement node = mDocument.createElement(nodeName);
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedChildrenItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedChildrenItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}


		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::createXMLAttributeNodeAppendChildren(const QString& nodeName, const QString& attributeName, int attributeValue, CRenderedItemLinker* renderedChildrenItemLinker) {
			QDomElement node = mDocument.createElement(nodeName);
			node.setAttribute(attributeName, QString(attributeValue));
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedChildrenItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedChildrenItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}


		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::createXMLAttributeNodeAppendChildren(const QString& nodeName, const QString& attributeName, const QString& attributeValue, CRenderedItemLinker* renderedChildrenItemLinker) {
			QDomElement node = mDocument.createElement(nodeName);
			node.setAttribute(attributeName, QString(attributeValue));
			for (CRenderedQtXMLItemLinker* linkerIt = (CRenderedQtXMLItemLinker*)renderedChildrenItemLinker; linkerIt; linkerIt = linkerIt->getNextItemLinker()) {
				node.appendChild(linkerIt->getDomElement());
			}
			releaseRenderedItemLinkers(renderedChildrenItemLinker);
			return createRenderedItemLinker()->setDomElement(node);
		}


		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"SubClassOf",renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"Declaration", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"EquivalentClasses", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DisjointClasses", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DisjointUnion", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ClassAssertion", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectPropertyAssertion", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"NegativeObjectPropertyAssertion", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"SameIndividual", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DifferentIndividuals", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			if (renderedItemLinker->getCount() > 2) {
				CRenderedItemLinker* lastRenderedItemLinker = renderedItemLinker;
				for (CRenderedItemLinker* renderedItemLinkerIt = renderedItemLinker; renderedItemLinkerIt; renderedItemLinkerIt = renderedItemLinkerIt->getNext()) {
					if (!renderedItemLinkerIt->hasNext()) {
						lastRenderedItemLinker->clearNext();
						lastRenderedItemLinker = renderedItemLinkerIt;
					} else {
						lastRenderedItemLinker = renderedItemLinkerIt;
					}
				}
				CRenderedItemLinker* chainItemLinker = createXMLNodeAppendChildren(mPrefixNamespace+"ObjectPropertyChain", renderedItemLinker);
				chainItemLinker->append(lastRenderedItemLinker);
				return createXMLNodeAppendChildren(mPrefixNamespace+"SubObjectPropertyOf", chainItemLinker);
			} else {
				return createXMLNodeAppendChildren(mPrefixNamespace+"SubObjectPropertyOf", renderedItemLinker);
			}
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectPropertyChain", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"EquivalentObjectProperties", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DisjointObjectProperties", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectPropertyDomain", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectPropertyRange", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectInverseOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"InverseObjectProperties", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"TransitiveObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"FunctionalObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"SymmetricObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"AsymmetricObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ReflexiveObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"IrreflexiveObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"InverseFunctionalObjectProperty", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataPropertyAssertion", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"NegativeDataPropertyAssertion", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"SubDataPropertyOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"EquivalentDataProperties", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DisjointDataProperties", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataPropertyDomain", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataPropertyRange", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"FunctionalDataProperty", renderedItemLinker);
		}






		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectComplementOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectIntersectionOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectUnionOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectSomeValuesFrom", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectAllValuesFrom", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"ObjectMaxCardinality", "cardinality", cardinality, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"ObjectMinCardinality", "cardinality", cardinality, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"ObjectExactCardinality", "cardinality", cardinality, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectOneOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectHasValue", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"ObjectHasSelf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataAllValuesFrom", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataSomeValuesFrom", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataHasValue", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataIntersectionOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataOneOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataUnionOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DataComplementOf", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker) {
			return createXMLNodeAppendChildren(mPrefixNamespace+"DatatypeRestriction", renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"DataMaxCardinality", "cardinality", cardinality, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"DataMinCardinality", "cardinality", cardinality, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"DataExactCardinality", "cardinality", cardinality, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet) {
			return createXMLAttributeNodeAppendChildren(mPrefixNamespace+"FacetRestriction", "facet", facet, renderedItemLinker);
		}




		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::createXMLNode(const QString& nodeName, const QString& iriName) {
			QDomElement node = mDocument.createElement(nodeName);
			node.setAttribute("IRI", iriName);
			return createRenderedItemLinker()->setDomElement(node);
		}



		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderClassExpression(const QString& name) {
			return createXMLNode(mPrefixNamespace+"Class", name);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderObjectPropertyExpression(const QString& name) {
			return createXMLNode(mPrefixNamespace+"ObjectProperty", name);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataPropertyExpression(const QString& name) {
			return createXMLNode(mPrefixNamespace+"DataProperty", name);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderNamedIndividualExpression(const QString& name) {
			return createXMLNode(mPrefixNamespace+"NamedIndividual", name);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderAnonymousIndividualExpression(const QString& name) {
			return createXMLNode(mPrefixNamespace+"AnonymousIndividual", name);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDatatypeExpression(const QString& name) {
			return createXMLNode(mPrefixNamespace+"Datatype", name);
		}

		CRenderedItemLinker* COntologyOWL2QtXMLRenderer::endRenderDataLiteralExpression(const QString& name, const QString& datatype) {
			QDomElement node = mDocument.createElement(mPrefixNamespace+"Literal");
			node.setAttribute("datatypeIRI", datatype);
			node.appendChild(mDocument.createTextNode(name));
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
