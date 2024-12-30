# Notes for a communication protocol

## Operations needed

### Primitive operations

- request a drawing area
- go to an xy position
- at the xy position define a "box"
- set the coordinate system in the current box
- rotate the current box with &#x3BD; (nu) degrees
- rotate the contents of the current box with &#x3BD; (nu) degrees
- (at the xy position; in the box) write a text
- (at the xy position; in the box) draw a triangle
- (at the xy position; in the box) draw a line sequence

### Derived operations

These are only theoretically derived, and may be implemented as primitive:

- (at the xy position; in the box) draw a rectangle
- rotate the current box and its contents with &#x3BD; (nu) degrees
- (at the xy position; in the box) draw a line sequence including bezier
  adaptions
