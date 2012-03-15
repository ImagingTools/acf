//*****************************************************************************************
// Recursive copy PRO project files to sibling directory (eg. from QMake to VC9).
// Additionaly copied files will be corrected to provide correct functionality with ACF structures.
//*****************************************************************************************
var qmakeDirName = "QMake"
var projectExt = "pro";
var projectExp = new RegExp(".*\." + projectExt + "$");
var projectIncExt = "pri";
var projectIncExp = new RegExp(".*\." + projectIncExt + "$");


function ProcessFolder(shell, fileSystem, folder, vcDirName)
{
	var retVal = new String;

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd(); subFolderIter.moveNext()){
		var subfolder = subFolderIter.item();
		if (subfolder.Name == qmakeDirName){
		    var destDir = folder + "\\" + vcDirName;
		    if (!fileSystem.FolderExists(destDir)){
		        fileSystem.CreateFolder(destDir);
		    }

    		var fileIter = new Enumerator(subfolder.files);
    		for (; !fileIter.atEnd(); fileIter.moveNext()){
        		var file = fileIter.item();

    		    if (projectExp.exec(file.Name)){
                    // Move project to destination dir
    		        var outputPath = destDir + "\\" + file.Name;

                    if (fileSystem.FileExists(outputPath)){
                        fileSystem.DeleteFile(outputPath);
                    }

                    var inputFile = fileSystem.OpenTextFile(file, 1);

                    var text = inputFile.readAll();
                    inputFile.close();

                    var re1 = new RegExp(qmakeDirName, "ig");
                    text = text.replace(re1, vcDirName);

                    var re2 = new RegExp("Config\\" + vcDirName, "ig");
                    text = text.replace(re2, "Config/" + qmakeDirName);

                    var re3 = new RegExp("Config/" + vcDirName, "ig");
                    text = text.replace(re3, "Config/" + qmakeDirName);

                    var outputFile = fileSystem.OpenTextFile(outputPath, 2, true);
                    outputFile.write(text);
    		    }
    		    else if (projectIncExp.exec(file.Name)) {
    		        // Move project to destination dir
    		        var outputPath = destDir + "\\" + file.Name;

    		        if (fileSystem.FileExists(outputPath)) {
    		            fileSystem.DeleteFile(outputPath);
    		        }

    		        var inputFile = fileSystem.OpenTextFile(file, 1);

    		        var text = inputFile.readAll();
    		        inputFile.close();

    		        var outputFile = fileSystem.OpenTextFile(outputPath, 2, true);
    		        outputFile.write(text);
    		    }
    		}
	    }
	    else{
		    ProcessFolder(shell, fileSystem, subfolder, vcDirName);
		}
	}
}


var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var shell = WScript.CreateObject("WScript.Shell");

if (WScript.Arguments.length >= 1){
    var vcDirName = WScript.Arguments(0).toString();

    ProcessFolder(shell, fileSystem, fileSystem.GetFolder("."), vcDirName);
}
