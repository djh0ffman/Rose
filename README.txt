Rose - a programming language for temporal turtle graphics


INTRODUCTION

A Rose program produces an animation drawn by a number of "turtles"
moving around on a canvas. Each turtle has the following state:

- Position (x and y)
- Direction
- Pen size
- Color index, called "tint"
- Random number generator state.

Unless specific otherwise, all numeric values in Rose are 16.16 bits
fixed-point values.

The position and direction are available as global variables x, y and dir.


SYNTAX

Rose is whitespace-agnostic: line breaks can be inserted or omitted anywhere
between tokens.

Comments start with # and extend to the end of the line.

Just for the fun of it, and because I enjoy using a thesaurus, all keywords
in Rose have four letters.

A Rose program starts with an optional size directive, specifying the width
and height (respectively) in pixels:

- size <expression> <expression>

If the size is not specified, the resolution will be 352x280 (full overscan).

Next is an optional layer configuration, specifying the number of layers and
the number of colors in each layer (respectively):

- form <expression> <expression>

The tints are distributed over the layers, so if, for instance, there are 4
colors per layer, tints 0-3 are in the first (backmost) layer, tints 4-7 are
in the next layer and so on.

If no layer configuration is specified, there is 1 layer with 4 colors.

After this comes the color script, which has the syntax:

- plan <event>*

where <event> is one of:

- wait <expression>
  Wait a number of frames. The expression cannot contain variables.
- <tint>:<color>
  Set the given tint (integer) to the given color (three-digit hex value).

The first tint in each layer except the first is transparent, showing the
layers behind it. Thus, the colors for these tints are ignored.

The rest of the program is a sequence of procedures. A procedure has the
syntax:

- proc <name> <param>* <statement>*

where <name> and <param> are identifiers, and <statement> is one of:

- move <expression>
  Move forward a number of pixels.
- jump <expression> <expression>
  Jump to a pixel coordinate position.
- turn <expression>
  Turn a number of 256-on-a-circle degrees clockwise.
- face <expression>
  Turn to a number of 256-on-a-circle degrees clockwise from horizontal right.
- size <expression>
  Set pen radius in pixels (rounded to nearest integer-and-a-half).
- tint <expression>
  Set the current tint (rounded down to an integer).
- draw
  Draw a circular dot at the current position.
- plot
  Draw a square dot at the current position.
- wait <expression>
  Wait a number of frames.
- fork <procedure> <expression>*
  Branch off a new turtle with a copy of this turtle's state, running the
  given procedure with the given arguments. The old turtle continues executing
  the rest of the current procedure in parallel.
- temp <variable> = <expression>
  Assign a value to a local variable.
- seed <expression>
  Seed the random number generator.
- when <expression> <statement>* done
  Run the block of statements if the expression evaluates to non-zero.
- when <expression> <statement>* else <statement>* done
  Run the first block of statements if the expression evaluates to non-zero,
  or the second block of statements otherwise.
- defy
  Suppress all warnings for the current source line.

An <expression> is one of:

- <digit>+[.<digit>+]
  Constant value.
- $<hexdigit>+
  Constant value in hexadecimal. The value is used directly as the
  32-bit fixed-point representation, i.e. the last four digits are
  implicitly fractional.
- <name>
  Procedure or value of parameter, temporary variable or global variable.
- <expression> op <expression>
  Operation, where op is one of (*, /, +, -, ==, !=, <, <=, >, >=, &, |).
  Operator semantics and precedence are as in C, except * and / which do
  fixed-point adjustment to match fixed-point multiplication and division.
- ~ <expression>
  Negate value. Used instead of - to enable airy, delimiter-sparse syntax.
- sine ( <expression> )
  Computes the sine of 2pi * argument.
- rand
  Produces a random number between 0 and 1.
- <expression> ? <expression> : <expression>
  If the first expression evaulates to non-zero, evaluate the second
  expression, otherwise evaluate the third expression.
- ( <expression> )
  For grouping.

Procedures can be assigned to temporaries and passed as arguments to
procedures. The procedure part of a fork statement can refer directly to
a procedure or to a parameter or local variable containing a procedure.

And just to mention it one more time, because this could be a common
pitfall: Remember to use ~ and not - for negation, including for
writing negative numbers!


THE VISUALIZER

Run the visualizer with these arguments:

rose <filename> [<scale>] [<framerate> [<music>]]

where <filename> is the name of a file containing a Rose program,
<scale> is a scale factor for the visualizer window (with an 'x' in
front, i.e. 'x3' for a scaling factor of 3), <framerate> is the frame
rate in frames per second (default 50) and <music> is a 16-bit stereo
WAV file containing the music to play to the animation.

The total number of frames for the animation will be inferred from
the length of the music, or set to 10000 if no music is specified.

The visualizer will continuously monitor the file and reload it whenever
its modification time changes.

Keyboard shortcuts:
- SPACE: start/stop animation.
- RIGHT/LEFT: Step one frame forward/backward.
- PGDOWN/PGUP: Step 50 frames forward/backward.
- BACKSPACE: Go back to frame animation was last started.
- HOME: Go to first frame.
- TAB: Toggle statistics overlay.
- ESCAPE: Quit the visualizer.

Clicking or holding the left mouse button sets the time proportionally to
the mouse X position within the window, up to the total number of frames.

