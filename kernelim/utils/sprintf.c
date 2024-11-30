#include "uart.h"
#include "../heap.h"
#include "fd.h"
/*chatGPT generatet weil ich hatte keinen bock das string formatting selbers zu machen*/


void int_to_string(long num, char *str) {
    char temp[20]; // Temporärer Puffer für bis zu 20 Zeichen (z. B. für 64-Bit-Integer)
    int i = 0;
    int is_negative = 0;

    // Prüfe, ob die Zahl negativ ist
    if (num < 0) {
        is_negative = 1;
        num = -num; // Mache die Zahl positiv
    }

    // Extrahiere die Ziffern
    do {
        temp[i++] = (num % 10) + '0'; // Modulo gibt die letzte Ziffer, + '0' konvertiert sie zu einem Zeichen
        num /= 10;                   // Entferne die letzte Ziffer
    } while (num > 0);

    // Füge das negative Vorzeichen hinzu
    if (is_negative) {
        temp[i++] = '-';
    }

    // Drehe die Ziffern um und kopiere sie ins Ergebnis
    int j = 0;
    while (i > 0) {
        str[j++] = temp[--i];
    }

    str[j] = '\0'; // Null-Terminierung
}


//kann bist jetzt nur %s und %d mit max 4 argumenten
unsigned int sprintf2(char *dst, const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3) {
    // Zeiger auf die Register-Argumente
    unsigned long args[4] = {arg0, arg1, arg2, arg3};

    // Argument-Index
    unsigned int arg_index = 0;
    unsigned int length = 0;

    // Format-String verarbeiten
    while (*fmt != '\0') {
        if (*fmt == '%' && *(fmt + 1) == 's') {
            // String-Argument erwartet
            char *str = (char *)args[arg_index++]; // Nächstes Argument holen
            while (*str != '\0') {
                *dst++ = *str++;
                length++;
            }
            fmt += 2; // Überspringe "%s"
        } else if (*fmt == '%' && *(fmt + 1) == 'd') {
            // Integer-Argument erwartet
            char temp[20]; // Puffer für int_to_string
            int_to_string((long)args[arg_index++], temp);
            char *str = temp;
            while (*str != '\0') {
                *dst++ = *str++;
                length++;
            }
            fmt += 2; // Überspringe "%d"
        } else {
            *dst++ = *fmt++; // Kopiere das Zeichen direkt
            length++;
        }
    }
    *dst = '\0'; // Null-Terminierung
    return length;
}


unsigned int calculateStringLengt( const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3) {
    // Zeiger auf die Register-Argumente
    unsigned long args[4] = {arg0, arg1, arg2, arg3};

    // Argument-Index
    unsigned int arg_index = 0;
    unsigned int length = 0;

    // Format-String verarbeiten
    while (*fmt != '\0') {
        if (*fmt == '%' && *(fmt + 1) == 's') {
            // String-Argument erwartet
            char *str = (char *)args[arg_index++]; // Nächstes Argument holen
            while (*str != '\0') {
                *str++;
                length++;
            }
            fmt += 2; // Überspringe "%s"
        } else if (*fmt == '%' && *(fmt + 1) == 'd') {
            // Integer-Argument erwartet
            char temp[20]; // Puffer für int_to_string
            int_to_string((long)args[arg_index++], temp);
            char *str = temp;
            while (*str != '\0') {
                *str++;
                length++;
            }
            fmt += 2; // Überspringe "%d"
        } else {
            *fmt++; // Kopiere das Zeichen direkt
            length++;
        }
    }
     // Null-Terminierung
    return length+1;
}

//null terminated string
void printf(char* fmt,unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3)
{
    char* curr=fmt;
    int lenght=0;
    //calculate lenght of whole string
    lenght =calculateStringLengt(fmt,arg0,arg1,arg2,arg3);

    //allokiere speicher für den printM
    char* dest= kmalloc(lenght);
    sprintf2(dest,fmt,arg0,arg1,arg2,arg3);
    //write to stdOut
    //sprintf2(dest,fmt,arg0,arg1,arg2,arg3);
    kwrite(1,dest,lenght);
    //kprint(dest,lenght);
}