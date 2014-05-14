SA-MP server interface for Whitetiger's Anti-Cheat.

1. Requires
	* [strlib](https://github.com/oscar-broman/strlib)

2. Defines
	* _sampac_TIMERMODE 		; change the internal workings of AC_Toggle to check each player individually instead of every player in the same website query. this should be left as the default (0) due to strain on my website this has caused in the past.
	* _sampac_DEBUG			; A debug define used only in my internal testing, again this one should be left alone.

3. Adds the following functions
	* AC_Running(playerid);		; return true if playerid is running Whitetiger's Anti-Cheat
	* AC_HasTrainer(playerid);	; return true if playerid is detected as using a trainer
	* AC_ASI(playerid);		; return true if playerid is using a NOT ALLOWED .ASI script
	* AC_Toggle(bool:set)		; turn on and off automatic AC checking
	* AC_GetEnabled();		; return true if automatic AC checking is enabled. (every 30 or so seconds)
	* AC_GetInfo(playerid)		; update AC info for playerid
	* AC_GetAllInfo()		; update AC info for all connected players
	#if sampac_TIMERMODE == 1
	* AC_EnableForPlayer(playerid)	; enable automatic AC checking for a playerid only
	* AC_DisableForPlayer(playerid) ; disable automatic AC checking for a playerid only
	#endif

	all other functions are internal and should not be called!
	
4. Callbacks
	* OnACToggled(bool:set);	; called when AC_Toggle is called, indicating the new mode of automatic AC checking
	* OnACUpdated(playerid);	; the threaded return callback of AC_GetInfo(). this is the function where you should check the new values for AC_* functions.
	* OnUsingAnotherPC(playerid) 	; called when some flags come up and we think the playerid is using 2 PC same IP trick to evade AC
	* OnACFileModified(playerid, file[])	; called when a data file is modified.
