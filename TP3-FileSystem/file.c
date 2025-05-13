#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "file.h"
#include "inode.h"
#include "diskimg.h"

int file_getblock(struct unixfilesystem *fs, int inumber, int blockNum, void *buf) {
    if (fs != NULL && buf != NULL && inumber >= 1 && blockNum >= 0){
        struct inode in;
        if (inode_iget(fs, inumber, &in) >= 0){                         // Caso que no se pueda obtener el inodo
            int sectorNum = inode_indexlookup(fs, &in, blockNum);
            if (sectorNum >= 0){                                        // Caso que no se pueda obtener el bloque
                if (diskimg_readsector(fs->dfd, sectorNum, buf) >= 0){  // Caso que no se pueda leer el bloque
                    int filesize = inode_getsize(&in);
                    int offset = blockNum * DISKIMG_SECTOR_SIZE;
                    if (filesize <= offset) return 0;
                    int validBytes = filesize - offset;
                    return (validBytes < DISKIMG_SECTOR_SIZE) ? validBytes : DISKIMG_SECTOR_SIZE;
                } 
            }                                   
        }                
    } return -1;
}

