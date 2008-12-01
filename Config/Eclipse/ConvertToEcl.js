var projectExt = "vcproj"
var projectExp = new RegExp(".*\." + projectExt + "$");


function ProcessFolder(fileSystem, shell, folder, subPath, parentFolder, parentSubPath, projectPrefix, beQuiet, isTest)
{
	var retVal = new String;

	if (parentFolder != null){
		var fileIter = new Enumerator(folder.files);
		for (; !fileIter.atEnd(); fileIter.moveNext()){
			var file = fileIter.item();

			if (projectExp.exec(file.Name)){
				var outputDir = parentFolder + "/Ecl";
				var templatePath = shell.ExpandEnvironmentStrings("%ACFDIR%/Config/Eclipse/ProjTemplDir");
				if (!beQuiet){
		            WScript.Echo("Copy template from " + templatePath + " to folder: " + outputDir);
		        }

				fileSystem.CopyFolder(templatePath, outputDir);

//		        WScript.Echo("Creating folder: " + outputDir);
//				fileSystem.CreateFolder(outputDir);

                var eclipseProjectFile = outputDir + "/" + ".project";

				var xalanCommand = "%XALANDIR%/Bin/Xalan.exe";

				xalanCommand +=
				            " -p SourcePath '" + projectPrefix + parentSubPath + "'" +
				            " -o " + eclipseProjectFile + " " +
				            " " + folder + "/" + file.Name +
				            " %ACFDIR%/Config/Eclipse/VC2Ecl.xslt";
				            
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
    				retVal += outputDir + "\n";
				}
			}
		}
	}

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd() && (!isTest || retVal ==""); subFolderIter.moveNext()){
		var childFile = subFolderIter.item();
		retVal += ProcessFolder(fileSystem, shell, childFile, subPath + "/" + childFile.Name, folder, subPath, projectPrefix, beQuiet, isTest);
	}
	
	return retVal;
}


var projectPrefix = "";
var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var fullPath = fileSystem.GetParentFolderName(WScript.ScriptFullName);
var shell = WScript.CreateObject("WScript.Shell");

var isTest = false;
var beQuiet = false;

for (var i = 0; i < WScript.Arguments.length; ++i){
    var argument = WScript.Arguments(i).toString();
    if (argument.substr(0, 1) == "-"){
        if (argument.toUpperCase() == "-T"){
            isTest = true;
        }
        else if (argument.toUpperCase() == "-Q"){
            beQuiet = true;
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
    var message = ProcessFolder(fileSystem, shell, fileSystem.GetFolder("."), "", null, null, projectPrefix, beQuiet, isTest);
    if (message.length > 1){
	    WScript.Echo("Following projects was converted:\n\n" + message);
    }
    else{
	    WScript.Echo("No project was converted");
    }
}
else{
    WScript.Echo("Usage: ConvertToEcl [-t] [-q] ProjectPath\nwhere\n\t-t\t\ttest mode (only one project will be converted)\n\t-q\t\tbe quiet\n\tProjectPath\tproject path will be used to represent actual directory for eclipse");
}
   

WScript.Quit(0);

