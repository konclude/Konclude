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

#ifndef KONCLUDE_REASONER_PREPROCESS_CDATALITERALNORMALIZERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CDATALITERALNORMALIZERPREPROCESS_H

// Libraries includes
#include <QSet>
#include <QDomDocument>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"
#include "CConceptRoleIndividualLocator.h"


// Other includes
#include "Reasoner/Ontology/CBoxContext.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CMBox.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CDataLiteralNormalizerPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteralNormalizerPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CDataLiteralNormalizerPreProcess();

					//! Destructor
					virtual ~CDataLiteralNormalizerPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:
					CDataLiteralValue* createDataLiteralValue(CDataLiteral* dataLiteral, CPreProcessContext* context);

					bool normalizeDataLiteral(CConcept* dataLiteralConcept, CPreProcessContext* context);
					bool normalizeDatatypeRestriction(CConcept* datatypeRestrictionConcept, CPreProcessContext* context);
					CDataLiteral* getNormalizedDataLiteralForLengthRestriction(CDatatype* datatype, CDataLiteral* dataLiteral, bool roundUp, CPreProcessContext* context);

					CConcept* createConcept();
					CConcept* addConceptOperand(CConcept* concept, CConcept* operand, bool negated);


					CDataLiteralValue* parseIntegerValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseDecimalValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseRationalValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseRealValue(const QString& valueString, CPreProcessContext* context);

					CDataLiteralValue* parseStringValue(const QString& valueString, bool parseLanguageString, CPreProcessContext* context);
					CDataLiteralValue* parseDateTimeValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseBooleanValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseDoubleValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseFloatValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseIRIValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseXMLValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseBinaryHexValue(const QString& valueString, CPreProcessContext* context);
					CDataLiteralValue* parseBinaryBase64Value(const QString& valueString, CPreProcessContext* context);


					cint64 getBinaryFromHex(QChar character);
					cint64 getBinaryFromBase64(QChar character);

				// private methods
				private:

				// private variables
				private:
					CABox* mABox;
					CTBox* mTBox;
					CRBox* mRBox;
					CMBox* mMBox;
					CContext* mBoxContext;
					CIndividualVector* mIndiVec;
					CRoleVector* mRolesVector;
					CConceptVector* mConceptVector;
					CConcreteOntology* mOntology;

					CMemoryAllocationManager* mMemMan;

					static bool mConvArraysInitialized;
					static cint64* mHexStringBinaryDataConvArray;
					static cint64* mBase64StringBinaryDataConvArray;

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CDATALITERALNORMALIZERPREPROCESS_H
