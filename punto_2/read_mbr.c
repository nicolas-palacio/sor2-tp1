#include <stdio.h>
#include <stdlib.h>

void is_bootable(unsigned int first_byte){
    int bit7=(first_byte >> 7) & 1;
    if(bit7==1){
        printf(" La particion es booteable \n");
    }else{
        printf(" La particion no es booteable\n");
    }
}


int main() {
    FILE * in = fopen("mbr.bin", "rb");
    unsigned int i,first_byte, start_sector, length_sectors; //Variables hexa
    
    fseek(in,446,SEEK_SET); // Voy al inicio. Completar donde dice ...
    
    for(i=0; i<4; i++) { // Leo las entradas
        first_byte=fgetc(in);
        printf("#Partition entry %d: \n First byte %02X\n", i+1,first_byte);
        is_bootable(first_byte);
        printf("  Comienzo de partición en CHS: %02X:%02X:%02X\n", fgetc(in), fgetc(in), fgetc(in));
        printf("  Partition type 0x%02X\n", fgetc(in));
        printf("  Fin de partición en CHS: %02X:%02X:%02X\n", fgetc(in), fgetc(in), fgetc(in));
      
        fread(&start_sector, 4, 1, in);
        fread(&length_sectors, 4, 1, in);
        printf("  Dirección LBA relativa 0x%08X, de tamaño en sectores %d\n", start_sector, length_sectors);
    }
    
    fclose(in);
    return 0;
}



