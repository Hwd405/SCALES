SCALES: A Spyro Palette Tool, by Hwd405
------------------------------------------------------------------------

Contained in this release:
* Scales.exe - the patching tool
* resources/ - a folder full of resources that can be used to build your Spyro skin
  * BMP files - default skins, to be edited by the user
  * PSD files - default skins split into separate layers for ease of use, Photoshop format
  * PDN files - default skins split into separate layers for ease of use, Paint.NET format
* dumpsxiso.exe - Lameguy64's iso extraction tool
* mkpsxiso.exe - Lameguy64's iso creation tool
* README.txt - this file
* CREDITS.txt - full credits
* CHANGELOG.txt - changelog

------------------------------------------------------------------------

* Use the example skins to create a new skin file. The skin file created must match the version of the game being used:
  * spyro-s1-u.bmp  - Spyro 1 NTSC-U
  * spyro-s1-ej.bmp - Spyro 1 PAL, NTSC-J
  * spyro-s2.bmp    - Spyro 2
  * spyro-s3.bmp    - Spyro 3
* Open Windows Explorer and create a directory for your reskinned version.
* Put the disc image files (e.g. bin/cue), the patcher exe, both dumpsxiso.exe and mkpsxiso.exe, and the desired skin file into a folder.
* In Windows Explorer, in the same directory as before, enter "cmd" into the address bar and press enter. A cmd window should appear.
* Run the following command, with each of the `$VARIABLES` replaced with their intended values:

  `.\Scales.exe -i "$SPYRO" -c "$COLOURS"`

  * $SPYRO - the .bin, .iso or .img file for the version of Spyro you'd like to patch.
  * $COLOURS - the .bmp skin file for Spyro. Note that the tool will not accept PSD or PDN files, so these must be saved as BMP files when they're ready.
* Wait for this to complete.

------------------------------------------------------------------------

Due to the nature of Spyro 2 and 3's PAL versions and the behaviour of dumpsxiso, I cannot confirm whether the file produced by using Spyro 2 or 3 PAL as a base will work as intended.
Note also that making a pixel completely black will result in that colour being completely transparent.
This tool does not currently update Spyro's scales in title screens or story cutscenes. At present, the tool only updates gameplay and in-game cutscenes.

Please enjoy the tool.
