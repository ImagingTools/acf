// Custom build settings

Module {
	ARX_COMPILER: Arxc
	ACF_TOOL: Acf

	// custom build for Subversion Transformations

    Rule {
		inputs: ["arx"]

		Artifact {
			fileName: 'Generated/C' + input.completeBaseName + '.cpp'
			fileTags: ["cpp"]
		}

		prepare: {
			var mytool = "Arxc";

			var cmd = new Command(mytool, [inputs["arx"][0].fileName, "-o", output.fileName, "-config", inputs.configFile]);
			cmd.description = "Generate\t" + input.baseName;
			cmd.highlight = "codegen";
			return cmd;
		}
    }
}
