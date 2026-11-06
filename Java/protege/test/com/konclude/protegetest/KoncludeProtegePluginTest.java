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

package com.konclude.protegetest;

import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.BundleException;
import org.osgi.framework.Constants;
import org.osgi.framework.launch.Framework;
import org.osgi.framework.launch.FrameworkFactory;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.ServiceLoader;

/**
 * Loads the Protege plug-in of Konclude into the OSGi framework the way Protege does, and runs
 * a scenario against it from inside the framework, see run-protege-plugin-test.sh.
 *
 * The framework is Felix, taken from the bundles directory of a Protege installation together
 * with every bundle in it, so that the plug-in resolves against the very bundles it will meet
 * in Protege: the OWL API, the Protege editor and their dependencies. The plug-in bundle is
 * started, which is what makes the framework unpack the shared library named by
 * Bundle-NativeCode, and then a test bundle that requires the plug-in is started and asked to
 * run the scenario, so that every class of the plug-in is loaded through its own bundle, as it
 * is in Protege.
 *
 * Arguments: the Protege installation, the plug-in jar, the test bundle jar, the framework
 * cache directory and the scenario.
 */
public class KoncludeProtegePluginTest {

	/** the bundles of the directory that Protege's launcher does not install as bundles */
	private static final List<String> NOT_BUNDLES = Arrays.asList(
			"org.apache.felix.main.jar", "protege-launcher.jar");

	public static void main(String[] arguments) throws Exception {
		if (arguments.length != 5) {
			System.err.println("usage: <Protege directory> <plug-in jar> <test bundle jar> <cache directory> <scenario>");
			System.exit(2);
		}
		File protegeDirectory = new File(arguments[0]);
		File pluginJar = new File(arguments[1]);
		File testBundleJar = new File(arguments[2]);
		File cacheDirectory = new File(arguments[3]);
		String scenario = arguments[4];

		File bundlesDirectory = new File(protegeDirectory, "bundles");
		File[] bundleJars = bundlesDirectory.listFiles();
		if (bundleJars == null) {
			System.err.println("'" + bundlesDirectory + "' is not the bundles directory of a Protege installation.");
			System.exit(2);
		}

		// the framework properties of conf/config.xml of Protege
		Map<String, String> configuration = new HashMap<String, String>();
		configuration.put(Constants.FRAMEWORK_STORAGE, cacheDirectory.getAbsolutePath());
		configuration.put(Constants.FRAMEWORK_STORAGE_CLEAN, Constants.FRAMEWORK_STORAGE_CLEAN_ONFIRSTINIT);
		configuration.put(Constants.FRAMEWORK_BOOTDELEGATION, "sun.*,com.sun.*,apple.*,com.apple.*");
		configuration.put(Constants.FRAMEWORK_SYSTEMPACKAGES_EXTRA,
				"javax.xml.parsers,org.xml.sax,org.xml.sax.ext,org.xml.sax.helpers");

		FrameworkFactory factory = ServiceLoader.load(FrameworkFactory.class).iterator().next();
		Framework framework = factory.newFramework(configuration);
		framework.init();
		framework.start();
		BundleContext context = framework.getBundleContext();
		System.out.println("framework " + context.getBundle(0).getSymbolicName() + " "
				+ context.getBundle(0).getVersion() + " from " + bundlesDirectory);

		List<Bundle> installed = new ArrayList<Bundle>();
		Arrays.sort(bundleJars);
		for (File jar : bundleJars) {
			if (!jar.getName().endsWith(".jar") || NOT_BUNDLES.contains(jar.getName())) {
				continue;
			}
			installed.add(context.installBundle(jar.toURI().toString()));
		}
		System.out.println(installed.size() + " bundles of Protege installed");

		Bundle plugin = context.installBundle(pluginJar.toURI().toString());
		System.out.println("plug-in " + plugin.getSymbolicName() + " " + plugin.getVersion()
				+ ", Bundle-NativeCode: " + plugin.getHeaders().get(Constants.BUNDLE_NATIVECODE));
		try {
			plugin.start();
		} catch (BundleException exception) {
			System.out.println("FAILED, the plug-in does not start: " + exception.getMessage());
			framework.stop();
			System.exit(1);
		}
		System.out.println("plug-in started, state " + stateName(plugin.getState()));

		Bundle testBundle = context.installBundle(testBundleJar.toURI().toString());
		testBundle.start();

		// every class the scenario touches is loaded through the bundles, as in Protege
		Class<?> scenarios = testBundle.loadClass("com.konclude.protegetest.KoncludeProtegePluginScenarios");
		int failures;
		try {
			failures = ((Integer) scenarios.getMethod("run", String.class).invoke(null, scenario)).intValue();
		} finally {
			framework.stop();
			framework.waitForStop(10000);
		}
		if (failures > 0) {
			System.out.println("FAILED, " + failures + " check(s) did not hold");
			System.exit(1);
		}
		System.out.println("PASSED");
	}

	private static String stateName(int state) {
		switch (state) {
			case Bundle.ACTIVE: return "ACTIVE";
			case Bundle.RESOLVED: return "RESOLVED";
			case Bundle.INSTALLED: return "INSTALLED";
			case Bundle.STARTING: return "STARTING";
			default: return String.valueOf(state);
		}
	}
}
