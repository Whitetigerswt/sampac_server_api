/*

	Example usage: https://u-army.com/tiger/script/ac2.txt

*/


#include <a_samp>
#include <a_http>

#if defined _unitedac_api_included
	#endinput
#endif
#define _unitedac_api_included

#if !defined _uac_PLUGINS

#define _uac_PLUGINS 0


#endif

#define _uac_DEBUG 0

#if _uac_PLUGINS == 1

#include <sscanf2>
#define QUERY_URL 			"u-army.com/AC_API/pInfo.php"
#define UPDATE_URL          "u-army.com/AC_API/update.php"

#else

#include <strlib>
#define QUERY_URL 			"u-army.com/AC_API/pInfo_.php"
#define UPDATE_URL          "u-army.com/AC_API/update_.php"

#endif

#define BAN_URL             "u-army.com/AC_API/ban.php"

// in hindsight, pretty stupid to have different PHP scripts for with and without plugin, have to keep them now compatibility, can't be FUCKED to change it either.
// (why is there even options for with and without plugin, anyway?)

#include <YSI\y_iterate>
#include <YSI\y_bit>
#include <YSI\y_timers>
#include <YSI\y_hooks>

#define VERSION 1.5

#define THREAD_OFFSET 100
#define FILES 31

#if THREAD_OFFSET > MAX_PLAYERS
	#undef THREAD_OFFSET
	#define THREAD_OFFSET MAX_PLAYERS
#endif

new Timer:AC_Update;

new BitArray:ACRunning<MAX_PLAYERS>;
new BitArray:HasASI<MAX_PLAYERS>;
new BitArray:HasTrainer<MAX_PLAYERS>;
new BitArray:ACOn<1>;
new BitArray:checkedUpdate<1>;

new bool:Files[MAX_PLAYERS][FILES];

stock bool:AC_Toggle(bool:set);

new fileNames[FILES][50] = {
	!"anim/ped.ifp",             // file_0
    !"data/carmods.dat",         // file_1
    !"data/animgrp.dat",         // file_2
    !"data/melee.dat",           // file_3
    !"data/clothes.dat",         // file_4
    !"data/object.dat",          // file_5
    !"data/default.dat",         // file_6
    !"data/surface.dat",         // file_7
    !"data/default.ide",         // file_8
    !"data/gta.dat",             // file_9
    !"data/surfinfo.dat",        // file_10
    !"data/peds.ide",            // file_11
    !"data/vehicles.ide",        // file_12
    !"data/water.dat",           // file_13
    !"data/txdcut.ide",          // file_14
    !"data/water1.dat",          // file_15
    !"models/coll/weapons.col",  // file_16
    !"data/plants.dat",          // file_17
    !"data/furnitur.dat",        // file_18
    !"data/procobj.dat",         // file_19
    !"data/handling.cfg",        // file_20
    !"models/coll/peds.col",     // file_21
    !"models/coll/vehicles.col", // file_22
    !"samp.saa",                 // file_23
    !"samp.exe",                 // file_24
    !"vorbis.dll",               // file_25
    !"data/pedstats.dat",        // file_26
    !"data/ar_stats.dat",        // file_27
    !"vorbisfile.dll",           // file_28
    !"gta_sa.exe",               // file_29
    !"samp.dll"                  // file_30
    //                              ^^ internal file names for AC

};


hook OnFilterScriptInit()
{
	if(Bit_Get(checkedUpdate, 0) == false) {
		HTTP(THREAD_OFFSET, HTTP_GET, UPDATE_URL, "", "UpdateResponse");
		Bit_Let(checkedUpdate, 0);
		AC_Update = repeat AC_MainUpdate();
		Bit_Vet(ACOn, 0);
	}
	return 1;
}

hook OnGameModeInit() {
    if(Bit_Get(checkedUpdate, 0) == false) {
		HTTP(THREAD_OFFSET, HTTP_GET, UPDATE_URL, "", "UpdateResponse");
		Bit_Let(checkedUpdate, 0);
		AC_Update = repeat AC_MainUpdate();
		Bit_Vet(ACOn, 0);
	}
	return 1;
}

hook OnGameModeExit() {
	stop AC_Update;
	return 1;
}

hook OnFilterScriptExit() {
	stop AC_Update;
	return 1;
}


