[U] Anticheat
======
SA-MP server interface for United Army Anti-Cheat. <br />
Created by Whitetiger. <br />
The objective of this fork is to improve the server-side of the anticheat. <br />

Features
======
* In this fork the ban check is done only if the anticheat is enabled
* Callback OnACBanned called if the player is banned from the anticheat (#define CALL_CALLBACK_BAN to enable)

Requires
======
If you use Plugins version:
* [sscanf2](http://forum.sa-mp.com/showthread.php?t=120356)

Without plugins:
* [strlib](https://github.com/oscar-broman/strlib)

You should have:
* [YSI](https://github.com/Y-Less/YSI)

Warning
======
<b>This fork cant fix anticheat bugs (like detect bugs or something similar.)</b><br />
Unlike the original version [click here](https://github.com/Whitetigerswt/unitedac_server_api) may be less tested than the original.
This fork is only used to solve small problems.
