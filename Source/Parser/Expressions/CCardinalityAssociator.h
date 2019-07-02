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

#ifndef KONCLUDE_PARSER_CCARDINALITYASSOCIATOR_H
#define KONCLUDE_PARSER_CCARDINALITYASSOCIATOR_H


// Libraries includes
#include <QString>


// Namespace includes
#include "ExpressionSettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *	\class		CCardinalityAssociator
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CCardinalityAssociator {
				// public methods
				public:
					//! Constructor
					CCardinalityAssociator(cint64 cardinality);

					//! Destructor
					virtual ~CCardinalityAssociator();

					CCardinalityAssociator* setCardinality(cint64 cardinality);
					cint64 getCardinality();


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mCard;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CCARDINALITYASSOCIATOR_H
