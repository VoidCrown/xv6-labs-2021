#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void find(char *path, char *filename) 
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:           
            // for debug
            /* printf("current file: %s, target file: %s, result: %d\n", fmtname(path), filename, strcmp(fmtname(path), filename)); */

            // compare with target file.
            if (strcmp(fmtname(path), filename) == 0) {
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            
            // add '/' to the end.
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            // read one content in the directory once.
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                // add current file name and end '\0' to the buf string.
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, filename);
            }
            break;
    }

    close(fd);
}

void main(int argc, char *argv[]) 
{
    if(argc < 3){
        fprintf(2, "Usage: find path filename\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}