forward UpdateResponse(index, code, data[]);
public UpdateResponse(index, code, data[]) {
    if(code != 200) {
        HTTP(THREAD_OFFSET, HTTP_GET, UPDATE_URL, "", "UpdateResponse");
	    return printf("United Anti-cheat error: Failed to connect to website API");
	}
	#pragma unused index

	new Float:version = 0.0, url[128];

	#if _uac_PLUGINS == 0

	new str[5][128];
	strexplode(str, data, ",");
	version = floatstr(str[0]);
	url = str[1];

	if(!strlen(url) || version == 0.0) {
	    return printf("United AC Error: Failed to check for an update.");
	}

	#else

	if(sscanf(data, "fs[128]", version, url)) {
	    return printf("United AC Error: Failed to check for an update.");
	}

	#endif

	if(version > VERSION) {
	    printf("__________________________________________________");
	    printf("");
	    printf("** Your United Anti-Cheat API include is out of date, your old version may not continue to work.");
		printf("A new version is available at: %s", url);
		printf("");
		printf("__________________________________________________");

	} else if(!strcmp(url, "- Server not approved for Anti-Cheat usage.")) {
	    printf(data);
	    printf("__________________________________________________");
	    printf("");
	    printf("** Server not approved for [U]nited Anti-cheat Usage, Disabled.");
	    printf("");
	    printf("__________________________________________________");
	} else if(!strcmp(url, "Under Maintenance.")) {
	    printf("__________________________________________________");
	    printf("");
	    printf("** [U]nited Anti-Cheat is current under scheduled maintenance, it cannot be enabled at this time.");
	    printf("");
	    printf("__________________________________________________");
	    AC_Toggle(false);
	} else {
	    printf("__________________________________________________");
	    printf("");
	    printf("** United Army Anti-Cheat is on the most up-to-date version!");
	    printf("");
	    printf("__________________________________________________");
	}
	return 1;
}

