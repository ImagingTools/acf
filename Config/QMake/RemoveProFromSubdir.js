//*****************************************************************************************
// Recursive remove all PRO project files from specified directory (eg. from VC9).
//*****************************************************************************************
var projectExt = "pro";
var projectExp = new RegExp(".*\." + projectExt + "$");
var projectIncExt = "pri";
var projectIncExp = new RegExp(".*\." + projectIncExt + "$");


function ProcessFolder(shell, fileSystem, folder, subDirName)
{
	var retVal = new String;

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd(); subFolderIter.moveNext()){
		var subfolder = subFolderIter.item();
		if (subfolder.Name == subDirName) {
    		var fileIter = new Enumerator(subfolder.files);
    		for (; !fileIter.atEnd(); fileIter.moveNext()){
        		var file = fileIter.item();

        		if (projectExp.exec(file.Name)) {
        		    // Remove project file
        		    fileSystem.DeleteFile(file);
        		}
        		else if (projectIncExp.exec(file.Name)) {
        		    // Remove project file
        		    fileSystem.DeleteFile(file);
        		}
            }
	    }
	    else{
		    ProcessFolder(shell, fileSystem, subfolder, subDirName);
		}
	}
}


var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var shell = WScript.CreateObject("WScript.Shell");

if (WScript.Arguments.length >= 1){
    var subDirName = WScript.Arguments(0).toString();

    ProcessFolder(shell, fileSystem, fileSystem.GetFolder("."), subDirName);
}
