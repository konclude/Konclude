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

#ifndef KONCLUDE_TEST_COWLTRANSITIVECONNECTEDELCLASSESTESTDATAGENERATOR_H
#define KONCLUDE_TEST_COWLTRANSITIVECONNECTEDELCLASSESTESTDATAGENERATOR_H

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
		 *		\class		COWLTransitiveConnectedELClassesTestDataGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLTransitiveConnectedELClassesTestDataGenerator : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLTransitiveConnectedELClassesTestDataGenerator();

				//! Destructor
				virtual ~COWLTransitiveConnectedELClassesTestDataGenerator();

				void generateData(cint64 size, const QString& filename);

			// protected methods
			protected:
				void createTransistiveConnectedELClassesData(cint64 size);
				void createClassesDeclarations(cint64 size);
				void createPropertyDeclarations();
				void createClassCycleDefinitions(const QString& propertyName, cint64 maxNumber);
				void createPropertyChains();


				void writePropertyDeclaration(const QString& propertyName);
				void writeClassDeclaration(const QString& indiviualName);
				void writeClassSubsetExistentialDefinition(const QString& propertyName, const QString& className1, const QString& className2);

				QString getClassName(cint64 classNumber);

			// protected variables
			protected:
				QString owllinkNS;
				QString owlNS;

				cint64 mClassesNumberSize;
				QString mClassBaseName;
				QString mPropertyBaseName;
				QXmlStreamWriter* mXMLWriter;



			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLTRANSITIVECONNECTEDELCLASSESTESTDATAGENERATOR_H
