# VitaBackup

Purpose
--------------------------------------------------------------------------------
VitaBackup is a minimal, user friendly backup tool for the Playstation VITA. VitaBackup gives the user 100% flexibility on what they want to back up on their device. (Please look at editing backup config below). Currently the way it works is that it packs all the contents of a folder, or file to a single .tar file. After this is done, it adds the .tar file into a .zip file and compresses it if the user selects a level of compression. 

Current features
--------------------------------------------------------------------------------
- Backup multiple folders/files into archives.
- Restore multiple folders/files into archives.
- Ability to delete backups created by the software.
- Offer 11 levels of compression. (0 being no compression, which is usually recommeneded)
- Select backup location. (ur0:/ or ux0:/)

Editing Backup config to allow for more backups:
--------------------------------------------------------------------------------
VitaBackup gives the user full flexibility on what they want to backup. The user can decide on a name and path in the following format:
Modify the **path.txt** file in **ur0:/data/VitaBackup/path.txt** and add entries in the following format:

**[name]~[path]**

Example:
`Trophies~ux0:/user/00/trophy
Licenses~ux0:/license`

If it's a directory do not put a forward `/` at the end of the path.

Credits:
--------------------------------------------------------------------------------

- Berion for all the graphics and design used in this program.
- richgel999 for miniz.
- rxi for microtar.
