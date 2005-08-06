# Microsoft Developer Studio Project File - Name="main" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=main - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "main.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "main.mak" CFG="main - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "main - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "main - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "main - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_mixer.lib SDL_net.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "main - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GR /GX /ZI /I "c:/tom/gl-117/gl-117/src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_mixer.lib SDL_net.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "main - Win32 Release"
# Name "main - Win32 Debug"
# Begin Group "model3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model3d\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Model3d.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Model3d.h
# End Source File
# Begin Source File

SOURCE=.\model3d\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Quad.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Rotation.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Space.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\SpaceObj.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\SpherePart.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Transformation.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Vector2.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Vector3.cpp
# End Source File
# Begin Source File

SOURCE=.\model3d\Vertex.cpp
# End Source File
# End Group
# Begin Group "opengl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\opengl\Frustum.cpp
# End Source File
# Begin Source File

SOURCE=.\opengl\GlPrimitives.cpp
# End Source File
# Begin Source File

SOURCE=.\opengl\GlPrimitives.h
# End Source File
# Begin Source File

SOURCE=.\opengl\includegl.h
# End Source File
# Begin Source File

SOURCE=.\opengl\VertexArray.cpp
# End Source File
# End Group
# Begin Group "loadmodel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\loadmodel\BinaryFile3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\loadmodel\Load3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\loadmodel\Load3ds.h
# End Source File
# Begin Source File

SOURCE=.\loadmodel\LoadObj.cpp
# End Source File
# Begin Source File

SOURCE=.\loadmodel\LoadObj.h
# End Source File
# Begin Source File

SOURCE=.\loadmodel\Model3dFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\loadmodel\Model3dFactory.h
# End Source File
# Begin Source File

SOURCE=.\loadmodel\Model3dRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\loadmodel\Model3dRegistry.h
# End Source File
# Begin Source File

SOURCE=.\loadmodel\TextFileObj.cpp
# End Source File
# End Group
# Begin Group "loadbitmap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\loadbitmap\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\loadbitmap\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\loadbitmap\BitmapFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\loadbitmap\LoadTga.cpp
# End Source File
# End Group
# Begin Group "landscape"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\landscape\Landscape.cpp
# End Source File
# Begin Source File

SOURCE=.\landscape\Landscape.h
# End Source File
# End Group
# Begin Group "gllandscape"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gllandscape\GlLandscape.cpp
# End Source File
# Begin Source File

SOURCE=.\gllandscape\GlLandscape.h
# End Source File
# End Group
# Begin Group "filetokenizer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\filetokenizer\FileTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\filetokenizer\FileTokenizer.h
# End Source File
# End Group
# Begin Group "effects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\effects\BlackSmoke.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\effects.h
# End Source File
# Begin Source File

SOURCE=.\effects\Explosion.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\Flash.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\HighClouds.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\Smoke.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\Star.cpp
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sound\SoundSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\sound\SoundSystem.h
# End Source File
# Begin Source File

SOURCE=.\sound\WaveFile.cpp
# End Source File
# End Group
# Begin Group "menu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\menu\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\Component.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\Component.h
# End Source File
# Begin Source File

SOURCE=.\menu\Container.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\EditField.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\EditJoystick.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\EditKey.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\menu\TextField.cpp
# End Source File
# End Group
# Begin Group "math"

# PROP Default_Filter "*.*"
# Begin Source File

SOURCE=.\math\Math.cpp
# End Source File
# Begin Source File

SOURCE=.\math\Math.h
# End Source File
# End Group
# Begin Group "logging"

# PROP Default_Filter "*"
# Begin Source File

SOURCE=.\logging\Logging.cpp
# End Source File
# Begin Source File

SOURCE=.\logging\Logging.h
# End Source File
# End Group
# Begin Group "configuration"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\configuration\ConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\configuration\Configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\configuration\Configuration.h
# End Source File
# Begin Source File

SOURCE=.\configuration\Dirs.cpp
# End Source File
# Begin Source File

SOURCE=.\configuration\Dirs.h
# End Source File
# Begin Source File

SOURCE=.\configuration\OptionFile.cpp
# End Source File
# Begin Source File

SOURCE=.\configuration\OptionFileFactory.cpp
# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter "*"
# Begin Source File

SOURCE=.\net\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\net\net.cpp
# End Source File
# Begin Source File

SOURCE=.\net\net.h
# End Source File
# Begin Source File

SOURCE=.\net\Server.cpp
# End Source File
# End Group
# Begin Group "pilot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pilot\Pilot.cpp
# End Source File
# Begin Source File

SOURCE=.\pilot\Pilot.h
# End Source File
# Begin Source File

SOURCE=.\pilot\PilotList.cpp
# End Source File
# Begin Source File

SOURCE=.\pilot\TeamPilot.cpp
# End Source File
# End Group
# Begin Group "cockpit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cockpit\Cockpit.cpp
# End Source File
# Begin Source File

SOURCE=.\cockpit\Cockpit.h
# End Source File
# End Group
# Begin Group "mission"

# PROP Default_Filter "*"
# Begin Source File

SOURCE=.\mission\MapLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MapLoader.h
# End Source File
# Begin Source File

SOURCE=.\mission\Mission.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\Mission.h
# End Source File
# Begin Source File

SOURCE=.\mission\MissionAirBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionBase.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionCanyon1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionCanyon2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionCanyon3.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionConvoy.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionConvoy2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDeathMatch1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDeathMatch2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDeathMatch3.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDefend1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDemo1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDepot.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDogfight1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDogfight2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionDogfight3.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionFreeFlight1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionGround1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionMoonBase1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionMoonBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionMoonDefense1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionMoonDogfight1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionMultiDogfight1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionScout.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionShip1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionShip2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionShip3.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTank1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTeamBase1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTest1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTest2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTransport.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTunnel1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTutorial1.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTutorial2.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionTutorial3.cpp
# End Source File
# Begin Source File

SOURCE=.\mission\MissionWaves1.cpp
# End Source File
# End Group
# Begin Group "aiobject"

# PROP Default_Filter "*"
# Begin Source File

SOURCE=.\aiobject\AiObj.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\AiObj.h
# End Source File
# Begin Source File

SOURCE=.\aiobject\AiObjPrototype.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\DynamicObj.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\DynamicObjPrototype.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\Fighter.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\FighterPrototype.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\Missile.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\Ship.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\StaticAa.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\StaticPassive.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\Tank.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\UnitDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\UnitDescriptorRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\aiobject\UnitPrototype.h
# End Source File
# End Group
# Begin Group "game"

# PROP Default_Filter "*"
# Begin Source File

SOURCE=.\game\Callbacks.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Callbacks.h
# End Source File
# Begin Source File

SOURCE=.\game\Events.h
# End Source File
# Begin Source File

SOURCE=.\game\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Game.h
# End Source File
# Begin Source File

SOURCE=.\game\GameState.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GameState.h
# End Source File
# Begin Source File

SOURCE=.\game\globals.h
# End Source File
# End Group
# Begin Group "render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\render\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\render\Render.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\util\StringUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\util\Util.h
# End Source File
# End Group
# End Target
# End Project
