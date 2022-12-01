#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include        <stdlib.h>      /* for using standard library functions such as 'exit' */
#include        <stdbool.h>     /* for using bool types */
#include        <sys/types.h>   /* for using the struct stat - file status */
#include        <sys/stat.h>    /* for using stat functions */
#include        <errno.h>       /* for using errno variable in linux */
#include        <string.h>      /* for using strcat function */

#define BUFFERSIZE      4096
#define COPYMODE        0644

/* For compare two file */
enum {SAME = 0, NOT_SAME};

void oops(char *, char *);
int is_same_file(char *src, char *dest);
int sync_file_permission(char *src, char *dest);
char * dest_file_name(char * src, char* dest);
bool is_file_existed(char *filename);
void free_mem(void *ptr);

int main(int ac, char *av[])
{
    int     in_fd, out_fd, n_chars;
    char    buf[BUFFERSIZE];
    char * 	dest_name = NULL;
    int 	result = 0;
    if ( ac != 3 ) {
        fprintf( stderr, "usage: %s source destination\n", *av);
        exit(1);
    }

    /* Thirdly,
     * Allow second argument to be a directory,
     * Create the file with the same name of the source file
     * under directory which input by user as second argument.
     */
    dest_name = dest_file_name(av[1], av[2]);

    /* Firstly,
     * Don't copy in case the destination is same as source file.
     * Just out put the message like real copy command.
     */
    result = is_same_file(av[1], dest_name);
    if(result == -1) {
        fprintf(stderr, "can not compare two file '%s' and '%s'\n", av[1], dest_name);
        free_mem(dest_name);
        exit(1);
    }
    else if(result == SAME) {
        fprintf(stderr, "%s: '%s' and '%s' are the same file\n", av[0], av[1], dest_name);
        free_mem(dest_name);
        exit(1);
    }


    if ((in_fd = open(av[1], O_RDONLY)) == -1 ) {
        free_mem(dest_name);
        oops("Cannot open ", av[1]);
    }

    if ((out_fd = creat( dest_name, COPYMODE)) == -1 ) {
        free_mem(dest_name);
        oops( "Cannot creat", dest_name);
    }


    while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
        if ( write( out_fd, buf, n_chars ) != n_chars ) {
            free_mem(dest_name);
            oops("Write error to ", dest_name);
        }
    if ( n_chars == -1 ) {
        free_mem(dest_name);
        oops("Read error from ", av[1]);
    }

    if ( close(in_fd) == -1 || close(out_fd) == -1 ) {
        free_mem(dest_name);
        oops("Error closing files","");
    }

    /* Secondly,
     * Assign the same file permission to the destination file as were
     * on the source.
     */
    if(sync_file_permission(av[1], dest_name) != 0) {
        fprintf(stderr, "Warring : can not copy file permission from '%s' to '%s'\n", av[1], dest_name);
        free_mem(dest_name);
        exit(1);
    }

    free_mem(dest_name);
    return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}

/*
 * free_mem: free allocated memory.
 * ptr :  pointer to allocated memory need to be free.
 * return: None
 */
void free_mem(void *ptr)
{
    if(ptr) {
        free(ptr);
        ptr = NULL;
    }
}

/*
 * Compare two file to make sure it same file or not.
 * src : file name or full path of source file.
 * dest : file name or full path of destination file.
 * return true if src and dest is the same file and fale if not.
 */
int is_same_file(char *src, char *dest)
{
    struct stat s_stat; /* source file status */
    struct stat d_stat; /* destination status */
    int f1 = 0;
    int f2 = 0;

    /*
     * Open source file in read only mode to get file status
     */
    if ( (f1 = open(src, O_RDONLY)) == -1 ) {
        fprintf(stderr,"Error: %s ", src);
        perror(src);
        return (-1);
    }

    /*
     * Get files status to compare
     */
    if (fstat(f1, &s_stat) < 0) {
        if (errno != ENOENT) {
            perror("Error: ");
            return (-1);
        }
    }

    /*
     * Open destination file in read only mode to get file status
     */
    if ( (f2 = open(dest, O_RDONLY)) == -1 ) {
        if(stat(dest, &d_stat) < 0) {
            if (errno != ENOENT) {
                perror("Error: ");
                return (-1);
            }
        }

    } else {
        if (fstat(f2, &d_stat) < 0) {
            if (errno != ENOENT) {
                perror("Error: ");
                return (-1);
            }
        }
    }

    /*
     * Close two file after get status
     */
    if(f1 != 0 && f1 != -1) {
        if ( close(f1) == -1 ) {
            perror("Error: ");
            return (-1);
        }
    }
    if(f2 != 0 && f2 != -1) {
        if ( close(f2) == -1 ) {
            perror("Error: ");
            return (-1);
        }
    }

    /* Two files are the same if they are on
     * the same device and have the same i-node number.
     */
    if (s_stat.st_dev == d_stat.st_dev
            && s_stat.st_ino == d_stat.st_ino) {
        return SAME;
    }

    /*
     * Two files is not same
     */
    return NOT_SAME;
}

/*
 * Copy file permission from source file to destination file.
 * parameter : char *src pointer to the file path of the source file
 * parameter : char *dest pointer to the file path of the destination file
 * return : return 0 on success and -1 on error
 */
int sync_file_permission(char *src, char *dest)
{
    struct stat s_stat; /* source stat */
    //struct stat d_stat; /* destination stat */

    /*
     * Get source file status.
     */
    if (stat(src, &s_stat) < 0) {
        if (errno != ENOENT) {
            perror("Error: ");
            return (-1);
        }
    }

    /*
     * Copy file's permission.
     */
    if (chmod(dest, s_stat.st_mode) < 0) {
        perror("Error: ");
        return (-1);
    }

    return 0;
}

/*
 * Create destination file name from src and dest input file.
 * parameter : char * src pointer to the file name or full path of the source file.
 * parameter : char * dest pointer to the file name or full path of the destination file.
 * return : return pointer to the destination file name, the destination file name is create
 * by the rule :
 * if dest is not directory, just simply create file name same as "dest"
 * if dest is an directory, create destination file with the same name of src file
 * under the that directory (append the string "src" to the "dest" after append '/' character.
 */
char *dest_file_name(char * src, char* dest)
{
    char *tmp = NULL;
    struct stat d_stat; /* destination stat */

    /*
     * Get destination file status.
     */
    if (stat(dest, &d_stat) < 0) {
        if (errno != ENOENT) {
            oops("can't stat '%s'", dest);
        }
    }
    /*
     * In case the destination file is directory,
     * Create destination file with the same name of the source file.
     * under that directory.
     */
    if(!S_ISDIR(d_stat.st_mode)) {
        tmp = strdup(dest);
    } else {
        int newlen = 0;
        /* In case user input '/' character at the end of destination name
         * just simply append the the src string to the dest string
         */
        if(dest[strlen(dest)] == '/') {
            newlen = strlen(src) + strlen(dest) + 1;
            tmp = (char *)malloc(newlen * sizeof(char));
            strcpy(tmp, dest);
            strcat(tmp, src);
        }
        /* In case user did not input '/' character at the end of destination name
         * add this character before append the the src string to the dest string.
         */
        else {
            newlen = strlen(src) + strlen(dest) + 2;
            tmp = (char *)malloc(newlen * sizeof(char));
            strcpy(tmp, dest);
            strcat(tmp, "/");
            strcat(tmp, src);
        }
    }
    return tmp;
}

