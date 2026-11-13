# Descripción general

## Objetivo

Diseñar e implementar una interfaz gráfica que permita seleccionar hasta 10 imágenes BMP de gran formato, elegir una transformación y procesarlas en paralelo, mostrando el progreso y los tiempos de ejecución.

## Alcance de la Etapa 1

- Transformaciones: `inv_hor_color`, `inv_hor_bn`, `inv_ver_color`, `inv_ver_bn`.
- Documentación del proceso de cada transformación y de sus funciones principales.
- Estructura para el benchmark de rendimiento (4 transformaciones en paralelo).

Fuera de esta etapa: desenfoques, detección de bordes y el benchmark con 8 transformaciones.

## Arquitectura

| Capa | Responsabilidad | Tecnología |
|---|---|---|
| Procesamiento | Lectura y escritura de BMP, transformaciones de píxeles | Librería en C |
| Paralelismo | Un hilo de trabajo por imagen, hasta 10 imágenes simultáneas | ⚠️ Confirmar: OpenMP o pthreads |
| Interfaz | Selección de archivos y transformación, progreso y tiempos | ⚠️ Confirmar: GTK, Qt u otra |

## Formato BMP (aspectos relevantes)

- **Cabecera:** 54 bytes en BMP de 24 bits (14 bytes de `BITMAPFILEHEADER` + 40 bytes de `BITMAPINFOHEADER`). Los datos de píxeles empiezan en el offset indicado en la cabecera.
- **Orden de canales:** cada píxel ocupa 3 bytes en orden **B, G, R**.
- **Padding:** cada fila se alinea a múltiplos de 4 bytes. El tamaño real de fila (stride) es:

  `stride = (ancho × 3 + 3) & ~3`

  Ignorar el padding corrompe la imagen al recorrer las filas.

## Consideraciones de diseño

- **Independencia de datos:** cada imagen se procesa por separado, por lo que el paralelismo no requiere sincronización entre hilos; solo se sincroniza al terminar.
- **Memoria:** una imagen de 2000 × 2000 px ocupa unos 12 MB en 24 bits. Diez imágenes a la vez implican unos 120 MB, por lo que conviene liberar cada imagen al terminar su escritura.
- **Medición:** el benchmark mide tiempo por imagen y tiempo total, con reloj de alta resolución, excluyendo lectura y escritura de disco si se reporta solo el cómputo.
