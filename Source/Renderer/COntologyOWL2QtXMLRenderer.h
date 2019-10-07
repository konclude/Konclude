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
				COntologyOWL2QtXMLRenderer(const QString& prefixNamespace = "owl:");

				//! Destructor
				virtual ~COntologyOWL2QtXMLRenderer();


				virtual bool beginRender();
				virtual bool endRender(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker);



				virtual CRenderedItemLinker* endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual CRenderedItemLinker* endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker);





				virtual CRenderedItemLinker* endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet);




				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name);

				virtual CRenderedItemLinker* endRenderDataPropertyExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderNamedIndividualExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderAnonymousIndividualExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderDatatypeExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderDataLiteralExpression(const QString& name, const QString& datatype);


				CRenderedQtXMLItemLinker* createRenderedItemLinker();
				COntologyOWL2QtXMLRenderer* releaseRenderedItemLinkers(CRenderedItemLinker* linkers);


				bool addRenderedAxiomsToDomElement(QDomElement& domElement);


			// protected methods
			protected:
				CRenderedItemLinker* createXMLAttributeNodeAppendChildren(const QString& nodeName, const QString& attributeName, const QString& attributeValue, CRenderedItemLinker* renderedChildrenItemLinker);
				CRenderedItemLinker* createXMLAttributeNodeAppendChildren(const QString& nodeName, const QString& attributeName, int attributeValue, CRenderedItemLinker* renderedChildrenItemLinker);
				CRenderedItemLinker* createXMLNodeAppendChildren(const QString& nodeName, CRenderedItemLinker* renderedChildrenItemLinker);
				CRenderedItemLinker* createXMLNode(const QString& nodeName, const QString& iriName);

			// protected variables
			protected:
				QList<CRenderedQtXMLItemLinker*> mRenderedItemContainer;
				CRenderedQtXMLItemLinker* mTmpRenderedItemLinker;

				QDomDocument mDocument;

				CRenderedQtXMLItemLinker* mAxiomRenderedItemLinker;
				QString mPrefixNamespace;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CONTOLOGYOWL2QTXMLRENDERER_H
