import qbs.base 1.0
import qbs.TextFile
import qbs.FileInfo
import "AcfService.js" as AcfService

AcfModule{
    condition: true

	Depends{ name: "Qt.core" }

    name: "acf"

	Rule{
		id: arxCompiler
		inputs: ["arx"]
		usings: ["application", "dynamiclibrary", "xpc"]

		Artifact{
			fileName: AcfService.getGeneratedPath(product) + "/C" + input.completeBaseName + ".cpp"
			fileTags: ["cpp"]
		}
		Artifact{
			fileName: AcfService.getGeneratedPath(product) + "/C" + input.completeBaseName + ".h"
			fileTags: ["hpp", "c++_pch"]
		}

		prepare:{
			// get the ACF binary directory
			var acfBinDirectory = product.moduleProperty("ArxcExe", "acfBinDirectory");
			if (acfBinDirectory == null){
				acfBinDirectory = product.buildDirectory;
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
				throw new Error("No ACF configuration specified (using dependency or acf.acfConfigurationFile) in " + product.name);
			}

			var cmd = new Command(acfBinDirectory + "/" + product.moduleProperty("cpp", "executablePrefix") + "Arxc" + product.moduleProperty("cpp", "executableSuffix"), [
						inputs.arx[0].filePath,
						'-config', acfConfigurationFile,
						'-o', outputs.cpp[0].filePath]);
			cmd.description = 'arxc ' + FileInfo.fileName(inputs.arx[0].filePath)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = product.moduleProperty(product.moduleName, "qtBinPath");

			return cmd;
		}
	}

	Rule{
		id: acftransform
		inputs: ["xtracf"]

		Artifact{
			fileName: AcfService.getGeneratedPath(product) + "/" + input.completeBaseName
			fileTags: { return product.moduleProperty("acf", "trOutputType"); }
		}

		prepare:{
			// get the ACF binary directory
			var acfBinDirectory = product.moduleProperty("AcfExe", "acfBinDirectory");
			if (acfBinDirectory == null){
				acfBinDirectory = product.buildDirectory;
			}

			var acfRegistryFile = product.moduleProperty("acf", "trRegFile");

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

			// if there is no registry file - error
			if (acfRegistryFile == null){
				throw new Error("no ACF registry specified for transformation (using dependency or acf.trRegFile) in " + product.name);
			}

			// if there is no configuration - error
			if (acfConfigurationFile == null){
				throw new Error("no ACF configuration specified (using dependency or acf.trConfigurationFile) in " + product.name);
			}

			var cmd = new Command(acfBinDirectory + '/' + product.moduleProperty("cpp", "executablePrefix") + 'Acf' + product.moduleProperty("cpp", "executableSuffix"), [
						acfRegistryFile,
						'-config', acfConfigurationFile,
						'-input', input.filePath,
						'-o', output.filePath]);
			cmd.description = 'acf transformation ' + FileInfo.fileName(input.filePath)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = product.moduleProperty(product.moduleName, "qtBinPath");

			return cmd;
		}
	}

	// local property to get path of Qt
	readonly property string qtBinPath: Qt.core.binPath

	// Special rule for rcc compiler for resource files using generated files
	// It set the rcc root to project in generated directory
	Rule{
		inputs: ["qrc_generated"]
		explicitlyDependsOn: ["qm"]

		Artifact{
			fileName: AcfService.getGeneratedPath(product) + "/qrc_" + input.completeBaseName + ".cpp"
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
			var rccCmd = new Command(product.moduleProperty(product.moduleName, "qtBinPath") + "/rcc", [
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
				var outputFilePath = output.filePath;

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
				var outputFilePath = output.filePath;
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
					configsList.push(AcfService.relativePath(outputDir, inputProduct.filePath));
				}

				for (var dependencyIndex in dependencies){
					var dependency = dependencies[dependencyIndex];
					var dependencyFilePath = product.moduleProperty(dependency.name, "xpcFilePath");
					if (dependencyFilePath != null){
						configsList.push(AcfService.relativePath(outputDir, dependencyFilePath));
					}
					else if (dependency.type.contains("xpc")){
						configsList.push(AcfService.relativePath(product.destinationDirectory, dependency.destinationDirectory + "/" + dependency.name + ".xpc"));
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
					packagesList.push(AcfService.relativePath(outputDir, inputProduct.filePath));
				}

				for (var dependencyIndex in dependencies) {
					var dependency = dependencies[dependencyIndex];
					if (dependency.type.contains("acfComponent")){
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
