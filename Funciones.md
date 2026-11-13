# Funciones principales

> ⚠️ Los nombres y firmas de esta tabla son una propuesta basada en las transformaciones del proyecto. Reemplazarlos por los reales del código antes de la entrega del 13 de noviembre.

## Librería de imágenes BMP

| Función | Responsabilidad | Firma propuesta |
|---|---|---|
| `load_bmp` | Lee el archivo, valida la cabecera de 24 bits y reserva memoria para los píxeles | `image_t *load_bmp(const char *path)` |
| `save_bmp` | Escribe la imagen en disco con la cabecera original | `int save_bmp(const char *path, const image_t *img)` |
| `free_image` | Libera la memoria de la imagen | `void free_image(image_t *img)` |
| `row_stride` | Calcula el tamaño de fila con padding a 4 bytes | `int row_stride(int width)` |

## Transformaciones (Etapa 1)

| Función | Transformación | Firma propuesta |
|---|---|---|
| `inv_hor_color` | Inversión horizontal a color | `void inv_hor_color(image_t *img)` |
| `inv_hor_bn` | Inversión horizontal a escala de grises | `void inv_hor_bn(image_t *img)` |
| `inv_ver_color` | Inversión vertical a color | `void inv_ver_color(image_t *img)` |
| `inv_ver_bn` | Inversión vertical a escala de grises | `void inv_ver_bn(image_t *img)` |
| `to_grayscale_pixel` | Convierte un píxel RGB a luminancia (usada por las versiones en gris) | `uint8_t to_grayscale_pixel(uint8_t r, uint8_t g, uint8_t b)` |

Las transformaciones modifican la imagen en su lugar (*in-place*). Esto evita reservar una segunda imagen de 12 MB por cada hilo.

## Paralelismo e interfaz

| Función | Responsabilidad | Firma propuesta |
|---|---|---|
| `process_image_task` | Trabajo de un hilo: carga, aplica la transformación, guarda y libera | `void *process_image_task(void *arg)` |
| `run_batch` | Lanza hasta 10 tareas en paralelo y espera a que terminen | `int run_batch(batch_t *batch)` |
| `measure_time` | Devuelve el tiempo transcurrido con reloj de alta resolución | `double measure_time(void)` |

⚠️ Confirmar si el paralelismo se implementa con OpenMP (`#pragma omp parallel for`) o con pthreads. Si se usa OpenMP, `run_batch` es un bucle paralelo; si se usa pthreads, es un arreglo de hilos con `pthread_join`.
