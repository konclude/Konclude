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

#include "COWLTransitiveConnectedELClassesTestDataGenerator.h"


namespace Konclude {

	namespace Test {



		COWLTransitiveConnectedELClassesTestDataGenerator::COWLTransitiveConnectedELClassesTestDataGenerator() : CLogIdentifier("::Konclude::Test::OWLRecursionTestDataGenerator",this) {
			owllinkNS = "http://www.owllink.org/owllink#";
			owlNS = "http://www.w3.org/2002/07/owl#";

			mClassBaseName = "http://ex.text/Class/";
			mPropertyBaseName = "http://ex.text/Prop/";
		}


		COWLTransitiveConnectedELClassesTestDataGenerator::~COWLTransitiveConnectedELClassesTestDataGenerator() {
		}



		QString COWLTransitiveConnectedELClassesTestDataGenerator::getClassName(cint64 classNumber) {
			QString numberString(QString::number(classNumber));
			while (numberString.count() < mClassesNumberSize) {
				numberString.insert(0,"0");
			}
			return mClassBaseName+numberString;
		}


		void COWLTransitiveConnectedELClassesTestDataGenerator::generateData(cint64 size, const QString& filename) {

			QFile file(filename);
			if (file.open(QIODevice::WriteOnly)) {

				QString mSizeNumber(QString::number(size-1));
				mClassesNumberSize = mSizeNumber.count();

				mXMLWriter = new QXmlStreamWriter(&file);
				mXMLWriter->setAutoFormatting(true);
				mXMLWriter->writeStartDocument("1.0");

				mXMLWriter->writeStartElement("Ontology");
				mXMLWriter->writeDefaultNamespace("http://www.w3.org/2002/07/owl#");

				createTransistiveConnectedELClassesData(size);

				mXMLWriter->writeEndElement();
				mXMLWriter->writeEndDocument();
				delete mXMLWriter;

				file.close();
			}


		}


		void COWLTransitiveConnectedELClassesTestDataGenerator::createTransistiveConnectedELClassesData(cint64 size) {
			createPropertyDeclarations();
			createClassesDeclarations(size);
			createClassCycleDefinitions(mPropertyBaseName+"r",size);
			createPropertyChains();
		}


		void COWLTransitiveConnectedELClassesTestDataGenerator::createPropertyDeclarations() {
			writePropertyDeclaration(mPropertyBaseName+"r");
		}

		void COWLTransitiveConnectedELClassesTestDataGenerator::createPropertyChains() {
			mXMLWriter->writeStartElement("SubClassOf");

			mXMLWriter->writeStartElement("ObjectSomeValuesFrom");

			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",mPropertyBaseName+"r");
			mXMLWriter->writeEndElement();

			mXMLWriter->writeStartElement("Class");
			mXMLWriter->writeAttribute("IRI",mClassBaseName+"A");
			mXMLWriter->writeEndElement();

			mXMLWriter->writeEndElement();

			mXMLWriter->writeStartElement("Class");
			mXMLWriter->writeAttribute("IRI",mClassBaseName+"A");
			mXMLWriter->writeEndElement();

			mXMLWriter->writeEndElement();



			mXMLWriter->writeStartElement("SubObjectPropertyOf");

			mXMLWriter->writeStartElement("ObjectPropertyChain");

			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",mPropertyBaseName+"r");
			mXMLWriter->writeEndElement();
			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",mPropertyBaseName+"r");
			mXMLWriter->writeEndElement();

			mXMLWriter->writeEndElement();

			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",mPropertyBaseName+"r");
			mXMLWriter->writeEndElement();

			mXMLWriter->writeEndElement();
		}


		void COWLTransitiveConnectedELClassesTestDataGenerator::createClassesDeclarations(cint64 size) {
			writeClassDeclaration(mClassBaseName+"A");
			for (cint64 classID = 0; classID < size; ++classID) {
				QString className = getClassName(classID);
				writeClassDeclaration(className);
			}
		}



		void COWLTransitiveConnectedELClassesTestDataGenerator::createClassCycleDefinitions(const QString& propertyName, cint64 maxNumber) {
			for (cint64 classID = 0; classID < maxNumber-1; ++classID) {
				cint64 classID1 = classID;
				cint64 classID2 = classID+1;
				QString class1String = getClassName(classID1);
				QString class2String = getClassName(classID2);
				writeClassSubsetExistentialDefinition(propertyName,class1String,class2String);
			}
			QString class1String = getClassName(maxNumber-1);
			QString class2String = getClassName(0);
			writeClassSubsetExistentialDefinition(propertyName,class1String,class2String);
			writeClassSubsetExistentialDefinition(propertyName,class1String,mClassBaseName+"A");
		}


		void COWLTransitiveConnectedELClassesTestDataGenerator::writeClassSubsetExistentialDefinition(const QString& propertyName, const QString& className1, const QString& className2) {
			mXMLWriter->writeStartElement("SubClassOf");

			mXMLWriter->writeStartElement("Class");
			mXMLWriter->writeAttribute("IRI",className1);
			mXMLWriter->writeEndElement();

			mXMLWriter->writeStartElement("ObjectSomeValuesFrom");

			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",propertyName);
			mXMLWriter->writeEndElement();

			mXMLWriter->writeStartElement("Class");
			mXMLWriter->writeAttribute("IRI",className2);
			mXMLWriter->writeEndElement();

			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}



		void COWLTransitiveConnectedELClassesTestDataGenerator::writeClassDeclaration(const QString& classviualName) {
			mXMLWriter->writeStartElement("Declaration");
			mXMLWriter->writeStartElement("Class");
			mXMLWriter->writeAttribute("IRI",classviualName);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}


		void COWLTransitiveConnectedELClassesTestDataGenerator::writePropertyDeclaration(const QString& propertyName) {
			mXMLWriter->writeStartElement("Declaration");
			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",propertyName);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}



	}; // end namespace Test

}; // end namespace Konclude
