# Kråkfot

## Purpose

Kråkfot /crawkfoot/ [krɔːkfuːt] intends to be a general purpose viewer / GUI for
simple interaction with text and graphics based on OpenGL. Explicit goals are to
fulfill the functionality of:

1. markdown editor
2. file browser
3. graphical viewer
4. gloss exercise tool

Kråkfot is based on a perfectly independent stroke font system, since Truetype
and Opentype font system do everything to be hard to configure and new
mathematical symbols is one of the purposes of this GUI viewer.

## Requirements

Requires OpenGL, GLU and glut, on Linux it is:

      apt get install libopengl-dev # probably automatic

and

      apt get install libglu1-mesa  # probably automatic

and

      apt get install freeglut3     # most certainly *not* automatic

## Technical documentation

- [Cubic Bézier](doc/cubic-bezier.md)
- [Parser](doc/parser.md)

## Internal Encoding

[Unicode](https://www.unicode.org/charts/) doesn't do well with ligatures and variation usage, among other problems. The internal encoding is therefore something else. **Kråkfot** can therefore store text in its internal coding and in UTF-8 using variation selectors (U+E0100&mdash;U+E01EF).

Problems with Unicode:

* what's uppercase or lowercase depends on language encoding
  that isn't encodable within Unicode. Example:
    * Turkish claims that İi Iı are uppercase/lowercase
      pairs, while most other non-Turkish languages regard
      Ii as the sole uppercase/lowercase pair and İ and ı
      are just variations of I and i.
* Chinese is in reality a glyph collation system, not a
  character system; glyph variants in Unicode are regulated
  by locale and other stuff that pushes the semantics aside.
* Unicode pushes aside private characters to private fonts,
  which is incredibly inconvenient.

## Possible futures of Kråkfot

- [Future](doc/future.md)

## Notes

- [Notes](doc/notes.md)
