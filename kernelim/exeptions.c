#include "utils/uart.h"
#include "utils/armMakros.h"
#include "processes.h"
#include "utils/cpuContext.h"
#include "syscalls.h"
#include "exeptions.h"


void exc_handler(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far,unsigned long sp_el0)
{

    if(type==1){//SYNC exeption 
        if((esr>>26)==0b010101)//SVC erkannt
        { 
            int ret=syscallEntryPoint(type,esr,elr,spsr,far);
            if(ret!=0)
            {
                panic();
            }
            return;
            
        }else 
        {
            //kann data abort, undefined instruction oder irgendwas sein
            //falls page fault hole seite from disk =>sehr extendet aber hier TODO maybe irgendwann
            //panicking
            kprint("\nEXEPTION\n",20);
            kprint("type:",5);
            uart_print_hex(type);
            kprint("esr:",5);
            uart_print_hex(esr);
            kprint("elr:",5);
            uart_print_hex(elr);
            kprint("spsr:",5);
            uart_print_hex(spsr);
            kprint("far:",5);
            uart_print_hex(far);

            uart_print_hex(elr);
            uart_print_hex(esr);

            uart_print_hex(spsr);
            panic();
        }
    }
    if(type==2){//IRQ exeption
        //TODO schaue ob timer interrupt ist
        //werfe scheduler an und schedule den nächsten Process der in der scheduling liste ist
        scheduleNextTask();
        asm volatile ("tlbi vmalle1is");
        asm volatile ("dsb ish");
        asm volatile ("isb");
        return;
    }

    //table cache invalidierug
    asm volatile ("tlbi vmalle1is");
    asm volatile ("dsb ish");
    asm volatile ("isb");
    
    return;
}


void panic()
{
        kprint("panicking\n",10);
        while (1){}
}