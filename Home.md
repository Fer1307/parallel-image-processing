# Interfaz gráfica para procesamiento paralelo de imágenes BMP

Wiki del desarrollo de una interfaz gráfica que procesa hasta **10 imágenes de gran formato** (más de 2000 px de ancho y alto) en paralelo, aplicando transformaciones implementadas en C.

> Estado: **Etapa 1 en curso** (4 transformaciones). Las secciones marcadas con ⏳ se completarán con capturas y datos del benchmark.

## Contenido

1. [Descripción general](Descripcion-general)
2. [Transformaciones (Etapa 1)](Transformaciones)
3. [Funciones principales](Funciones)
4. [Resultados y benchmark](Resultados) ⏳
5. [Conclusiones](Conclusiones) ⏳
6. Video demostrativo (máx. 3 min) ⏳

## Cronograma de avances

| Fecha | Entregable |
|---|---|
| 13 de noviembre | Comparación de métricas de rendimiento con 4 transformaciones en paralelo. 10 imágenes de gran formato. Documentación en GitHub (códigos, imágenes, documentación de las funciones empleadas). |
| 19 de noviembre | Comparación de métricas de rendimiento con 8 transformaciones en paralelo. 10 imágenes de gran formato. Documentación en GitHub. |
| 23 de noviembre | Interfaz gráfica con los elementos operando al 60%. Más de la mitad de los elementos gráficos deben funcionar al ejecutar la aplicación en su versión alfa. |

## Transformaciones del proyecto

| Etapa | Transformación | Descripción breve |
|---|---|---|
| 1 | `inv_hor_color` | Inversión horizontal a color |
| 1 | `inv_hor_bn` | Inversión horizontal a escala de grises |
| 1 | `inv_ver_color` | Inversión vertical a color |
| 1 | `inv_ver_bn` | Inversión vertical a escala de grises |
| 2 | `blur_color` | Desenfoque a color con kernel definido ⏳ |
| 2 | `blur_bn` | Desenfoque a escala de grises con kernel definido ⏳ |
| 2 | Bordes en X | Detección de bordes, derivada parcial respecto a x ⏳ |
| 2 | Bordes en Y | Detección de bordes, derivada parcial respecto a y ⏳ |
