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


				virtual bool beginRenderEquivalentClassesAxiom();
				virtual CRenderedItemLinker* endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDisjointClassesAxiom();
				virtual CRenderedItemLinker* endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDisjontUnionAxiom();
				virtual CRenderedItemLinker* endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderClassAssertionAxiom();
				virtual CRenderedItemLinker* endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectPropertyAssertionAxiom();
				virtual CRenderedItemLinker* endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderNegativeObjectPropertyAssertionAxiom();
				virtual CRenderedItemLinker* endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderSameIndividualAxiom();
				virtual CRenderedItemLinker* endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDifferentIndividualsAxiom();
				virtual CRenderedItemLinker* endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderSubObjectPropertyOfAxiom();
				virtual CRenderedItemLinker* endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectPropertyChainAxiom();
				virtual CRenderedItemLinker* endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderEquivalentObjectPropertiesAxiom();
				virtual CRenderedItemLinker* endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDisjointObjectPropertiesAxiom();
				virtual CRenderedItemLinker* endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectPropertyDomainAxiom();
				virtual CRenderedItemLinker* endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectPropertyRangeAxiom();
				virtual CRenderedItemLinker* endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderInverseObjectPropertyOf();
				virtual CRenderedItemLinker* endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderInverseObjectPropertiesAxiom();
				virtual CRenderedItemLinker* endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderTransetiveObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderFunctionalObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderSymmetricObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderAsymmetricObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderReflexiveObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderIrreflexiveObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderInverseFunctionalObjectPropertyAxiom();
				virtual CRenderedItemLinker* endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderDataPropertyAssertionAxiom();
				virtual CRenderedItemLinker* endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderNegativeDataPropertyAssertionAxiom();
				virtual CRenderedItemLinker* endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderSubDataPropertyOfAxiom();
				virtual CRenderedItemLinker* endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderEquivalentDataPropertiesAxiom();
				virtual CRenderedItemLinker* endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDisjointDataPropertiesAxiom();
				virtual CRenderedItemLinker* endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataPropertyDomainAxiom();
				virtual CRenderedItemLinker* endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataPropertyRangeAxiom();
				virtual CRenderedItemLinker* endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderFunctionalDataPropertyAxiom();
				virtual CRenderedItemLinker* endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker);





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

				virtual bool beginRenderObjectMaxCardinalityExpression(cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);

				virtual bool beginRenderObjectMinCardinalityExpression(cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);

				virtual bool beginRenderObjectExactCardinalityExpression(cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);

				virtual bool beginRenderObjectOneOfExpression();
				virtual CRenderedItemLinker* endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectHasValueExpression();
				virtual CRenderedItemLinker* endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderObjectHasSelfExpression();
				virtual CRenderedItemLinker* endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderDataAllValuesExpression();
				virtual CRenderedItemLinker* endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataSomeValuesFromExpression();
				virtual CRenderedItemLinker* endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);


				virtual bool beginRenderDataHasValueExpression();
				virtual CRenderedItemLinker* endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataIntersectionOfExpression();
				virtual CRenderedItemLinker* endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataOneOfExpression();
				virtual CRenderedItemLinker* endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataUnionOfExpression();
				virtual CRenderedItemLinker* endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataComplementOfExpression();
				virtual CRenderedItemLinker* endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDatatypeRestrictionExpression();
				virtual CRenderedItemLinker* endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker);

				virtual bool beginRenderDataMaxCardinalityExpression(cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);

				virtual bool beginRenderDataMinCardinalityExpression(cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);

				virtual bool beginRenderDataExactCardinalityExpression(cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);

				virtual bool beginRenderDataFacetRestrictionExpression(const QString& facet);
				virtual CRenderedItemLinker* endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet);




				virtual bool beginRenderClassExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name);

				virtual bool beginRenderObjectPropertyExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name);


				virtual bool beginRenderDataPropertyExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderDataPropertyExpression(const QString& name);

				virtual bool beginRenderNamedIndividualExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderNamedIndividualExpression(const QString& name);
				
				virtual bool beginRenderAnonymousIndividualExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderAnonymousIndividualExpression(const QString& name);
				
				virtual bool beginRenderDatatypeExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderDatatypeExpression(const QString& name);
				
				virtual bool beginRenderDataLiteralExpression(const QString& name, const QString& datatype);
				virtual CRenderedItemLinker* endRenderDataLiteralExpression(const QString& name, const QString& datatype);

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
