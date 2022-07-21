# Possible futures of Kråkfot

## Program architecture

Protocol based embedding, the protocol:

- the protocol should be fixed, and it should provide grammar
  rules as well as rules for buffer limitations,
- terms:
    - the **remote** is a program that provides functionality,
    - the **local** is kråkfot that provides graphics and GUI,
- the remote and the local could be running on the same or
  different computers,
    - the communication channel could be a pipe, a shared memory,
      a socket or a direct bluetooth channel,
- the protocol should be utf-8 textual and inspectable on both
  sides,
- should be able to:
    - *set up widgets* with an ID (as opposed to the *drawing*
      of VT220),
    - there should be a facility for *buffer mirroring*, suitable
      for text and image buffers, as well as Ajax-like input
      field suggestions,
    - the glyph system is hardcoded and unrelated to the OS
      "font" setup, the
    - the remote provides the extra glyphs to be used,

## GUI/GL glue

- sourceforge: [freeglut](http://freeglut.sourceforge.net/docs/api.php#Display) rather the past
- opengl.org: [GLUT-like Windowing, GUI, and Media Control toolkits](https://www.opengl.org/resources/libraries/windowtoolkits/)
    - [cpw](http://mathies.com/cpw/download.html) possible
- [glfw](https://www.glfw.org/docs/latest/index.html)
    - [glfw download](https://www.glfw.org/download.html)
    - github: [glfw](https://github.com/glfw/glfw)
    - [glad](https://glad.dav1d.de/)
        - github: [Dav1dde / glad](https://github.com/Dav1dde/glad)
        - [glad](https://gen.glad.sh/)
- [GLX](https://en.wikipedia.org/wiki/GLX)

## Fifo stuff

- [Named Pipe or FIFO with example C program](https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/)