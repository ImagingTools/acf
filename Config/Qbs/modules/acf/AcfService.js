function getGeneratedPath(product)
{
    var outputDir = product.moduleProperty("acf", "generatedOutputDir");
	if ((outputDir === undefined) || (outputDir === null)){
        return "GeneratedFiles/" + product.name;
    }
    else{
        return outputDir;
    }
}

function getTargetPath(product, project, subDir)
{
    var useSubdir = product.moduleProperty("acf", "installProjectSubdirs");
    if (useSubdir && project.projectSubDir !== undefined) {
        return project.projectSubDir + "/" + subDir;
    }
    else {
        return subDir;
    }
}

function isSubpath(base, rel)
{
    var basel = base.split('/');
    var rell = rel.split('/');
    var i = 0;
    var j = 0;
    while (i < basel.length) {
        if (basel[i] === '.' || basel[i] === '') {
            i++;
            continue;
        }

        if (j >= rell.length) {
            return false;
        }

        if (rell[j] === '.' || rell[j] === '') {
            j++;
            continue;
        }

        if (basel[i] !== rell[j]) {
            return false;
        }

        i++;
        j++;
    }

    return true;
}

function relativePath(base, rel) {
    var basel = base.split('/');
    var rell = rel.split('/');
    var i;
    for (i = basel.length; i-- >= 0; ) {
        if (basel[i] === '.')
            basel.splice(i, 1);
    }
    for (i = basel.length; i-- >= 1; ) {
        if (basel[i] === '')
            basel.splice(i, 1);
    }
    for (i = rell.length; i-- >= 0; ) {
        if (rell[i] === '.')
            rell.splice(i, 1);
    }
    for (i = rell.length; i-- >= 1; ) {
        if (rell[i] === '')
            rell.splice(i, 1);
    }

    i = 0;
    while (i < basel.length && i < rell.length && basel[i] === rell[i])
        i++;

    var j = i;
    var r = [];

    var useAbsolutePath = (i == 0) && (rell.length > 0) && (rell[0] == "" || (rell[0].search(":") >= 0));

    if (!useAbsolutePath) {
        for (; i < basel.length; i++)
            r.push('..');
    }

    for (; j < rell.length; j++)
        r.push(rell[j]);

    return r.join('/');
}
