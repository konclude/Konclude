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

#ifndef KONCLUDE_INFO_H
#define KONCLUDE_INFO_H

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes
#include "KoncludeSettings.h"


// Other includes


// Logger includes


namespace Konclude {

	/*! 
	 *
	 *		\class		CKoncludeInfo
	 *		\author		Andreas Steigmiller
	 *		\version	0.1
	 *		\brief		TODO
	 *
	 */
	class CKoncludeInfo {
		// public methods
		public:
			//! Constructor
			CKoncludeInfo();

			static QString getKoncludeName();
			static QString getKoncludeNameExtension();
			static QString getKoncludeDescription();
			static QString getKoncludeVersionString();
			static QString getKoncludeCompilationDateString();
			static QString getKoncludeBitPlatformString();


			static int getKoncludeMajorVersionNumber();
			static int getKoncludeMinorVersionNumber();
			static int getKoncludeBuildVersionNumber();
			static int getKoncludeRevisionVersionNumber();

		// protected methods
		protected:

		// protected variables
		protected:		
			
		// private methods
		private:

		// private variables
		private:

	};

}; // end namespace Konclude

#endif // KONCLUDE_INFO_H
