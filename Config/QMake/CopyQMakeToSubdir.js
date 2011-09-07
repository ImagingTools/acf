var qmakeDirName = "QMake"
var projectExt = "vcproj";
var projectExp = new RegExp(".*\." + projectExt + "$");
var solutionExt = "sln";
var solutionExp = new RegExp(".*\." + solutionExt + "$");


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

                    fileSystem.MoveFile(file, outputPath);
    		    }
    		    else if (solutionExp.exec(file.Name)){
                    // Move corrected solution to destination dir
    		        var outputPath = destDir + "\\" + file.Name;

                    if (fileSystem.FileExists(outputPath)){
                        fileSystem.DeleteFile(outputPath);
                    }

                    var inputFile = fileSystem.OpenTextFile(file, 1);

                    var text = inputFile.readAll();
                    inputFile.close();
                    var re1 = /QMake/g;
                    text = text.replace(re1, vcDirName);

                    var outputFile = fileSystem.OpenTextFile(outputPath, 2, true);
                    outputFile.write(text);
                    
                    fileSystem.DeleteFile(file);
    		    }
    		}
    		
    		// Copy directories for Qt generated files
    		if (fileSystem.FolderExists(subfolder + "\\debug")){
    		    if (fileSystem.FolderExists(destDir + "\\Debug")){
            		fileSystem.DeleteFolder(subfolder + "\\debug");
            	}
            	else{
            		fileSystem.MoveFolder(subfolder + "\\debug", destDir + "\\Debug");
            	}
        	}
    		if (fileSystem.FolderExists(subfolder + "\\release")){
    		    if (fileSystem.FolderExists(destDir + "\\Release")){
            		fileSystem.DeleteFolder(subfolder + "\\release");
            	}
            	else{
            		fileSystem.MoveFolder(subfolder + "\\release", destDir + "\\Release");
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

if (WScript.Arguments.length == 1){
    var vcDirName = WScript.Arguments(0).toString();

    ProcessFolder(shell, fileSystem, fileSystem.GetFolder("."), vcDirName);
}
