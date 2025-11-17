# How to Install WAVs into your game

##  Index 
- ### [Introduction](#Introduction)
- ### [Plan](#Plan)
- ### [Tools](#Tools)
- ### [How To Modify](#How-To-Modify)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction
A lot of the guides for music/sound on FEUniverse focus on MIDI installation.
This makes sense, given the inexpensive nature of MIDI files from a storage perspective
and the relatively expensive nature of WAVs by comparison.

But while the GBA isn't terribly well known for its audio capabilities,
it is still possible to play voiced audio. Examples include the GBA video
catridges from back in the day. This is where WAVS shine, as they are the best
audio format for this task.

## Plan
This guide aims to focus on the installation of WAVs specifically, if you want to get
inventive with your game and add some voiced dialogue or some small snippets of songs.

## Tools
To get started you'll need the following:

- WAV2EA (For converting the WAV into something we can insert into buildfile)
- WAV2GBA ( should be included with the above)
- FEBuilder (For additional compression)
- A good ear for sound

WAVE2EA - https://feuniverse.us/t/wav2ea-convert-wavs-to-a-ea-insertable-format/2686
FEBuilder - https://github.com/FEBuilderGBA/FEBuilderGBA/releases/tag/ver_20240521.05

## How to Modify

1) Grab your WAV from whatever source you like.

2) Navigate to FEBuilder's ``Song Track`` section in the menu, select a track section and click ``Import Music`` and use the following settings
    - Reduce Autio Quality - ``11025Hz - 22050Hz``
    - Remove Silence - ``1 = Remove Silence``
    - Channel - ``1 = 8bit mono``
    - Volume - ``200% - 300%``
    - DCPM Compression - ``1 - Compress, Lookahead - 3``

That last one requires that you install the ``m4a mixer`` via the patches menu section

You can select ``preview`` at any given time to check how the audio sounds

3) Now select ``import``. We don't bother with the loop setting as we'll be re-exporting
4) Click on the ``Instrument Set`` for the track you just entered
5) Now click on ``Export WAV Data`` and choose your filepath

Now you should have a WAV that's as small as reasonably possible and doesn't sound like total shit.

6) Now drag the WAV onto one of the two batch files (or Linux equivalents) in WAV2GBA to produce a ``BIN`` file
7) Now open WAV2EA and locate your bin, and fill in the textboxes (The ids for Replacing IDs can be found in Music List.txt, and for group IDs in Group List.txt)
8) Click save and this should be produce an installer in ``TXT`` format in the same folder
9) Copy paste ``MPlayDef.event`` in said folder along with the installer and WAV to your buildfile
10) One thing that you don't get told is that you need to append the name of your bin file to line 19 of the txt file where ``loop_sfx_mvl`` is. So if you called the file ``my_cool_track.bin`` then ``loop_sfx_mvl`` needs to become ``loop_sfx_mvl_my_cool_track`` or it won't install.
11) Install via event assembler aaaaaaand DONE!
12) Now you can call the track as either as sound effect or a music track using SOUN or MUSC (and the music list ID as a parameter) respectively.

## Limitations and Bugs

We use WAVs instead of MP3s as GBA hardware just isn't powerful enough to decode
MP3s in real time. If we could, we'd probably x3 or x4 the amount of storage
we have available at the same quality level. There are some decent compression
utilities out there like 8ad but they require the installation of a whole new
sound engine and getting that to play nice with Fire Emblem 8 is outside the
scope of this document.