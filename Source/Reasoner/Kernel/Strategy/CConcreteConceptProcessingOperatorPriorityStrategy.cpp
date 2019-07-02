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

#include "CConcreteConceptProcessingOperatorPriorityStrategy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {

				CConcreteConceptProcessingOperatorPriorityStrategy::CConcreteConceptProcessingOperatorPriorityStrategy() {
					priCount = 200;
					priorities = new double[priCount];
					symAccessPri = priorities+priCount/2;

					for (int i = 0; i < priCount; ++i) {
						priorities[i] = 0;
					}

					// The following priorities are similar to the FaCT++ standard IAOEFLG options and have the same meaning.

					// FaCT++ register "IAOEFLG" option:
					// Option 'IAOEFLG' define the priorities of different operations in TODO list. Possible values are 
					// 7-digit strings with only possible digit are 0-6. The digits on the places 1, 2, ..., 7 are for 
					// priority of Id, And, Or, Exists, Forall, LE and GE operations respectively. The smaller number means 
					// the higher priority. All other constructions (TOP, BOTTOM, etc) has priority 0.

					// The operations are ordered in a priority queue and are applied also from lower to highest priority.
					//symAccessPri[CCATOM]    = 2;
					//symAccessPri[CCAND]     = 3;
					//symAccessPri[CCOR]      = 7;
					//symAccessPri[CCSOME]    = 4;
					//symAccessPri[CCALL]     = 1;
					//symAccessPri[CCATLEAST] = 1;
					//symAccessPri[CCATMOST]  = 6;
					cint64 nextPriority = 14;


					symAccessPri[CCTOP]				= nextPriority;
					symAccessPri[-CCBOTTOM]			= nextPriority;
					symAccessPri[CCATOM]			= nextPriority;
					symAccessPri[-CCATOM]			= nextPriority;

					nextPriority = 13;

					symAccessPri[CCAND]				= nextPriority;
					symAccessPri[-CCOR]				= nextPriority;
					symAccessPri[CCSUB]				= nextPriority;
					symAccessPri[CCEQ]				= nextPriority;
					symAccessPri[CCIMPLTRIG]		= nextPriority;
					symAccessPri[CCBRANCHTRIG]		= nextPriority;
					symAccessPri[CCPBINDTRIG]		= nextPriority;
					symAccessPri[CCPBINDAND]		= nextPriority;
					symAccessPri[CCVARBINDTRIG]		= nextPriority;
					symAccessPri[CCVARBINDAND]		= nextPriority;
					symAccessPri[CCVARPBACKTRIG]	= nextPriority;
					symAccessPri[CCBACKACTIVTRIG]	= nextPriority;

					symAccessPri[CCOR]				= nextPriority;
					symAccessPri[-CCAND]			= nextPriority;
					symAccessPri[-CCEQ]				= nextPriority;

					mDisjDelConsidPriOffset = -11.0;
					mDisjDelProcessPriOffset = -11.5;


					nextPriority = 12;

					symAccessPri[CCALL]				= nextPriority;
					symAccessPri[-CCSOME]			= nextPriority;
					symAccessPri[CCAQALL]			= nextPriority;
					symAccessPri[CCIMPLALL]			= nextPriority;
					symAccessPri[CCBRANCHALL]		= nextPriority;
					symAccessPri[CCIMPLAQALL]		= nextPriority;
					symAccessPri[CCBRANCHAQALL]		= nextPriority;
					symAccessPri[CCPBINDALL]		= nextPriority;
					symAccessPri[CCVARBINDALL]		= nextPriority;
					symAccessPri[CCVARBINDAQALL]	= nextPriority;
					symAccessPri[CCVARPBACKAQALL]	= nextPriority;
					symAccessPri[CCVARPBACKALL]		= nextPriority;

					nextPriority = 11;

					symAccessPri[CCAQAND]			= nextPriority;
					symAccessPri[CCIMPLAQAND]		= nextPriority;
					symAccessPri[CCBRANCHAQAND]		= nextPriority;
					symAccessPri[CCPBINDAQAND]		= nextPriority;
					symAccessPri[CCVARBINDAQAND]	= nextPriority;
					symAccessPri[CCVARPBACKAQAND]	= nextPriority;

					nextPriority = 10;

					symAccessPri[CCAQCHOOCE]		= nextPriority;
					symAccessPri[-CCAQCHOOCE]		= nextPriority;

					nextPriority = 9;

					symAccessPri[CCIMPL]			= nextPriority;
					symAccessPri[CCBRANCHIMPL]		= nextPriority;
					symAccessPri[CCPBINDIMPL]		= nextPriority;
					symAccessPri[CCPBINDVARIABLE]	= nextPriority;
					symAccessPri[CCPBINDCYCLE]		= nextPriority;
					symAccessPri[CCVARBINDJOIN]		= nextPriority;
					symAccessPri[CCVARBINDVARIABLE]	= nextPriority;
					symAccessPri[CCBACKACTIVIMPL]	= nextPriority;
					symAccessPri[CCVARBINDIMPL]		= nextPriority;

					nextPriority = 8;
					symAccessPri[CCSELF]			= nextPriority;
					symAccessPri[-CCSELF]			= nextPriority;
					symAccessPri[CCVALUE]			= nextPriority;
					symAccessPri[-CCVALUE]			= nextPriority;

					// intermediately processing limit

					nextPriority = 7;

					symAccessPri[CCNOMINAL]			= nextPriority;
					symAccessPri[-CCNOMINAL]		= nextPriority;

					nextPriority = 6;

					symAccessPri[CCPBINDGROUND]		= nextPriority;
					symAccessPri[CCVARBINDGROUND]	= nextPriority;
					symAccessPri[-CCPBINDGROUND]	= nextPriority;
					symAccessPri[-CCVARBINDGROUND]	= nextPriority;

					nextPriority = 5;

					symAccessPri[CCATLEAST]			= nextPriority;
					symAccessPri[-CCATMOST]			= nextPriority;

					nextPriority = 4;

					symAccessPri[CCSOME]			= nextPriority;
					symAccessPri[-CCALL]			= nextPriority;
					symAccessPri[CCAQSOME]			= nextPriority;

					// deterministic processing limit

					nextPriority = 3;

					symAccessPri[CCATMOST]			= nextPriority;
					symAccessPri[-CCATLEAST]		= nextPriority;

					nextPriority = 2;

					// disjunctions + processing offsets






				}


				CConcreteConceptProcessingOperatorPriorityStrategy::~CConcreteConceptProcessingOperatorPriorityStrategy() {
					delete [] priorities;
				}

				double CConcreteConceptProcessingOperatorPriorityStrategy::getPriorityOffsetForDisjunctionDelayedConsidering(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode *individual) {
					return mDisjDelConsidPriOffset;
				}

				double CConcreteConceptProcessingOperatorPriorityStrategy::getPriorityOffsetForDisjunctionDelayedProcessing(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode *individual) {
					return mDisjDelProcessPriOffset;
				}


				CConceptProcessPriority CConcreteConceptProcessingOperatorPriorityStrategy::getPriorityForConcept(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode *individual) {
					qint64 cCode = conceptDescriptor->getData()->getOperatorCode();
					bool negated = false;
					if (conceptDescriptor->isNegated()) {
						cCode *= -1;
						negated = true;
					}
					double priority = 0;
					priority = symAccessPri[cCode];

					CSortedNegLinker<CConcept *> *opConLinkerIt = conceptDescriptor->getData()->getOperandList();
					if (cCode == CCATMOST) {
						qint64 param = conceptDescriptor->getData()->getParameter();
						param = param - 1*negated;
						if (param <= 1 && !opConLinkerIt) {
							priority = 5.5;
						} else {
							double priorityOffset = (exp(-param / 10.)) * 0.5;
							priority += priorityOffset;
						}
					} else if (cCode == CCATLEAST) {
						qint64 param = conceptDescriptor->getData()->getParameter();
						param = param + 1*negated;
						if (param <= 2 && !opConLinkerIt) {
							priority = 5.5;
						} else {
							double priorityOffset = (1. - exp(-param / 10.)) * 0.5;
							priority += priorityOffset;
						}
					}
					return CConceptProcessPriority(priority);
				}

			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
