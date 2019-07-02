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

#ifndef KONCLUDE_REASONER_PREPROCESS_CDATATYPENORMALIZERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CDATATYPENORMALIZERPREPROCESS_H

// Libraries includes
#include <QSet>

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
			 *		\class		CDatatypeNormalizerPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeNormalizerPreProcess : public CConcreteOntologyContinuablePreProcess {
				// public methods
				public:
					//! Constructor
					CDatatypeNormalizerPreProcess();

					//! Destructor
					virtual ~CDatatypeNormalizerPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);
					virtual CConcreteOntology* continuePreprocessing();

				// protected methods
				protected:
					CDatatypeNormalizerPreProcess* normalizeDatatype(CDatatype* datatype, CPreProcessContext* context);

					CConcept* createDatatypeConcept(CDatatype* datatype);
					CConcept* createConcept();
					CConcept* addConceptOperand(CConcept* concept, CConcept* operand, bool negated);


					CDatatype* getDatatype(const QString& datatypeIRI);


					CConcept* getNumberRestrictionConcept(cint64 restrictionCode, const QString& value);
					CConcept* getIntegerRestrictionConcept();

					CConcept* getStringRestrictionConcept();
					CConcept* getNormalizedStringRestrictionConcept();
					CConcept* getTokenRestrictionConcept();
					CConcept* getNMTokenRestrictionConcept();
					CConcept* getNameRestrictionConcept();
					CConcept* getNCNameRestrictionConcept();
					CConcept* getLanguageRestrictionConcept();
					CConcept* getDateTimeStampRestrictionConcept();


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

					CConcept* mTopDataRangeConcept;

					CDatatypeVector* mDatatypeVector;

					CDatatypeValueSpaceTypes* mValueSpaceTypes;

					QHash<QString,CDatatype*> mIRIDatatypeHash;

					CBUILDHASH<CDataRangeTermExpression*,CConcept*>* mDataRangeConceptHash;
					CBUILDHASH<CConcept*,CDataRangeTermExpression*>* mConceptDataRangeHash;

					CBUILDHASH<CDatatypeExpression*,CDatatype*>* mDatatypeExpressionDatatpyeHash;
					CBUILDHASH<CDatatype*,CDatatypeExpression*>* mDatatpyeDatatypeExpressionHash;

					CMemoryAllocationManager* mMemMan;


					QHash< QPair<cint64,QString>,CConcept* > mNumberRestrictionConceptHash;

					CConcept* mIntegerRestrictionConcept;
					CConcept* mStringRestrictionConcept;
					CConcept* mNormalizedStringRestrictionConcept;
					CConcept* mTokenRestrictionConcept;
					CConcept* mNMTokenRestrictionConcept;
					CConcept* mNameRestrictionConcept;
					CConcept* mNCNameRestrictionConcept;
					CConcept* mLanguageRestrictionConcept;
					CConcept* mDatetimeStampRestrictionConcept;

					CPreProcessContext* mContext;

					cint64 mLastDataTypeId;

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CDATATYPENORMALIZERPREPROCESS_H
