import qbs.base 1.0
// Custom build settings

Module{
	name: "AcfRules"

	property path configFile
	property bool verbose: false

	ARX_COMPILER: Arxc
	ACF_TOOL: Acf

	FileTagger{
		pattern: "*.ui"
		fileTags: ["ui"]
	}

	// custom build for Subversion Transformations

	Rule {
		inputs: ["arx"]

		Artifact {
			fileName: 'Generated/C' + input.completeBaseName + '.cpp'
			fileTags: ["cpp"]
		}

		prepare: {
			var mytool = "Arxc";

			if (verbose){
				var cmd = new Command(mytool, [inputs["arx"][0].fileName, "-o", output.fileName, "-v", "-config", inputs.configFile]);
			}
			else{
				var cmd = new Command(mytool, [inputs["arx"][0].fileName, "-o", output.fileName, "-config", inputs.configFile]);
			}

			cmd.description = "Generate\t" + input.baseName;
			cmd.highlight = "codegen";
			return cmd;
		}
	}
}
