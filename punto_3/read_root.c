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
    unsigned char ext_boot_signaturel; //Byte 38.
    unsigned int volume_id;//Bytes 39-42.
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct {  
	unsigned char filename[8]; //Bytes 0-8.
    unsigned char file_extension[3]; 
    unsigned char file_attribute; //Byte 11.
    unsigned char reserved; //Byte 12. -----
    unsigned char file_creation_time; //Byte 13. (en decimas).
    unsigned short file_creation_time_hours; //Byte 14-15.
    unsigned short file_creation_day; //Byte 16-17.
    unsigned short file_accessed_day; //Byte 18-19. 
    unsigned short first_cluster_add; //Byte 20-21. 
    unsigned short written_time; //Byte 22-23.
    unsigned short written_day; //Byte 24-25.
    unsigned short first_cluster_add_2; //Byte 26-27.
    unsigned long file_size; //Bytes 28-31.
} __attribute((packed)) Fat12Entry;

unsigned int cluster_size,cluster_2_start;
unsigned short bytes_per_sector;

void show_dir_files(Fat12Entry *entry);

void print_dir_info(Fat12Entry *entry,int i) {

   switch(entry->filename[0]) {
        case 0x00:
            break; // unused entry
        case 0x0E5: // Completar los ...            
            printf("  --Archivo borrado: [?%.7s.%.3s]\n", entry->filename, entry->file_extension);// COMPLETAR
            break;
        default:
            switch(entry->file_attribute) {
                case 0x10:
                    printf("  --Directorio: [%.8s.%.3s], Cluster de inicio:%d \n", entry->filename, entry->file_extension,entry->first_cluster_add_2);
                    show_dir_files(entry);
                    break;
                case 0x20:
                    printf("  --Archivo:%s --Entrada: #%i,Cluster de inicio:%d\n",entry->filename,i,entry->first_cluster_add_2);
                    break;        
            }  
    
    }
            
}


void show_dir_files(Fat12Entry *entry){ //Muestro los archivos de un directorio
    Fat12Entry new_entry;
    FILE * in = fopen("test_punto3.img", "rb");
    unsigned int file_cluster_start=((entry->first_cluster_add_2-2)*cluster_size)+cluster_2_start;// -2 porque arranco a contar desde el Cluster #2;
    unsigned int file_cluster_end=file_cluster_start+cluster_size-1;
    int b;
    int index=1;

    fseek(in,file_cluster_start,SEEK_SET); 
    fread(&new_entry, sizeof(new_entry), 1, in);

     printf("  --Byte inicial: %i |Byte final: %i  \n",file_cluster_start,file_cluster_end);   

    for(b=file_cluster_start+32; b<=file_cluster_end; b+=32) {//En cada iteracion se mueve 32bytes, el tamanio de cada entrada;       
        fseek(in,b,SEEK_SET); 
        fread(&new_entry, sizeof(new_entry), 1, in);
        print_dir_info(&new_entry,index);
    }
    fclose(in);
}

void print_file_info(Fat12Entry *entry,int i) {

   switch(entry->filename[0]) {
        case 0x00:
            break; // unused entry
        case 0x0E5: // Completar los ...
            
            printf("Archivo borrado: [?%.7s.%.3s] --Entrada: #%i,Cluster de inicio:%d\n", entry->filename, entry->file_extension,i,entry->first_cluster_add_2);
            break;
        default:
            switch(entry->file_attribute) {
                case 0x10:
                    printf("Directorio: [%.8s.%.3s], Cluster de inicio:%d \n", entry->filename, entry->file_extension,entry->first_cluster_add_2);
                    show_dir_files(entry);
                    break;
                case 0x20:
                    printf("Archivo:%s --Entrada: #%i,Cluster de inicio:%d\n",entry->filename,i,entry->first_cluster_add_2);
                    break;        
        }
    
    }
              
}

int main() {
    FILE * in = fopen("test_punto3.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;
   
	//{...} Completar 
    fseek(in,446,SEEK_SET);
    fread(pt,sizeof(PartitionTable),4,in);

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
    fread(&bs,sizeof(Fat12BootSector),1,in);
    
    cluster_size=bs.sectors_per_cluster*bs.sector_size;
    bytes_per_sector=bs.sector_size;

    printf("En  0x%lX, sector size %d, FAT size %d sectors, %d FATs\n\n", 
           ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);

    int root_directory_start=((bs.fat_size_sectors*512)*bs.number_of_fats)+512; //Se le suma los 512 bytes del Boot Sector; 
   
    
    printf("Root dir_entries %d \n", bs.root_dir_entries);
    printf("Inicio del Root Directory: byte #%i\n",root_directory_start);

    unsigned int fats_size=(bs.sector_size * bs.fat_size_sectors) * bs.number_of_fats;
    cluster_2_start=fats_size+(32*bs.sector_size)+512;//El Root Directory, por definicion, ocupa 32 sectores;
    printf("Fin del Root Directory : byte #%i \n",cluster_2_start-1);

    for(i=0; i<bs.root_dir_entries; i++) {      
        fseek(in,root_directory_start+32*i,SEEK_SET); //En cada iteracion se mueve 32bytes, el tamanio de cada entrada;
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry,i);
    }

    fclose(in);
    return 0;
}
