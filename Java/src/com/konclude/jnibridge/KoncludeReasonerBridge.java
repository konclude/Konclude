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

package com.konclude.jnibridge;

/**
 * Entry point of the JNI bridge, corresponds to
 * Source/Control/Interface/JNI/com_konclude_jnibridge_KoncludeReasonerBridge.cpp.
 *
 * The native side reads and writes the field mNativeDataPointer, see
 * CJNIHandler::getJNIInstanceManagerBaseFieldID, so neither its name nor its type must be
 * changed.
 *
 * The expected order of the calls is
 *
 *   initKoncludeLibraryInstance
 *     initAxiomExpressionVisitingBridge, build the axioms, finalizeAxiomExpressionVisitingBridge
 *     initQueryingBridge, query, finalizeQueryingBridge
 *   closeKoncludeLibraryInstance
 *
 * where the axioms are told to the ontology while they are built and the ontology is
 * installed by finalizeAxiomExpressionVisitingBridge.
 */
public class KoncludeReasonerBridge {

	/** written by the native side, holds the CJNIInstanceManager of this bridge */
	private long mNativeDataPointer = 0;

	/**
	 * Initialises the library instance.
	 *
	 * A non-empty configuration string replaces the default loading arguments instead of
	 * extending them, whence it also has to contain '-JNICommandProcessorLoader'. A
	 * KoncludeReasonerException is thrown if no command processor could be loaded.
	 */
	public native void initKoncludeLibraryInstance(String configString);

	public native void closeKoncludeLibraryInstance();

	public native void initAxiomExpressionVisitingBridge(AxiomExpressionBuildingBridge builder);

	public native void finalizeAxiomExpressionVisitingBridge(AxiomExpressionBuildingBridge builder);

	public native void initQueryingBridge(QueryingBridge processor);

	public native void finalizeQueryingBridge(QueryingBridge processor);

	/**
	 * The log messages Konclude wrote since the last call, for all library instances of the
	 * process: three entries each, the level (30 information, 60 warning, 70 and above error),
	 * the domain, for instance '::Konclude::Reasoner::Preprocess', and the text. Empty before
	 * the first library instance was initialised; null when there is nothing to take.
	 */
	public static native String[] takeKoncludeLogMessages();

	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}
