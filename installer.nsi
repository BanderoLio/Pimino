; NSIS Installer Script for Pimino
; Requires NSIS 3.0+
; Usage: makensis /DPRODUCT_VERSION="1.0.0" /DSTAGING_DIR="C:\path\to\staging" installer.nsi

!include "MUI2.nsh"
!include "FileFunc.nsh"

;--------------------------------
; General

!ifndef PRODUCT_VERSION
  !define PRODUCT_VERSION "1.0.0"
!endif

!ifndef STAGING_DIR
  !define STAGING_DIR "staging"
!endif

Name "Pimino"
OutFile "pimino-${PRODUCT_VERSION}-setup.exe"
Unicode True

; Default installation folder
InstallDir "$PROGRAMFILES64\Pimino"

; Get installation folder from registry if available
InstallDirRegKey HKCU "Software\Pimino" ""

; Request application privileges
RequestExecutionLevel admin

;--------------------------------
; Variables

Var StartMenuFolder

;--------------------------------
; Interface Settings

!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

;--------------------------------
; Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

; Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Pimino"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

;--------------------------------
; Installer Sections

Section "Pimino" SecMain
    SectionIn RO
    
    ; Copy all files from staging directory
    ; STAGING_DIR передается через /DSTAGING_DIR при вызове makensis
    SetOutPath "$INSTDIR"
    
    ; Копируем все содержимое staging директории
    ; Используем /x для исключения скрытых файлов и .git
    File /r /x ".git" /x "*.pdb" "${STAGING_DIR}\*.*"
    
    ; Store installation folder
    WriteRegStr HKCU "Software\Pimino" "" $INSTDIR
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Create shortcuts
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
        CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Pimino.lnk" "$INSTDIR\bin\pimino.exe"
        CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
    
    ; Add to Add/Remove Programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino" \
        "DisplayName" "Pimino"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino" \
        "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino" \
        "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino" \
        "DisplayVersion" "${PRODUCT_VERSION}"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino" \
        "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino" \
        "NoRepair" 1
SectionEnd

Section "Desktop Shortcut" SecDesktop
    CreateShortcut "$DESKTOP\Pimino.lnk" "$INSTDIR\bin\pimino.exe"
SectionEnd

;--------------------------------
; Descriptions

LangString DESC_SecMain ${LANG_ENGLISH} "Install Pimino application files."
LangString DESC_SecDesktop ${LANG_ENGLISH} "Create a desktop shortcut."

LangString DESC_SecMain ${LANG_RUSSIAN} "Установить файлы приложения Pimino."
LangString DESC_SecDesktop ${LANG_RUSSIAN} "Создать ярлык на рабочем столе."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} $(DESC_SecDesktop)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; Uninstaller Section

Section "Uninstall"
    ; Remove files
    RMDir /r "$INSTDIR"
    
    ; Remove shortcuts
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    RMDir /r "$SMPROGRAMS\$StartMenuFolder"
    Delete "$DESKTOP\Pimino.lnk"
    
    ; Remove registry keys
    DeleteRegKey HKCU "Software\Pimino"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pimino"
SectionEnd

