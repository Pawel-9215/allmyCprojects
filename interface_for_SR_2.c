#include <simple2d.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>


S2D_Window *window;
void interface();
void button_box(int x, int y, int no);
bool overlap_check(int size, int x, int y);
void send_message(int no);
int message_sent = 0;

int check(enum sp_return result);
int chosen_port = 0;

struct sp_port *tx_port;
struct sp_port *rx_port;


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
	
	/* A pointer to a null-terminated array of pointers to
         * struct sp_port, which will contain the ports found.*/
        struct sp_port **port_list;
 
        printf("Getting port list.\n");
 
        /* Call sp_list_ports() to get the ports. The port_list
         * pointer will be updated to refer to the array created. */
        enum sp_return result = sp_list_ports(&port_list);
 
        if (result != SP_OK) {
                printf("sp_list_ports() failed!\n");
                return -1;
        }
 
        /* Iterate through the ports. When port_list[i] is NULL
         * this indicates the end of the list. */
        int i;
        for (i = 0; port_list[i] != NULL; i++) {
                struct sp_port *port = port_list[i];
 
                /* Get the name of the port. */
                char *port_name = sp_get_port_name(port);
 
                printf("Port number: %d: Found port: %s\n", i, port_name);
        }
 
        printf("Found %d ports.\n", i);
        
        printf("Choose port that Arduino is plugged to\n");
        scanf("%d", &chosen_port);
        
        printf("Trying to open the port %d\n", chosen_port);
        
        check(sp_open(port_list[chosen_port], SP_MODE_READ_WRITE));
        check(sp_set_baudrate(port_list[chosen_port], 9600));
        check(sp_set_bits(port_list[chosen_port], 8));
        check(sp_set_parity(port_list[chosen_port], SP_PARITY_NONE));
        check(sp_set_stopbits(port_list[chosen_port], 1));
        check(sp_set_flowcontrol(port_list[chosen_port], SP_FLOWCONTROL_NONE));
        
        //reciving_port
        
        tx_port = port_list[chosen_port];
        rx_port = port_list[chosen_port];

  window = S2D_CreateWindow(
    "Control Shift Register", screen_x, screen_y, update, render, 0
  );
  
  S2D_Show(window);
  return 0;
}

void button_box(int x, int y, int no)
{	
	int size = 40;
	float closed;
	bool overlap = overlap_check(size, x, y);
	
	if((overlap == true) && (message_sent != no))
	{
		message_sent = no;
		printf("Button overlapped: %d\n", no);
		send_message(no);
	}
	
	if(overlap == true)
	{
		closed = 1.0;
		S2D_DrawQuad(x-size, y-size*2, 1, 0.5, 1, 0.3,
					x+size, y-size*2, 1, 0.5, 1, 0.3,
					x+size, y+size*2, 1, 0.5, 1, 0.3,
					x-size, y+size*2, 1, 0.5, 1, 0.3);
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
		button_box(screen_x/9*i, screen_y/2, i*20);
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

void send_message(int no)
{
	int timeout = 1000;
	int payload = no;
	printf("Trying to send data\n");
	int result = check(sp_blocking_write(tx_port, &payload, 1, timeout));
	
	printf("Trying to recieve data\n");
	int size = strlen("Hello from Arduino");
	unsigned char *buf = malloc(size+1);
	
	result = check(sp_blocking_read_next(rx_port, buf, size, timeout));
	
	buf[result] = '\0';
	printf("recieved: %d\n", *buf);
	free(buf);
}

int check(enum sp_return result)
{
        /* For this example we'll just exit on any error by calling abort(). */
        char *error_message;
 
        switch (result) {
        case SP_ERR_ARG:
                printf("Error: Invalid argument.\n");
                abort();
        case SP_ERR_FAIL:
                error_message = sp_last_error_message();
                printf("Error: Failed: %s\n", error_message);
                sp_free_error_message(error_message);
                abort();
        case SP_ERR_SUPP:
                printf("Error: Not supported.\n");
                abort();
        case SP_ERR_MEM:
                printf("Error: Couldn't allocate memory.\n");
                abort();
        case SP_OK:
        default:
                return result;
        }
}
