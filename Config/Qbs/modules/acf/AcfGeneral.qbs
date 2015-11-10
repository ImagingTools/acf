import qbs.base 1.0
import qbs.TextFile
import qbs.FileInfo
import "AcfService.js" as AcfService

AcfModule{
    condition: true

	Depends{ name: "Qt.core" }
	Depends{ name: "Qt.gui" }

    name: "acf"

	Rule{
		id: arxCompiler
		inputs: ["acc"]

		Artifact{
            filePath: AcfService.getGeneratedPath(product) + "/C" + input.completeBaseName + ".cpp"
			fileTags: ["cpp"]
		}
		Artifact{
            filePath: AcfService.getGeneratedPath(product) + "/C" + input.completeBaseName + ".h"
			fileTags: ["hpp", "c++_pch"]
		}

		prepare:{
			// get the ACF binary directory
			var acfBinDirectory = product.moduleProperty("ArxcExe", "acfBinDirectory");
			var acfConfigurationFile = product.moduleProperty("acf", "acfConfigurationFile");

			// check all dependencies
			var dependencies = product.dependencies;
			for (var dependencyIndex in dependencies) {
				var dependency = dependencies[dependencyIndex];
				if (acfConfigurationFile == null){
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						acfConfigurationFile = dependencyFilePath;
					}
					else if (dependency.type != null && dependency.type.contains("awc")){
						acfConfigurationFile = AcfService.joinPaths(product.buildDirectory, dependency.destinationDirectory) + "/" + dependency.name + ".awc";
					}
				}

				if ((acfBinDirectory == null) && (dependency.name == "ArxcExe")){
					acfBinDirectory = dependency.buildDirectory;
				}
			}

			// if there is no configuration - error
			if (acfConfigurationFile == null){
				throw new Error("No ACF configuration specified (using dependency or acf.acfConfigurationFile) in " + product.name);
			}

			// if there is no ArxcExe directory - error
			if (acfBinDirectory == null){
				throw new Error("No ArxcExe dependency specified in " + product.name);
			}

			var parameters = [
						inputs.acc[0].filePath,
						'-config', acfConfigurationFile,
						'-o', outputs.cpp[0].filePath];
			if (product.moduleProperty("acf", "arxcToBinary") === false){
				parameters.push("-no_binary");
			}

			var cmd = new Command(acfBinDirectory + "/" + product.moduleProperty("cpp", "executablePrefix") + "Arxc" + product.moduleProperty("cpp", "executableSuffix"), parameters);
			cmd.description = 'arxc ' + FileInfo.fileName(inputs.acc[0].filePath)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = product.moduleProperty("Qt.core", "binPath");

			return cmd;
		}
	}

	Rule{
		id: acftransform
		inputs: ["xtracf"]

		Artifact{
            filePath: AcfService.getGeneratedPath(product) + "/" + input.completeBaseName
			fileTags: { return product.moduleProperty("acf", "trOutputType"); }
		}

		prepare:{
			// get the ACF binary directory
			var acfBinDirectory = product.moduleProperty("AcfExe", "acfBinDirectory");
			var acfRegistryFile = product.moduleProperty("acf", "trRegFile");
			var acfConfigurationFile = product.moduleProperty("acf", "trConfigurationFile");

			// check all dependencies
			var dependencies = product.dependencies;
			for (var dependencyIndex in dependencies) {
				var dependency = dependencies[dependencyIndex];

				if (acfConfigurationFile == null){
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						acfConfigurationFile = dependencyFilePath;
					}
					else if (dependency.type != null && dependency.type.contains("awc")){
						acfConfigurationFile = AcfService.joinPaths(product.buildDirectory, dependency.destinationDirectory) + "/" + dependency.name + ".awc";
					}
				}

				if ((acfBinDirectory == null) && (dependency.name == "AcfExe")){
					acfBinDirectory = dependency.buildDirectory;
				}
			}

			// if there is no registry file - error
			if (acfRegistryFile == null){
				throw new Error("no ACF registry specified for transformation (using dependency or acf.trRegFile) in " + product.name);
			}

			// if there is no configuration - error
			if (acfConfigurationFile == null){
				throw new Error("no ACF configuration specified (using dependency or acf.trConfigurationFile) in " + product.name);
			}

			// if there is no AcfExe directory - error
			if (acfBinDirectory == null){
				throw new Error("No ArxcExe dependency specified in " + product.name);
			}

			var cmd = new Command(acfBinDirectory + '/' + product.moduleProperty("cpp", "executablePrefix") + 'Acf' + product.moduleProperty("cpp", "executableSuffix"), [
						acfRegistryFile,
						'-config', acfConfigurationFile,
						'-input', input.filePath,
						'-o', output.filePath]);
			cmd.description = 'acf transformation ' + FileInfo.fileName(input.filePath)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = product.moduleProperty("Qt.core", "binPath");

			return cmd;
		}
	}

	Rule{
		condition: (project.acfGenerateShare == true) && (product.name.indexOf("_") != 0)	// prefix '_' will be used for temporary products
		id: acfShareGenerator
		multiplex: true
		inputs: ["cpp", "c", "objcpp", "objc", "awc_file"]

		Artifact{
            filePath: "share/qbs/modules/" + product.name + "/" + product.name + ".qbs"
			fileTags: ["acf_share"]
		}
		prepare:{
			var cmd = new JavaScriptCommand();
			cmd.description = "generating shared module " + product.name;
			cmd.highlight = "codegen";
			cmd.sourceCode = function(){
				var outputFilePath = output.filePath;

				var pkginfo = new TextFile(outputFilePath, TextFile.WriteOnly);
				pkginfo.write("import qbs 1.0\n");
				pkginfo.write("\n");
				pkginfo.write("Module{\n");

				if (!product.type.contains("awc")){
					var dependencies = product.dependencies;
					for (var dependencyIndex in dependencies) {
						var dependency = dependencies[dependencyIndex];
						if ((dependency != null) && (dependency.name != null)){
							var dependencyName = dependency.name.replace("/", ".");
							if ((dependencyName != "qbs") && (dependencyName.indexOf("_") != 0)){
								pkginfo.write("	Depends{ name: '" + dependencyName + "' }\n");
							}
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
				if (product.type.contains("awc")){
					pkginfo.write("	readonly property path xpcFilePath: path + '/../../../../" + product.destinationDirectory + "/" + product.name + ".awc'\n");
				}
				if (product.type.contains("acfComponent")){
					pkginfo.write("	readonly property path componentFilePath: path + '/../../../../" + product.destinationDirectory + "/" + product.name + ".arp'\n");
				}

				if (!product.type.contains("awc")){
					pkginfo.write("\n");

					var includePaths = product.moduleProperties("cpp", "includePaths");

					var outputDir = FileInfo.path(outputFilePath);

					var projectRoot = project.projectRoot;
					if (projectRoot !== undefined && !FileInfo.isAbsolutePath(projectRoot)){
						projectRoot = FileInfo.joinPaths(product.sourceDirectory, projectRoot);
					}

					var correctedPathsMap = {};
					for (i in includePaths){
						var includePath = includePaths[i];
                        if (		AcfService.isSubpath(product.buildDirectory, includePath) ||
                                    (projectRoot !== undefined && AcfService.isSubpath(projectRoot, includePath))){
							correctedPathsMap[AcfService.relativePath(outputDir, includePath)] = true;
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
		inputs: ["awc_file"]

		Artifact{
            filePath: product.destinationDirectory + "/" + product.name + ".awc"
			fileTags: ["awc"]
		}
		prepare:{
			var cmd = new JavaScriptCommand();
			cmd.description = "Create XPC file " + product.name;
			cmd.highlight = "codegen";
			cmd.sourceCode = function(){
				var outputFilePath = output.filePath;
				var outputDir = FileInfo.path(outputFilePath);

				var pkginfo = new TextFile(outputFilePath, TextFile.WriteOnly);
				pkginfo.write("<Acf>\n");
				pkginfo.write("	<AcfHeader>\n");
				pkginfo.write("		<VersionInfos>\n");
				pkginfo.write("			<Version Number=\"4005\" Id=\"0\" Description=\"ACF\"/>\n");
				pkginfo.write("		</VersionInfos>\n");
				pkginfo.write("	</AcfHeader>\n");

				var configsList = [];

				for (var inputIndex in inputs.awc_file){
					var inputProduct = inputs.awc_file[inputIndex];
					configsList.push(AcfService.relativePath(outputDir, inputProduct.filePath));
				}

				var dependencies = product.dependencies;
				for (var dependencyIndex in dependencies){
					var dependency = dependencies[dependencyIndex];
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						configsList.push(AcfService.relativePath(outputDir, dependencyFilePath));
					}
					else if (dependency.type != null && dependency.type.contains("awc")){
						configsList.push(AcfService.relativePath(product.destinationDirectory, dependency.destinationDirectory + "/" + dependency.name + ".awc"));
					}
				}

				pkginfo.write("	<ConfigFiles>\n");
				for (var configIndex in configsList){
					var configFilePath = configsList[configIndex];
					pkginfo.write("		<FilePath>" + configFilePath + "</FilePath>\n");
				}
				pkginfo.write("	</ConfigFiles>\n");

				var packagesList = [];

				for (var inputIndex in inputs.acfComponent) {
					var inputProduct = inputs.acfComponent[inputIndex];
					packagesList.push(AcfService.relativePath(outputDir, inputProduct.filePath));
				}

				for (var dependencyIndex in dependencies) {
					var dependency = dependencies[dependencyIndex];
					if (dependency.type != null && dependency.type.contains("acfComponent")){
						var dependencyFilePath = product.moduleProperty(dependency.name, "componentFilePath");
						if (dependencyFilePath != null){
							packagesList.push(AcfService.relativePath(outputDir, dependencyFilePath));
						}
						else{
							packagesList.push(AcfService.relativePath(product.destinationDirectory, dependency.destinationDirectory + "/" + dependency.name + ".arp"));
						}
					}
				}

				var packageDirsList = product.moduleProperty("acf", "xpcPackageDirs");

				pkginfo.write("	<PackageDirs count=\"0\">\n");
				for (var packageDirIndex in packageDirsList) {
					var packageDirPath = AcfService.relativePath(outputDir, packageDirsList[packageDirIndex]);
					pkginfo.write("		<Dir>" + packageDirPath + "</Dir>\n");
				}
				pkginfo.write("	</PackageDirs>\n");

				pkginfo.write("	<PackageFiles>\n");
				for (var packageIndex in packagesList) {
					var packageFilePath = packagesList[packageIndex];
					pkginfo.write("		<FilePath>" + packageFilePath + "</FilePath>\n");
				}
				pkginfo.write("	</PackageFiles>\n");

				pkginfo.write("	<RegistryFiles/>\n");

				pkginfo.write("</Acf>\n");
				pkginfo.close();
			}
			return cmd;
		}
    }

	// Special rule for rcc compiler for resource files using generated files
	// It set the rcc root to project in generated directory
	Rule{
		inputs: ["qrc_generated"]
		explicitlyDependsOn: ["qm"]

		Artifact{
            filePath: AcfService.getGeneratedPath(product) + "/qrc_" + input.completeBaseName + ".cpp"
			fileTags: ["cpp"]
		}
		prepare:{
			var tempResourceDir = product.buildDirectory + "/GeneratedFiles/";

			var copyCmd;
			if (product.moduleProperty("qbs", "targetOS").contains("windows")){
				copyCmd = new Command('xcopy', ['/Y', FileInfo.toWindowsSeparators(input.filePath), FileInfo.toWindowsSeparators(tempResourceDir)]);
			}
			else{
				copyCmd = new Command('cp', [input.filePath, tempResourceDir]);
			}
			var rccCmd = new Command(product.moduleProperty("Qt.core", "binPath") + "/rcc", [
						tempResourceDir + FileInfo.fileName(input.filePath),
						"-name", FileInfo.completeBaseName(input.filePath),
						"-o", output.filePath]);

			copyCmd.description = 'copy to generated ' + FileInfo.fileName(input.filePath);
			copyCmd.highlight = 'codegen';

			rccCmd.description = 'rcc from generated ' + FileInfo.fileName(input.filePath);
			rccCmd.highlight = 'codegen';

			var commands = [];
			commands.push(copyCmd);
			commands.push(rccCmd);
			return commands;
		}
	}

	// Correction of QT directory structure
    Rule{
        inputs: ["ui_corr"]

        Artifact{
            filePath: AcfService.getGeneratedPath(product) + '/ui_' + input.completeBaseName + '.h'
            fileTags: ["hpp"]
        }

        prepare:{
            var cmd = new Command(product.moduleProperty("Qt.core", "binPath") + '/'
                                  + product.moduleProperty("Qt.gui", "uicName"),
                                  [input.filePath, '-o', output.filePath])
            cmd.description = 'uic ' + input.fileName;
            cmd.highlight = 'codegen';
            return cmd;
        }
    }

	cpp.includePaths: product.buildDirectory
	Qt.core.generatedFilesDir : product.buildDirectory + "/GeneratedFiles/" + product.name
}
