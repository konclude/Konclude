/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACEBOOLEANTYPE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACEBOOLEANTYPE_H

// Libraries includes
#include <QString>


// Namespace includes
#include "OntologySettings.h"
#include "CDatatypeValueSpaceCompareType.h"
#include "CDataLiteralBooleanValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDatatypeValueSpaceBooleanType
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceBooleanType : public CDatatypeValueSpaceCompareType {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceBooleanType();


					CDataLiteralBooleanValue* getMinimumDataLiteralBooleanValue();
					CDataLiteralBooleanValue* getMaximumDataLiteralBooleanValue();

					virtual CDataLiteralCompareValue* getMinimumDataLiteralCompareValue();
					virtual CDataLiteralCompareValue* getMaximumDataLiteralCompareValue();

					virtual bool hasInfiniteManyValues();
					virtual cint64 getMaximumValueCount();


				// protected methods
				protected:

				// protected variables
				protected:
					CDataLiteralBooleanValue* mMinValue;
					CDataLiteralBooleanValue* mMaxValue;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACEBOOLEANTYPE_H
