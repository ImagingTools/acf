function getGeneratedPath()
{
    var outputDir = product.moduleProperty("acf", "generatedOutputDir");
	if (outputDir === null){
        return "GeneratedFiles/" + product.name;
    }
    else{
        return outputDir;
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

