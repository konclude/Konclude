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

#include "CReasonerEvaluationRequestResponseOWLlink.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {


			CReasonerEvaluationRequestResponseOWLlink::CReasonerEvaluationRequestResponseOWLlink() {
			}


			CReasonerEvaluationRequestResponseOWLlink* CReasonerEvaluationRequestResponseOWLlink::setRequestNode(const QDomElement& reqNode) {
				mRequestNode = reqNode;
				return this;
			}

			CReasonerEvaluationRequestResponseOWLlink* CReasonerEvaluationRequestResponseOWLlink::setResponseNode(const QDomElement& resNode) {
				mResponseNode = resNode;
				return this;
			}

			CReasonerEvaluationRequestResponseOWLlink* CReasonerEvaluationRequestResponseOWLlink::setModifiedNode(const QDomElement& resNode) {
				mModifiedNode = resNode;
				return this;
			}

			QDomElement CReasonerEvaluationRequestResponseOWLlink::getRequestNode() {
				return mRequestNode;
			}

			QDomElement CReasonerEvaluationRequestResponseOWLlink::getResponseNode() {
				return mResponseNode;
			}

			QDomElement CReasonerEvaluationRequestResponseOWLlink::getModifiedNode() {
				return mModifiedNode;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
