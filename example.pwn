#include <a_samp>

#define _uac_PLUGINS 0

#include <unitedac_api>

#include <YSI\y_iterate>
#include <YSI\y_timers>
#include <YSI\y_commands>

#define COLOR_RED 	"{FF0000}"
#define COLOR_BLUE 	"{0000FF}"
#define COLOR_WHITE "{FFFFFF}"

new g_szOriginalHostname[80];

YCMD:actoggle(playerid, params[], help) {

	if(help) {
	    return SendClientMessage(playerid, -1, ""COLOR_RED"Help: "COLOR_WHITE"This command turns on or off United Anti-Cheat");
	}

	new str[128];
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	
	if(AC_GetEnabled() == true) {
	
	    AC_Toggle(false);
	    format(str, sizeof(str), "Admin "COLOR_RED"%s"COLOR_WHITE" Has disabled United Army Anti-Cheat", name);
	    
	} else {
	    AC_Toggle(true);
	    format(str, sizeof(str), "Admin "COLOR_RED"%s"COLOR_WHITE" Has enabled United Army Anti-Cheat", name);
	}

	SendClientMessageToAll(-1, str);
	
	return 1;
}

public OnPlayerConnect(playerid) {
	Command_SetPlayerNamed("actoggle", playerid, false);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success) {

	if(success) {
	    new IP[MAX_PLAYER_NAME];
	    foreach(new i : Player) {
			GetPlayerIp(i, IP, sizeof(IP));
			if(!strcmp(IP, ip, true)) {
			    Command_SetPlayerNamed("actoggle", i, true);
			    break;
			}
	    }
	}
	return 1;
}


public OnFilterScriptInit() {
	AC_Toggle(false);
	
	foreach(new i : Player) {
	    if(IsPlayerAdmin(i)) {
	        Command_SetPlayerNamed("actoggle", i, true);
	    }
	}
	
	GetServerVarAsString("hostname", g_szOriginalHostname, sizeof(g_szOriginalHostname));
	
	return 1;
}

public OnACUpdated(playerid) {
    
	if(!AC_Running(playerid))
	{
	    SendClientMessage(playerid, -1, ""COLOR_RED"Error: "COLOR_WHITE"You are not running the "COLOR_BLUE"United Anti-Cheat");
		defer DelayedKick(playerid, 0);
	}
	else if(AC_HasTrainer(playerid))
	{
	    SendClientMessage(playerid, -1, ""COLOR_RED"Error: "COLOR_WHITE"You are not allowed to run trainers on this server.");
		defer DelayedKick(playerid, 1);
	}
	else if(AC_ASI(playerid))
	{
	    SendClientMessage(playerid, -1, ""COLOR_RED"Error: "COLOR_WHITE"You are using .ASI scripts that are not allowed to be used on "COLOR_BLUE"United Anti-Cheat");
	    defer DelayedKick(playerid, 0);
	}
	
	return 1;
}

timer DelayedKick[1000](playerid, type) {
	if(type) BanEx(playerid, "[U] AC Ban");
	else Kick(playerid);
}

public OnACFileModified(playerid, file[]) {

	new string[100];
	format(string, sizeof(string), ""COLOR_RED"Error: "COLOR_WHITE"You have modified %s which is not allowed on this server.", file);
	
	
	SendClientMessage(playerid, -1, string);
	defer DelayedKick(playerid, 0);
	
	return 1;
}

public OnACToggled(bool:set) {

	new result[100];
	if(set) {
	    new hostname[80];
	    GetServerVarAsString("hostname", hostname, sizeof(hostname));
		format(result, sizeof(result), "hostname %s (AC)", hostname);
	} else {
	    format(result, sizeof(result), "hostname %s", g_szOriginalHostname);
	}
	
	SendRconCommand(result);
}
