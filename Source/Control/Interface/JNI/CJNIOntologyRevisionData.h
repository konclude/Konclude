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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIONTOLOGYREVISIONDATA_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIONTOLOGYREVISIONDATA_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "JNISettings.h"
#include "CJNIEntityMappingHash.h"

// Other includes
#include "Reasoner/Revision/COntologyRevision.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner::Revision;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIOntologyRevisionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIOntologyRevisionData {
					// public methods
					public:
						CJNIOntologyRevisionData(CJNIOntologyRevisionData* ontRevData);

						~CJNIOntologyRevisionData();

						COntologyRevision* getOntologyRevision();
						CJNIOntologyRevisionData* setOntologyRevision(COntologyRevision* ontRev);

						CJNIEntityMappingHash* getEntityMappingHash();

						bool isQueryingPrepared();
						CJNIOntologyRevisionData* setQueryingPrepared(bool prepared);

					// protected methods
					protected:

					// protected variables
					protected:
						COntologyRevision* mOntRev;
						CJNIEntityMappingHash mMappingHash;
						bool mQueryingPrepared;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIONTOLOGYREVISIONDATA_H
