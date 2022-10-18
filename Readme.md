# Simple Paint for BMP format files
This project deals with simple operations such as drawing a line, rectangle, filling in a zone, on files that have the ".bmp" format.

  The general manner in which the code runs is the user uploads a photo having the ".bmp" extension and then the program receives commands from the user, executes them and then stops when receiving the "exit" command.
  Available commands :
- "edit" : loads the given image in a matrix, corresponding to the current workspace, with attention to padding
- "save" : display the image stored in the matrix to an output file
- "insert" : inserts an image over the current stored image, at given coordinates
- "set" : sets the color and width of your brush (with which you draw lines and fill in zones)
- "touch" : draws a point on the image, according to the type of brush you are using (with the already set color and width)
- "draw line" : given 2 sets of coordinates, draws a line from one point to another
- "draw rectangle" : given a set of coordinates and a length for width and height, draws a rectangle with those points as top left corner and given dimensions
- "fill" : given a set of coordinates, fills the adjecent pixels in a recursive manner, as long as they originally have the same color as the chosen starting point

(to mention that the programm can also take the command "draw triangle", but since the user is required to input 3 sets of coordinates, we use the function "draw_line" to create said triangle)

  Any other command given to the program is interpreted as "exit", at which point it frees the memory used and closes the program.
