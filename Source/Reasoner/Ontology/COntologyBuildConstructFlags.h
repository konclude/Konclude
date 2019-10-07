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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDCONSTRUCTFLAGS_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDCONSTRUCTFLAGS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyBuildConstructFlags
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyBuildConstructFlags {
				// public methods
				public:
					//! Constructor
					COntologyBuildConstructFlags(COntologyContext* ontoContext = nullptr);

					COntologyBuildConstructFlags* resetFlags();

					COntologyBuildConstructFlags* referenceBuildConstructFlags(COntologyBuildConstructFlags* buildConstructFlags);


					bool isBottomObjectRoleUsed();
					COntologyBuildConstructFlags* setBottomObjectRoleUsed(bool used = true);

					bool isTopObjectRoleUsed();
					COntologyBuildConstructFlags* setTopObjectRoleUsed(bool used = true);

					bool isBottomDataRoleUsed();
					COntologyBuildConstructFlags* setBottomDataRoleUsed(bool used = true);

					bool isTopDataRoleUsed();
					COntologyBuildConstructFlags* setTopDataRoleUsed(bool used = true);

					bool isNonELConstructUsed();
					COntologyBuildConstructFlags* setNonELConstructUsed(bool used = true);

					bool isComplexRoleUsed();
					COntologyBuildConstructFlags* setComplexRoleUsed(bool used = true);

					bool isNominalSchemaUsed();
					COntologyBuildConstructFlags* setNominalSchemaUsed(bool used = true);

					bool isRetractionUsed();
					COntologyBuildConstructFlags* setRetractionUsed(bool used = true);

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;

					bool mBottomObjectRoleUsed;
					bool mTopObjectRoleUsed;

					bool mBottomDataRoleUsed;
					bool mTopDataRoleUsed;

					bool mNonELConstructUsed;

					bool mNominalSchemaUsed;
					bool mComplexRoleUsed;

					bool mRetractionUsed;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDCONSTRUCTFLAGS_H
