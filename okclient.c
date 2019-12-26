#include <sys/types.h>
#include <sys/stat.h>
#include "str.h"
#include "ip4.h"
#include "okclient.h"
#include "log.h"
#include <dirent.h>
#include <string.h>

static char fn[3 + IP4_FMT];

static char ip_str[IP4_FMT];
char ** allowed_ips;
int n_files;

void initialize_access_control(void) {
    n_files = 0;
    int i = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir("ip");

    //Determine the number of files
    while((dir = readdir(d)) != NULL) {
        if ( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") ) {
            n_files++;
        }
    }
    rewinddir(d);

    char *filesList[n_files];

    //Put file names into the array
    while((dir = readdir(d)) != NULL) {
        if ( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") ) {
            filesList[i] = (char*) malloc (strlen(dir->d_name)+1);
            strncpy( filesList[i],dir->d_name, strlen(dir->d_name) );
            i++;
        }
    }

    closedir(d);
    allowed_ips = (char*) malloc(sizeof(char*)*n_files);
    for (i = 0; i < n_files; i++)
        allowed_ips[i] = filesList[i];
}

int okclient_no_stat_call(char ip[4]) {
    // Copy ip bytes into ip_str as a IP with dots
    int i = 0;
    int length = ip4_fmt(ip_str,ip);
    ip_str[length] = 0;

    // Return 1 if the IP is found within the allowed IPs
    for (i = 0; i < sizeof(allowed_ips) / sizeof(IP4_FMT); i++) {
        if (!strcmp(ip_str, allowed_ips[i])) {
            paco_print("Accepted DNS query without a stat call\n");
            return 1;
        }
    }
    return 0;
}


int okclient(char ip[4])
{
  struct stat st;
  int i;

  fn[0] = 'i';
  fn[1] = 'p';
  fn[2] = '/';
  fn[3 + ip4_fmt(fn + 3,ip)] = 0;

  for (;;) {
    if (stat(fn,&st) == 0) return 1;
    /* treat temporary error as rejection */
    i = str_rchr(fn,'.');
    if (!fn[i]) return 0;
    fn[i] = 0;
  }
}
