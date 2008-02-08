var name="ACFDIR";

var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var path = fileSystem.GetParentFolderName(WScript.ScriptFullName);
var registryPath = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\" + name

var shell = WScript.CreateObject("WScript.Shell");
shell.RegWrite(registryPath, path, "REG_SZ");

WScript.Quit(0);
