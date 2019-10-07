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

#ifndef KONCLUDE_REASONER_PREPROCESS_CDATALITERALNORMALIZERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CDATALITERALNORMALIZERPREPROCESS_H

// Libraries includes
#include <QSet>
#include <QDomDocument>

// Namespace includes
#include "CConcreteOntologyContinuablePreProcess.h"
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
			class CDataLiteralNormalizerPreProcess : public CConcreteOntologyContinuablePreProcess {
				// public methods
				public:
					//! Constructor
					CDataLiteralNormalizerPreProcess();

					//! Destructor
					virtual ~CDataLiteralNormalizerPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

					virtual CConcept *preprocessDataConcept(CConcept* dataConcept, CConcreteOntology *ontology, CPreProcessContext* context);
					virtual CConcreteOntology* continuePreprocessing();

					CDataLiteralValue* createPreprocessedDataLiteralValue(CDataLiteral* dataLiteral, CPreProcessContext* context, CContext* boxContext);


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
					CABox* mABox = nullptr;
					CTBox* mTBox = nullptr;
					CRBox* mRBox = nullptr;
					CMBox* mMBox = nullptr;
					CContext* mBoxContext = nullptr;
					CIndividualVector* mIndiVec = nullptr;
					CRoleVector* mRolesVector = nullptr;
					CConceptVector* mConceptVector = nullptr;
					CConcreteOntology* mOntology = nullptr;

					CMemoryAllocationManager* mMemMan = nullptr;

					static bool mConvArraysInitialized;
					static cint64* mHexStringBinaryDataConvArray;
					static cint64* mBase64StringBinaryDataConvArray;


					CPreProcessContext* mContext = nullptr;
					cint64 mLastConceptId = 0;
					cint64 mLastIndiId = 0;

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CDATALITERALNORMALIZERPREPROCESS_H
