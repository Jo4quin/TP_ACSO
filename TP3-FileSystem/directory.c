#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int directory_findname(struct unixfilesystem *fs, const char *name, int dirinumber, struct direntv6 *dirEnt) {
  if (fs != NULL && name != NULL && dirEnt != NULL && dirinumber >= 1) {
    struct inode dir_inode;
    if (inode_iget(fs, dirinumber, &dir_inode) >= 0) {  // No pudo obtener el inodo del directorio
      // Verifico que realmente sea un directorio
      if ((dir_inode.i_mode & IFMT) == IFDIR) {
        int filesize = inode_getsize(&dir_inode);
        int entries_per_block = DISKIMG_SECTOR_SIZE / sizeof(struct direntv6);
        int total_entries = filesize / sizeof(struct direntv6);
        for (int i = 0; i < total_entries; i += entries_per_block) {
            struct direntv6 block[entries_per_block];
            int bytes_read = file_getblock(fs, dirinumber, i / entries_per_block, block);

            if (bytes_read < 0) return -1;

            int entries_in_block = bytes_read / sizeof(struct direntv6);
            for (int j = 0; j < entries_in_block; j++) {
              // Comparar el nombre (14 chars)
              if (strncmp(block[j].d_name, name, 14) == 0) {
                *dirEnt = block[j]; // Copiar la entrada encontrada
                return 0;
              }
            }
        }
      }
    }
  } return -1; // No se encontró el nombre
}
