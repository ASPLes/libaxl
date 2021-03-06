; Script generated by the HM NIS Edit Script Wizard.

!include "axl_product_version.nsh"

; HM NIS Edit Wizard helper definesdot
!define PRODUCT_NAME "Axl Library for win${PLATFORM_BITS} (${PLATFORM_BITS} bits)"
!define PRODUCT_PUBLISHER "ASPL"
!define PRODUCT_WEB_SITE "http://xml.aspl.es"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "doc\axl-logo-48x48.ico"
!define MUI_UNICON "doc\axl-logo-48x48.ico"
; !define MUI_WELCOMEFINISHPAGE_BITMAP "vortex-lateral-image.bmp"
; !define MUI_UNWELCOMEFINISHPAGE_BITMAP "vortex-lateral-image.bmp"

; Welcome page
!define MUI_WELLCOMEPAGE_TITLE "$(PRODUCT_NAME): Open source XML toolkit $(PRODUCT_VERSION) installer"
!define MUI_WELCOMEPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_WELCOME

!insertmacro MUI_PAGE_LICENSE "COPYING"

!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!define MUI_WELCOMEPAGE_TITLE_3LINES
!insertmacro MUI_UNPAGE_WELCOME

!insertmacro MUI_UNPAGE_INSTFILES

!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_UNPAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "axl-installer-w${PLATFORM_BITS}-${PRODUCT_VERSION}.exe"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "Core Binaries" SEC01
  SectionIn RO
  SetOutPath "$INSTDIR"
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "src\libaxl.dll"
SectionEnd

Section "Namespace support" SEC011
  SetOutPath "$INSTDIR"
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "ns\libaxl-ns.dll"
SectionEnd

Section "Babel support (extended encoding support)" SEC012
  SetOutPath "$INSTDIR"
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "babel\libaxl-babel.dll"
SectionEnd

Section "Axl knife (command line tool)" SEC013
  SetOutPath "$INSTDIR"
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "knife\axl-knife.exe"
SectionEnd

Section "Test examples" SEC02
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer

  File "test\test_01.exe"
  File "test\*.xml"
  File "test\*.dtd"
  File "test\*.xdl"

SectionEnd


Section /o "Development Headers" SEC03
  SetOutPath "$INSTDIR\include"
  SetOverwrite ifnewer

  SetOutPath "$INSTDIR\include\axl"
  File "src\axl.h"
  File "src\axl_decl.h"
  File "src\axl_doc.h"
  File "src\axl_dtd.h"
  File "src\axl_error.h"
  File "src\axl_list.h"
  File "src\axl_log.h"
  File "src\axl_node.h"
  File "src\axl_stack.h"
  File "src\axl_stream.h"
  File "src\axl_factory.h"
  File "src\axl_hash.h"
  File "src\axl_config.h"
  File "ns\axl_ns.h"
  File "ns\axl_ns_node.h"
  File "ns\axl_ns_doc.h"
  File "babel\axl_babel.h"
SectionEnd

Section /o "Developement libs" SEC04
  SetOutPath "$INSTDIR\lib"
  SetOverwrite ifnewer
  
  File "src\libaxl.dll.a"
  File "src\libaxl.def"
  File "src\libaxl.exp"
  File "src\libaxl.lib"

  File "ns\libaxl-ns.dll.a"
  File "ns\libaxl-ns.def"
  File "ns\libaxl-ns.exp"
  File "ns\libaxl-ns.lib"

  File "babel\libaxl-babel.dll.a"
  File "babel\libaxl-babel.def"
  File "babel\libaxl-babel.exp"
  File "babel\libaxl-babel.lib"
SectionEnd

Section -AdditionalIcons
  ReadRegStr $R9 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" "Common Programs"
  StrCmp $R9 "" 0 ContinueInstallation
  MessageBox MB_OK "No se pudo obtener el directorio de instalación general, saliendo..."
  Abort ; or anything else...
  ContinueInstallation:
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  WriteIniStr "$INSTDIR\ASPL.url" "InternetShortcut" "URL" "http://www.aspl.es"

  CreateDirectory "$R9\Axl Library"
  CreateShortCut "$R9\Axl Library\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$R9\Axl Library\ASPL.lnk" "$INSTDIR\ASPL.url"
  CreateShortCut "$R9\Axl Library\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"

  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) uninstall process have finished properly."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to uninstall $(^Name)?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ReadRegStr $R9 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" "Common Programs"
  StrCmp $R9 "" 0 ContinueInstallation
 ContinueInstallation:

  Delete "$R9\Axl Library\Website.lnk"
  Delete "$R9\Axl Library\ASPL.lnk"
  Delete "$R9\Axl Library\Uninstall.lnk"

  RMDir "$R9\Axl Library"
  RMDir /r "$INSTDIR"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd

LangString DESC_SEC01 ${LANG_ENGLISH} "Includes minimal core DLL binaries installation for Axl Library."

LangString DESC_SEC011 ${LANG_ENGLISH} "Includes minimal core DLL binaries for Axl Library XML Namespaces support."

LangString DESC_SEC012 ${LANG_ENGLISH} "Includes minimal core DLL binaries for Axl Library BABEL support."

LangString DESC_SEC013 ${LANG_ENGLISH} "Includes axl-knife tool (the command line tool built on top of Axl Library)."

LangString DESC_SEC02 ${LANG_ENGLISH} "Includes several test examples, XML files and DTD files. "

LangString DESC_SEC03 ${LANG_ENGLISH} "Includes all development headers (*.h) required by compilers."

LangString DESC_SEC04 ${LANG_ENGLISH} "Includes all development libraries (import library)."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} $(DESC_SEC01)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC011} $(DESC_SEC011)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC012} $(DESC_SEC012)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC013} $(DESC_SEC013)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} $(DESC_SEC02)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} $(DESC_SEC03)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} $(DESC_SEC04)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
