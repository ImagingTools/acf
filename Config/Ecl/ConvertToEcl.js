var projectExt = "vcproj"
var projectExp = new RegExp(".*\." + projectExt + "$");


function TransformDocument(shell, inputPath, outputPath, templatePath, logicalPath, includes, beQuiet, isTest)
{
	var retVal = "";

	var xalanCommand = "%XALANDIR%/Bin/Xalan.exe";
	xalanCommand +=
				" -p SourcePath '" + logicalPath + "'" +
				" -p ExtraIncludes '" + includes + "'" +
				" -p CompilerCode 'Ecl'" +
				" -o " + outputPath + " " +
				" " + inputPath +
				" " + templatePath;
	xalanCommand = shell.ExpandEnvironmentStrings(xalanCommand);

	if (!beQuiet){
		WScript.Echo("Running: " + xalanCommand);
	}
	var errorCode = shell.Run(xalanCommand, 0, true)
	if (errorCode != 0){
		WScript.Echo("Error Running: " + xalanCommand + "\n" + "Error code =" + errorCode);
		if (isTest){
			retVal += " ";
		}
	}
	else{
		retVal += outputPath + "\n";
	}
	
	return retVal;
}


function ProcessFolder(fileSystem, shell, folder, subPath, parentFolder, parentSubPath, projectPrefix, include, beQuiet, isTest)
{
	var retVal = new String;

	if (parentFolder != null){
		var fileIter = new Enumerator(folder.files);
		for (; !fileIter.atEnd(); fileIter.moveNext()){
			var file = fileIter.item();

			if (projectExp.exec(file.Name)){
				var outputDir = parentFolder + "/Ecl";

				if (!fileSystem.FolderExists(outputDir)){
					if (!beQuiet){
						WScript.Echo("Create folder " + outputDir);
					}

					fileSystem.CreateFolder(outputDir);
				}

				retVal += TransformDocument(
							shell,
							folder + "/" + file.Name,
							outputDir + "/" + ".project",
							"%ACFDIR%/Config/Eclipse/VC2Ecl.xslt",
							projectPrefix + parentSubPath,
							includes,
							beQuiet,
							isTest);
				retVal += TransformDocument(
							shell,
							folder + "/" + file.Name,
							outputDir + "/" + ".cproject",
							"%ACFDIR%/Config/Eclipse/VC2EclC.xslt",
							projectPrefix + parentSubPath,
							includes,
							beQuiet,
							isTest);
			}
		}
	}

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd() && (!isTest || retVal ==""); subFolderIter.moveNext()){
		var childFile = subFolderIter.item();
		retVal += ProcessFolder(fileSystem, shell, childFile, subPath + "/" + childFile.Name, folder, subPath, projectPrefix, includes, beQuiet, isTest);
	}
	
	return retVal;
}


var projectPrefix = "";
var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var fullPath = fileSystem.GetParentFolderName(WScript.ScriptFullName);
var shell = WScript.CreateObject("WScript.Shell");

var isTest = false;
var beQuiet = false;
var includes = "";

for (var i = 0; i < WScript.Arguments.length; ++i){
    var argument = WScript.Arguments(i).toString();
    if (argument.substr(0, 1) == "-"){
        if (argument.toUpperCase() == "-T"){
            isTest = true;
        }
        else if (argument.toUpperCase() == "-Q"){
            beQuiet = true;
        }
        else if (argument.substr(0, 2).toUpperCase() == "-I"){
			includes = argument.substr(2);
        }
        else{
			WScript.Echo("Unknown parameter: " + argument);
        }
    }
    else{
        projectPrefix = argument;
    }
}

if (projectPrefix.length > 0){
    var message = ProcessFolder(fileSystem, shell, fileSystem.GetFolder("."), "", null, null, projectPrefix, includes, beQuiet, isTest);
    if (message.length > 1){
	    WScript.Echo("Following projects was converted:\n\n" + message);
    }
    else{
	    WScript.Echo("No project was converted");
    }
}
else{
    WScript.Echo("Usage: ConvertToEcl [-t] [-q] [-i<<ExtraIncludePath>>] ProjectPath\nwhere\n\t-t\t\ttest mode (only one project will be converted)\n\t-q\t\tbe quiet\n\t-i\t\textra include path\n\tProjectPath\tproject path will be used to represent actual directory for eclipse");
}
   

WScript.Quit(0);

