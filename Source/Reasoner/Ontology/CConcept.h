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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPT_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPT_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CRole.h"
#include "CTerminology.h"
#include "CName.h"
#include "CTagItem.h"
#include "CNamedItem.h"
#include "CConceptData.h"
#include "CVariable.h"
#include "CConceptOperator.h"
#include "CDataLiteral.h"

// Other includes
#include "Utilities/CSortedNegLinker.hpp"
#include "Utilities/CAllocationObject.h"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CLinker.hpp"
#include "Utilities/CSortedLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CConcept
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcept : public CTagItem, public CNamedItem, public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CConcept();

					CConcept* initConcept();
					CConcept *initConceptCopy(CConcept *concept, CMemoryAllocationManager* memMan = nullptr);

					CConcept *setConceptTag(qint64 conTag);
					qint64 getConceptTag() const;

					CConcept *setOperatorCode(qint64 opCode);
					qint64 getOperatorCode();

					CConceptOperator* getConceptOperator();

					CConcept *setTerminology(CTerminology *terminology);
					CTerminology *getTerminology();

					qint64 getTerminologyTag() const;
					QPair<qint64,qint64> getTerminologyConceptTagPair();

					CConcept *setParameter(qint64 parameter);
					qint64 getParameter();

					CConcept *setOperandCount(qint64 count);
					qint64 getOperandCount();
					CConcept* incOperandCount(cint64 incCount = 1);

					CConcept *setOperandList(CSortedNegLinker<CConcept *> *operandList);
					CConcept *addOperandLinker(CSortedNegLinker<CConcept *> *operandLinker);
					CSortedNegLinker<CConcept *> *getOperandList();

					CConcept *setRole(CRole *role);
					CRole *getRole();

					CVariable* getVariable();
					CSortedLinker<CVariable*>* getVariableLinker();
					CConcept* setVariableLinker(CSortedLinker<CVariable*>* variableLinker);
					CConcept* addVariableLinker(CSortedLinker<CVariable*>* variableLinker);

					bool hasClassName();
					CConcept *setClassNameLinker(CLinker<CName *> *conceptNameLinker);
					CConcept *addClassNameLinker(CLinker<CName *> *conceptNameLinker);
					CLinker<CName *> *getClassNameLinker();

					bool hasOperandConceptTag(CConcept *concept);
					bool hasOperandConceptTag(qint64 conceptTag);

					bool hasOperandConceptTag(CConcept *concept, bool negation);
					bool hasOperandConceptTag(qint64 conceptTag, bool negation);

					bool hasOperandConcept(CConcept *concept, bool negation);
					bool hasOperandConcept(CConcept *concept);


					CIndividual* getNominalIndividual();
					CConcept* setNominalIndividual(CIndividual* nominalIndividual);
					bool hasNominalIndividual();


					bool hasConceptData();
					CConceptData* getConceptData();
					CConcept* setConceptData(CConceptData* conceptData);




					bool hasMappingNegation();
					CConcept* setMappingNegation(bool negation);


					CDataLiteral* getDataLiteral();
					CConcept* setDataLiteral(CDataLiteral* dataLiteral);

					CDatatype* getDatatype();
					CConcept* setDatatype(CDatatype* datatype);

					// deprecated

					CConcept *init(qint64 conTag);
					CConcept *init(CConcept *concept);

					CConcept *setOperatorTag(qint64 opTag);
					qint64 getDefinitionOperatorTag();
					qint64 getProcessingOperatorTag();

					bool operator<=(const CConcept &concept);
					bool operator==(const CConcept &concept);

					bool isGeneratingOperator(bool negated = false);
					bool isNegatedGeneratingOperator();
					bool isNonGeneratingOperator();

					bool isEqualsToBOTTOM(bool negated = false);
					bool isEqualsToTOP(bool negated = false);


				// protected methods
				protected:

				// protected variables
				protected:
					CTerminology *tax;
					CConceptOperator* mOperatorCode;
					qint64 opParameter;
					qint64 operandCount;
					CRole *operatorRole;
					CSortedNegLinker<CConcept*> *operands;
					CIndividual* mNominalIndiviual;
					CSortedLinker<CVariable*>* mVariableLinker;
					CDataLiteral* mDataLiteral;
					CDatatype* mDatatype;

					CConceptData* mConceptData;
					bool mMappingNegated;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPT_H
