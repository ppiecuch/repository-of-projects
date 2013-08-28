; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSetMap
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GameEditor.h"

ClassCount=5
Class1=CGameEditorApp
Class2=CGameEditorDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_EDITOR
Resource2=IDR_MAINFRAME
Resource3=IDD_GAMEEDITOR_DIALOG
Class4=Ceditor
Resource4=IDD_ABOUTBOX
Class5=CSetMap
Resource5=IDD_DIALOG1

[CLS:CGameEditorApp]
Type=0
HeaderFile=GameEditor.h
ImplementationFile=GameEditor.cpp
Filter=N

[CLS:CGameEditorDlg]
Type=0
HeaderFile=GameEditorDlg.h
ImplementationFile=GameEditorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CGameEditorDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=GameEditorDlg.h
ImplementationFile=GameEditorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GAMEEDITOR_DIALOG]
Type=1
Class=CGameEditorDlg
ControlCount=11
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816
Control6=IDC_BUTTON5,button,1342242816
Control7=IDC_BUTTON6,button,1342242816
Control8=IDC_BUTTON7,button,1342242816
Control9=IDC_TAB1,SysTabControl32,1350565888
Control10=IDC_EDITO,button,1342177287
Control11=IDC_STATIC,button,1342177287

[CLS:Ceditor]
Type=0
HeaderFile=editor.h
ImplementationFile=editor.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO2

[DLG:IDD_EDITOR]
Type=1
Class=Ceditor
ControlCount=12
Control1=IDC_STATICSELECT,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_COMBO1,combobox,1344340226
Control5=IDC_STATIC,button,1342177287
Control6=IDC_PREVIEW,button,1342177287
Control7=IDC_COMBO2,combobox,1344340226
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT1,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT2,edit,1350631552
Control12=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG1]
Type=1
Class=CSetMap
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_BUTTON1,button,1342242816
Control7=IDC_BUTTON2,button,1342242816

[CLS:CSetMap]
Type=0
HeaderFile=SetMap.h
ImplementationFile=SetMap.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

