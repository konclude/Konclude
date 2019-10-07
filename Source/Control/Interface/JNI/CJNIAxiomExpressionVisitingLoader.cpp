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

#include "CJNIAxiomExpressionVisitingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIAxiomExpressionVisitingLoader::CJNIAxiomExpressionVisitingLoader(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData) : CConcreteOntologyUpdateCollectorBuilder(ontRevData->getOntologyRevision()->getOntology()) {
					mOntRevData = ontRevData;
					mJNIInstanceManager = jniInstanceManager;
				}

				CJNIAxiomExpressionVisitingLoader::~CJNIAxiomExpressionVisitingLoader() {					
				}


				CJNIAxiomExpressionVisitingLoader* CJNIAxiomExpressionVisitingLoader::initArgumentExpressionList() {
					mArgumentExpressionListStack.push( new CEXPRESSIONLIST<CBuildExpression*>() );
					return this;
				}

				CJNIAxiomExpressionVisitingLoader* CJNIAxiomExpressionVisitingLoader::closeArgumentExpressionList() {
					return this;
				}

				CJNIAxiomExpressionVisitingLoader* CJNIAxiomExpressionVisitingLoader::addArgumentExpression(CBuildExpression* expression) {
					mArgumentExpressionListStack.top()->append(expression);
					return this;
				}

				CEXPRESSIONLIST<CBuildExpression*>* CJNIAxiomExpressionVisitingLoader::popArgumentExpressionList() {
					return mArgumentExpressionListStack.pop();
				}


				CJNIOntologyRevisionData* CJNIAxiomExpressionVisitingLoader::getOntologyRevisionData() {
					return mOntRevData;
				}


			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
