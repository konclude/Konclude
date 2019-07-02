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

#ifndef KONCLUDE_REASONER_ONTOLOGY_ONTOLOGYSETTINGS
#define KONCLUDE_REASONER_ONTOLOGY_ONTOLOGYSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/Container/CQtHash.h"
#include "Utilities/Container/CQtList.h"
#include "Utilities/Container/CQtSet.h"
#include "Utilities/Memory/CObjectAllocator.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\file		OntologySettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations

			class COntology;
			class CConcept;
			class CVariable;
			class CRole;
			class CRoleChain;
			class CBoxContext;
			class CIndividual;
			class CTerminology;
			class CConcreteOntology;
			class CTBox;
			class CABox;
			class CNamePrefix;
			class CConceptVector;
			class CProcessReference;

			class COntologyProcessingSteps;
			class COntologyProcessingStepVector;
			class COntologyProcessingStepData;
			class COntologyProcessingStep;
			class COntologyProcessingRequirement;
			class COntologyProcessingStatus;

			// Concept Constructor Codes
			const qint64 CCNONE = 0;
			const qint64 CCATOM = 0;

			const qint64 CCTOP = 1;
			const qint64 CCBOTTOM = -1;

			const qint64 CCNOT = -2;

			const qint64 CCAND = 3;
			const qint64 CCOR = -3;


			const qint64 CCATMOST = 4;
			const qint64 CCATLEAST = -4;

			const qint64 CCALL = 5;
			const qint64 CCSOME = -5;

			const qint64 CCEQ = 6;
			const qint64 CCSUB = 7;

			const qint64 CCNOMINAL = 8;

			const qint64 CCSELF = 9;

			const qint64 CCAQCHOOCE = 10;
			const qint64 CCAQALL = 11;
			const qint64 CCAQSOME = -11;
			const qint64 CCAQAND = 12;

			const qint64 CCVALUE = 13;

			const qint64 CCNOMVAR = 14;
			const qint64 CCNOMTEMPLREF = 15;

			const qint64 CCIMPL = 16;
			const qint64 CCIMPLTRIG = 17;
			const qint64 CCIMPLALL = 18;
			const qint64 CCIMPLAQALL = 19;
			const qint64 CCIMPLAQAND = 20;

			const qint64 CCBRANCHIMPL = 21;
			const qint64 CCBRANCHTRIG = 22;
			const qint64 CCBRANCHALL = 23;
			const qint64 CCBRANCHAQALL = 24;
			const qint64 CCBRANCHAQAND = 25;

			const qint64 CCEQCAND = 26;

			const qint64 CCPBINDTRIG = 27;
			const qint64 CCPBINDIMPL = 28;
			const qint64 CCPBINDGROUND = 29;
			const qint64 CCPBINDALL = 30;
			const qint64 CCPBINDAND = 31;
			const qint64 CCPBINDAQAND = 32;
			const qint64 CCPBINDAQALL = 33;
			const qint64 CCPBINDVARIABLE = 34;
			const qint64 CCPBINDCYCLE = 35;

			const qint64 CCVARBINDTRIG = 36;
			const qint64 CCVARBINDJOIN = 37;
			const qint64 CCVARBINDGROUND = 38;
			const qint64 CCVARBINDALL = 39;
			const qint64 CCVARBINDAND = 40;
			const qint64 CCVARBINDAQAND = 41;
			const qint64 CCVARBINDAQALL = 42;
			const qint64 CCVARBINDVARIABLE = 43;
			const qint64 CCVARBINDIMPL = 44;

			const qint64 CCVARPBACKTRIG = 45;
			const qint64 CCVARPBACKALL = 46;
			const qint64 CCVARPBACKAQAND = 47;
			const qint64 CCVARPBACKAQALL = 48;

			const qint64 CCBACKACTIVTRIG = 49;
			const qint64 CCBACKACTIVIMPL = 50;


#define CMAPPINGHASH CQtHash
#define CMAPPINGLIST CQtList

#define CBUILDHASH CQtHash
#define CBUILDSET CQtSet
#define CBUILDLIST CQtList

#define CONTOLOGYAXIOMSET CQtSet
#define CONTOLOGYAXIOMLIST CQtList

#define CBOXSET CQtSet
#define CBOXHASH CQtHash
#define CBOXLIST CQtList


const cint64 DEFAULTVECTOR2POTSIZE = 9;


const cint64 CCONCEPTVECTOR2POTSIZE						= DEFAULTVECTOR2POTSIZE;
const cint64 CROLEVECTOR2POTSIZE						= DEFAULTVECTOR2POTSIZE;
const cint64 CINDIVIDUALVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CCONCEPTDEPENDENCEVECTOR2POTSIZE			= DEFAULTVECTOR2POTSIZE;
const cint64 CROLEDEPENDENCEVECTOR2POTSIZE				= DEFAULTVECTOR2POTSIZE;
const cint64 CINDIVIDUALDEPENDENCEVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;
const cint64 CPSEUDOMODELVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CROLECHAINVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CCONCEPTDATAVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CBRANCHINGTRIGGERVECTOR2POTSIZE			= DEFAULTVECTOR2POTSIZE;
const cint64 CIMPLICATIONREPLACEMENTVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;
const cint64 CNOMINALSCHEMATEMPLATEVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_ONTOLOGY_ONTOLOGYSETTINGS