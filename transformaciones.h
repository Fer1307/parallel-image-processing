#ifndef TRANSFORMACIONES_H
#define TRANSFORMACIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. INVERSIÓN HORIZONTAL A COLOR (inv_hor_color)
extern void inv_hor_color(const char* path, const char* mask) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

    image = fopen(path, "rb");          // Imagen original
    outputImage = fopen(add_char, "wb"); // Imagen transformada

    if (!image || !outputImage) {
        printf("Error abriendo archivos en inv_hor_color.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    // variables 
    long ancho, tam, bpp, alto;
    unsigned char r, g, b;
    unsigned char xx[54];

    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage); // Copia cabecera a nueva imagen
    }

    tam   = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    bpp   = (long)xx[29] * 256 + (long)xx[28];
    ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto  = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    int padding = (4 - ((ancho * 3) % 4)) % 4;
    long total_pixeles = ancho * alto;

    unsigned char* arr_in_b = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));//reserva de memoria
    unsigned char* arr_in_g = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));

    
    long j = 0;
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            arr_in_b[j] = b;
            arr_in_g[j] = g;
            arr_in_r[j] = r;
            j++;
        }
        for (int p = 0; p < padding; p++) {
            fgetc(image); // Saltar bytes de relleno
        }
    }

    for (long i = 0; i < alto; i++) {
        for (long x = ancho - 1; x >= 0; x--) {
            long index = (i * ancho) + x;
            fputc(arr_in_b[index], outputImage);
            fputc(arr_in_g[index], outputImage);
            fputc(arr_in_r[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0, outputImage); // Escribir padding
        }
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in_b);
        free(arr_in_g);
        free(arr_in_r);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Inversion de imagen horizontal a colores\n");
    fprintf(outputLog, "Función: inv_hor_color, con %s\n", path);
    fprintf(outputLog, "Localidades totales leídas: %ld\n", total_pixeles);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", total_pixeles);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    fclose(image);
    fclose(outputImage);
}


// 2. INVERSIÓN HORIZONTAL EN ESCALA DE GRISES (inv_hor_bn)
extern void inv_hor_bn(const char* path, const char* mask) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

    image = fopen(path, "rb");
    outputImage = fopen(add_char, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en inv_hor_bn.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    long ancho, tam, bpp, alto;
    unsigned char r, g, b, pixel;
    unsigned char xx[54];

    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage); // Copia cabecera a nueva imagen
    }

    tam   = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    bpp   = (long)xx[29] * 256 + (long)xx[28];
    ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto  = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    int padding = (4 - ((ancho * 3) % 4)) % 4;
    long total_pixeles = ancho * alto;

    // Un solo arreglo para escala de grises (optimización de memoria 3x)
    unsigned char* arr_in_bn = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));

    long j = 0;
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            pixel = (unsigned char)(0.21 * r + 0.72 * g + 0.07 * b);
            arr_in_bn[j] = pixel;
            j++;
        }
        for (int p = 0; p < padding; p++) {
            fgetc(image);
        }
    }

    for (long i = 0; i < alto; i++) {
        for (long x = ancho - 1; x >= 0; x--) {
            long index = (i * ancho) + x;
            unsigned char val = arr_in_bn[index];
            fputc(val, outputImage); // B
            fputc(val, outputImage); // G
            fputc(val, outputImage); // R
        }
        for (int p = 0; p < padding; p++) {
            fputc(0, outputImage);
        }
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in_bn);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Inversion de imagen horizontal a escala de grises\n");
    fprintf(outputLog, "Función: inv_hor_bn, con %s\n", path);
    fprintf(outputLog, "Localidades totales leídas: %ld\n", total_pixeles);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", total_pixeles);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_bn);
    fclose(image);
    fclose(outputImage);
}

