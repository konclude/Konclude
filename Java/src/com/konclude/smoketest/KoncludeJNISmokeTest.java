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

package com.konclude.smoketest;

import com.konclude.jnibridge.AxiomExpressionBuildingBridge;
import com.konclude.jnibridge.KoncludeReasonerBridge;
import com.konclude.jnibridge.KoncludeReasonerException;
import com.konclude.jnibridge.QueryingBridge;
import com.konclude.jnibridge.SetOfObjectSetCallbackListener;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

/**
 * Checks that the JNI bridge of Konclude can be driven from Java, so that it is known
 * whether the bridge itself works before a wrapper is built on top of it.
 *
 * Every scenario is selected by its name and is supposed to be started in its own virtual
 * machine, see Java/run-jni-smoke-test.sh. The exit code is 0 if the scenario succeeded.
 *
 *   subclass           builds SubClassOf(A, B) and retrieves the sub classes of B
 *   hierarchy          builds A < B < C and compares the direct and the indirect answers
 *   lifecycles         initialises, uses and closes three reasoners in one virtual machine
 *   missing-processor  expects a KoncludeReasonerException instead of a crash if the
 *                      configuration string drops '-JNICommandProcessorLoader'
 */
public class KoncludeJNISmokeTest {

	/**
	 * Stands in for an entity of a client, an OWL API OWLClass for instance. The native side
	 * only remembers the object and hands it back with the query results.
	 */
	private static final class NamedEntity {

		private final String mIRI;

		NamedEntity(String iri) {
			mIRI = iri;
		}

		String getIRI() {
			return mIRI;
		}

		String getLocalName() {
			return mIRI.substring(mIRI.indexOf('#') + 1);
		}

		@Override
		public String toString() {
			return getLocalName();
		}
	}

	private static void report(String message) {
		System.out.println("  " + message);
	}

	/** collects the local names of the reported entities, so that they can be compared */
	private static Set<String> localNames(SetOfObjectSetCallbackListener callback) {
		Set<String> names = new TreeSet<String>();
		for (Object object : callback.getObjects()) {
			if (object instanceof NamedEntity) {
				names.add(((NamedEntity) object).getLocalName());
			}
		}
		return names;
	}

	private static Set<String> expected(String... names) {
		return new TreeSet<String>(Arrays.asList(names));
	}

	/**
	 * Builds a class hierarchy of the given sub class relations and asks for the sub classes
	 * of the given class. The relations are pairs of local names.
	 */
	private static Set<String> subClassesOf(String namespace, String[][] subClassOfPairs,
			String queriedLocalName, boolean direct) {

		List<NamedEntity> entities = new ArrayList<NamedEntity>();
		KoncludeReasonerBridge bridge = new KoncludeReasonerBridge();
		bridge.initKoncludeLibraryInstance("");
		try {
			AxiomExpressionBuildingBridge builder = new AxiomExpressionBuildingBridge();
			bridge.initAxiomExpressionVisitingBridge(builder);

			// build every class once and remember the association of name, entity and address
			Set<String> localNames = new TreeSet<String>();
			for (String[] pair : subClassOfPairs) {
				localNames.add(pair[0]);
				localNames.add(pair[1]);
			}
			List<String> orderedNames = new ArrayList<String>(localNames);
			long[] classPointers = new long[orderedNames.size()];
			for (int i = 0; i < orderedNames.size(); ++i) {
				NamedEntity entity = new NamedEntity(namespace + orderedNames.get(i));
				entities.add(entity);
				classPointers[i] = builder.buildOWLClass(bridge, entity.getIRI(), entity);
				if (classPointers[i] == 0) {
					throw new IllegalStateException("no expression was built for " + entity.getIRI());
				}
				builder.buildOWLDeclarationAxiom(bridge, classPointers[i]);
			}
			for (String[] pair : subClassOfPairs) {
				long subClass = classPointers[orderedNames.indexOf(pair[0])];
				long superClass = classPointers[orderedNames.indexOf(pair[1])];
				builder.buildOWLSubClassOfAxiom(bridge, subClass, superClass);
			}
			bridge.finalizeAxiomExpressionVisitingBridge(builder);

			QueryingBridge querying = new QueryingBridge();
			bridge.initQueryingBridge(querying);
			try {
				NamedEntity queried = entities.get(orderedNames.indexOf(queriedLocalName));
				SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
				querying.queryOWLSubClasses(bridge, queried.getIRI(), queried, callback, direct);
				return localNames(callback);
			} finally {
				bridge.finalizeQueryingBridge(querying);
			}
		} finally {
			bridge.closeKoncludeLibraryInstance();
		}
	}

	private static boolean runSubClass() {
		Set<String> subClasses = subClassesOf("http://konclude.com/test/smoke#",
				new String[][] { { "A", "B" } }, "B", false);
		report("subClasses(B, direct=false) = " + subClasses + ", expected " + expected("A"));
		return subClasses.equals(expected("A"));
	}

	private static boolean runHierarchy() {
		String namespace = "http://konclude.com/test/smoke#";
		String[][] hierarchy = new String[][] { { "A", "B" }, { "B", "C" } };

		Set<String> indirect = subClassesOf(namespace, hierarchy, "C", false);
		report("subClasses(C, direct=false) = " + indirect + ", expected " + expected("A", "B"));

		Set<String> direct = subClassesOf(namespace, hierarchy, "C", true);
		report("subClasses(C, direct=true)  = " + direct + ", expected " + expected("B"));

		return indirect.equals(expected("A", "B")) && direct.equals(expected("B"));
	}

	private static boolean runLifecycles() {
		boolean succeeded = true;
		for (int i = 1; i <= 3; ++i) {
			Set<String> subClasses = subClassesOf("http://konclude.com/test/smoke" + i + "#",
					new String[][] { { "A", "B" } }, "B", false);
			report("reasoner " + i + ": subClasses(B, direct=false) = " + subClasses);
			succeeded &= subClasses.equals(expected("A"));
		}
		return succeeded;
	}

	private static boolean runMissingProcessor() {
		KoncludeReasonerBridge bridge = new KoncludeReasonerBridge();
		try {
			// a non-empty configuration string replaces the default loading arguments, so the
			// command processor is not loaded, which used to end in a segmentation fault
			bridge.initKoncludeLibraryInstance("-DefaultReasonerLoader ");
			report("no exception was thrown, the missing command processor is not reported");
			return false;
		} catch (KoncludeReasonerException exception) {
			report("KoncludeReasonerException: " + exception.getMessage());
			return true;
		}
	}

	public static void main(String[] arguments) {
		String scenario = arguments.length > 0 ? arguments[0] : "subclass";
		System.out.println("scenario '" + scenario + "'");

		System.loadLibrary("Konclude");

		boolean succeeded;
		if ("subclass".equals(scenario)) {
			succeeded = runSubClass();
		} else if ("hierarchy".equals(scenario)) {
			succeeded = runHierarchy();
		} else if ("lifecycles".equals(scenario)) {
			succeeded = runLifecycles();
		} else if ("missing-processor".equals(scenario)) {
			succeeded = runMissingProcessor();
		} else {
			System.out.println("  unknown scenario, expected subclass, hierarchy, lifecycles or missing-processor");
			succeeded = false;
		}

		System.out.println(succeeded ? "PASSED" : "FAILED");
		if (!succeeded) {
			System.exit(1);
		}
	}
}
