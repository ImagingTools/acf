import qbs.base 1.0
import qbs.FileInfo

Module{
	name: "acf"

	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	property string COMPILER_NAME: "Qbs"

	Properties{
		condition: cpp.compilerPath.contains("2005") || cpp.compilerPath.contains("VC8")
		COMPILER_NAME: "VC10"
	}
	Properties{
		condition: cpp.compilerPath.contains("2008") || cpp.compilerPath.contains("VC9")
		COMPILER_NAME: "VC10"
	}
	Properties{
		condition: cpp.compilerPath.contains("2010") || cpp.compilerPath.contains("VC10")
		COMPILER_NAME: "VC9"
	}

	property string COMPILER_DIR
	Properties{
		condition: cpp.debugInformation == true
		COMPILER_DIR: "Debug" + COMPILER_NAME
	}
	Properties{
		condition: cpp.debugInformation == false
		COMPILER_DIR: "Release" + COMPILER_NAME
	}

	readonly property path acfConfigurationFile: "undefined_file"
	property path trConfigurationFile: acfConfigurationFile
	property path trRegFile

	FileTagger{
		pattern: "*.arx"
		fileTags: ["arx"]
	}

	FileTagger{
		pattern: "*.xtracf"
		fileTags: ["xtracf"]
	}

	Rule{
		id: arxCompiler
		inputs: ["arx"]
		auxiliaryInputs: ["dynamiclibrary", "application"]
		usings: ["dynamiclibrary", "application"]
		explicitlyDependsOn: ["acfTool", "acfComponent"]

		Artifact{
			fileName: product.name + '/Generated/C' + input.baseName + '.cpp'
			fileTags: ["cpp"]
		}
		Artifact{
			fileName: product.name + '/Generated/C' + input.baseName + '.h'
			fileTags: ["hpp"]
		}

		prepare:{
			var cmd = new Command(product.buildDirectory + '/Bin/' + product.moduleProperty("cpp", "executablePrefix") + 'Arxc' + product.moduleProperty("cpp", "executableSuffix"), [
						inputs.arx[0].fileName,
						'-config', product.moduleProperty("acf", "acfConfigurationFile"),
						'-o', outputs.cpp[0].fileName]);
			cmd.description = 'arxc ' + FileInfo.fileName(inputs.arx[0].fileName)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = product.buildDirectory + '/Bin/';

			return cmd;
		}
	}

	Rule{
		id: acftransform
		inputs: ["xtracf"]

		Artifact{
			fileName: product.name + '/Generated/' + input.completeBaseName
		}

		prepare:{
			var cmd = new Command(product.buildDirectory + '/Bin/' + product.moduleProperty("cpp", "executablePrefix") + 'Acf' + product.moduleProperty("cpp", "executableSuffix"), [
						product.moduleProperty("acf", "trRegFile").fileName,
						'-config', product.moduleProperty("acf", "trConfigurationFile").fileName,
						'-input', input.fileName,
						'-o', output.fileName]);
			cmd.description = 'acf transformation ' + FileInfo.fileName(input.fileName)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = 'Bin';

			return cmd;
		}
	}

	// Special rule for rcc compiler for resource files using generated files
	// It set the rcc root to project in generated directory
	Rule{
		inputs: ["qrc_generated"]
		explicitlyDependsOn: ["qm"]

		Artifact{
			fileName: product.name + '/Generated/qrc_' + input.completeBaseName + '.cpp'
			fileTags: ["cpp"]
		}
		prepare:{
			var tempResourceDir = product.buildDirectory + "/" + product.name + "/";
			var tempResourceFile = tempResourceDir + FileInfo.fileName(input.fileName);

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
				copyCmd = new Command('copy', [input.fileName, tempResourceDir]);
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
}
