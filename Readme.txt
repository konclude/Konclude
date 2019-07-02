OVERVIEW
==========

Konclude is a tableau-based reasoner for the Description Logic SROIQV(D) 
(SROIQ(D) + Nominal Schemas). Konclude is still under active development 
and the current version probably contains bugs and other errors. 
If you detect an unexpected/strange behaviour you are cordially invited 
to report it. See http://konclude.com/feedback.html for details.

	
	
LICENSING
===========

Konclude is released as free software, i.e., you can redistribute it and/or modify
it under the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1) as
published by the Free Software Foundation.

You should have received a copy of the GNU Lesser General Public License
along with Konclude. If not, see http://www.gnu.org/licenses/.

Konclude is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
details see GNU Lesser General Public License.

Konclude uses the following libraries:
- Qt 4.8.* or above, released unter LGPL 2.1 (http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).
  Note, Konclude uses slightly modified versions of Qt containers (QMap, QHash, QList) for an
  integrated memory management.
	  
	 
	  
USAGE
=======

The binaries of Konclude (and possibly some shared libraries) are located 
in the './Binaries ' directory. However, you can use the starter scripts in 
the main directory for directly invoking Konclude with "Konclude" (Windows) 
or "./Konclude" (Linux, Mac). The command line parameter '-h' shows 
the usage options for Konclude. Some examples are located in the './Tests' 
directory. See http://konclude.com/ for detailed usage instructions.

- COMMAND LINE:

	Basic reasoning tasks such as classification, consistency, class satisfiability and
	realisation can be directly invoked with the command line interface. For this, Konclude 
	has to be called with the corresponding command ('classification', 'consistency',
	'satisfiability', or 'realisation') and at least the input file has to be specified with 
	'-i FILEPATH'. The output file is optional and can be set with the '-o FILEPATH' parameter. 
	The	class satisfiability checking additionally requires the IRI of the class that has to 
	be tested, which has to be specified with '-x IRI'.
	
	Please note that Konclude currently only supports ontologies in the OWL 2 XML [1] or in the 
	OWL 2 Functional Style format [2]. For ontologies in other syntaxes, you may use the protege 
	editor [3] for converting ontologies into a supported format.

	Examples (on Windows you have to omit ./):
		./Konclude classification -i Tests/roberts-family-full-D.owl.xml -o roberts-family-full-class-hierarchy.owl.xml
		./Konclude satisfiability -i Tests/roberts-family-full-D.owl.xml -x http://www.co-ode.org/roberts/family-tree.owl#Aunt


- OWLLINK:

	Furthermore, Konclude processes OWLlink [4] requests with the XML binding [5]. 
	Konclude is optimised to read/write the OWLlink requests/responses from/to files. 
	For this, Konclude has to be called with the 'owllinkfile' command and at least 
	with the parameter '-i FILEPATH', where FILEPATH specifies the OWLlink request/query 
	file. The OWLlink response file can be specified with the '-o FILEPATH' parameter.

	Examples (on Windows you have to omit ./):
		./Konclude owllinkfile -i Tests/galen-classify-request.xml -o response.xml
		./Konclude owllinkfile -i Tests/roberts-family-full-D-classify-realize-request.xml
		./Konclude owllinkfile -c Configs/default-config.xml -i Tests/1b-satisfiability-request.xml


	Currently, Konclude supports only the following subset of OWLlink commands:
	- GetDescription, CreateKB, GetSettings, Set, ReleaseKB, Tell, 
	  LoadOntologies (IRIs must be files), Classify, Realize
	- GetAllObjectProperties, GetAllIndividuals, GetAllClasses, GetKBLanguage
	- IsClassSatisfiable, GetSubClassHierarchy, GetSubClasses (only for atomic concepts), 
	  GetSuperClasses (only for atomic concepts), GetEquivalentClasses (only for atomic concepts)
	- GetTypes, GetFlattenedTypes, GetInstances (only for atomic concepts), 
	  GetFlattenedInstances (only for atomic concepts)

	Furthermore, all OWL 2 ontologies/axioms used in LoadOntologies/Tell 
	commands must be in the OWL 2 XML [1] or in the OWL 2 Functional Style [2] 
	format and Konclude does currently NOT process annotations. The protege 
	editor [3] can be used to convert OWL 2 ontologies between different 
	formats.
	

	Konclude can also be used as an OWLlink server that answers OWLlink requests
	sent over HTTP. For this, Konclude has to be called with the 'owllinkserver'
	command. The listening port can optionally be specified by the parameter '-p', 
	otherwise the default port 8080 is used.

	Examples (on Windows you have to omit ./):
		./Konclude owllinkserver -p 8080
	

	Large ontologies should separately be loaded with the LoadOntologies 
	OWLlink command, because the request files are completely loaded 
	into the main memory of the system. In contrast, the files referenced by
	the LoadOntologies commands are parsed in a stream-based manner.

- CONFIGURATION:

	Konclude can be configured with configuration files, which are also 
	OWLlink request files where only the 'Set' command is used. 'Set' 
	commands without the specification of a specific knowledge base 
	configure the default settings in Konclude, which are used to instantiate 
	the configurations of newly created knowledge bases. It is highly 
	recommended to NOT change the default configuration of Konclude, because 
	many available configurations are only for debugging, testing and 
	experimenting. However, if Konclude requires too much main memory, then 
	you can try to deactivate the satisfiability or unsatisfiability caching 
	(see example configuration file in './Configs' directory). The parameter 
	'-c FILEPATH' can be used to load a configuration file.

		

REQUIREMENTS, INSTALLATION, BUILD
===================================

Konclude requires Qt 4.8 libraries or above [6]. If they are not included 
in the release (or integrated in the binary), then you should install 
them manually on your system. 


On Windows-based platforms it is further necessary to install the 
.Net Framework 4.0 [7] and the Microsoft Visual C++ 2005 Redistributable 
Package [8] for the execution of Konclude.


Konclude can be build by running qmake (which is part of the Qt Framework) 
on 'Konclude.pro', e.g., 
	qmake -o Makefile Konclude.pro
and then by compiling the sources with the generated makefile, i.e.,
	make
has to be executed/called.
	

		

CHANGELOG
===========

Konclude v0.6.0:
	- Several fixed bugs and memory leaks.
	- Many performance improvements (especially for realisation).
	- Improved error reporting.
	- Almost complete datatype support.
	- OWL 2 Functional Style support.
	- Loading of ontologies via HTTP and FTP.

Konclude v0.5.0:
	- First release of Konclude.
		
		
		
REFERENCES
============

[1] http://www.w3.org/TR/owl2-xml-serialization/
[2] http://www.w3.org/TR/owl2-syntax/
[3] http://protege.stanford.edu/
[4] http://www.w3.org/Submission/owllink-structural-specification/ 
[5] http://www.w3.org/Submission/2010/SUBM-owllink-httpxml-binding-20100701/ 	
[6] http://qt-project.org/downloads
[7] http://www.microsoft.com/en-us/download/details.aspx?id=17718 
[7] http://www.microsoft.com/en-us/download/details.aspx?id=21254 