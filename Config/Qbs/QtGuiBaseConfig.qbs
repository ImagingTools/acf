// Standard settings for a Qt based project

Module {
    Rule {
        inputs: ["ui"]

        Artifact {
            fileName: 'Generated/' + product.name + '/ui_' + input.completeBaseName + '.h'
            fileTags: ["hpp"]
        }

        prepare: {
            var cmd = new Command(ModUtils.moduleProperty(product, "binPath") + '/'
                                  + ModUtils.moduleProperty(product, "uicName"),
                                  [input.fileName, '-o', output.fileName])
            cmd.description = 'uic ' + FileInfo.fileName(input.fileName);
            cmd.highlight = 'codegen';
            return cmd;
        }
    }
}
