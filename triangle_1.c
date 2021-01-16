#include <simple2d.h>

void interface();
void button_box(int x, int y);

int screen_x = 800, screen_y = 600;

void render() {
  interface();
}

int main() {

  S2D_Window *window = S2D_CreateWindow(
    "Control Shift Register", screen_x, screen_y, NULL, render, 0
  );

  S2D_Show(window);
  return 0;
}

void button_box(int x, int y)
{
	int size = 20;
	
	S2D_DrawQuad(x-size, y-size, 1, 1, 1, 1,
				x+size, y-size, 1, 1, 1, 1,
				x+size, y+size, 1, 1, 1, 1,
				x-size, y+size, 1, 1, 1, 1);
}
void interface()
{	
	
	button_box(60, 40);
}