// 3. INVERSIÓN VERTICAL A COLOR (inv_ver_color)
extern void inv_ver_color(const char* path, const char* mask) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

    image = fopen(path, "rb");
    outputImage = fopen(add_char, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en inv_ver_color.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    long ancho, tam, bpp, alto;
    unsigned char r, g, b;
    unsigned char xx[54];

    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    tam   = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    bpp   = (long)xx[29] * 256 + (long)xx[28];
    ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto  = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    int padding = (4 - ((ancho * 3) % 4)) % 4;
    long total_pixeles = ancho * alto;

    unsigned char* arr_in_b = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));

    long j = 0;
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            arr_in_b[j] = b;
            arr_in_g[j] = g;
            arr_in_r[j] = r;
            j++;
        }
        for (int p = 0; p < padding; p++) {
            fgetc(image);
        }
    }

    for (long i = alto - 1; i >= 0; i--) {
        for (long x = ancho - 1; x >= 0; x--) {
            long index = (i * ancho) + x;
            fputc(arr_in_b[index], outputImage);
            fputc(arr_in_g[index], outputImage);
            fputc(arr_in_r[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0, outputImage);
        }
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in_b);
        free(arr_in_g);
        free(arr_in_r);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Inversion de imagen vertical a colores\n");
    fprintf(outputLog, "Función: inv_ver_color, con %s\n", path);
    fprintf(outputLog, "Localidades totales leídas: %ld\n", total_pixeles);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", total_pixeles);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    fclose(image);
    fclose(outputImage);
}

// 4. INVERSIÓN VERTICAL EN ESCALA DE GRISES (inv_ver_bn)
extern void inv_ver_bn(const char* path, const char* mask) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

    image = fopen(path, "rb");
    outputImage = fopen(add_char, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en inv_ver_bn.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    long ancho, tam, bpp, alto;
    unsigned char r, g, b, pixel;
    unsigned char xx[54];

    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    tam   = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    bpp   = (long)xx[29] * 256 + (long)xx[28];
    ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto  = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    int padding = (4 - ((ancho * 3) % 4)) % 4;
    long total_pixeles = ancho * alto;

    unsigned char* arr_in_bn = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));

    long j = 0;
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            pixel = (unsigned char)(0.21 * r + 0.72 * g + 0.07 * b);
            arr_in_bn[j] = pixel;
            j++;
        }
        for (int p = 0; p < padding; p++) {
            fgetc(image);
        }
    }

    // Escribir píxeles en escala de grises invertidos verticalmente
    for (long i = alto - 1; i >= 0; i--) {
        for (long x = ancho - 1; x >= 0; x--) {
            long index = (i * ancho) + x;
            unsigned char val = arr_in_bn[index];
            fputc(val, outputImage); // B
            fputc(val, outputImage); // G
            fputc(val, outputImage); // R
        }
        for (int p = 0; p < padding; p++) {
            fputc(0, outputImage);
        }
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in_bn);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Inversion de imagen vertical a escala de grises\n");
    fprintf(outputLog, "Función: inv_ver_bn, con %s\n", path);
    fprintf(outputLog, "Localidades totales leídas: %ld\n", total_pixeles);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", total_pixeles);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_bn);
    fclose(image);
    fclose(outputImage);
}

