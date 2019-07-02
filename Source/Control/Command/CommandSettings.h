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

#ifndef KONCLUDE_Control_COMMAND_COMMANDSETTINGS
#define KONCLUDE_Control_COMMAND_COMMANDSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes


// Logger includes


namespace Konclude {

	namespace Control {

		namespace Command {


			/*! 
			 *
			 *		\file		CommandSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			// forward declarations
			class CCommandRecorder;
			class CCommandRecordData;
			class CCommand;
			class CCommandRecordRouter;


			// Command Tags
			const qint64 INITIALIZECONFIGURATIONCOMMAND							= 2001;
			const qint64 INITIALIZEREASONERCOMMAND								= 2002;
			const qint64 GETCONFIGURATIONCOMMAND								= 2003;
			const qint64 SETCONFIGURATIONCOMMAND								= 2004;
			const qint64 GETDESCRIPTIONCOMMAND									= 2005;
			const qint64 PARSECOMMANDSCOMMAND									= 2006;
			const qint64 PARSEOWLLINKCOMMANDSCOMMAND							= 2007;
			const qint64 CREATEKNOWLEDGEBASECOMMAND								= 2008;
			const qint64 TELLKNOWLEDGEBASEOWL2XMLCOMMAND						= 2009;
			const qint64 CREATEKNOWLEDGEBASEREVISIONCOMMAND						= 2010;
			const qint64 PARSEOWL2XMLONTOLOGYCOMMAND							= 2011;
			const qint64 INSTALLKNOWLEDGEBASEREVISIONCOMMAND					= 2012;
			const qint64 PARSEOWL2XMLQUERYCOMMAND								= 2013;
			const qint64 CALCULATEQUERYCOMMAND									= 2014;
			const qint64 GETCURRENTKNOWLEDGEBASEREVISIONCOMMAND					= 2015;
			const qint64 SATISFIABLEQUERYKNOWLEDGEBASEOWL2XMLCOMMAND			= 2016;
			const qint64 SKIPUNSUPPORTEDCOMMANDCOMMAND							= 2017;
			const qint64 RELEASEKNOWLEDGEBASECOMMAND							= 2018;
			const qint64 GETREASONERCOMMAND										= 2019;
			const qint64 EQUIVALENCEQUERYKNOWLEDGEBASEOWL2XMLCOMMAND			= 2020;
			const qint64 SUBSUMEDQUERYKNOWLEDGEBASEOWL2XMLCOMMAND   			= 2021;
			const qint64 DISJOINTQUERYKNOWLEDGEBASEOWL2XMLCOMMAND				= 2022;
			const qint64 INSTANCEQUERYKNOWLEDGEBASEOWL2XMLCOMMAND				= 2023;
			const qint64 BUILDQUERYCOMMAND										= 2024;
			const qint64 TESTEXTENDEDCALCULATEQUERYCOMMAND						= 2025;
			const qint64 TESTTESTSUITECOMMAND									= 2026;
			const qint64 PARSETESTSUITECOMMANDSCOMMAND							= 2027;
			const qint64 TESTTESTFILECOMMAND									= 2028;
			const qint64 PARSETESTSUITEXMLCOMMAND								= 2029;
			const qint64 INSERTRESPONSECOMMENT									= 2030;
			const qint64 GETKNOWLEGEBASECONFIGURATIONCOMMAND					= 2031;
			const qint64 SETKNOWLEDGEBASECONFIGURATIONCOMMAND					= 2032;
			const qint64 GETSETTINGSCOMMAND										= 2033;
			const qint64 GETKNOWLEGEBASESETTINGSCOMMAND							= 2034;
			const qint64 SCHIERARCHYQUERYKNOWLEDGEBASEOWL2XMLCOMMAND			= 2035;
			const qint64 CONSTRUCTSUBCLASSHIERARCHYQUERYCOMMAND					= 2036;
			const qint64 GETCLASSIFICATIONMANAGERCOMMAND						= 2037;
			const qint64 LOADKNOWLEDGEBASEKRSSTESTCONCEPTCOMMAND				= 2038;
			const qint64 READPARSEKRSSTESTCONCEPTONTOLOGYCOMMAND				= 2039;
			const qint64 READTEXTCOMMAND										= 2040;
			const qint64 SAVEOWL2XMLKNOWLEDGEBASECOMMAND						= 2041;
			const qint64 WRITEOWL2XMLONTOLOGYCOMMAND							= 2042;
			const qint64 EXTRACTTOLDEXPRESSIONONTOLOGYCOMMAND					= 2043;
			const qint64 WRITEOWLLINKOWL2XMLSATISFIABLETESTCOMMAND				= 2044;
			const qint64 SAVEOWLLINKOWL2XMLSATTESTKBCOMMAND						= 2045;
			const qint64 LOADKNOWLEDGEBASEOWLXMLONTOLOGYCOMMAND					= 2046;
			const qint64 SAXPARSEOWL2XMLONTOLOGYCOMMAND							= 2047;
			const qint64 STREAMPARSEOWL2XMLONTOLOGYCOMMAND						= 2048;
			const qint64 CHOOSEPARSEINSTALLKNOWLEDGEBASEOWLXMLONTOLOGYCOMMAND	= 2049;
			const qint64 PREPAREKNOWLEDGEBASEFORQUERYCOMMANDCOMMAND				= 2050;
			const qint64 GENERATEQUERYCOMMANDCOMMAND							= 2051;
			const qint64 PREPAREKNOWLEDGEBASEFORREVISIONCOMMANDCOMMAND			= 2052;
			const qint64 FLATTENEDTYPESQUERYKNOWLEDGEBASEOWL2XMLCOMMAND			= 2053;
			const qint64 CONSTRUCTFLATTENEDTYPESQUERYCOMMAND					= 2054;
			const qint64 PROCESSQUERYKNOWLEDGEBASEOWL2XMLCOMMAND				= 2055;
			const qint64 PROCESSKNOWLEDGEBASECLASSIFYCOMMAND					= 2056;
			const qint64 PROCESSKNOWLEDGEBASEREALIZECOMMAND						= 2057;
			const qint64 PREPAREKNOWLEDGEBASECOMMANDCOMMAND						= 2058;
			const qint64 EVALUATETESTSUITECOMMAND								= 2059;
			const qint64 CONSTRUCTWRITEFUNCTIONALSUBCLASSHIERARCHYQUERYCOMMAND	= 2060;
			const qint64 WRITEFUNCTIONALSUBCLASSHIERARCHYQUERYCOMMAND			= 2061;
			const qint64 ISCONSISTENTKNOWLEDGEBASEOWL2XMLCOMMAND   				= 2062;
			const qint64 CONSTRUCTISCONSISTENTQUERYCOMMAND						= 2063;
			const qint64 PROCESSCLASSNAMESATISFIABLEQUERYKNOWLEDGEBASECOMMAND	= 2064;
			const qint64 CONSTRUCTCLASSSATISFIABLEQUERYCOMMAND					= 2065;
			const qint64 ISCONSISTENTKNOWLEDGEBASECOMMAND   					= 2066;
			const qint64 CLASSIFYQUERYCOMMAND   								= 2067;
			const qint64 CCONSTRUCTCLASSIFYQUERYCOMMAND   						= 2068;
			const qint64 REALIZEQUERYCOMMAND   									= 2069;
			const qint64 CCONSTRUCTREALIZEQUERYCOMMAND   						= 2070;
			const qint64 EXTRACTKNOWLEDGEBASEENTITIESCOMMAND   					= 2071;
			const qint64 EXTRACTKNOWLEDGEBASECLASSENTITIESCOMMAND   			= 2072;
			const qint64 EXTRACTKNOWLEDGEBASEOBJECTPROPERTYENTITIESCOMMAND   	= 2073;
			const qint64 EXTRACTKNOWLEDGEBASENAMEDINDIVIDUALENTITIESCOMMAND   	= 2074;
			const qint64 EXTRACTKNOWLEDGEBASELANGUAGECOMMAND   					= 2075;
			const qint64 GETKNOWLEDGEBASELANGUAGECOMMAND	   					= 2076;
			const qint64 WRITEXMLSUBCLASSHIERARCHYQUERYCOMMAND					= 2077;
			const qint64 CONSTRUCTWRITEXMLSUBCLASSHIERARCHYQUERYCOMMAND			= 2078;
			const qint64 CONSTRUCTWRITEXMLINDIVIDUALTYPESQUERYCOMMAND			= 2079;
			const qint64 WRITEXMLINDIVIDUALTYPESQUERYCOMMAND					= 2080;
			const qint64 LOADKNOWLEDGEBASEOWLFUNCTIONALONTOLOGYCOMMAND			= 2081;
			const qint64 STREAMPARSEOWL2FUNCTIONALONTOLOGYCOMMAND				= 2082;
			const qint64 LOADKNOWLEDGEBASEOWLAUTOSELECTIONONTOLOGYCOMMAND		= 2083;
			const qint64 CHOOSEPARSEINSTALLKNOWLEDGEBASEOWLAUTOONTOLOGYCOMMAND	= 2084;
			const qint64 LOADKNOWLEDGEBASESTARTDOWNLOADCOMMAND					= 2085;
			const qint64 LOADKNOWLEDGEBASEFINISHDOWNLOADCOMMAND					= 2086;
			const qint64 IMPORTKNOWLEDGEBASEOWLAUTOONTOLOGYCOMMAND				= 2087;
			const qint64 WRITEFUNCTIONALINDIVIDUALTYPESQUERYCOMMAND				= 2088;
			const qint64 CONSTRUCTWRITEFUNCTIONALINDIVIDUALTYPESQUERYCOMMAND	= 2089;
			const qint64 CONSTRUCTISTRIVIALLYCONSISTENTQUERYCOMMAND				= 2090;
			const qint64 ISTRIVIALLYCONSISTENTKNOWLEDGEBASECOMMAND   			= 2091;




			// Custom Events >= 2000
			const QEvent::Type EVENTREALIZECOMMAND					= (QEvent::Type)2000;
			const QEvent::Type EVENTCOMMANDPRECONDITIONCHANGE		= (QEvent::Type)2001;
			const QEvent::Type EVENTCOMMANDPROCESSEDCALLBACK		= (QEvent::Type)2002;
			const QEvent::Type EVENTINITIALIZE						= (QEvent::Type)2003;
			const QEvent::Type EVENTCOMMANDCALCULATEDQUERYCALLBACK	= (QEvent::Type)2004;
			const QEvent::Type EVENTONTOLOGYPREPAREDCALLBACK		= (QEvent::Type)2005;

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude


#endif // end KONCLUDE_Control_COMMAND_COMMANDSETTINGS
