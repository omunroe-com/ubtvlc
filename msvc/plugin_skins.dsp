# Microsoft Developer Studio Project File - Name="plugin_skins" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=plugin_skins - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plugin_skins.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plugin_skins.mak" CFG="plugin_skins - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plugin_skins - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "plugin_skins - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plugin_skins - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_MT" /D "_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_MT" /D "_DLL" /YX /FD -I..\include /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /entry:_CRT_INIT@12
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib netapi32.lib winmm.lib /nologo /dll /machine:I386 /entry:_CRT_INIT@12

!ELSEIF  "$(CFG)" == "plugin_skins - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_MT" /D "_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_MT" /D "_DLL" /FR /YX /FD /GZ -I..\include /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /subsystem:console /debug /machine:I386 /pdbtype:sept /entry:_CRT_INIT@12
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib netapi32.lib winmm.lib /nologo /dll /subsystem:console /debug /machine:I386 /pdbtype:sept /entry:_CRT_INIT@12
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "plugin_skins - Win32 Release"
# Name "plugin_skins - Win32 Debug"

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"

# Begin Source File
SOURCE="..\modules\gui\skins\controls\button.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\checkbox.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\generic.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\image.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\playlist.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\rectangle.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\slider.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\text.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\parser\flex.c"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\parser\skin.c"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\parser\wrappers.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\anchor.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\banks.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\bezier.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\bitmap.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\dialogs.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\event.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\font.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\graphics.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\skin_main.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\theme.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\themeloader.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\vlcproc.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\window.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_api.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_bitmap.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_dragdrop.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_event.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_font.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_graphics.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_run.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_theme.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_window.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_api.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_bitmap.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_dragdrop.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_event.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_font.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_graphics.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_run.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_theme.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_timer.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_window.cpp"
# ADD CPP /D "__VLC__" /D "__PLUGIN__"  /D "MODULE_NAME=skins" /D "MODULE_NAME_IS_skins" 
!IF "$(CFG)" == "plugin_skins - Win32 Release"
# PROP Output_Dir "Release\modules\gui\skins"
# PROP Intermediate_Dir "Release\modules\gui\skins"
!ELSEIF "$(CFG)" == "plugin_skins - Win32 Debug"
# PROP Output_Dir "Debug\modules\gui\skins"
# PROP Intermediate_Dir "Debug\modules\gui\skins"
# End Source File

# End Group

# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"

# Begin Source File
SOURCE="..\modules\gui\skins\os_api.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\os_bitmap.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\os_event.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\os_font.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\os_graphics.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\os_theme.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\os_window.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\controls.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\button.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\checkbox.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\generic.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\image.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\playlist.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\rectangle.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\slider.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\controls\text.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\parser\skin.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\parser\wrappers.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\anchor.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\banks.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\bezier.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\bitmap.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\dialogs.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\event.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\font.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\graphics.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\skin_common.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\theme.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\themeloader.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\vlcproc.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\src\window.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_bitmap.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_dragdrop.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_event.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_font.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_graphics.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_theme.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\win32\win32_window.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_bitmap.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_dragdrop.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_event.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_font.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_graphics.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_theme.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_timer.h"
# End Source File
# Begin Source File
SOURCE="..\modules\gui\skins\x11\x11_window.h"
# End Source File

# End Group

# End Target
# End Project
