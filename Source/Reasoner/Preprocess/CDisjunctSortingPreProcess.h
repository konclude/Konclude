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

#ifndef KONCLUDE_REASONER_PREPROCESS_CDISJUNCTSORTINGPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CDISJUNCTSORTINGPREPROCESS_H

// Libraries includes

// Namespace includes
#include "CConcreteOntologyContinuablePreProcess.h"
#include "CConceptRoleIndividualLocator.h"


// Other includes
#include "Reasoner/Ontology/CConceptRoleBranchingTrigger.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CDisjunctSortingPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDisjunctSortingPreProcess : public CConcreteOntologyContinuablePreProcess {
				// public methods
				public:
					//! Constructor
					CDisjunctSortingPreProcess();

					//! Destructor
					virtual ~CDisjunctSortingPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);
					virtual CConcreteOntology* continuePreprocessing();

				// protected methods
				protected:
					CDisjunctSortingPreProcess* sortDisjuncts();

					void sortDisjuncts(CConcept* concept, bool negated);

					void addDisjunctFromList(QList< CSortedNegLinker<CConcept*>* >& disjunctList, CSortedNegLinker<CConcept*>*& newOperandLinker, CSortedNegLinker<CConcept*>*& lastNewOperandLinker);

					void addDisjunctToSort(CSortedNegLinker<CConcept*>* conLinker, bool negated);


				// protected variables
				protected:
					CMemoryAllocationManager* mMemMan;

					cint64 mSortedDisjunctCount;
					cint64 mPotentialDisjunctionCount;

					CConcreteOntology* mOnto;
					CConceptVector* mConceptVec;
					CTBox* mTBox;


					QList< CSortedNegLinker<CConcept*>* > mAtomicDisjList;
					QList< CSortedNegLinker<CConcept*>* > mPrimNegDisjList;
					QList< CSortedNegLinker<CConcept*>* > mSimpleExistDisjList;
					QList< CSortedNegLinker<CConcept*>* > mExistDisjList;
					QList< CSortedNegLinker<CConcept*>* > mNegNominalDisjList;
					QList< CSortedNegLinker<CConcept*>* > mPosNominalDisjList;
					QList< CSortedNegLinker<CConcept*>* > mPropDisjList;
					QList< CSortedNegLinker<CConcept*>* > mTriggerDisjList;
					QList< CSortedNegLinker<CConcept*>* > mImplDisjList;
					QList< CSortedNegLinker<CConcept*>* > mOtherDisjList;

					cint64 mLastConceptId;


				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CDISJUNCTSORTINGPREPROCESS_H
