/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */  
    
#include <stdio.h>
#include <system.h>
#include <assert.h>
#include <nios2.h>
#include <io.h>
#include <rf212.h>
    
#define RF212_BASE (RF212_0_BASE + 0xE0000000)

{
	
	    //kprintf("rf212_int_handler\n");
	    uint32_t status;
	
	
	
		len = IORD(RF212_BASE, 0x80) & 0xFF;	//length
		++total;
		
		
		
			
		
		
		
		
			kprintf("[len=%d lqi=%02x ed=%02x] ", len - 3, lqi, ed);
			
				
			
		
			
			    ("recv tot=%d len=%d number=0x%x lqi=%02x ed=%02x\n",
			     total, len, *number, lqi, ed);
			
			    /*
			       for (i = 0; i < len; ++i)
			       kprintf("%02x ", (uint8_t)buf[i]);
			       kprintf("\n");
			     */ 
		}
		
	}



{
	
	
	
	do {
		
		
			
		
	
	



{
	
	
	
	
	do {
		
	
	



{
	
	

{
	
	    //kprintf("rf212_send len=%d first=0x%x\n", len, data[0]);
	    if (len > 125) {
		
		    ("rf212_send : len = %d must not be more than 125!!!\n");
		
	
	
	
	do {
		
	
	int i;
	
		
	
	
	
	


