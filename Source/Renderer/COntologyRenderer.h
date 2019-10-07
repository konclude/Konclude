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


				virtual bool beginRenderEquivalentClassesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDisjointClassesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDisjontUnionAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderClassAssertionAxiom() = 0;
				virtual CRenderedItemLinker* endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectPropertyAssertionAxiom() = 0;
				virtual CRenderedItemLinker* endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderNegativeObjectPropertyAssertionAxiom() = 0;
				virtual CRenderedItemLinker* endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderSameIndividualAxiom() = 0;
				virtual CRenderedItemLinker* endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDifferentIndividualsAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderSubObjectPropertyOfAxiom() = 0;
				virtual CRenderedItemLinker* endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectPropertyChainAxiom() = 0;
				virtual CRenderedItemLinker* endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderEquivalentObjectPropertiesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDisjointObjectPropertiesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectPropertyDomainAxiom() = 0;
				virtual CRenderedItemLinker* endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectPropertyRangeAxiom() = 0;
				virtual CRenderedItemLinker* endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderInverseObjectPropertyOf() = 0;
				virtual CRenderedItemLinker* endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderInverseObjectPropertiesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderTransetiveObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderFunctionalObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderSymmetricObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderAsymmetricObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderReflexiveObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderIrreflexiveObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderInverseFunctionalObjectPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderDataPropertyAssertionAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderNegativeDataPropertyAssertionAxiom() = 0;
				virtual CRenderedItemLinker* endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderSubDataPropertyOfAxiom() = 0;
				virtual CRenderedItemLinker* endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderEquivalentDataPropertiesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDisjointDataPropertiesAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataPropertyDomainAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataPropertyRangeAxiom() = 0;
				virtual CRenderedItemLinker* endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderFunctionalDataPropertyAxiom() = 0;
				virtual CRenderedItemLinker* endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker) = 0;






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


				virtual bool beginRenderObjectMaxCardinalityExpression(cint64 cardinality) = 0;
				virtual CRenderedItemLinker* endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) = 0;

				virtual bool beginRenderObjectMinCardinalityExpression(cint64 cardinality) = 0;
				virtual CRenderedItemLinker* endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) = 0;

				virtual bool beginRenderObjectExactCardinalityExpression(cint64 cardinality) = 0;
				virtual CRenderedItemLinker* endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) = 0;

				virtual bool beginRenderObjectOneOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectHasValueExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderObjectHasSelfExpression() = 0;
				virtual CRenderedItemLinker* endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker) = 0;


				virtual bool beginRenderDataAllValuesExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker) = 0;
			
				virtual bool beginRenderDataSomeValuesFromExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataHasValueExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataIntersectionOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataOneOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataUnionOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataComplementOfExpression() = 0;
				virtual CRenderedItemLinker* endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDatatypeRestrictionExpression() = 0;
				virtual CRenderedItemLinker* endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker) = 0;

				virtual bool beginRenderDataMaxCardinalityExpression(cint64 cardinality) = 0;
				virtual CRenderedItemLinker* endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) = 0;

				virtual bool beginRenderDataMinCardinalityExpression(cint64 cardinality) = 0;
				virtual CRenderedItemLinker* endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) = 0;

				virtual bool beginRenderDataExactCardinalityExpression(cint64 cardinality) = 0;
				virtual CRenderedItemLinker* endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) = 0;

				virtual bool beginRenderDataFacetRestrictionExpression(const QString& facet) = 0;
				virtual CRenderedItemLinker* endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet) = 0;




				virtual bool beginRenderClassExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name) = 0;

				virtual bool beginRenderObjectPropertyExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name) = 0;

				virtual bool beginRenderDataPropertyExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderDataPropertyExpression(const QString& name) = 0;

				virtual bool beginRenderNamedIndividualExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderNamedIndividualExpression(const QString& name) = 0;
				
				virtual bool beginRenderAnonymousIndividualExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderAnonymousIndividualExpression(const QString& name) = 0;
				
				virtual bool beginRenderDatatypeExpression(const QString& name) = 0;
				virtual CRenderedItemLinker* endRenderDatatypeExpression(const QString& name) = 0;
				
				virtual bool beginRenderDataLiteralExpression(const QString& name, const QString& datatype) = 0;
				virtual CRenderedItemLinker* endRenderDataLiteralExpression(const QString& name, const QString& datatype) = 0;

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
