
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "transformaciones.h"

#define NUM_THREADS 8
#define NUM_IMAGENES 10

int main() {

    omp_set_num_threads(NUM_THREADS);

    const char *imagenes[NUM_IMAGENES] = {
        "./img/prueba1.bmp",
        "./img/prueba2.bmp",
        "./img/prueba3.bmp",
        "./img/prueba4.bmp",
        "./img/prueba5.bmp",
        "./img/prueba6.bmp",
        "./img/prueba7.bmp",
        "./img/prueba8.bmp",
        "./img/prueba9.bmp",
        "./img/prueba10.bmp"
    };

    double t1, t2, tiempo_secuencial, tiempo_paralelo;

    printf("INICIANDO BENCHMARK (%d IMAGENES | %d HILOS)\n",
           NUM_IMAGENES, NUM_THREADS);

    // 1. EJECUCION SECUENCIAL
    printf("\n[1/2] Ejecutando modo SECUENCIAL...\n");
    t1 = omp_get_wtime();

    for (int i = 0; i < NUM_IMAGENES; i++) {

        char ver_bn[64], hor_bn[64], ver_col[64], hor_col[64];
        char blur_b[64], blur_c[64], borde_x[64], borde_y[64];

        sprintf(ver_bn, "sec_img%d_inv_ver_bn", i + 1);
        sprintf(hor_bn, "sec_img%d_inv_hor_bn", i + 1);
        sprintf(ver_col, "sec_img%d_inv_ver_color", i + 1);
        sprintf(hor_col, "sec_img%d_inv_hor_color", i + 1);
        sprintf(blur_b, "sec_img%d_blur_bn", i + 1);
        sprintf(blur_c, "sec_img%d_blur_color", i + 1);
        sprintf(borde_x, "sec_img%d_bordes_x", i + 1);
        sprintf(borde_y, "sec_img%d_bordes_y", i + 1);

        inv_ver_bn(imagenes[i], ver_bn);
        inv_hor_bn(imagenes[i], hor_bn);
        inv_ver_color(imagenes[i], ver_col);
        inv_hor_color(imagenes[i], hor_col);
        blur_bn(imagenes[i], blur_b, 101);
        blur_color(imagenes[i], blur_c, 101);
        bordes_x(imagenes[i], borde_x);
        bordes_y(imagenes[i], borde_y);
    }

    t2 = omp_get_wtime();
    tiempo_secuencial = t2 - t1;

    printf("Tiempo secuencial: %lf segundos\n", tiempo_secuencial);

    // 2. EJECUCION PARALELA
    printf("\n[2/2] Ejecutando modo PARALELO...\n");
    t1 = omp_get_wtime();

    for (int i = 0; i < NUM_IMAGENES; i++) {

        char ver_bn[64], hor_bn[64], ver_col[64], hor_col[64];
        char blur_b[64], blur_c[64], borde_x[64], borde_y[64];

        sprintf(ver_bn, "par_img%d_inv_ver_bn", i + 1);
        sprintf(hor_bn, "par_img%d_inv_hor_bn", i + 1);
        sprintf(ver_col, "par_img%d_inv_ver_color", i + 1);
        sprintf(hor_col, "par_img%d_inv_hor_color", i + 1);
        sprintf(blur_b, "par_img%d_blur_bn", i + 1);
        sprintf(blur_c, "par_img%d_blur_color", i + 1);
        sprintf(borde_x, "par_img%d_bordes_x", i + 1);
        sprintf(borde_y, "par_img%d_bordes_y", i + 1);

        #pragma omp parallel sections
        {
            #pragma omp section
            inv_ver_bn(imagenes[i], ver_bn);

            #pragma omp section
            inv_hor_bn(imagenes[i], hor_bn);

            #pragma omp section
            inv_ver_color(imagenes[i], ver_col);

            #pragma omp section
            inv_hor_color(imagenes[i], hor_col);

            #pragma omp section
            blur_bn(imagenes[i], blur_b, 101);

            #pragma omp section
            blur_color(imagenes[i], blur_c, 101);

            #pragma omp section
            bordes_x(imagenes[i], borde_x);

            #pragma omp section
            bordes_y(imagenes[i], borde_y);
        }
    }

    t2 = omp_get_wtime();
    tiempo_paralelo = t2 - t1;

    printf("Tiempo paralelo: %lf segundos\n", tiempo_paralelo);

    // 3. RESULTADOS
    double speedup = tiempo_secuencial / tiempo_paralelo;
    double eficiencia = (speedup / NUM_THREADS) * 100.0;

    printf("\n=======================================\n");
    printf("       RESULTADOS DEL BENCHMARK\n");
    printf("=======================================\n");

    printf("Tiempo Secuencial : %lf s\n", tiempo_secuencial);
    printf("Tiempo Paralelo   : %lf s\n", tiempo_paralelo);
    printf("Speedup           : %.2fx\n", speedup);
    printf("Eficiencia        : %.2f%%\n", eficiencia);

    return 0;
}
