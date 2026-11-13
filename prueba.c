#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "transformaciones.h"

#define NUM_THREADS 4
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

    printf("   INICIANDO BENCHMARK (%d IMAGENES | %d HILOS)\n", NUM_IMAGENES, NUM_THREADS);

    // 1. EJECUCIÓN SECUENCIAL (1 solo hilo, imagen por imagen)

    printf("[1/2] Ejecutando modo SECUENCIAL...\n");
    t1 = omp_get_wtime();

    for (int i = 0; i < NUM_IMAGENES; i++) {
        char out_ver_bn[64], out_hor_bn[64], out_ver_col[64], out_hor_col[64];
        
        // Creamos nombres de salida únicos por cada imagen
        sprintf(out_ver_bn,  "sec_img%d_inv_ver_bn", i + 1);
        sprintf(out_hor_bn,  "sec_img%d_inv_hor_bn", i + 1);
        sprintf(out_ver_col, "sec_img%d_inv_ver_color", i + 1);
        sprintf(out_hor_col, "sec_img%d_inv_hor_color", i + 1);

        inv_ver_bn(imagenes[i], out_ver_bn);
        inv_hor_bn(imagenes[i], out_hor_bn);
        inv_ver_color(imagenes[i], out_ver_col);
        inv_hor_color(imagenes[i], out_hor_col);
    }

    t2 = omp_get_wtime();
    tiempo_secuencial = t2 - t1;
    printf(" -> Terminado en: %lf segundos\n\n", tiempo_secuencial);


    // 2. EJECUCIÓN PARALELA (OpenMP)
    printf("[2/2] Ejecutando modo PARALELO...\n");
    t1 = omp_get_wtime();

   
    #pragma omp parallel
    {
        for (int i = 0; i < NUM_IMAGENES; i++) {
            char out_ver_bn[64], out_hor_bn[64], out_ver_col[64], out_hor_col[64];
            
            sprintf(out_ver_bn,  "par_img%d_inv_ver_bn", i + 1);
            sprintf(out_hor_bn,  "par_img%d_inv_hor_bn", i + 1);
            sprintf(out_ver_col, "par_img%d_inv_ver_color", i + 1);
            sprintf(out_hor_col, "par_img%d_inv_hor_color", i + 1);

           
            #pragma omp sections nowait
            {
                #pragma omp section
                inv_ver_bn(imagenes[i], out_ver_bn);

                #pragma omp section
                inv_hor_bn(imagenes[i], out_hor_bn);

                #pragma omp section
                inv_ver_color(imagenes[i], out_ver_col);

                #pragma omp section
                inv_hor_color(imagenes[i], out_hor_col);
            }
        }
    }

    t2 = omp_get_wtime();
    tiempo_paralelo = t2 - t1;
    printf(" -> Terminado en: %lf segundos\n\n", tiempo_paralelo);


    // 3. RESULTADOS DEL BENCHMARK
    double speedup = tiempo_secuencial / tiempo_paralelo;
    double eficiencia = (speedup / NUM_THREADS) * 100.0;

    printf("             RESULTADOS DEL BENCHMARK             \n");
    printf(" Tiempo Secuencial : %lf s\n", tiempo_secuencial);
    printf(" Tiempo Paralelo   : %lf s\n", tiempo_paralelo);
    printf(" Speedup (Mejora)  : %.2fx mas rapido\n", speedup);
    printf(" Eficiencia x Hilo : %.2f%%\n", eficiencia);

    return 0;
}