hook OnPlayerConnect(playerid)
{
    new IP[MAX_PLAYER_NAME], postdata[50];
	GetPlayerIp(playerid, IP, sizeof(IP));

	#if _uac_DEBUG == 1
	if(!strcmp(IP, "127.0.0.1", true)) {
		IP = "";
	}
	#endif

	format(postdata, sizeof(postdata), "IP=%s", IP);
	
	#if _uac_DEBUG == 1
	printf(postdata);
	#endif
	 	
 	HTTP(playerid + THREAD_OFFSET + THREAD_OFFSET, HTTP_POST, BAN_URL, postdata, "BanResponse");
   	AC_GetInfo(playerid);
   	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {

	Bit_Vet(ACRunning, 	playerid);
	Bit_Vet(HasASI, 	playerid);
	Bit_Vet(HasTrainer, playerid);

	for(new i=0; i < FILES; ++i) {
	    Files[playerid][i] = true;
	}

	return 1;
}

timer AC_MainUpdate[120000]() {
	if(Bit_Get(ACOn, 0) == true) {
		foreach(new i : Player) {
		    AC_GetInfo(i);
		}
	} else {
		stop AC_Update;
	}
}

stock AC_GetInfo(playerid) {
	if(Bit_Get(ACOn, 0) == true) {
	    new IP[MAX_PLAYER_NAME], url[128], postdata[50];
		GetPlayerIp(playerid, IP, sizeof(IP));
		
		#if _uac_DEBUG == 1
		if(!strcmp(IP, "127.0.0.1", true)) {
			IP = "";
		}
		#endif

	 	format(url, sizeof(url), QUERY_URL);
	 	format(postdata, sizeof(postdata), "IP=%s", IP);
		HTTP(playerid + THREAD_OFFSET, HTTP_POST, url, postdata, "ACResponse");
	}
	return 1;
}

forward BanResponse(index, code, data[]);
public BanResponse(index, code, data[]) {
	new playerid = (index - THREAD_OFFSET) - THREAD_OFFSET;
	if(code != 200) {
	    new IP[MAX_PLAYER_NAME], postdata[50];
		GetPlayerIp(playerid, IP, sizeof(IP));

		#if _uac_DEBUG == 1
		if(!strcmp(IP, "127.0.0.1", true)) {
			IP = "";
		}
		#endif
		format(postdata, sizeof(postdata), "IP=%s", IP);

		HTTP(playerid + THREAD_OFFSET + THREAD_OFFSET, HTTP_POST, BAN_URL, postdata, "BanResponse");
		return printf("United Anti-cheat error: Failed to connect to website API");
	}
	
	if(strval(data) == 1) {
	    // 1 ms ? too low i think
	    SetTimerEx("KickPlayer", 5, 0, "d", playerid); // 1 ms timer fix so player who gets kicked actually recieves this message
	    // if the gamemode will be noticed of this kick might be better
	    new str[128], name[MAX_PLAYER_NAME];
	    GetPlayerName(playerid, name, sizeof(name));
	    format(str, sizeof(str), "{FF0000}%s{FFFFFF} has been kicked from the server for being banned from [U]nited Anti-Cheat", name);
	    SendClientMessageToAll(-1, str);
	}
	return 1;
}

forward KickPlayer(playerid);
public KickPlayer(playerid) {
	Kick(playerid);
}

forward ACResponse(index, code, data[]);
public ACResponse(index, code, data[]) {
    if(code != 200) {
        // try again?
        AC_GetInfo(index - THREAD_OFFSET);
	    return printf("United Anti-cheat error: Failed to connect to website API");
	}

	if(strlen(data) > 1) {
		
		if(!strcmp(data, "Under Maintenance")) {
		    printf("__________________________________________________");
		    printf("");
		    printf("** [U]nited Anti-Cheat is current under scheduled maintenance, it cannot be enabled at this time.");
		    printf("");
		    printf("__________________________________________________");
		    AC_Toggle(false);
		    return 1;
		}
		
		#if _uac_DEBUG == 1
		printf(data);
		#endif
		
	    #if _uac_PLUGINS == 1
	    
		new bool:acrunning, bool:hasasi, bool:hastrainer;
		sscanf(data, "p< >lllA<l>(false)["#FILES"]", acrunning, hasasi, hastrainer, Files[index - THREAD_OFFSET]);

		Bit_Set(ACRunning,  index - THREAD_OFFSET, acrunning);
		Bit_Set(HasASI, 	index - THREAD_OFFSET, hasasi);
		Bit_Set(HasTrainer, index - THREAD_OFFSET, hastrainer
		);
		#else
		

		new str[35][20];
		strexplode(str, data, ",");

        Bit_Set(ACRunning,  index - THREAD_OFFSET, strcmp(str[0], "false") == 0 ? false :
																							strcmp(str[0], "0") == 0 ? false : true );
		Bit_Set(HasASI, 	index - THREAD_OFFSET, strcmp(str[1], "false") == 0 ? false :
																							strcmp(str[1], "0") == 0 ? false : true );
		Bit_Set(HasTrainer, index - THREAD_OFFSET, strcmp(str[2], "false") == 0 ? false :
																							strcmp(str[2], "0") == 0 ? false : true );

		for(new i=0; i < FILES; ++i) {
			Files[index - THREAD_OFFSET][i] = strcmp(str[i+3], "false") == 0 ? false : strcmp(str[i+3], "0") == 0 ? false : true;
		}
		#endif


	} else {
		Bit_Vet(ACRunning, index - THREAD_OFFSET);
	}

	// check if they're still connected when the HTTP thread finished.
	if(IsPlayerConnected(index - THREAD_OFFSET)) {
		CallLocalFunction("OnACUpdated", "d", index - THREAD_OFFSET);

		
		for(new i=0; i < FILES; ++i) {
			if(Files[index - THREAD_OFFSET][i] == false && Bit_Get(ACRunning, index - THREAD_OFFSET) == true) {
			    // check if player is connected again (yes, in the loop), in case they got kicked or banned when OnACUpdated got called.
			    if(IsPlayerConnected(index - THREAD_OFFSET)) {
			    	CallLocalFunction("OnACFileModified", "ds", index - THREAD_OFFSET, fileNames[i]);
				}
    		}
		}
	}
	return 0;
}

// for some reason this function doesn't get called in other scripts when this include is included in any script
// something to do with YSI compatability since removing YSI fixes the problem.
// just hook it and do nothing and it will work again.

hook OnPlayerClickTextDraw(playerid, Text:clickedid) { }

#define AC_Running(%0) Bit_Get(ACRunning, %0)
#define AC_HasTrainer(%0) Bit_Get(HasTrainer, %0)
#define AC_ASI(%0) Bit_Get(HasASI, %0)
#define AC_GetEnabled() Bit_Get(ACOn, 0)

stock bool:AC_Toggle(bool:set) {
	Bit_Set(ACOn, 0, set);

	if(set) {
	    AC_MainUpdate();
	    AC_Update = repeat AC_MainUpdate();
	} else {
		stop AC_Update;
	}
	
	CallLocalFunction("OnACToggled", "d", set);
}

forward OnACToggled(bool:set);
forward OnACUpdated(playerid);
forward OnACFileModified(playerid, file[]);
