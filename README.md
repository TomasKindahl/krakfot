# krakfot

## Purpose

Kråkfot /crawkfoot/ [krɔːkfuːt] intends to be a general purpose viewer / GUI for simple interaction with text and graphics based on OpenGL.

## Requirements

Requires OpenGL, GLU and glut, on Linux it is:

      apt get install libopengl-dev # probably automatic

and

      apt get install libglu1-mesa  # probably automatic

and

      apt get install freeglut3     # most certainly *not* automatic

## Cubic Bézier

Per wikipedia

<style> .green { color: green;}
</style>

**B**(t) = (1-t)<sup>3</sup>**P**<sub>0</sub>
+ 3(1-t)<sup>2</sup>t**P**<sub>1</sub>
+ 3(1-t)t<sup>2</sup>**P**<sub>2</sub>
+ t<sup>3</sup>**P**<sub>3</sub>, 0&le;t&le;1

<pre style="font-size:80%; background: white; color: #00C; padding: 0.4em;">
<b>typedef</b> <b>struct</b> _point_S {
    <b class="green">float</b> x, y;
} point;

point bezier(point P[4], <b class="green">float</b> frac) {
    point Q;
    <b class="green">float</b> ifrac = 1-frac;
    <b class="green">float</b> ifrac2 = ifrac*ifrac;
    <b class="green">float</b> frac2 = frac*frac;
    <b class="green">float</b> c0 = ifrac*ifrac2;
    <b class="green">float</b> c1 = 3*ifrac2*frac;
    <b class="green">float</b> c2 = 3*ifrac*frac2;
    <b class="green">float</b> c3 = frac*frac2;
    Q.x = c0*P[0].x + c1*P[1].x + c2*P[2].x + c3*P[3].x;
    Q.y = c0*P[0].y + c1*P[1].y + c2*P[2].y + c3*P[3].y;
    <b>return</b> Q;
}
</pre>

## Internal Encoding

[Unicode](https://www.unicode.org/charts/) doesn't do well with ligatures and variation usage, among other problems. The internal encoding is therefore something else. **ᛘᛦ** can therefore store text in its internal coding and in UTF-8 using variation selectors (U+E0100&mdash;U+E01EF).

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
