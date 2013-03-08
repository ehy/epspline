/**********************************************************************\
	Script for epspline MSW binary installer program.
	Copyright (C) 2013 Ed Hynan covering modifications and
	additions in this derivative source.  This is "derivative"
	in that it is based on example source that is included with 
	NSIS distribution.
	
	The licence for use of this source is an exception to the
	to the license generally covering the distribution that
	includes this source.  This source is covered by the same
	license covering the source from which it is derived, and
	of the '.nsh' sources that it includes with "!include";
	that license, current with this writing, is stated to be
	the "zlib/libpng license".  See:

		http://nsis.sourceforge.net/License

	That license covers both the '.in' form of the file, which is
	written to be processed with the POSIX 'm4' macro processor,
	and the output result of processing with m4.  (The '.m4' macro
	definitions file, "cdata.m4", is licensed under the GPLv2 or later,
	which must not be construed as altering the license covering the
	result of processing the input form of this file.)
\**********************************************************************/

# this is based on an NSIS example, which has the following comment:
;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg
#
# Note that the script language accepts comments introduced by
# semicolon or hash (and C-style comments that may span lines),
# so the ';' will be reserved for comments from the original example,
# and '#' used for new comments (and C comments to disable code, in
# particular original example code to be left in place for reference).
#

# Note that the multi-user setup follows the example at:
# http://nsis.sourceforge.net/Battle_for_Wesnoth

# MACROS to control variable attributes
  !define VERSION "0.0.4.2"
  !define INST_ROOT "Epspline 0.0.4.2"
  # EXEC_LVL: 'highest', 'admin', 'user'
  !define EXEC_LVL     highest
  !define SRC_ZIP      "epspline-0.0.4.2.zip"
  # name used in control panel add/remove reg key
  !define ADDREMNAME   "Epspline 0.0.4.2"
  # MSW registry H-key, e.g. HKCU or HKLM or multiuser SHCTX
  # (What's the "H" in HKLM, etc.? 'Hierarchy'?)
  !define _HK_         SHCTX
# MACROS end

  CRCCheck on

  RequestExecutionLevel ${EXEC_LVL}

  !define MULTIUSER_EXECUTIONLEVEL Highest
  !define MULTIUSER_MUI "1"
  !define MULTIUSER_INSTALLMODE_COMMANDLINE "1"
  !define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY \
	"Software\GPLFreeSoftwareApplications\epspline\0.0.4.2"
  !define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME ""
  !define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY \
	"Software\GPLFreeSoftwareApplications\epspline\0.0.4.2"
  !define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME ""
  !define MULTIUSER_INSTALLMODE_INSTDIR	"${INST_ROOT}"
 
  !include "MultiUser.nsh"

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

# ASSOC.: from FAQ, to make MS style file associations
# SEE: http://nsis.sourceforge.net/File_Association
# THE RIGHTS TO THE CODE IN THE FOLLOWING INCLUDED FILE
# ARE THOSE OF THE RIGHTS HOLDER(S) AND SHALL NOT BE DEEMED
# TO BE HELD BY THOSE THAT DO NOT HOLD THEM.
#!include "FileAssociation.nsh" ; renamed
  !include "file_association.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Epspline Prism and Lathe Editor 0.0.4.2"
  OutFile "Epspline-0.0.4.2-msw32-install.exe"

;--------------------------------
;Variables
  # orig. from example "StartMenu.nsi"
  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_WELCOMEFINISHPAGE_BITMAP chisel.bmp
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP lamp.bmp
  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "../LICENSE.gpl,v3"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MULTIUSER_PAGE_INSTALLMODE
  !insertmacro MUI_PAGE_DIRECTORY
  
  # orig. from example "StartMenu.nsi"
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "${_HK_}" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY \
	"Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  # End orig. from example "StartMenu.nsi"
  
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
 
  # uninstall pages
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English" ;first is the default
  #!insertmacro MUI_LANGUAGE "French"
  #!insertmacro MUI_LANGUAGE "German"
  #!insertmacro MUI_LANGUAGE "Spanish"
  # add more as clue-bulb illuminates
 
;--------------------------------
;Installer Sections

Section "Epspline program" SecExe

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  # Added EH:
  File "../src/epspline.exe"
  
  ;Store installation folder
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"" "$INSTDIR"
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installation directory" "$INSTDIR"
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed version string" "0.0.4.2"
  WriteRegDWORD ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed version integer" 1026
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed executable file" "$INSTDIR\epspline.exe"
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed uninstall program" "$INSTDIR\uninstall.exe"
  
  WriteUninstaller "$INSTDIR\uninstall.exe"

  # orig. from example "StartMenu.nsi"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
  ;Create shortcuts
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Epspline.lnk" \
  	"$INSTDIR\epspline.exe"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" \
  	"$INSTDIR\uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  # End orig. from example "StartMenu.nsi"

  # ASSOC.: from FAQ, to make MS style file associations
  ${registerExtension} "$INSTDIR\epspline.exe" \
	 ".pse" "Epspline Prism and Lathe Editor"
 
  # This should make an entry in MSW control panel "Add/Remove Software"
  # from example at:
  #		http://nsis.sourceforge.net/Add_uninstall_information_to_Add/Remove_Programs
  WriteRegStr ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}" \
     "DisplayName" "Epspline - Prism and Lathe Object Editor for POV-Ray"
  WriteRegStr ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}" \
     "Publisher" "GPLFreeSoftwareApplications"
  WriteRegStr ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}" \
     "DisplayVersion" "0.0.4.2"
  WriteRegDWORD ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}" \
     "NoModify" 1
  WriteRegDWORD ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}" \
     "NoRepair" 1
  WriteRegStr ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}" \
    "UninstallString" "$\"$INSTDIR\uninstall.exe$\""

