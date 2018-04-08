# Shapes moving

Having a MFC SDI C++ Windows application project without any fancy user interface elements, draw a filled blue shape in the middle of the client area window (a square of 50 pixels diameter). 

When clicking the left mouse button inside the shape will switch between square and circle by every click. Also, the color should change between blue, red and green.

When clicking the right mouse button inside the shape, it starts moving inside the window or stops if it was moving before. The movement is diagonal (at 45 degrees) and the shape bounces away from the window’s borders, i.e. if it reaches the window’s border it changes the direction as if it would be bouncing away from a wall. Allow shape size (50 to 200 pixels) and speed (100 to 500 pixels per second) of the movement change by an interface Window, accessible by a new application menu.

Implement an additional command in the application menu which adds a further more moving shapes to the application, up to 4. These shapes should be able to move in the same way as one shape it does.
 