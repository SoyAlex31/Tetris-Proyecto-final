# 🎮 TETRIS

## 📌 Nombre del juego
**Tetris**

---

## 🌌 Temática y ambientación

La temática de este juego se basa en el clásico juego retro muy conocido llamado Tetris,  
pero nosotros quisimos llevarlo a otro nivel, adaptándolo a una plataforma diferente:  
la consola de Windows. Aunque conserva la esencia del juego original,  
le agregamos un enfoque personal que lo hace único.  
El entorno de texto fue aprovechado al máximo para representar visualmente  
las piezas, el tablero y los efectos, logrando una experiencia original e inmersiva.

---

## 🎮 Mecánica principal

* El jugador controla una pieza que cae desde la parte superior.
* Puede moverla con las teclas:  
  * `A`: mover a la izquierda  
  * `D`: mover a la derecha  
  * `W`: rotar  
  * `S`: acelerar la caída  
  * `Q`: salir del juego  
* Las piezas se fijan al tocar otras piezas o el fondo.
* Si se forma una línea completa, se elimina.
* El juego termina cuando los bloques alcanzan la **línea de game over**.

---

## 🧠 Idea general de la jugabilidad

* El juego incluye 4 tipos de piezas: I, O, T y L
* Cada una tiene 4 rotaciones posibles
* Las piezas caen automáticamente con el tiempo
* El jugador puede rotar y mover las piezas antes de que toquen fondo
* Al completar una línea, esta desaparece con un efecto visual de parpadeo
* El jugador debe mantenerse en juego el mayor tiempo posible, acumulando líneas completas sin permitir que las piezas superen la línea límite del tablero.

---

## 📚 Aplicación de temas vistos en clase
### Uso de variables:
* Se ocupa variables en todo el codigo, de distintos tipos y tanto como temporales como contastes en todo el codigo
### if, else, else-if
* Verificación de colisiones
* Condiciones de eliminación de líneas y game over

### Switch
* Control de teclas para manejar las acciones del jugador

```cpp
switch (tolower(_getch())) {
    case 'a': tmp.x--; break;
    case 'd': tmp.x++; break;
    case 's': tmp.y++; break;
    case 'w': tmp.r = (tmp.r + 1) % 4; break;
    case 'q': return 0;
}
```

###  For
* Recorridos del tablero
* Revisión de líneas completas
* Movimiento y dibujo de piezas

###  While y Do-While
* Bucle principal del juego con `while (true)`
* Parpadeo visual de líneas eliminadas con bucles internos

###  Funciones
* Uso de funciones para modularizar el código:
  * `draw()`, `checkCollision()`, `clearLines()`, `checkGameOver()`

---
##
## 🖼️ Mockup del tablero (pieza L cayendo)

```
+----------+
|          |
|     $    |
|     $    |
|     $$   |
|   ##     |
|  ####    |
| ##  ##   |
|##### ### |
|..........|
|####### # |
+----------+
```

* `$` → pieza en movimiento (forma de L)
* `#` → bloques fijos ya colocados
* `.` → línea de game over
* Espacios vacíos → celdas libres
## 🖼️ Mockup del tablero (pieza T cayendo)

```
+----------+
|          |
|    $$$   |
|     $    |
|   ##     |
|  ####    |
| ##  ##   |
|##### ### |
|..........|
|####### # |
+----------+
```

* `$` → pieza en movimiento (forma de T)
* `#` → bloques fijos ya colocados
* `.` → línea de game over
* Espacios vacíos → celdas libres
##  Consideraciones técnicas del desarrollo

* **Lenguaje de programación:** C++
* **Entorno de ejecución:** Consola de Windows (CMD o PowerShell)
* **Librerías utilizadas:**
  * `<iostream>`: Entrada y salida estándar
  * `<conio.h>`: Para detectar teclas presionadas sin necesidad de presionar Enter
  * `<windows.h>`: Para funciones como `Sleep()` y configuración del modo de salida (UTF-8)
  * `<ctime>`: Para inicializar la generación aleatoria de piezas
* **Estructura del código:**
  * Uso de `struct` para representar piezas (`Piece`)
  * Uso de arreglos 4D para las formas y rotaciones de las piezas (`shapes[4][4][4][4]`)
  * Separación de funciones por responsabilidad (`draw()`, `checkCollision()`, `clearLines()`, etc.)
* **Interacción:** El juego se controla exclusivamente con el teclado mediante teclas:
  * `A` → mover izquierda
  * `D` → mover derecha
  * `W` → rotar
  * `S` → bajar más rápido
  * `Q` → salir del juego
* **Innovación visual:**
  * Uso de efectos como parpadeo al eliminar líneas
  * Lograr dar el efecto de movimiento continuo ocupando solo la consola
  * Indicadores visuales de la zona de game over 


## 👥 Integrantes del equipo

  * Miguel Leonardo Nuñez Lemus 00070822
  * Diego Alexander Perez Reyes 00049825
  * Diego Gerardo Sánchez Valladares