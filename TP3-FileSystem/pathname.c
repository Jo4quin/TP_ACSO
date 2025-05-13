
#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int pathname_lookup(struct unixfilesystem *fs, const char *pathname) {
    if (fs != NULL && pathname != NULL && pathname[0] == '/') {
        // Copo el path para no modificar el original
        char pathcopy[1024];
        strncpy(pathcopy, pathname, sizeof(pathcopy));
        pathcopy[sizeof(pathcopy) - 1] = '\0';
        
        // Empiezo desde el inodo raíz
        int current_inumber = ROOT_INUMBER;
        char *saveptr;
        char *token = strtok_r(pathcopy, "/", &saveptr);
        while (token != NULL) {
            struct direntv6 dir_entry;
            if (directory_findname(fs, token, current_inumber, &dir_entry) < 0) {
                return -1; // caso no encontró el componente
            }
            current_inumber = dir_entry.d_inumber;
            token = strtok_r(NULL, "/", &saveptr);
        }
        return current_inumber;
    } return -1;
}
