#include <stdio.h>
#include <stdlib.h>

#define ONE_STL_IMPLEMENTATION
#include "one-stl-parser.h"

char *read_binary_file(char const *file_path, size_t *file_size) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "file %s does not exist\n", file_path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    char *data = malloc(*file_size);
    if (fread(data, 1, *file_size, file) != *file_size) {
        fprintf(stderr, "I/O error while reading %s\n", file_path);
        exit(EXIT_FAILURE);
    }
    fclose(file);

    return data;
}

int main() {
    char const *file_name = "suzanne.stl";
    
    size_t file_size;
    char *file_contents = read_binary_file(file_name, &file_size);

    size_t buf_size;
    if (one_stl_buf_size(&buf_size, file_contents, file_size, ONE_STL_NVVV)) {
        fprintf(stderr, "file \"%s\" is not valid stl data\n", file_name);
        exit(EXIT_FAILURE);
    }

    float *buf = (float*)malloc(buf_size);
    size_t trig_count = one_stl_parse(buf, file_contents, ONE_STL_NVVV);
    
    printf("triangle count: %ld\n", trig_count);

    typedef struct {
        float normal[3];
        float v0[3];
        float v1[3];
        float v2[3];
    } trig_nvvv_s;
    trig_nvvv_s *triangles = (trig_nvvv_s*)buf;

    for (size_t i = 0; i < trig_count; ++i) {
        /* Do something useful with triangles */
    }

    free(file_contents);
    free(buf);
}