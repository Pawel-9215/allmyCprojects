#include <simple2d.h>
#include <math.h>

S2D_Window *window;
void interface();
void button_box(int x, int y, int no);
bool overlap_check(int size, int x, int y);
int message_sent = 0;

int m_pos_x;
int m_pos_y;

int screen_x = 800, screen_y = 600;


void update()
{
	m_pos_x = window->mouse.x;
	m_pos_y = window->mouse.y;
	//printf("message %d\n", message_sent);
}

void render() {
  interface();
}

int main() {

  window = S2D_CreateWindow(
    "Control Shift Register", screen_x, screen_y, update, render, 0
  );
  
  S2D_Show(window);
  return 0;
}

void button_box(int x, int y, int no)
{	
	int size = 30;
	float closed;
	bool overlap = overlap_check(size, x, y);
	
	if((overlap == true) && (message_sent != no))
	{
		message_sent = no;
		printf("Button overlapped: %d\n", no);
		
	}
	
	if(overlap == true)
	{
		closed = 1.0;
	}
	else
	{
		closed = 0.6;
	}
	
	float distance = sqrtf(pow(x-m_pos_x, 2)+pow(y-m_pos_y, 2));
	float proximity;
	
	if(distance <= 300)
	{
		proximity = 1 - distance/300;
	}
	else
	{
		proximity = 0.0;
	}
		
	
	S2D_DrawQuad(x-size, y-size, proximity, proximity/2+0.5, 1, closed,
				x+size, y-size, proximity, proximity/2+0.5, 1, closed,
				x+size, y+size, proximity, proximity/2+0.5, 1, closed,
				x-size, y+size, proximity, proximity/2+0.5, 1, closed);
}
void interface()
{	
	//int a = 30;
	for(int i = 1; i<=8; i++)
	{
		button_box(screen_x/9*i, screen_y/2, i);
	}
	//button_box(m_pos_x, m_pos_y, 1);

}

bool overlap_check(int size, int x, int y)
{
	if((x-size <= m_pos_x) && (x+size >= m_pos_x) && (y-size <= m_pos_y) && (y+size >= m_pos_y))
	{	
		return true;
	}
	else
	{	
		return false;
	}
}
