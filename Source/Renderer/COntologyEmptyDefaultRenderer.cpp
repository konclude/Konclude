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

#include "COntologyEmptyDefaultRenderer.h"


namespace Konclude {

	namespace Renderer {


		COntologyEmptyDefaultRenderer::COntologyEmptyDefaultRenderer() {
		}



		bool COntologyEmptyDefaultRenderer::beginRender() {
			return true;
		}

		bool COntologyEmptyDefaultRenderer::endRender(CRenderedItemLinker* renderedItemLinker) {
			return true;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderSubClassOfAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderDeclarationAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}






		bool COntologyEmptyDefaultRenderer::beginRenderEquivalentClassesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderDisjointClassesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderDisjontUnionAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderClassAssertionAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderObjectPropertyAssertionAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderNegativeObjectPropertyAssertionAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderSameIndividualAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderDifferentIndividualsAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderSubObjectPropertyOfAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}



		bool COntologyEmptyDefaultRenderer::beginRenderObjectPropertyChainAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderEquivalentObjectPropertiesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderDisjointObjectPropertiesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderObjectPropertyDomainAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderObjectPropertyRangeAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderInverseObjectPropertyOf() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderInverseObjectPropertiesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderTransetiveObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderFunctionalObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderSymmetricObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderAsymmetricObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderReflexiveObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderIrreflexiveObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderInverseFunctionalObjectPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


		bool COntologyEmptyDefaultRenderer::beginRenderDataPropertyAssertionAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderNegativeDataPropertyAssertionAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderSubDataPropertyOfAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderEquivalentDataPropertiesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDisjointDataPropertiesAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataPropertyDomainAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataPropertyRangeAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderFunctionalDataPropertyAxiom() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}


















		bool COntologyEmptyDefaultRenderer::beginRenderObjectComplementOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectIntersectionOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectUnionOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectSomeValuesFromExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectAllValuesFromExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectMaxCardinalityExpression(cint64 cardinality) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectMinCardinalityExpression(cint64 cardinality) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectExactCardinalityExpression(cint64 cardinality) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectOneOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectHasValueExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectHasSelfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataAllValuesExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataSomeValuesFromExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataHasValueExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataIntersectionOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataOneOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataUnionOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataComplementOfExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDatatypeRestrictionExpression() {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataMaxCardinalityExpression(cint64 cardinality) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataMinCardinalityExpression(cint64 cardinality) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataExactCardinalityExpression(cint64 cardinality) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataFacetRestrictionExpression(const QString& facet) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet) {
			return nullptr;
		}
















		bool COntologyEmptyDefaultRenderer::beginRenderClassExpression(const QString& name) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderClassExpression(const QString& name) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderObjectPropertyExpression(const QString& name) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderObjectPropertyExpression(const QString& name) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataPropertyExpression(const QString& name) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataPropertyExpression(const QString& name) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderNamedIndividualExpression(const QString& name) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderNamedIndividualExpression(const QString& name) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderAnonymousIndividualExpression(const QString& name) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderAnonymousIndividualExpression(const QString& name) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDatatypeExpression(const QString& name) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDatatypeExpression(const QString& name) {
			return nullptr;
		}

		bool COntologyEmptyDefaultRenderer::beginRenderDataLiteralExpression(const QString& name, const QString& datatype) {
			return true;
		}

		CRenderedItemLinker* COntologyEmptyDefaultRenderer::endRenderDataLiteralExpression(const QString& name, const QString& datatype) {
			return nullptr;
		}



	}; // end namespace Renderer

}; // end namespace Konclude
