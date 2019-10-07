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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATALITERALCOMPAREVALUE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATALITERALCOMPAREVALUE_H

// Libraries includes



// Namespace includes
#include "OntologySettings.h"
#include "CDataLiteralValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDataLiteralValueCompare
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteralCompareValue : public CDataLiteralValue {
				// public methods
				public:

					virtual CDataLiteralCompareValue* initValue(CDataLiteralCompareValue* value) = 0;

					virtual bool isLessEqualThan(CDataLiteralCompareValue* value) = 0;
					virtual bool isLessThan(CDataLiteralCompareValue* value) = 0;
					virtual bool isEqualTo(CDataLiteralCompareValue* value) = 0;
					virtual bool isGreaterEqualThan(CDataLiteralCompareValue* value) = 0;
					virtual bool isGreaterThan(CDataLiteralCompareValue* value) = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATALITERALCOMPAREVALUE_H
