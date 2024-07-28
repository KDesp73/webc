
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


static const char *get_file_extension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if(!dot || dot == file_name) {
        return "";
    }
    return dot + 1;
}

static int count_md_files_in_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return -1; // Return -1 to indicate an error
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strcmp(get_file_extension(entry->d_name), "md")){
            file_count++;
        }
    }

    closedir(dir);

    return file_count;
}

static char** listMarkdownFiles(const char *dir_path, int *num_files) {
    DIR *dir;
    struct dirent *entry;
    char **markdown_files = NULL;
    int count = 0;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error opening directory");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it is a regular file
            const char *file_name = entry->d_name;
            const char *file_extension = strrchr(file_name, '.');
            if (file_extension != NULL && strcmp(file_extension, ".md") == 0) {
                markdown_files = (char**)realloc(markdown_files, (count + 1) * sizeof(char*));
                markdown_files[count] = (char*)malloc(strlen(dir_path) + strlen(file_name) + 2); // +2 for '/' and '\0'
                sprintf(markdown_files[count], "%s/%s", dir_path, file_name);
                count++;
            }
        }
    }

    closedir(dir);

    *num_files = count;
    return markdown_files;
}

static const char *get_filename_without_extension(const char *path) {
    const char *filename = strrchr(path, '/');
    if (!filename) {
        filename = strrchr(path, '\\');
    }
    
    if (filename) {
        const char *dot = strrchr(filename, '.');
        if (dot) {
            return strndup(filename + 1, dot - filename - 1);
        } else {
            return filename + 1;
        }
    } else {
        return path;
    }
}

static void replaceUnderscoresAndCapitalize(char *str) {
    int capitalizeNext = 1;
    while (*str) {
        if (*str == '_' || *str == '-') {
            *str = ' ';
            capitalizeNext = 1;
        } else if (capitalizeNext && isalpha(*str)) {
            *str = toupper(*str);
            capitalizeNext = 0;
        } else {
            *str = tolower(*str);
        }
        str++;
    }
}
