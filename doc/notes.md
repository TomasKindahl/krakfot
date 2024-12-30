# Notes for development ideas

[Dir](dir.md) [Here](.)

<style>
  table, tr, td { border-collapse: collapse; padding: 0 4px; vertical-align: top; }
  span.wb { background: white; font-weight: bold; }
</style>

[TOC]

## Parser stuff

- [parser](parser.md) Parser stuff
- [C11 string literals](../../../C/ustringlit/dir.md) Characters' stuff

## Development plan

### Unprioritized (backlog)

#### Config

Open krakfot with a text config file: multiple windows, multiple terminals started. See [open thunar with multiple tabs in command line / Desktop / Xfce Forums](https://forum.xfce.org/viewtopic.php?id=12154) for bad example!

#### Network

|local                  |[Unix][Unix]      |description                                             |
|-----------------------|------------------|--------------------------------------------------------|
|                       |                  |option to communicating with a program through a network|
|<s>[pipes](pipes.md)</s>      |                  |GUI/program tiers communication by                      |
|pty                    |[eduterm][eduterm]|Also see [VT220][VT220]                                                       |
|[protocol](protocol.md)|                  |communication protocol                                  |
|                       |                  |data download (later: encrypted data download)          |
|                       |                  |data upload                                             |

[Unix]: ../../../Unix/dir.md
[eduterm]: ../../../Unix/eduterm/dir.md
[VT220]: /home/rursus/Documents/Länkar/Data/vt220.md

#### Authentication

- user authentication

#### Fonts and glyphs

- using alternate glyphs
- font editor

#### Text

#### Vague ideas about an improved markdown:

- [improved MD](improved-MD.md)

### Priority 1

<span class="wb">Glyphs:</span>

- numbers

<span class="wb">Load/reload:</span>

- load text files
- text reload
- text auto-reload

<span class="wb">Layout:</span>

- write from upper left corner instead of lower left

<span class="wb">Internal data:</span>

- glyph structure compaction (not 100 lines of paths with 100 nodes)

### DONE

- small letters
- glyph reload
- show texts at positions

### Abandoned

- ···(nothing yet)···
