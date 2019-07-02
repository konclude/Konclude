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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTOPERATOR_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTOPERATOR_H

// Libraries includes
#include <QMutex>

// Namespace includes
#include "OntologySettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CConceptOperator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptOperator {
				// public methods
				public:

					cint64 getOperatorCode();
					cint64 getOperatorFlag();


					static CConceptOperator* getConceptOperator(cint64 opCode);

					static void generateConceptOperators();


					const static cint64 CCF_NONE				= Q_UINT64_C(0x0000000000000001);
					const static cint64 CCF_ATOM				= Q_UINT64_C(0x0000000000000001);
					const static cint64 CCF_TOP					= Q_UINT64_C(0x0000000000000002);
					const static cint64 CCF_BOTTOM				= Q_UINT64_C(0x0000000000000004);
					const static cint64 CCF_NOT					= Q_UINT64_C(0x0000000000000008);
					const static cint64 CCF_AND					= Q_UINT64_C(0x0000000000000010);
					const static cint64 CCF_OR					= Q_UINT64_C(0x0000000000000020);
					const static cint64 CCF_ATMOST				= Q_UINT64_C(0x0000000000000040);
					const static cint64 CCF_ATLEAST				= Q_UINT64_C(0x0000000000000080);
					const static cint64 CCF_ALL					= Q_UINT64_C(0x0000000000000100);
					const static cint64 CCF_SOME				= Q_UINT64_C(0x0000000000000200);
					const static cint64 CCF_EQ					= Q_UINT64_C(0x0000000000000400);
					const static cint64 CCF_SUB					= Q_UINT64_C(0x0000000000000800);
					const static cint64 CCF_NOMINAL				= Q_UINT64_C(0x0000000000001000);
					const static cint64 CCF_SELF				= Q_UINT64_C(0x0000000000002000);
					const static cint64 CCF_AQCHOOCE			= Q_UINT64_C(0x0000000000004000);
					const static cint64 CCF_AQALL				= Q_UINT64_C(0x0000000000008000);
					const static cint64 CCF_AQSOME				= Q_UINT64_C(0x0000000000010000);
					const static cint64 CCF_AQAND				= Q_UINT64_C(0x0000000000020000);
					const static cint64 CCF_VALUE				= Q_UINT64_C(0x0000000000040000);
					const static cint64 CCF_NOMVAR				= Q_UINT64_C(0x0000000000080000);
					const static cint64 CCF_NOMTEMPLREF			= Q_UINT64_C(0x0000000000100000);
					const static cint64 CCF_IMPL				= Q_UINT64_C(0x0000000000200000);
					const static cint64 CCF_IMPLTRIG			= Q_UINT64_C(0x0000000000400000);
					const static cint64 CCF_IMPLALL				= Q_UINT64_C(0x0000000000800000);
					const static cint64 CCF_IMPLAQALL			= Q_UINT64_C(0x0000000001000000);
					const static cint64 CCF_IMPLAQAND			= Q_UINT64_C(0x0000000002000000);
					const static cint64 CCF_BRANCHIMPL			= Q_UINT64_C(0x0000000004000000);
					const static cint64 CCF_BRANCHTRIG			= Q_UINT64_C(0x0000000008000000);
					const static cint64 CCF_BRANCHALL			= Q_UINT64_C(0x0000000010000000);
					const static cint64 CCF_BRANCHAQALL			= Q_UINT64_C(0x0000000020000000);
					const static cint64 CCF_BRANCHAQAND			= Q_UINT64_C(0x0000000040000000);
					const static cint64 CCF_EQCAND				= Q_UINT64_C(0x0000000080000000);
					const static cint64 CCF_PBINDTRIG			= Q_UINT64_C(0x0000000100000000);
					const static cint64 CCF_PBINDIMPL			= Q_UINT64_C(0x0000000200000000);
					const static cint64 CCF_PBINDGROUND			= Q_UINT64_C(0x0000000400000000);
					const static cint64 CCF_PBINDALL			= Q_UINT64_C(0x0000000800000000);
					const static cint64 CCF_PBINDAND			= Q_UINT64_C(0x0000001000000000);
					const static cint64 CCF_PBINDAQAND			= Q_UINT64_C(0x0000002000000000);
					const static cint64 CCF_PBINDAQALL			= Q_UINT64_C(0x0000004000000000);
					const static cint64 CCF_PBINDVARIABLE		= Q_UINT64_C(0x0000008000000000);
					const static cint64 CCF_PBINDCYCLE			= Q_UINT64_C(0x0000010000000000);
					const static cint64 CCF_VARBINDTRIG			= Q_UINT64_C(0x0000020000000000);
					const static cint64 CCF_VARBINDJOIN			= Q_UINT64_C(0x0000040000000000);
					const static cint64 CCF_VARBINDGROUND		= Q_UINT64_C(0x0000080000000000);
					const static cint64 CCF_VARBINDALL			= Q_UINT64_C(0x0000100000000000);
					const static cint64 CCF_VARBINDAND			= Q_UINT64_C(0x0000200000000000);
					const static cint64 CCF_VARBINDAQAND		= Q_UINT64_C(0x0000400000000000);
					const static cint64 CCF_VARBINDAQALL		= Q_UINT64_C(0x0000800000000000);
					const static cint64 CCF_VARBINDVARIABLE		= Q_UINT64_C(0x0001000000000000);
					const static cint64 CCF_VARBINDIMPL			= Q_UINT64_C(0x0002000000000000);
					const static cint64 CCF_VARPBACKTRIG		= Q_UINT64_C(0x0004000000000000);
					const static cint64 CCF_VARPBACKALL			= Q_UINT64_C(0x0008000000000000);
					const static cint64 CCF_VARPBACKAQAND		= Q_UINT64_C(0x0010000000000000);
					const static cint64 CCF_VARPBACKAQALL		= Q_UINT64_C(0x0020000000000000);
					const static cint64 CCF_BACKACTIVTRIG		= Q_UINT64_C(0x0040000000000000);
					const static cint64 CCF_BACKACTIVIMPL		= Q_UINT64_C(0x0080000000000000);

					const static cint64 CCF_DATATYPE			= Q_UINT64_C(0x0100000000000000);
					const static cint64 CCF_DATALITERAL			= Q_UINT64_C(0x0200000000000000);
					const static cint64 CCF_DATARESTRICTION		= Q_UINT64_C(0x0400000000000000);

					const static cint64 CCF_MARKER				= Q_UINT64_C(0x0800000000000000);

					//const static cint64 CCF_NOMINALIMPLI		= Q_UINT64_C(0x1000000000000000);
					//const static cint64 CCF_DATATYPEIMPLI		= Q_UINT64_C(0x2000000000000000);
					//const static cint64 CCF_DATALITERALIMPLI	= Q_UINT64_C(0x4000000000000000);
					//const static cint64 CCF_DATARESTRICTIONIMPLI= Q_UINT64_C(0x8000000000000000);
					const static cint64 CCF_SPECIAL				 = Q_UINT64_C(0x8000000000000000);



					bool hasPartialOperatorCodeFlag(cint64 operatorCodeFlag);
					bool hasAllOperatorCodeFlags(cint64 operatorCodeFlags);



					const static cint64 CCFS_AND_TYPE						= CCF_AND | CCF_PBINDAND | CCF_VARBINDAND | CCF_TOP;
					const static cint64 CCFS_TRIG_TYPE						= CCF_IMPLTRIG | CCF_BRANCHTRIG | CCF_PBINDTRIG | CCF_VARBINDTRIG | CCF_VARPBACKTRIG;
					const static cint64 CCFS_ALL_TYPE						= CCF_VARPBACKALL | CCF_VARBINDALL | CCF_PBINDALL | CCF_BRANCHALL | CCF_IMPLALL | CCF_ALL;

					const static cint64 CCFS_AQAND_TYPE						= CCF_AQAND | CCF_IMPLAQAND | CCF_BRANCHAQAND | CCF_PBINDAQAND | CCF_VARBINDAQAND | CCF_VARPBACKAQAND;
					const static cint64 CCFS_AQALL_TYPE						= CCF_AQALL | CCF_IMPLAQALL | CCF_BRANCHAQALL | CCF_PBINDAQALL | CCF_VARBINDAQALL | CCF_VARPBACKAQALL;
					
					const static cint64 CCFS_AQ_TYPE						= CCFS_AQAND_TYPE | CCFS_AQALL_TYPE;
					const static cint64 CCFS_ALL_AQALL_TYPE 				= CCFS_ALL_TYPE | CCFS_AQALL_TYPE;
					const static cint64 CCFS_TRIG_AND_AQAND_TYPE 			= CCFS_AND_TYPE | CCFS_TRIG_TYPE | CCFS_AQAND_TYPE;
					const static cint64 CCFS_AND_AQAND_TYPE 				= CCFS_AND_TYPE | CCFS_AQAND_TYPE;
					const static cint64 CCFS_AQAND_AQALL_TYPE 				= CCFS_AQAND_TYPE | CCFS_AQALL_TYPE;


					const static cint64 CCFS_SOME_TYPE						= CCF_SOME | CCF_AQSOME;

					const static cint64 CCFS_IMPL_TYPE						= CCF_VARBINDIMPL | CCF_PBINDIMPL | CCF_BRANCHIMPL | CCF_IMPL | CCF_BACKACTIVIMPL;
					//const static cint64 CCFS_EXT_IMPL_TYPE					= CCFS_IMPL_TYPE | CCF_NOMINALIMPLI | CCF_DATATYPEIMPLI | CCF_DATALITERALIMPLI | CCF_DATARESTRICTIONIMPLI;

					const static cint64 CCFS_PROPAGATION_BIND_TYPE			= CCF_PBINDTRIG | CCF_PBINDIMPL | CCF_PBINDGROUND | CCF_PBINDALL | CCF_PBINDAND | CCF_PBINDAQAND | CCF_PBINDAQALL | CCF_PBINDVARIABLE | CCF_PBINDCYCLE;
					const static cint64 CCFS_VARIABLE_BIND_TYPE				= CCF_VARBINDTRIG | CCF_VARBINDJOIN | CCF_VARBINDGROUND | CCF_VARBINDALL | CCF_VARBINDAND | CCF_VARBINDAQAND | CCF_VARBINDAQALL | CCF_VARBINDVARIABLE | CCF_VARBINDIMPL;
					const static cint64 CCFS_BACK_PROPAGATION_TYPE			= CCF_VARPBACKTRIG | CCF_VARPBACKALL | CCF_VARPBACKAQAND | CCF_VARPBACKAQALL | CCF_BACKACTIVTRIG | CCF_BACKACTIVIMPL;

					const static cint64 CCFS_PROPAGATION_TYPE				= CCFS_PROPAGATION_BIND_TYPE | CCFS_VARIABLE_BIND_TYPE | CCFS_BACK_PROPAGATION_TYPE;


					const static cint64 CCFS_PROPAGATION_ALL_TYPE			= CCF_PBINDALL | CCF_VARBINDALL | CCF_VARPBACKALL | CCF_PBINDAQALL | CCF_VARBINDAQALL | CCF_VARPBACKAQALL;

					const static cint64 CCFS_PROPAGATION_AND_TYPE			= CCF_PBINDTRIG | CCF_VARBINDTRIG | CCF_VARPBACKTRIG | CCF_PBINDAND | CCF_VARBINDAND | CCF_PBINDAQAND | CCF_VARBINDAQAND | CCF_BACKACTIVTRIG;

					const static cint64 CCFS_POSSIBLE_ROLE_CREATION_TYPE	= CCF_SOME | CCF_AQSOME | CCF_ALL | CCF_ATLEAST | CCF_ATMOST;

					const static cint64 CCFS_ABSORPTION_RELEVANT_TYPE		= CCFS_TRIG_TYPE | CCFS_IMPL_TYPE | CCF_IMPLAQALL | CCF_BRANCHAQALL | CCF_PBINDAQALL | CCF_VARBINDAQALL | CCF_VARPBACKAQALL | CCF_IMPLAQAND | CCF_BRANCHAQAND | CCF_PBINDAQAND | CCF_VARBINDAQAND | CCF_VARPBACKAQAND;

					const static cint64 CCFS_DATATYPE_RELATED_TYPE			= CCF_DATATYPE | CCF_DATALITERAL | CCF_DATARESTRICTION;


				// protected methods
				protected:
					//! Constructor
					CConceptOperator(cint64 opCode);

				// protected variables
				protected:
					cint64 mOperatorCode;

					cint64 mTypeFlag;


					static CConceptOperator** mConceptOperatorVector;
					static QMutex* mCreateLockMutex;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTOPERATOR_H
