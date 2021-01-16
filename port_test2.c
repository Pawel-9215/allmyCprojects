#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/* Example of how to get a list of serial ports on the system.
 *
 * This example file is released to the public domain. */
 /* Helper function for error handling. */
int check(enum sp_return result);
 
int main(int argc, char **argv)
{
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
 
                printf("Found port: %s\n", port_name);
        }
 
        printf("Found %d ports.\n", i);
        
        printf("So the only port I should see now is arduino\n");
        printf("Arduino port: %s\n", sp_get_port_name(port_list[0]));
        
        printf("Trying to open the port\n");
        
        check(sp_open(port_list[0], SP_MODE_READ));
        check(sp_set_baudrate(port_list[0], 9600));
        check(sp_set_bits(port_list[0], 8));
        check(sp_set_parity(port_list[0], SP_PARITY_NONE));
        check(sp_set_stopbits(port_list[0], 1));
        check(sp_set_flowcontrol(port_list[0], SP_FLOWCONTROL_NONE));
        
        //reciving_port
        
        struct sp_port *rx_port = port_list[0];
        
        unsigned int timeout = 10000;
        
        //int result;
        for(int i=0; i<=20; i++)
        {
        
			printf("Trying to recieve data\n");
			int size = strlen("Hello from Arduino");
			int *buf = malloc(size+1);
			
			result = check(sp_blocking_read_next(rx_port, buf, size, timeout));
			
			buf[result] = '\0';
			printf("recieved: %d\n", *buf);
			free(buf);
			
        }
        printf("closing port");
        check(sp_close(port_list[0]));
        
 
        printf("Freeing port list.\n");
 
        /* Free the array created by sp_list_ports(). */
        sp_free_port_list(port_list);
 
        /* Note that this will also free all the sp_port structures
         * it points to. If you want to keep one of them (e.g. to
         * use that port in the rest of your program), take a copy
         * of it first using sp_copy_port(). */
 
        return 0;
}

/* Helper function for error handling. */
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
