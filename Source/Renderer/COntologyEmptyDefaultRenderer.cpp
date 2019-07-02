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



	}; // end namespace Renderer

}; // end namespace Konclude
