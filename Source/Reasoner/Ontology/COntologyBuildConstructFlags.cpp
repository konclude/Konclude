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

#include "COntologyBuildConstructFlags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyBuildConstructFlags::COntologyBuildConstructFlags(COntologyContext* ontoContext) {
				mOntoContext = ontoContext;
				resetFlags();
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::resetFlags() {
				mNonELConstructUsed = false;
				mBottomRoleUsed = false;
				mTopRoleUsed = false;
				mRetractionUsed = false;
				mNominalSchemaUsed = false;
				mComplexRoleUsed = false;
				return this;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::referenceBuildConstructFlags(COntologyBuildConstructFlags* buildConstructFlags) {
				mNonELConstructUsed = buildConstructFlags->mNonELConstructUsed;
				mBottomRoleUsed = buildConstructFlags->mBottomRoleUsed;
				mTopRoleUsed = buildConstructFlags->mTopRoleUsed;
				mRetractionUsed = buildConstructFlags->mRetractionUsed;
				mNominalSchemaUsed = buildConstructFlags->mNominalSchemaUsed;
				mComplexRoleUsed = buildConstructFlags->mComplexRoleUsed;
				return this;
			}


			bool COntologyBuildConstructFlags::isBottomRoleUsed() {
				return mBottomRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setBottomRoleUsed(bool used) {
				mBottomRoleUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isTopRoleUsed() {
				return mTopRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setTopRoleUsed(bool used) {
				mTopRoleUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isNonELConstructUsed() {
				return mNonELConstructUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setNonELConstructUsed(bool used) {
				mNonELConstructUsed = used;
				return this;
			}

			bool COntologyBuildConstructFlags::isComplexRoleUsed() {
				return mComplexRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setComplexRoleUsed(bool used) {
				mComplexRoleUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isNominalSchemaUsed() {
				return mNominalSchemaUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setNominalSchemaUsed(bool used) {
				mNominalSchemaUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isRetractionUsed() {
				return mRetractionUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setRetractionUsed(bool used) {
				mRetractionUsed = used;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
