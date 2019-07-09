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

#include "CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer(QByteArray* buffer, bool writeHeaderAndFooter) : CSPARQLXMLAnswerStreamingSerializer(buffer, writeHeaderAndFooter) {

					mTemporaryBuffer.reserve(50000);

					mResultsBegin = QString("\r\n<results>\r\n").toUtf8();
					mResultsEnd = QString("</results>").toUtf8();

					mResultBegin = QString(" <result>\r\n").toUtf8();
					mResultEnd = QString(" </result>\r\n").toUtf8();

					mBindingNameBegin = QString("  <binding name=\"").toUtf8();
					mBindingNameEnd = QString("\">").toUtf8();
					mBindingEnd = QString("</binding>\r\n").toUtf8();

					mUriBegin = QString("<uri>").toUtf8();
					mUriEnd = QString("</uri>").toUtf8();

					mBnodeBegin = QString("<bnode>").toUtf8();
					mBnodeEnd = QString("</bnode>").toUtf8();

					mLiteralDatatypeBegin = QString("<literal datatype=\"").toUtf8();
					mLiteralDatatypeEnd = QString("\">").toUtf8();
					mLiteralEnd = QString("</literal>").toUtf8();

				}

				CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::~CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer() {
				}


				CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::addTemporaryBuffer() {
					mCurrentArray->append(mTemporaryBuffer);
					return this;
				}


				CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::clearTemporaryBuffer() {
					mTemporaryBuffer.clear();
					return this;
				}


				CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::writeUTF8String(const QString& string) {
					mTemporaryBuffer.append(string.toUtf8());
					//const QChar* data = string.constData();
					//cint64 length = string.length();
					//cint64 pos = 0;
					//while (pos < length) {
					//	QChar character = data[pos++];
					//	mTemporaryBuffer.append(character.toLatin1());
					//}
					return this;
				}


				CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::addResultStreamingBindingToTemporaryBuffer(const QStringList& varList, CVariableBindingsAnswerResult* bindings) {
					mTemporaryBuffer.append(mResultBegin);

					mVarBindIt = bindings->getVariableBindingsIterator(mVarBindIt);
					QStringList::const_iterator varListIt = varList.constBegin();

					while (mVarBindIt->hasNext()) {
						QString varString = *varListIt;
						++varListIt;

						CVariableBindingResult* varBin = mVarBindIt->getNext();
						if (varBin) {
							mTemporaryBuffer.append(mBindingNameBegin);
							writeUTF8String(varString);
							mTemporaryBuffer.append(mBindingNameEnd);

							if (varBin->isNamedIndividualBindingType()) {
								mTemporaryBuffer.append(mUriBegin);
								writeUTF8String(varBin->getQueryResultString());
								mTemporaryBuffer.append(mUriEnd);
							} else if (varBin->isAnonymousIndividualBindingType()) {
								mTemporaryBuffer.append(mBnodeBegin);
								writeUTF8String(varBin->getQueryResultString());
								mTemporaryBuffer.append(mBnodeEnd);
							} else if (varBin->isLiteralBindingType()) {
								mTemporaryBuffer.append(mLiteralDatatypeBegin);
								writeUTF8String(varBin->getLiteralDatatypeBindingString());
								mTemporaryBuffer.append(mLiteralDatatypeEnd);
								writeUTF8String(varBin->getLiteralDatavalueBindingString());
								mTemporaryBuffer.append(mLiteralEnd);
							}
							mTemporaryBuffer.append(mBindingEnd);
						}
					}
					mTemporaryBuffer.append(mResultEnd);
					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::writeResultStreamingStart() {
					mCurrentArray->append(mResultsBegin);
					mCurrentBuffer->seek(mCurrentArray->length());
					return this;
				}

				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer::writeResultStreamingEnd() {
					mCurrentArray->append(mResultsEnd);
					mCurrentBuffer->seek(mCurrentArray->length());
					return this;
				}



			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude