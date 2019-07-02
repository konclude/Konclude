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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUEGROUP_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUEGROUP_H

// Libraries includes
#include <QMap>
#include <QString>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationDataValue.h"


// Other includes
#include "Config/CConfiguration.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationDataValueGroup
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationDataValueGroup {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationDataValueGroup();

					//! Destructor
					virtual ~CReasonerEvaluationDataValueGroup();

					CReasonerEvaluationDataValueGroup* setGroupName(const QString& string);
					CReasonerEvaluationDataValueGroup* addRequestDataValue(const QString& requestString, CReasonerEvaluationDataValue* dataValue);

					QMap<QString,CReasonerEvaluationDataValue*>* getRequestDataValueMap();
					QString getGroupName();
					bool isEmpty();

				// protected methods
				protected:

				// protected variables
				protected:
					QMap<QString,CReasonerEvaluationDataValue*> mRequestDataValueMap;
					QString mGroupName;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUEGROUP_H
