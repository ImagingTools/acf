import qbs.base 1.0
import qbs.TextFile
import "fileinfo.js" as FileInfo

Module{
	name: "acf"

	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	// root of the whole project
	property path projectRoot

	readonly property string acfRootDir: FileInfo.joinPaths(path, "/../../..")
	property string compilerName
	property string compileMode

	Properties{
		condition: (qbs.targetOS.contains("linux") || qbs.targetOS.contains("unix")) && !qbs.targetOS.contains("osx")
		compilerName: "QMake"
	}
	Properties{
		condition: qbs.targetOS.contains("osx")
		compilerName: "OSX"
	}
	Properties{
		condition: qbs.toolchain.contains("mingw")
		compilerName: "MinGW"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.toolchainInstallPath.search(/Studio 8/i) >= 0 || cpp.toolchainInstallPath.search(/2005/i) >= 0)
		compilerName: "VC8"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.toolchainInstallPath.search(/Studio 9/i) >= 0 || cpp.toolchainInstallPath.search(/2008/i) >= 0)
		compilerName: "VC9"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.toolchainInstallPath.search(/Studio 10/i) >= 0 || cpp.toolchainInstallPath.search(/2010/i) >= 0)
		compilerName: "VC10"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.toolchainInstallPath.search(/Studio 11/i) >= 0)
		compilerName: "VC11"
	}

	Properties{
		condition: cpp.debugInformation == true
		compileMode: "Debug"
	}
	Properties{
		condition: cpp.debugInformation == false
		compileMode: "Release"
	}

	property string compilerDir: compileMode + compilerName
	property string generatedOutputDir								// Path where stuff will be generated if undefined "GeneratedPath/" + product.name will be taken
	property path acfConfigurationFile								// ACF configuration file ARX compiler
	property path trConfigurationFile: acfConfigurationFile			// ACF configuration file for xtracf transformations
	property path trRegFile											// ACF registry file for xtracf transformations
	property stringList trOutputType								// ACF transformation output tags
	property stringList xpcPackageDirs								// Extra directories placed into generated XPC file

	FileTagger{
		pattern: "*.arx"
		fileTags: ["arx"]
	}

	FileTagger{
		pattern: "*.xtracf"
		fileTags: ["xtracf"]
	}

	FileTagger{
		pattern: "*.arp"
		fileTags: ["acfComponent"]
	}

	FileTagger{
		pattern: "*.xpc"
		fileTags: ["xpc_file"]
	}

	Rule{
		id: arxCompiler
		inputs: ["arx"]
		usings: ["application", "dynamiclibrary", "xpc"]

		Artifact{
			fileName: FileInfo.getGeneratedPath() + "/C" + input.baseName + ".cpp"
			fileTags: ["cpp"]
		}
		Artifact{
			fileName: FileInfo.getGeneratedPath() + "/C" + input.baseName + ".h"
			fileTags: ["hpp", "c++_pch"]
		}

		prepare:{
			// get the ACF binary directory
			var acfBinDirectory = product.moduleProperty("Arxc", "acfBinDirectory");
			if (acfBinDirectory == null){
				acfBinDirectory = product.buildDirectory + '/Bin/' + product.moduleProperty("acf", "compilerDir");
			}

			// get the ACF configuration file
			var acfConfigurationFile = product.moduleProperty("acf", "acfConfigurationFile");
			if (acfConfigurationFile == null){
				var dependencies = product.dependencies;
				for (var dependencyIndex in dependencies) {
					var dependency = dependencies[dependencyIndex];
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						acfConfigurationFile = dependencyFilePath;
					}
					else if (dependency.type.contains("xpc")){
						acfConfigurationFile = product.buildDirectory + "/" + dependency.destinationDirectory + "/" + dependency.name + ".xpc";
					}
				}
			}

			// if there is no configuration - error
			if (acfConfigurationFile == null){
				return null;
			}

			var cmd = new Command(acfBinDirectory + "/" + product.moduleProperty("cpp", "executablePrefix") + "Arxc" + product.moduleProperty("cpp", "executableSuffix"), [
						inputs.arx[0].fileName,
						'-config', acfConfigurationFile,
						'-o', outputs.cpp[0].fileName]);
			cmd.description = 'arxc ' + FileInfo.fileName(inputs.arx[0].fileName)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = acfBinDirectory;

			return cmd;
		}
	}

	Rule{
		id: acftransform
		inputs: ["xtracf"]

		Artifact{
			fileName: FileInfo.getGeneratedPath() + "/" + input.completeBaseName
			fileTags: { return product.moduleProperty("acf", "trOutputType"); }
		}

		prepare:{
			// get the ACF binary directory
			var acfBinDirectory = product.moduleProperty("Arxc", "acfBinDirectory");
			if (acfBinDirectory == null){
				acfBinDirectory = product.buildDirectory + '/Bin/' + compilerDir;
			}

			// get the ACF configuration file
			var acfConfigurationFile = product.moduleProperty("acf", "trConfigurationFile");
			if (acfConfigurationFile == null){
				var dependencies = product.dependencies;
				for (var dependencyIndex in dependencies) {
					var dependency = dependencies[dependencyIndex];
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						acfConfigurationFile = dependencyFilePath;
					}
					else if (dependency.type.contains("xpc")){
						acfConfigurationFile = product.buildDirectory + "/" + dependency.destinationDirectory + "/" + dependency.name + ".xpc";
					}
				}
			}

			// if there is no configuration - error
			if (acfConfigurationFile == null){
				return null;
			}

			var cmd = new Command(acfBinDirectory + '/' + product.moduleProperty("cpp", "executablePrefix") + 'Acf' + product.moduleProperty("cpp", "executableSuffix"), [
						product.moduleProperty("acf", "trRegFile").fileName,
						'-config', acfConfigurationFile,
						'-input', input.fileName,
						'-o', output.fileName]);
			cmd.description = 'acf transformation ' + FileInfo.fileName(input.fileName)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = acfBinDirectory;

			return cmd;
		}
	}

	// Special rule for rcc compiler for resource files using generated files
	// It set the rcc root to project in generated directory
	Rule{
		inputs: ["qrc_generated"]
		explicitlyDependsOn: ["qm"]

		Artifact{
			fileName: FileInfo.getGeneratedPath() + "/qrc_" + input.completeBaseName + ".cpp"
			fileTags: ["cpp"]
		}
		prepare:{
			var tempResourceDir = product.buildDirectory + "/GeneratedFiles/";

			var copyCmd;
			var rccCmd;
			if (product.moduleProperty("qbs", "targetOS").contains("windows")){
				copyCmd = new Command('xcopy', ['/Y', FileInfo.toWindowsSeparators(input.fileName), FileInfo.toWindowsSeparators(tempResourceDir)]);
				rccCmd = new Command('rcc',
							[tempResourceDir + FileInfo.fileName(input.fileName),
							'-name', FileInfo.completeBaseName(input.fileName),
							'-o', output.fileName]);
			}
			else{
				copyCmd = new Command('cp', [input.fileName, tempResourceDir]);
				rccCmd = new Command('rcc',
							[tempResourceDir + FileInfo.fileName(input.fileName),
							'-name', FileInfo.completeBaseName(input.fileName),
							'-o', output.fileName]);
			}

			copyCmd.description = 'copy to generated ' + FileInfo.fileName(input.fileName);
			copyCmd.highlight = 'codegen';

			rccCmd.description = 'rcc from generated ' + FileInfo.fileName(input.fileName);
			rccCmd.highlight = 'codegen';

			var commands = [];
			commands.push(copyCmd);
			commands.push(rccCmd);
			return commands;
		}
	}

	Rule{
		condition: product.name.indexOf("_") != 0	// prefix '_' will be used for temporary products
		id: acfShareGenerator
		multiplex: true
		inputs: ["cpp", "c", "objcpp", "objc", "xpc_file"]

		Artifact{
			fileName: "share/qbs/modules/" + product.name + "/" + product.name + ".qbs"
			fileTags: ["acf_share"]
		}
		prepare:{
			var cmd = new JavaScriptCommand();
			cmd.description = "generating shared module " + product.name;
			cmd.highlight = "codegen";
			cmd.sourceCode = function(){
				var outputFilePath = output.fileName;

				var pkginfo = new TextFile(outputFilePath, TextFile.WriteOnly);
				pkginfo.write("import qbs 1.0\n");
				pkginfo.write("\n");
				pkginfo.write("Module{\n");

				if (!product.type.contains("xpc")){
					var dependencies = product.dependencies;
					for (var dependencyIndex in dependencies) {
						var dependencyName = dependencies[dependencyIndex].name.replace("/", ".");
						if ((dependencyName != "qbs") && (dependencyName.indexOf("_") != 0)){
							pkginfo.write("	Depends{ name: '" + dependencyName + "' }\n");
						}
					}

					pkginfo.write("\n");
				}

				if (product.type.contains("staticlibrary")){
					var libraryFileName = product.moduleProperty("cpp", "staticLibraryPrefix") + product.targetName + product.moduleProperty("cpp", "staticLibrarySuffix");
					pkginfo.write("	cpp.staticLibraries: path + '/../../../../" + product.destinationDirectory + "/" + libraryFileName + "'\n");
				}
				if (product.type.contains("application")){
					pkginfo.write("	readonly property path acfBinDirectory: path + '/../../../../" + product.destinationDirectory + "'\n");
				}
				if (product.type.contains("xpc")){
					pkginfo.write("	readonly property path xpcFilePath: path + '/../../../../" + product.destinationDirectory + "/" + product.name + ".xpc'\n");
				}
				if (product.type.contains("acfComponent")){
					pkginfo.write("	readonly property path componentFilePath: path + '/../../../../" + product.destinationDirectory + "/" + product.name + ".arp'\n");
				}

				if (!product.type.contains("xpc")){
					pkginfo.write("\n");

					var includePaths = product.moduleProperties("cpp", "includePaths");

					var outputDir = FileInfo.path(outputFilePath);

					var projectRoot = product.moduleProperty("acf", "projectRoot");
					if (projectRoot !== undefined && !FileInfo.isAbsolutePath(projectRoot)){
						projectRoot = FileInfo.joinPaths(product.sourceDirectory, projectRoot);
					}

					var correctedPathsMap = {};
					for (i in includePaths){
						var includePath = includePaths[i];
						if (		FileInfo.isSubpath(product.buildDirectory, includePath) ||
									(projectRoot !== undefined && FileInfo.isSubpath(projectRoot, includePath))){
							correctedPathsMap[FileInfo.relativePath(outputDir, includePath)] = true;
						}
					}

					var isFirst = true;
					pkginfo.write("	cpp.includePaths: [");
					for (var correctedPath in correctedPathsMap){
						if (isFirst){
							pkginfo.write("\n");
						}
						else{
							pkginfo.write(",\n");
						}
						pkginfo.write("		path + '/" + correctedPath + "'");
						isFirst = false;
					}
					pkginfo.write("\n	]\n");
				}
				pkginfo.write("}\n");
				pkginfo.close();
			}
			return cmd;
		}
	}

	// Rule for generating XPC files
	Rule{
		id: acfXpcGenerator
		multiplex: true
		inputs: ["xpc_file"]

		Artifact{
			fileName: product.destinationDirectory + "/" + product.name + ".xpc"
			fileTags: ["xpc"]
		}
		prepare:{
			var cmd = new JavaScriptCommand();
			cmd.description = "Create XPC file " + product.name;
			cmd.highlight = "codegen";
			cmd.sourceCode = function(){
				var outputFilePath = output.fileName;
				var outputDir = FileInfo.path(outputFilePath);

				var pkginfo = new TextFile(outputFilePath, TextFile.WriteOnly);
				pkginfo.write("<?xml version=\"1.0\"?>\n");
				pkginfo.write("<Acf>\n");
				pkginfo.write("	<AcfHeader>\n");
				pkginfo.write("		<VersionInfos count=\"1\">\n");
				pkginfo.write("			<Version>\n");
				pkginfo.write("				<Id>\n");
				pkginfo.write("					0\n");
				pkginfo.write("				</Id>\n");
				pkginfo.write("				<Number>\n");
				pkginfo.write("					2484\n");
				pkginfo.write("				</Number>\n");
				pkginfo.write("				<Description>\n");
				pkginfo.write("					ACF\n");
				pkginfo.write("				</Description>\n");
				pkginfo.write("			</Version>\n");
				pkginfo.write("		</VersionInfos>\n");
				pkginfo.write("	</AcfHeader>\n");

				var dependencies = product.dependencies;

				var configsList = [];

				for (var inputIndex in inputs.xpc_file){
					var inputProduct = inputs.xpc_file[inputIndex];
					configsList.push(FileInfo.relativePath(outputDir, inputProduct.fileName));
				}

				for (var dependencyIndex in dependencies){
					var dependency = dependencies[dependencyIndex];
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						configsList.push(FileInfo.relativePath(outputDir, dependencyFilePath));
					}
					else if (dependency.type.contains("xpc")){
						configsList.push(FileInfo.relativePath(product.destinationDirectory, dependency.destinationDirectory + "/" + dependency.name + ".xpc"));
					}
				}

				pkginfo.write("	<ConfigFiles count=\"" + configsList.length + "\">\n");
				for (var configIndex in configsList){
					var configFilePath = configsList[configIndex];
					pkginfo.write("		<FilePath>\n");
					pkginfo.write("			" + configFilePath + "\n");
					pkginfo.write("		</FilePath>\n");
				}
				pkginfo.write("	</ConfigFiles>\n");

				var packagesList = [];

				for (var inputIndex in inputs.acfComponent) {
					var inputProduct = inputs.acfComponent[inputIndex];
					packagesList.push(FileInfo.relativePath(outputDir, inputProduct.fileName));
				}

				for (var dependencyIndex in dependencies) {
					var dependency = dependencies[dependencyIndex];
					if (dependency.type.contains("acfComponent")){
						var dependencyFilePath = product.moduleProperty(dependency.name, "componentFilePath");
						if (dependencyFilePath != null){
							packagesList.push(FileInfo.relativePath(outputDir, dependencyFilePath));
						}
						else{
							packagesList.push(FileInfo.relativePath(product.destinationDirectory, dependency.destinationDirectory + "/" + dependency.name + ".arp"));
						}
					}
				}

				var packageDirsList = product.moduleProperty("acf", "xpcPackageDirs");

				pkginfo.write("	<PackageDirs count=\"0\">\n");
				for (var packageDirIndex in packageDirsList) {
					var packageDirPath = packageDirsList[packageDirIndex];
					pkginfo.write("		<Dir>\n");
					pkginfo.write("			" + packageDirPath + "\n");
					pkginfo.write("		</Dir>\n");
				}
				pkginfo.write("	</PackageDirs>\n");

				pkginfo.write("	<PackageFiles count=\"" + packagesList.length + "\">\n");
				for (var packageIndex in packagesList) {
					var packageFilePath = packagesList[packageIndex];
					pkginfo.write("		<FilePath>\n");
					pkginfo.write("			" + packageFilePath + "\n");
					pkginfo.write("		</FilePath>\n");
				}
				pkginfo.write("	</PackageFiles>\n");

				pkginfo.write("	<RegistryFiles count=\"0\">\n");
				pkginfo.write("	</RegistryFiles>\n");

				pkginfo.write("</Acf>\n");
				pkginfo.close();
			}
			return cmd;
		}
	}
}