SectionEnd

Section "Epspline help documents" SecDoc

  SetOutPath "$INSTDIR\Help Documents"
  
  File "../doc/help.zip"

  ;Store installation folder
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed help documents" "$INSTDIR\Help Documents\help.zip"
  
SectionEnd

Section "Epspline translations" SecI18n

  !include i18n-sect

SectionEnd

Section "Epspline source archive" SecSrc

  SetOutPath "$INSTDIR\Source Code"
  
  File "../../${SRC_ZIP}"

  ;Store installation folder
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed source archive" \
	"$INSTDIR\Source Code\${SRC_ZIP}"
  
SectionEnd

Section "Epspline examples" SecExa

  SetOutPath "$INSTDIR\Examples"
  
  # Hmphh... cannot use '*.*' as a glob pattern if it's meant
  # to really require a '.' -- apparently nsis takes it as a
  # token equivalent to '*', because it is including "Makefile".
  # So, will have to give extensions individually (still better
  # than each file individually).
  #File /a "../examples/*.*"
  File  "../examples/*.pse"
  File  "../examples/*.inc"
  File  "../examples/*.pov"
  File  "../examples/*.jpeg"

  ;Store installation folder
  WriteRegStr ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2" \
	"installed examples" \
    "$INSTDIR\Examples\(*.pse|*.inc|*.pov|*.jpeg)"
  
SectionEnd

;--------------------------------
;Installer Functions
 
Function .onInit
  !insertmacro MULTIUSER_INIT
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

# after successful install offer to start program,
# and notify MSW shell of .pse extension change.
# this is a callback hook on nsis internal event
Function .onInstSuccess
  # from discussion at:
  # http://forums.winamp.com/showthread.php?s=&threadid=140254
  System::Call \
	'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
  # query user w/ offer to start
  MessageBox MB_YESNO \
	"Would you like to start Epspline now?" \
	IDNO no_do_not_invoke_the_program
  Exec '"$INSTDIR\epspline.exe" "$INSTDIR\Examples\*.pse"'
  no_do_not_invoke_the_program:
FunctionEnd
# end func


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecExe ${LANG_ENGLISH} \
	"The main program executable file (.EXE)."
 
  LangString DESC_SecDoc ${LANG_ENGLISH} \
	"The program help files."

  LangString DESC_SecI18n ${LANG_ENGLISH} \
	"The program translation files (not all locales have translations)."

  LangString DESC_SecSrc ${LANG_ENGLISH} \
	"The program source code (optional; free under GPL license)."

  LangString DESC_SecExa ${LANG_ENGLISH} \
	"Examples for Epspline, including .pov files ready to render, and some small rendered .jpeg files."
 
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecExe} $(DESC_SecExe)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDoc} $(DESC_SecDoc)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecI18n} $(DESC_SecI18n)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSrc} $(DESC_SecSrc)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecExa} $(DESC_SecExa)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  # ASSOC.: from FAQ, to make MS style file associations
  ${unregisterExtension} ".pse" "Epspline Prism and Lathe Editor"

  Delete "$INSTDIR\Help Documents\help.zip"
  Delete "$INSTDIR\en\epspline.mo"
  Delete "$INSTDIR\en_US\epspline.mo"
  Delete "$INSTDIR\Source Code\${SRC_ZIP}"
  Delete "$INSTDIR\Examples\*.*"
  Delete "$INSTDIR\epspline.exe"
  Delete "$INSTDIR\uninstall.exe"

  RMDir "$INSTDIR\Help Documents"
  RMDir "$INSTDIR\en"
  RMDir "$INSTDIR\en_US"
  RMDir "$INSTDIR\Source Code"
  RMDir "$INSTDIR\Examples"
  RMDir "$INSTDIR"

  # orig. from example "StartMenu.nsi"
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Epspline.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  # End orig. from example "StartMenu.nsi"
  
  # Note next unconditional (no /ifempty) removal at version -- things
  # like window placement will be lost -- maybe revisit this in future
  DeleteRegKey          ${_HK_} "Software\GPLFreeSoftwareApplications\epspline\0.0.4.2"
  DeleteRegKey /ifempty ${_HK_} "Software\GPLFreeSoftwareApplications\epspline"
  DeleteRegKey /ifempty ${_HK_} "Software\GPLFreeSoftwareApplications"
  # this removes the control panel "Add/Remove" entry
  DeleteRegKey          ${_HK_} \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\${ADDREMNAME}"

SectionEnd
 
;--------------------------------
;Uninstaller Functions

Function un.onInit
  !insertmacro MULTIUSER_UNINIT
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

# after successful uninstall,
# and notify MSW shell of .pse extension change.
# this is a callback hook on nsis internal event
Function un.onUninstSuccess
  # from discussion at:
  # http://forums.winamp.com/showthread.php?s=&threadid=140254
  System::Call \
	'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
FunctionEnd
# end func

