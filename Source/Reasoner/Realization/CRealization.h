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

#ifndef KONCLUDE_REASONER_REALIZATION_CREALIZATION_H
#define KONCLUDE_REASONER_REALIZATION_CREALIZATION_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CConceptRealization.h"
#include "CRoleRealization.h"
#include "CSameRealization.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CRealization
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealization {
				// public methods
				public:
					//! Constructor
					CRealization();

					bool hasConceptRealization();
					CConceptRealization* getConceptRealization();
					CRealization* setConceptRealization(CConceptRealization* conRealization);


					bool hasSameRealization();
					CSameRealization* getSameRealization();
					CRealization* setSameRealization(CSameRealization* sameRealization);

					bool hasRoleRealization();
					CRoleRealization* getRoleRealization();
					CRealization* setRoleRealization(CRoleRealization* roleRealization);

					bool isRealized();
					CRealization* setRealized(bool realized);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mRealized;
					CConceptRealization* mConceptRealization;
					CRoleRealization* mRoleRealization;
					CSameRealization* mSameRealization;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CREALIZATION_H
