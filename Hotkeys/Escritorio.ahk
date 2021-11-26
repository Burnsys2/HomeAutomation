#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

Hotkey, SC165,NodeRed
Hotkey, SC166,NodeRed
Hotkey, SC16B,NodeRed
Hotkey, SC16C,NodeRed
Hotkey, SC132,NodeRed
Hotkey, SC167,NodeRed
Hotkey, SC168,NodeRed
return

NodeRed:
	whr := ComObjCreate("WinHttp.WinHttpRequest.5.1")
	whr.Open("GET","http://nodered.burnsys.net.ar/hotkeys/escritorio/" . A_ThisHotkey, true)
	whr.Send()
	whr.WaitForResponse()

	return
