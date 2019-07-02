/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUEGROUPCOLLECTIONREASONERCOMPARISON_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUEGROUPCOLLECTIONREASONERCOMPARISON_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationDataValueGroupCollection.h"


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
			 *		\class		CReasonerEvaluationDataValueGroupCollectionReasonerComparison
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationDataValueGroupCollectionReasonerComparison {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationDataValueGroupCollectionReasonerComparison(QStringList reasonerList);

					//! Destructor
					virtual ~CReasonerEvaluationDataValueGroupCollectionReasonerComparison();

					CReasonerEvaluationDataValueGroupCollectionReasonerComparison* addReasonerCollection(const QString& reasoner, CReasonerEvaluationDataValueGroupCollection* collection);

					QMap<QString,CReasonerEvaluationDataValueGroupCollection*>* getReasonerCollectionMap();

					QStringList getReasonerList();

				// protected methods
				protected:

				// protected variables
				protected:
					QMap<QString,CReasonerEvaluationDataValueGroupCollection*> mReasonerCollectionMap;
					QStringList mReasonerList;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDATAVALUEGROUPCOLLECTIONREASONERCOMPARISON_H
