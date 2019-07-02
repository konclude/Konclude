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

#ifndef KONCLUDE_TEST_COWLLARGEJOINTESTDATAGENERATOR_H
#define KONCLUDE_TEST_COWLLARGEJOINTESTDATAGENERATOR_H

// Libraries includes
#include <QXmlStreamWriter>

// Namespace includes
#include "Utilities/UtilitiesSettings.h"

// Other includes
#include <cmath>


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Logger;
	using namespace Utilities;

	namespace Test {


		/*! 
		 *
		 *		\class		COWLLargeJoinTestDataGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLLargeJoinTestDataGenerator : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLLargeJoinTestDataGenerator();

				//! Destructor
				virtual ~COWLLargeJoinTestDataGenerator();

				void generateData(cint64 size, double rate, const QString& filename);

			// protected methods
			protected:
				void createLargeJoinData(cint64 size, double rate);
				void createPropertyDeclarations();
				void createIndividualDeclarations(cint64 size);


				void createPropertyAssertions(const QString& propertyName, cint64 count, cint64 maxNumber);


				void writePropertyDeclaration(const QString& propertyName);
				void writeIndividualDeclaration(const QString& indiviualName);
				void writeIndividualPropertyAssertion(const QString& propertyName, const QString& indiviualName1, const QString& indiviualName2);

				QString getIndividualName(cint64 indiNumber);

			// protected variables
			protected:
				QString owllinkNS;
				QString owlNS;

				cint64 mIndividualsNumberSize;
				QString mIndividualBaseName;
				QString mPropertyBaseName;
				QXmlStreamWriter* mXMLWriter;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLLARGEJOINTESTDATAGENERATOR_H
