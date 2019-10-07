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

#include "COWLRecursionTestDataGenerator.h"


namespace Konclude {

	namespace Test {



		COWLRecursionTestDataGenerator::COWLRecursionTestDataGenerator() : CLogIdentifier("::Konclude::Test::OWLRecursionTestDataGenerator",this) {
			owllinkNS = "http://www.owllink.org/owllink#";
			owlNS = "http://www.w3.org/2002/07/owl#";

			mIndividualBaseName = "http://ex.text/Indi/";
			mPropertyBaseName = "http://ex.text/Prop/";
		}


		COWLRecursionTestDataGenerator::~COWLRecursionTestDataGenerator() {
		}



		QString COWLRecursionTestDataGenerator::getIndividualName(cint64 indiNumber) {
			QString numberString(QString::number(indiNumber));
			while (numberString.count() < mIndividualsNumberSize) {
				numberString.insert(0,"0");
			}
			return mIndividualBaseName+numberString;
		}


		void COWLRecursionTestDataGenerator::generateData(cint64 size, double rate, const QString& filename) {

			QFile file(filename);
			if (file.open(QIODevice::WriteOnly)) {

				QString mSizeNumber(QString::number(size-1));
				mIndividualsNumberSize = mSizeNumber.count();

				mXMLWriter = new QXmlStreamWriter(&file);
				mXMLWriter->setAutoFormatting(true);
				mXMLWriter->writeStartDocument("1.0");

				mXMLWriter->writeStartElement("Ontology");
				mXMLWriter->writeDefaultNamespace("http://www.w3.org/2002/07/owl#");

				createLargeJoinData(size,rate);

				mXMLWriter->writeEndElement();
				mXMLWriter->writeEndDocument();
				delete mXMLWriter;

				file.close();
			}


		}


		void COWLRecursionTestDataGenerator::createLargeJoinData(cint64 size, double rate) {
			createPropertyDeclarations();
			createIndividualDeclarations(size);
			cint64 propertyCount = size*rate;
			createPropertyCycleAssertions(mPropertyBaseName+"par",size);
			createPropertyAssertions(mPropertyBaseName+"par",propertyCount,size);
		}


		void COWLRecursionTestDataGenerator::createPropertyDeclarations() {
			writePropertyDeclaration(mPropertyBaseName+"par");
		}

		void COWLRecursionTestDataGenerator::createIndividualDeclarations(cint64 size) {
			for (cint64 indiID = 0; indiID < size; ++indiID) {
				QString indiName = getIndividualName(indiID);
				writeIndividualDeclaration(indiName);
			}
		}



		void COWLRecursionTestDataGenerator::createPropertyAssertions(const QString& propertyName, cint64 count, cint64 maxNumber) {
			for (cint64 nr = 0; nr < count; ++nr) {
				cint64 indiID1 = qrand() % maxNumber;
				cint64 indiID2 = qrand() % maxNumber;
				if (!mPropPairSet.contains( QPair<cint64,cint64>(indiID1,indiID2) )) {
					mPropPairSet.insert( QPair<cint64,cint64>(indiID1,indiID2) );
					QString indi1String = getIndividualName(indiID1);
					QString indi2String = getIndividualName(indiID2);
					writeIndividualPropertyAssertion(propertyName,indi1String,indi2String);
				} else {
					--nr;
				}
			}
		}


		void COWLRecursionTestDataGenerator::createPropertyCycleAssertions(const QString& propertyName, cint64 maxNumber) {
			for (cint64 indiID = 0; indiID < maxNumber-1; ++indiID) {
				cint64 indiID1 = indiID;
				cint64 indiID2 = indiID+1;
				if (!mPropPairSet.contains( QPair<cint64,cint64>(indiID1,indiID2) )) {
					mPropPairSet.insert( QPair<cint64,cint64>(indiID1,indiID2) );
					QString indi1String = getIndividualName(indiID1);
					QString indi2String = getIndividualName(indiID2);
					writeIndividualPropertyAssertion(propertyName,indi1String,indi2String);
				}
			}
			if (!mPropPairSet.contains( QPair<cint64,cint64>(maxNumber-1,0) )) {
				mPropPairSet.insert( QPair<cint64,cint64>(maxNumber-1,0) );
				QString indi1String = getIndividualName(maxNumber-1);
				QString indi2String = getIndividualName(0);
				writeIndividualPropertyAssertion(propertyName,indi1String,indi2String);
			}
		}


		void COWLRecursionTestDataGenerator::writeIndividualPropertyAssertion(const QString& propertyName, const QString& indiviualName1, const QString& indiviualName2) {
			mXMLWriter->writeStartElement("ObjectPropertyAssertion");

			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",propertyName);
			mXMLWriter->writeEndElement();

			mXMLWriter->writeStartElement("NamedIndividual");
			mXMLWriter->writeAttribute("IRI",indiviualName1);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeStartElement("NamedIndividual");
			mXMLWriter->writeAttribute("IRI",indiviualName2);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}



		void COWLRecursionTestDataGenerator::writeIndividualDeclaration(const QString& indiviualName) {
			mXMLWriter->writeStartElement("Declaration");
			mXMLWriter->writeStartElement("NamedIndividual");
			mXMLWriter->writeAttribute("IRI",indiviualName);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}


		void COWLRecursionTestDataGenerator::writePropertyDeclaration(const QString& propertyName) {
			mXMLWriter->writeStartElement("Declaration");
			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",propertyName);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}



	}; // end namespace Test

}; // end namespace Konclude
