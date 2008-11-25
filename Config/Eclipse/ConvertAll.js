var projectExt = "vcproj"
var projectExp = new RegExp(".*\." + projectExt + "$");


function ProcessFolder(shell, folder, subPath, parentFolder, parentSubPath)
{
	var retVal = new String;

	if (parentFolder != null){
		var fileIter = new Enumerator(folder.files);
		for (; !fileIter.atEnd(); fileIter.moveNext()){
			var file = fileIter.item();

			if (projectExp.exec(file.Name)){
				var outputFile = parentFolder + "/Ecl";
				var mdCommand = "md " + outputFile;
				WScript.Echo("Running: " + mdCommand);
				shell.Run(mdCommand);

				var xalanCommand = "%XALANDIR%\Bin\Xalan -p 'ACFPath/" + parentSubPath + " 'ACFPath/Include/iser' -o " + outputFile + " " + file.Name + " %ACFDIR%/Config/Eclipse/VC2Ecl.xslt";
				WScript.Echo("Running: " + xalanCommand);
				shell.Run(xalanCommand);
			}
		}
	}

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd(); subFolderIter.moveNext()){
		var childFile = subFolderIter.item();
		retVal += ProcessFolder(shell, childFile, subPath + "/" + childFile.Name, folder, subPath);
	}
	
	return retVal;
}


var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var fullPath = fileSystem.GetParentFolderName(WScript.ScriptFullName);
var shell = WScript.CreateObject("WScript.Shell");
var message = ProcessFolder(shell, fileSystem.GetFolder(fullPath), "", null, null);
if (message.length > 0){
	WScript.Echo("Following projects was converted:\n\n" + message);
}
else{
	WScript.Echo("No project was converted");
}

WScript.Quit(0);

