var projectExt = "vcproj";
var projectExp = new RegExp(".*\." + projectExt + "$");


function TransformDocument(shell, inputPath, outputPath, templatePath, logicalPath, rootPath, includes, defines, beQuiet, isTest)
{
	var retVal = "";

	var xalanCommand = "%XALANDIR%/Bin/Xalan.exe";
	xalanCommand +=
				" -p SourcePath '" + logicalPath + "'" +
				" -p RootPath '" + rootPath + "'" +
				" -p ExtraIncludes '" + includes + "'" +
				" -p Defines '" + defines + "'" +
				" -p CompilerCode 'Xcd'" +
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


function ProcessFolder(fileSystem, shell, folder, subPath, parentFolder, parentSubPath, projectPrefix, rootPath, include, defines, beQuiet, isTest)
{
	var retVal = new String;

	if (parentFolder != null){
		var fileIter = new Enumerator(folder.files);
		for (; !fileIter.atEnd(); fileIter.moveNext()){
			var file = fileIter.item();

			if (projectExp.exec(file.Name)){
				var fileName = String(file.Name);
				var projectName = fileName.substr(0, fileName.length - projectExt.length - 1);
				var outputDir = parentFolder + "/Xcd";
				var outputSubDir = parentFolder + "/Xcd/" + projectName + ".xcodeproj";

				if (!fileSystem.FolderExists(outputDir)){
					if (!beQuiet){
						WScript.Echo("Create folder " + outputDir);
					}

					fileSystem.CreateFolder(outputDir);
				}

				if (!fileSystem.FolderExists(outputSubDir)){
					if (!beQuiet){
						WScript.Echo("Create folder " + outputSubDir);
					}

					fileSystem.CreateFolder(outputSubDir);
				}

				retVal += TransformDocument(
							shell,
							folder + "/" + file.Name,
							outputSubDir + "/" + "project.pbxproj",
							"%ACFDIR%/Config/Xcd/VC2Xcd.xslt",
							projectPrefix,
							rootPath,
							includes,
							defines,
							beQuiet,
							isTest);
			}
		}
	}

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd() && (!isTest || retVal ==""); subFolderIter.moveNext()){
		var childFile = subFolderIter.item();
		retVal += ProcessFolder(fileSystem, shell, childFile, subPath + "/" + childFile.Name, folder, subPath, projectPrefix, rootPath, includes, defines, beQuiet, isTest);
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
var defines = "";
var rootPath = "../../..";

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
        else if (argument.substr(0, 2).toUpperCase() == "-D"){
			if (defines != ""){
				defines += ",";
			}

			defines += argument.substr(2);
        }
        else if (argument.substr(0, 2).toUpperCase() == "-R"){
			rootPath = argument.substr(2);
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
    var message = ProcessFolder(fileSystem, shell, fileSystem.GetFolder("."), "", null, null, projectPrefix, rootPath, includes, defines, beQuiet, isTest);
    if (message.length > 1){
	    WScript.Echo("Following projects was converted:\n\n" + message);
    }
    else{
	    WScript.Echo("No project was converted");
    }
}
else{
    WScript.Echo("Usage: ConvertToXcd [-t] [-q] [-i<<ExtraIncludePath>>] ProjectPath\nwhere\n\t-t\t\ttest mode (only one project will be converted)\n\t-q\t\tbe quiet\n\t-i\t\textra include path\n\tProjectPath\tproject path will be used to represent actual directory for eclipse");
}
   

WScript.Quit(0);

