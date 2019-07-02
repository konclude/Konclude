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
				mBottomObjectRoleUsed = false;
				mTopObjectRoleUsed = false;
				mBottomDataRoleUsed = false;
				mTopDataRoleUsed = false;
				mRetractionUsed = false;
				mNominalSchemaUsed = false;
				mComplexRoleUsed = false;
				return this;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::referenceBuildConstructFlags(COntologyBuildConstructFlags* buildConstructFlags) {
				mNonELConstructUsed = buildConstructFlags->mNonELConstructUsed;
				mBottomObjectRoleUsed = buildConstructFlags->mBottomObjectRoleUsed;
				mTopObjectRoleUsed = buildConstructFlags->mTopObjectRoleUsed;
				mBottomDataRoleUsed = buildConstructFlags->mBottomDataRoleUsed;
				mTopDataRoleUsed = buildConstructFlags->mTopDataRoleUsed;
				mRetractionUsed = buildConstructFlags->mRetractionUsed;
				mNominalSchemaUsed = buildConstructFlags->mNominalSchemaUsed;
				mComplexRoleUsed = buildConstructFlags->mComplexRoleUsed;
				return this;
			}


			bool COntologyBuildConstructFlags::isBottomObjectRoleUsed() {
				return mBottomObjectRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setBottomObjectRoleUsed(bool used) {
				mBottomObjectRoleUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isTopObjectRoleUsed() {
				return mTopObjectRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setTopObjectRoleUsed(bool used) {
				mTopObjectRoleUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isBottomDataRoleUsed() {
				return mBottomDataRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setBottomDataRoleUsed(bool used) {
				mBottomDataRoleUsed = used;
				return this;
			}


			bool COntologyBuildConstructFlags::isTopDataRoleUsed() {
				return mTopDataRoleUsed;
			}

			COntologyBuildConstructFlags* COntologyBuildConstructFlags::setTopDataRoleUsed(bool used) {
				mTopDataRoleUsed = used;
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
