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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELDATA_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELDATA_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationClassPseudoModelRoleMap.h"
#include "CClassificationClassPseudoModelConceptMap.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationClassPseudoModelData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationClassPseudoModelData {
				// public methods
				public:
					//! Constructor
					CClassificationClassPseudoModelData(CContext* context);

					CClassificationClassPseudoModelData* initPseudoModelData(CClassificationClassPseudoModelData* data);

					CClassificationClassPseudoModelConceptMap* getPseudoModelConceptMap(bool create);
					CClassificationClassPseudoModelRoleMap* getPseudoModelRoleMap(bool create);

					bool hasValidRoleMap();
					bool hasValidConceptMap();

					CClassificationClassPseudoModelData* setValidRoleMap(bool valid);
					CClassificationClassPseudoModelData* setValidConceptMap(bool valid);

				// protected methods
				protected:

				// protected variables
				protected:
					CContext* mContext;

					CClassificationClassPseudoModelConceptMap* mLocConceptMap;
					CClassificationClassPseudoModelConceptMap* mUseConceptMap;

					CClassificationClassPseudoModelRoleMap* mLocRoleMap;
					CClassificationClassPseudoModelRoleMap* mUseRoleMap;

					bool mValidRoleMap;
					bool mValidConceptMap;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELDATA_H
