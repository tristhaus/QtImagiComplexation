set PATH=%PATH%;C:\Qt\6.0.3\msvc2019_64\bin

IF EXIST .\Deploy-Release (
    rmdir .\Deploy-Release /s /q
)
md .\Deploy-Release

                                                     ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release
windeployqt --dir .\Deploy-Release --no-translations ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release\QtImagiComplexation.exe
copy ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release\QtImagiComplexation.exe .\Deploy-Release\
copy .\LICENSE .\Deploy-Release\
copy .\README.md .\Deploy-Release\
copy .\LIESMICH.md .\Deploy-Release\

IF EXIST .\Deploy-Release-PDB (
    rmdir .\Deploy-Release-PDB /s /q
)
md .\Deploy-Release-PDB

windeployqt --pdb --dir .\Deploy-Release-PDB --no-translations ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release\QtImagiComplexation.exe
copy ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release\QtImagiComplexation.exe .\Deploy-Release-PDB\
copy ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release\QtImagiComplexation.map .\Deploy-Release-PDB\
copy ..\build-QtImagiComplexationSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtImagiComplexation\release\QtImagiComplexation.pdb .\Deploy-Release-PDB\
copy .\LICENSE .\Deploy-Release-PDB\
copy .\README.md .\Deploy-Release-PDB\
copy .\LIESMICH.md .\Deploy-Release-PDB\
