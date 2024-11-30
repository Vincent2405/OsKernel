/*
CHATGPT GENERIERT
standart string funktionen
*/

char *strcpy(char *dest, const char *src) {
    char *original_dest = dest; // Speichere den Anfang von dest
    while ((*dest++ = *src++) != '\0'); // Kopiere Zeichen, inklusive Null-Terminierung
    return original_dest; // Gib den Anfang von dest zurück
}
char *strncpy(char *dest, const char *src, int n) {
    int i;

    // Kopiere bis zu n Zeichen von src nach dest
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Fülle den Rest von dest mit '\0', falls nötig
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}
int strcmp(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0') {
        if (*s1 != *s2) {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }
        s1++;
        s2++;
    }

    // Wenn ein String früher endet
    return (unsigned char)*s1 - (unsigned char)*s2;
}
int strlen(const char *str) {
    int length = 0;

    // Iteriere durch den String, bis das Nullterminierungszeichen erreicht wird
    while (str[length] != '\0') {
        length++;
    }

    return length;
}
void *memset(void *ptr, int value, int num) {
    unsigned char *p = ptr; // Arbeite mit einem Byte-Pointer
    while (num--) {
        *p++ = (unsigned char)value; // Setze jedes Byte auf den angegebenen Wert
    }
    return ptr; // Gib den ursprünglichen Zeiger zurück
}

void printf(char* str)
{
    write(1,str,100);
}