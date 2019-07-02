/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "COWLLargeJoinTestDataGenerator.h"


namespace Konclude {

	namespace Test {



		COWLLargeJoinTestDataGenerator::COWLLargeJoinTestDataGenerator() : CLogIdentifier("::Konclude::Test::OWLLargeJoinTestDataGenerator",this) {
			owllinkNS = "http://www.owllink.org/owllink#";
			owlNS = "http://www.w3.org/2002/07/owl#";

			mIndividualBaseName = "http://ex.text/Indi/";
			mPropertyBaseName = "http://ex.text/Prop/";
		}


		COWLLargeJoinTestDataGenerator::~COWLLargeJoinTestDataGenerator() {
		}



		QString COWLLargeJoinTestDataGenerator::getIndividualName(cint64 indiNumber) {
			QString numberString(QString::number(indiNumber));
			while (numberString.count() < mIndividualsNumberSize) {
				numberString.insert(0,"0");
			}
			return mIndividualBaseName+numberString;
		}


		void COWLLargeJoinTestDataGenerator::generateData(cint64 size, double rate, const QString& filename) {

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


		void COWLLargeJoinTestDataGenerator::createLargeJoinData(cint64 size, double rate) {
			createPropertyDeclarations();
			createIndividualDeclarations(size);
			cint64 propertyCount = size*rate;
			createPropertyAssertions(mPropertyBaseName+"c2",propertyCount,size);
			createPropertyAssertions(mPropertyBaseName+"c3",propertyCount,size);
			createPropertyAssertions(mPropertyBaseName+"c4",propertyCount,size);
			createPropertyAssertions(mPropertyBaseName+"d1",propertyCount,size);
			createPropertyAssertions(mPropertyBaseName+"d2",propertyCount,size);
		}


		void COWLLargeJoinTestDataGenerator::createPropertyDeclarations() {
			writePropertyDeclaration(mPropertyBaseName+"c2");
			writePropertyDeclaration(mPropertyBaseName+"c3");
			writePropertyDeclaration(mPropertyBaseName+"c4");
			writePropertyDeclaration(mPropertyBaseName+"d1");
			writePropertyDeclaration(mPropertyBaseName+"d2");
		}

		void COWLLargeJoinTestDataGenerator::createIndividualDeclarations(cint64 size) {
			for (cint64 indiID = 0; indiID < size; ++indiID) {
				QString indiName = getIndividualName(indiID);
				writeIndividualDeclaration(indiName);
			}
		}


		void COWLLargeJoinTestDataGenerator::createPropertyAssertions(const QString& propertyName, cint64 count, cint64 maxNumber) {
			QSet< QPair<cint64,cint64> > propPairSet;
			for (cint64 nr = 0; nr < count; ++nr) {
				cint64 indiID1 = qrand() % maxNumber;
				cint64 indiID2 = qrand() % maxNumber;
				if (!propPairSet.contains( QPair<cint64,cint64>(indiID1,indiID2) )) {
					propPairSet.insert( QPair<cint64,cint64>(indiID1,indiID2) );
					QString indi1String = getIndividualName(indiID1);
					QString indi2String = getIndividualName(indiID2);
					writeIndividualPropertyAssertion(propertyName,indi1String,indi2String);
				} else {
					--nr;
				}
			}
		}



		void COWLLargeJoinTestDataGenerator::writeIndividualPropertyAssertion(const QString& propertyName, const QString& indiviualName1, const QString& indiviualName2) {
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



		void COWLLargeJoinTestDataGenerator::writeIndividualDeclaration(const QString& indiviualName) {
			mXMLWriter->writeStartElement("Declaration");
			mXMLWriter->writeStartElement("NamedIndividual");
			mXMLWriter->writeAttribute("IRI",indiviualName);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}


		void COWLLargeJoinTestDataGenerator::writePropertyDeclaration(const QString& propertyName) {
			mXMLWriter->writeStartElement("Declaration");
			mXMLWriter->writeStartElement("ObjectProperty");
			mXMLWriter->writeAttribute("IRI",propertyName);
			mXMLWriter->writeEndElement();
			mXMLWriter->writeEndElement();
		}



	}; // end namespace Test

}; // end namespace Konclude
