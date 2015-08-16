import qbs.base 1.0
import qbs.TextFile
import qbs.File
import qbs.FileInfo
import qbs.PathTools
import qbs.ModUtils
import "../AcfService.js" as AcfService
import "../AcfModule.qbs" as AcfModule

AcfModule{
    condition: qbs.hostOS.contains('windows') && qbs.targetOS.contains('windows') && qbs.toolchain.contains('msvc')

	Depends{ name: "acf" }
	Depends{ name: "Qt.core" }
	Depends{ name: "Qt.gui" }

    name: "acf"

    Rule {
        id: dynamicLibraryLinker
        multiplex: true
        inputs: ["obj"]
        inputsFromDependencies: ["staticlibrary", "dynamiclibrary_import", "debuginfo"]

        outputFileTags: ["acf_dynamiclibrary", "dynamiclibrary_import", "debuginfo"]
        outputArtifacts: {
            var artifacts = [
                {
                    fileTags: ["acf_dynamiclibrary"],
                    filePath: product.destinationDirectory + "/" + PathTools.dynamicLibraryFilePath(product)
                },
                {
                    fileTags: ["dynamiclibrary_import"],
                    filePath: product.destinationDirectory + "/" + PathTools.importLibraryFilePath(product),
                    alwaysUpdated: false
                }
            ];
            if (product.moduleProperty("cpp", "debugInformation") && product.moduleProperty("cpp", "separateDebugInformation")){
                var lib = artifacts[0];
                artifacts.push({
                    fileTags: ["debuginfo"],
                    filePath: lib.filePath.substr(0, lib.filePath.length - 4)
                              + product.moduleProperty("cpp", "debugInfoSuffix")
                });
            }
            return artifacts;
        }

        prepare: {
			var primaryOutput = (outputs.acf_dynamiclibrary[0]);
			var debugInformation = product.moduleProperty("cpp", "debugInformation");

			var linkerArgs = [];
			var dumpbinArgs = ["/LINKERMEMBER"];

			var allInputs = (inputs.obj || []).concat(inputs.staticlibrary || []).concat(inputs.dynamiclibrary_import || []);
			for (i in allInputs) {
				var fileName = FileInfo.toWindowsSeparators(allInputs[i].filePath);
				linkerArgs.push(fileName);
				dumpbinArgs.push(fileName);
			}

			linkerArgs.push('/NOLOGO', '/DLL', "/ERRORREPORT:PROMPT", "/SAFESEH", "/SUBSYSTEM:CONSOLE", "/MACHINE:X86", "/TLBID:1", "/DYNAMICBASE", "/NXCOMPAT");
			linkerArgs.push('/IMPLIB:' + FileInfo.toWindowsSeparators(outputs.dynamiclibrary_import[0].filePath));

			if (debugInformation){
				linkerArgs.push("/DEBUG");
				if (outputs.debuginfo)
					linkerArgs.push("/PDB:" + outputs.debuginfo[0].fileName);
			}
			else{
				linkerArgs.push('/INCREMENTAL:NO')
			}

			function pushLibs(libs)
			{
				if (!libs)
					return;
				var s = {};
				var c = libs.length;
				for (var i = 0; i < c; ++i) {
					var lib = FileInfo.toWindowsSeparators(libs[i]);
					if (!lib.match(/\.lib$/i)){
						lib += ".lib";
					}
					if (s[lib])
						continue;
					s[lib] = true;
					linkerArgs.push(lib);
				}
			}

			pushLibs(product.moduleProperty("cpp", "staticLibraries"));
			pushLibs(product.moduleProperty("cpp", "dynamicLibraries"));

			if (product.moduleProperty("cpp", "entryPoint")){
				linkerArgs.push("/ENTRY:" + product.moduleProperty("cpp", "entryPoint"));
			}

			var linkerOutputFilePath = FileInfo.toWindowsSeparators(primaryOutput.filePath);
			linkerArgs.push('/OUT:' + linkerOutputFilePath)
			var libraryPaths = product.moduleProperty("cpp", 'libraryPaths');
			for (var i in libraryPaths) {
				linkerArgs.push('/LIBPATH:' + FileInfo.toWindowsSeparators(libraryPaths[i]))
			}
			var linkerFlags = product.moduleProperty("cpp", 'platformLinkerFlags').concat(
						product.moduleProperty("cpp", 'linkerFlags'));
			linkerArgs = linkerArgs.concat(linkerFlags);
			if (product.moduleProperty("cpp", "allowUnresolvedSymbols")){
				linkerArgs.push("/FORCE:UNRESOLVED");
			}

			var commands = [];
			var cmd;

			var symbolsFilePath = linkerOutputFilePath + ".def";

			dumpbinArgs.push('/OUT:' + symbolsFilePath);

			cmd = new Command("dumpbin", dumpbinArgs)
			cmd.description = 'generating symbol list ' + symbolsFilePath;
			cmd.highlight = 'linker';
			cmd.workingDirectory = FileInfo.path(primaryOutput.filePath)
			commands.push(cmd);

			cmd = new Command(product.moduleProperty("cpp", "linkerPath"), linkerArgs)
			cmd.description = 'linking ' + primaryOutput.fileName;
			cmd.highlight = 'linker';
			cmd.workingDirectory = FileInfo.path(primaryOutput.filePath)
			cmd.responseFileUsagePrefix = '@';
			cmd.stdoutFilterFunction = function(output) {
				return output.replace(/^ +Creating library.*\r\n$/, "");
			};
			commands.push(cmd);

			return commands;
		}
    }
}