// 5. DESENFOQUE A COLOR (blur_color) - Convolución Separable 2 Pasos O(W*H*K)
extern void blur_color(const char* input_path, const char* name_output, int kernel_size) {
    FILE *image, *outputImage;
    char output_path[100] = "./img/";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");
    printf("%s\n", output_path);

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en blur_color.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);

    unsigned char** input_rows  = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** output_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** temp_rows   = (unsigned char**)malloc(height * sizeof(unsigned char*));

    for (int i = 0; i < height; i++) {
        input_rows[i]  = (unsigned char*)malloc(row_padded);
        output_rows[i] = (unsigned char*)malloc(row_padded);
        temp_rows[i]   = (unsigned char*)malloc(row_padded);
        fread(input_rows[i], sizeof(unsigned char), row_padded, image);
    }

    int k = kernel_size / 2;

    // Paso intermedio: desenfoque horizontal
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumB = 0, sumG = 0, sumR = 0, count = 0;

            for (int dx = -k; dx <= k; dx++) {
                int nx = x + dx;
                if (nx >= 0 && nx < width) {
                    int idx = nx * 3;
                    sumB += input_rows[y][idx + 0];
                    sumG += input_rows[y][idx + 1];
                    sumR += input_rows[y][idx + 2];
                    count++;
                }
            }

            int index = x * 3;
            temp_rows[y][index + 0] = sumB / count;
            temp_rows[y][index + 1] = sumG / count;
            temp_rows[y][index + 2] = sumR / count;
        }
        // Copiar padding sin modificar
        for (int p = width * 3; p < row_padded; p++) {
            temp_rows[y][p] = input_rows[y][p];
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumB = 0, sumG = 0, sumR = 0, count = 0;

            for (int dy = -k; dy <= k; dy++) {
                int ny = y + dy;
                if (ny >= 0 && ny < height) {
                    int idx = x * 3;
                    sumB += temp_rows[ny][idx + 0];
                    sumG += temp_rows[ny][idx + 1];
                    sumR += temp_rows[ny][idx + 2];
                    count++;
                }
            }

            int index = x * 3;
            output_rows[y][index + 0] = sumB / count;
            output_rows[y][index + 1] = sumG / count;
            output_rows[y][index + 2] = sumR / count;
        }
        for (int p = width * 3; p < row_padded; p++) {
            output_rows[y][p] = temp_rows[y][p];
        }
    }

    for (int i = 0; i < height; i++) {
        fwrite(output_rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(input_rows[i]);
        free(temp_rows[i]);
        free(output_rows[i]);
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(input_rows);
        free(temp_rows);
        free(output_rows);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Desenfoque de imagen a colores\n");
    fprintf(outputLog, "Función: blur_color, con %s\n", input_path);
    fprintf(outputLog, "Localidades totales leídas: %d\n", width * height);
    fprintf(outputLog, "Localidades totales escritas: %d\n", width * height);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(input_rows);
    free(temp_rows);
    free(output_rows);
    fclose(image);
    fclose(outputImage);
}

// 6. DESENFOQUE EN ESCALA DE GRISES (blur_bn) - Convolución Separable Monocanal
extern void blur_bn(const char* input_path, const char* name_output, int kernel_size) {
    FILE *image, *outputImage;
    char output_path[100] = "./img/";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");
    printf("%s\n", output_path);

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en blur_bn.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);

    unsigned char** input_rows  = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** output_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** temp_rows   = (unsigned char**)malloc(height * sizeof(unsigned char*));

    // Lectura y conversión inmediata a luminancia (reduce cómputo del kernel a 1/3)
    for (int i = 0; i < height; i++) {
        input_rows[i]  = (unsigned char*)malloc(row_padded);
        output_rows[i] = (unsigned char*)malloc(row_padded);
        temp_rows[i]   = (unsigned char*)malloc(row_padded);
        fread(input_rows[i], sizeof(unsigned char), row_padded, image);

        for (int x = 0; x < width; x++) {
            int idx = x * 3;
            unsigned char b = input_rows[i][idx + 0];
            unsigned char g = input_rows[i][idx + 1];
            unsigned char r = input_rows[i][idx + 2];
            unsigned char pixel = (unsigned char)(0.21 * r + 0.72 * g + 0.07 * b);
            input_rows[i][idx + 0] = pixel;
            input_rows[i][idx + 1] = pixel;
            input_rows[i][idx + 2] = pixel;
        }
    }

    int k = kernel_size / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumGray = 0, count = 0;

            for (int dx = -k; dx <= k; dx++) {
                int nx = x + dx;
                if (nx >= 0 && nx < width) {
                    sumGray += input_rows[y][nx * 3];
                    count++;
                }
            }

            unsigned char avg = sumGray / count;
            int index = x * 3;
            temp_rows[y][index + 0] = avg;
            temp_rows[y][index + 1] = avg;
            temp_rows[y][index + 2] = avg;
        }
        // Copiar padding sin modificar
        for (int p = width * 3; p < row_padded; p++) {
            temp_rows[y][p] = input_rows[y][p];
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumGray = 0, count = 0;

            for (int dy = -k; dy <= k; dy++) {
                int ny = y + dy;
                if (ny >= 0 && ny < height) {
                    sumGray += temp_rows[ny][x * 3];
                    count++;
                }
            }

            unsigned char avg = sumGray / count;
            int index = x * 3;
            output_rows[y][index + 0] = avg;
            output_rows[y][index + 1] = avg;
            output_rows[y][index + 2] = avg;
        }
        for (int p = width * 3; p < row_padded; p++) {
            output_rows[y][p] = temp_rows[y][p];
        }
    }

    for (int i = 0; i < height; i++) {
        fwrite(output_rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(input_rows[i]);
        free(temp_rows[i]);
        free(output_rows[i]);
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(input_rows);
        free(temp_rows);
        free(output_rows);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Desenfoque de imagen a escala de grises\n");
    fprintf(outputLog, "Función: blur_bn, con %s\n", input_path);
    fprintf(outputLog, "Localidades totales leídas: %d\n", width * height);
    fprintf(outputLog, "Localidades totales escritas: %d\n", width * height);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(input_rows);
    free(temp_rows);
    free(output_rows);
    fclose(image);
    fclose(outputImage);
}


// 7. DETECCIÓN DE BORDES EN X (bordes_x)
extern void bordes_x(const char* path, const char* mask) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

    image = fopen(path, "rb");
    outputImage = fopen(add_char, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en bordes_x.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    long ancho, tam, bpp, alto;
    unsigned char r, g, b, pixel;
    unsigned char xx[54];

    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    tam   = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    bpp   = (long)xx[29] * 256 + (long)xx[28];
    ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto  = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    int padding = (4 - ((ancho * 3) % 4)) % 4;
    long total_pixeles = ancho * alto;

    unsigned char* arr_in_bn = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));
    unsigned char* arr_out = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));

    long j = 0;
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            pixel = (unsigned char)(0.21 * r + 0.72 * g + 0.07 * b);
            arr_in_bn[j] = pixel;
            j++;
        }
        for (int p = 0; p < padding; p++) {
            fgetc(image);
        }
    }

    // Detección de bordes en dirección X
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            long index = (i * ancho) + x;

            if (x > 0 && x < ancho - 1) {
                int izquierda = arr_in_bn[index - 1];
                int derecha = arr_in_bn[index + 1];

                int diferencia = abs(derecha - izquierda) / 2;
                arr_out[index] = (unsigned char)diferencia;
            }
            else {
                arr_out[index] = 0;
            }
        }
    }

    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            long index = (i * ancho) + x;
            unsigned char val = arr_out[index];

            fputc(val, outputImage); // B
            fputc(val, outputImage); // G
            fputc(val, outputImage); // R
        }
        for (int p = 0; p < padding; p++) {
            fputc(0, outputImage);
        }
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in_bn);
        free(arr_out);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Deteccion de bordes en X\n");
    fprintf(outputLog, "Función: bordes_x, con %s\n", path);
    fprintf(outputLog, "Localidades totales leídas: %ld\n", total_pixeles);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", total_pixeles);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_bn);
    free(arr_out);
    fclose(image);
    fclose(outputImage);
}


