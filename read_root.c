#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char starting_cluster[4];
    char file_size[4];
} __attribute((packed)) PartitionTable;

typedef struct { //Unsigned = hexa
    unsigned char jmp[3]; //Bytes 0-2
    char oem[8]; //Bytes 3-10
    unsigned short sector_size; //Bytes 11-12 Tam del sector, en nuestro caso 512 bytes. short = 2bytes. char= 1byte . long=4bytes.
	unsigned char sectors_per_cluster; //Byte 13
    unsigned short reserved_sectors; //Bytes 14-15, en nuestro caso 1.
    unsigned char number_of_fats; //Byte 16.Cant de FATs, en nuestro caso 2.
    unsigned short root_dir_entries; //Byte 17-18. Cant maxima de entradas de dir, en nuestro caso 512.
    unsigned short root_num_sectors; //Bytes 19-20.
    unsigned char media_type; //Byte 21.
    unsigned short fat_size_sectors; //Bytes 22-23, en nuestro caso 2 sectores= 2*512bytes=1024bytes.
    unsigned short sectors_per_track;//Bytes 24-25.
    unsigned short number_of_heads; //Bytes 26-27
    unsigned long number_of_sectors; //Bytes 28-31.
    unsigned long number_of_sectors_fs; //Bytes 32-35.
    unsigned char bios_int; //Byte 36.
    unsigned char not_used; //Byte 37.
    unsigned char ext_boot_signaturel //Byte 38.
    unsigned int volume_id;//Bytes 39-42.
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct {
	// {...} COMPLETAR
} __attribute((packed)) Fat12Entry;

void print_file_info(Fat12Entry *entry) {
    switch(entry->filename[0]) {
    case 0x00:
        return; // unused entry
    case ...: // Completar los ...
        printf("Archivo borrado: [?%.7s.%.3s]\n", // COMPLETAR
        return;
    case ...: // Completar los ...
        printf("Archivo que comienza con 0xE5: [%c%.7s.%.3s]\n", 0xE5, // COMPLETAR 
        break;
    case ...: // Completar los ...
        printf("Directorio: [%.8s.%.3s]\n", // COMPLETAR 
        break;
    default:
        printf("Archivo: [%.8s.%.3s]\n", // COMPLETAR 
    }
    
}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;
   
	//{...} Completar 
    
    for(i=0; i<4; i++) {        
        if(pt[i].partition_type == 1) {
            printf("Encontrada particion FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No encontrado filesystem FAT12, saliendo...\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_SET);
	//{...} Leo boot sector
    
    printf("En  0x%X, sector size %d, FAT size %d sectors, %d FATs\n\n", 
           ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);
           
    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) *
          bs.sector_size, SEEK_CUR);
    
    printf("Root dir_entries %d \n", bs.root_dir_entries);
    for(i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry);
    }
    
    printf("\nLeido Root directory, ahora en 0x%X\n", ftell(in));
    fclose(in);
    return 0;
}
