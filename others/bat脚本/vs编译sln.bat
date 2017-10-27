
cls
@echo on

set devnev="e:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe"  
%devnev% D:\TEMP\obs-build\plugins\lj-source\lj-source.sln /build "Release|win32"  
REM %devnev% D:\TEMP\obs-build\plugins\lj-source\lj-source.sln /rebuild "Release|win32"  


del  "E:\bitbrothers\ljlive\release\obs-plugins\32bit\lj-source.dll"
copy "D:\TEMP\obs-build\plugins\lj-source\Release\lj-source.dll" "E:\bitbrothers\ljlive\release\obs-plugins\32bit"
pause