// 8. DETECCIÓN DE BORDES EN Y (bordes_y)
extern void bordes_y(const char* path, const char* mask) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

    image = fopen(path, "rb");
    outputImage = fopen(add_char, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos en bordes_y.\n");
        if (image) fclose(image);
        if (outputImage) fclose(outputImage);
        return;
    }

    long ancho, tam, bpp, alto;
    unsigned char r, g, b, pixel;
    unsigned char xx[54];

    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    tam   = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    bpp   = (long)xx[29] * 256 + (long)xx[28];
    ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto  = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    int padding = (4 - ((ancho * 3) % 4)) % 4;
    long total_pixeles = ancho * alto;

    unsigned char* arr_in_bn = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));
    unsigned char* arr_out = (unsigned char*)malloc(total_pixeles * sizeof(unsigned char));

    long j = 0;
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            pixel = (unsigned char)(0.21 * r + 0.72 * g + 0.07 * b);
            arr_in_bn[j] = pixel;
            j++;
        }
        for (int p = 0; p < padding; p++) {
            fgetc(image);
        }
    }

    // Detección de bordes en dirección Y
    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            long index = (i * ancho) + x;

            if (i > 0 && i < alto - 1) {
                int arriba = arr_in_bn[index - ancho];
                int abajo = arr_in_bn[index + ancho];

                int diferencia = abs(abajo - arriba) / 2;
                arr_out[index] = (unsigned char)diferencia;
            }
            else {
                arr_out[index] = 0;
            }
        }
    }

    for (long i = 0; i < alto; i++) {
        for (long x = 0; x < ancho; x++) {
            long index = (i * ancho) + x;
            unsigned char val = arr_out[index];

            fputc(val, outputImage); // B
            fputc(val, outputImage); // G
            fputc(val, outputImage); // R
        }
        for (int p = 0; p < padding; p++) {
            fputc(0, outputImage);
        }
    }

    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in_bn);
        free(arr_out);
        fclose(image);
        fclose(outputImage);
        return;
    }

    printf("Deteccion de bordes en Y\n");
    fprintf(outputLog, "Función: bordes_y, con %s\n", path);
    fprintf(outputLog, "Localidades totales leídas: %ld\n", total_pixeles);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", total_pixeles);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_bn);
    free(arr_out);
    fclose(image);
    fclose(outputImage);
}

#endif // TRANSFORMACIONES_H