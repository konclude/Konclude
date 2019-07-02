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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYEXPRESSIVITY_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYEXPRESSIVITY_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyExpressivity
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyExpressivity {
				// public methods
				public:
					//! Constructor
					COntologyExpressivity();
					COntologyExpressivity(const QString& string);

					COntologyExpressivity* initExpressivity(const QString& string);

					QString getExpressivityString();


					const static cint64 EXPRESSIVITY_AL = 1;
					const static cint64 EXPRESSIVITY_C = 2;
					const static cint64 EXPRESSIVITY_D = 4;
					const static cint64 EXPRESSIVITY_E = 8;
					const static cint64 EXPRESSIVITY_EL = 16;
					const static cint64 EXPRESSIVITY_EPLUSPLUS = 32;
					const static cint64 EXPRESSIVITY_F = 64;
					const static cint64 EXPRESSIVITY_H = 128;
					const static cint64 EXPRESSIVITY_I = 256;
					const static cint64 EXPRESSIVITY_N = 512;
					const static cint64 EXPRESSIVITY_O = 1024;
					const static cint64 EXPRESSIVITY_Q = 2048;
					const static cint64 EXPRESSIVITY_R = 4096;
					const static cint64 EXPRESSIVITY_S = 8192;
					const static cint64 EXPRESSIVITY_TRANS = 16384;
					const static cint64 EXPRESSIVITY_U = 32768;

					bool isExpressivityEntailed(COntologyExpressivity* expressivity);
					bool isExpressivityEqual(COntologyExpressivity* expressivity);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mExpressivityFlags;
					QString mExpressivityString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYEXPRESSIVITY_H
