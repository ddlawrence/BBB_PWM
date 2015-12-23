# BBB_PWM

README  Beaglebone Black  PWM Demo Program

It demonstrates PWM operation for RC-servos in poll mode.
Also GPIO under interrupt control (BOOT button & LED USR2).
It requires an RC-servo, get a sub-miniture one.
Do NOT power servos from the BBB!  You can splice into the barrel jack.

Main prog written in C and drivers/runtime services in GNU-Assembly.  
Makefile & loadscript provided for GCC :) in Win32 :( .  I use the XDS100V2 jtag 
to load programs.  It is more work up front, but much easier in the long run
and it is cheap, $70.  You can also boot from SDcard, see the makefile.  

It is all there in very concise format, documentation is within the code.  
So it should be easy for noobs to understand/test/hack/mod for your next 
BBB bare metal project requiring PWM control.  Suitable for embedded jobs.  

I included a MMC-bootable file you may test: rts.bin

Initial BBB-PWM driver hacked from Rodrigo Fagundez, e2e.ti.com. 

I am bringing up a bare metal system for the BBB.  Volunteer!  Get involved 
and hack more modules with me.  It seems hard at first, but a minimal system 
is available to help you get started.  You will learn the ARM architecture and 
peripherals.  Impress your friends (if you have any) and be a hero/ubergeek!  
PLUS you will have a FAST and SECURE platform for further hackjobs!  

You can provide feedback at www.baremetal.tech  
