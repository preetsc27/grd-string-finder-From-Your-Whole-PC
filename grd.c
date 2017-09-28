#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

// function to read file
void readFile(const char *fname, const char *str ){
    FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];

	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		printf("Cannot open file");
        return;
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
            printf("%s\n",  fname);
			printf("A match found on line: %d\n", line_num);
			printf("\n%s\n", temp);
			find_result++;
		}
		line_num++;
	}

	if(find_result == 0) {
		//printf("\nSorry, couldn't find a match.\n");
	}

	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
}

// function to concat 2 pointer chars

char* concat(const char* str1, const char* str2){
    char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
    strcpy(str3, str1);
    strcat(str3, str2);
    return str3;
}

// this is to check if it is a directory
bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

int readingDir(const char* path, char* word){

    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(path);

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    char *p;
    p = ".";
    char *q;
    q = "..";
    char *r;
    r = ".DS_Store";

    // for readdir()
    while ((de = readdir(dr)) != NULL){
        if (!strcmp(de->d_name, p) ||!strcmp(de->d_name, q) ||!strcmp(de->d_name, r)) {
            continue;
        }else{
            char *slash = "/";
            char *concatedPath = concat(path, slash);
            concatedPath = concat(concatedPath, de->d_name);
            //printf("%s\n",concatedPath);
            bool x = is_dir(concatedPath);
            if (x) {
                readingDir(concatedPath, word);
                //printf("Is dir\n");
            }else{
                readFile(concatedPath, word);
                //printf("Is file\n");
            }
        }
    }

    closedir(dr);
    return 1;
}

int main(void)
{
    char word[500];
    printf("Enter what you want to search:");
    scanf("%s",word );
    readingDir(".", word);
    return 0;
}
