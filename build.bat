rmdir temp\bloodworks /s /q
mkdir temp
mkdir temp\bloodworks

xcopy game\*.txt .\temp\bloodworks /sy
xcopy game\*.json .\temp\bloodworks /sy
xcopy game\*.lua .\temp\bloodworks /sy
xcopy game\*.dll .\temp\bloodworks /sy
xcopy game\*.png .\temp\bloodworks /sy
xcopy game\*.ogg .\temp\bloodworks /sy
xcopy game\*.wav .\temp\bloodworks /sy
xcopy game\*.exe .\temp\bloodworks /sy
xcopy game\*.ps .\temp\bloodworks /sy
xcopy game\*.vs .\temp\bloodworks /sy

powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('temp\bloodworks', 'bloodworks.zip'); }"