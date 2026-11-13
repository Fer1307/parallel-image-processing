# Transformaciones (Etapa 1)

Las cuatro transformaciones operan sobre el arreglo de píxeles en memoria. Las de escala de grises comparten una misma conversión por píxel.

## Conversión a escala de grises

Se usa en `inv_hor_bn` e `inv_ver_bn`. Cada píxel se reemplaza por una luminancia Y que se escribe en los tres canales (R = G = B = Y).

- Fórmula estándar (ITU-R BT.601): `Y = 0.299·R + 0.587·G + 0.114·B`
- Versión entera, sin punto flotante: `Y = (77·R + 150·G + 29·B) >> 8`

La versión entera usa 77 + 150 + 29 = 256, por lo que el desplazamiento de 8 bits mantiene el rango 0–255.

## 1. `inv_hor_color`: inversión horizontal a color

**Descripción:** espeja la imagen respecto a su eje vertical (izquierda ↔ derecha). Los colores no cambian.

**Proceso:**
1. Recorrer cada fila `y`.
2. Para `x` desde 0 hasta `ancho/2 − 1`, intercambiar el píxel en la posición `x` con el de la posición `ancho − 1 − x`.
3. Cada intercambio mueve los 3 bytes del píxel (B, G, R) completos, usando un píxel temporal.

**Costo:** se recorre la mitad de los píxeles por fila; no requiere memoria adicional de tamaño de imagen.

## 2. `inv_hor_bn`: inversión horizontal a escala de grises

**Descripción:** igual que la anterior, pero la salida es en escala de grises.

**Proceso:**
1. Aplicar la conversión a gris a cada píxel.
2. Ejecutar la inversión horizontal sobre el resultado.

**Nota:** la conversión a gris es una operación puntual (cada píxel depende solo de sí mismo), así que el orden entre conversión e inversión no altera el resultado. Hacerlo en una sola pasada reduce accesos a memoria.

## 3. `inv_ver_color`: inversión vertical a color

**Descripción:** espeja la imagen respecto a su eje horizontal (arriba ↔ abajo).

**Proceso:**
1. Para `y` desde 0 hasta `alto/2 − 1`, intercambiar la fila `y` con la fila `alto − 1 − y`.
2. El intercambio se hace copiando bloques de `stride` bytes con una fila temporal.

**Ventaja frente a la horizontal:** los bloques son contiguos en memoria, por lo que se usan copias de bloque (`memcpy`) en lugar de intercambios píxel a píxel.

## 4. `inv_ver_bn`: inversión vertical a escala de grises

**Descripción:** inversión vertical con salida en escala de grises.

**Proceso:**
1. Convertir cada píxel a gris.
2. Ejecutar la inversión vertical sobre el resultado.

Comparte la función de conversión con `inv_hor_bn`.

## Verificación de cada transformación

Para validar cada salida se recomienda:

- Aplicar la misma transformación dos veces y comparar con la imagen original (la inversión es una operación involutiva: aplicarla dos veces debe devolver la imagen original).
- Comprobar que las dimensiones del archivo de salida coinciden con las de la entrada.
- Revisar visualmente una imagen pequeña de referencia.
