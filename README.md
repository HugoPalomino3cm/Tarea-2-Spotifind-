<div align="center">

# Spotifind 🎵

<img src="https://upload.wikimedia.org/wikipedia/commons/1/19/Spotify_logo_without_text.svg" width="100px" alt="Logo de Spotify"/>

</div>

## 👥 Integrantes

| ![Felipe Romero](https://github.com/HugoPalomino3cm/Tarea-2-Spotifind-/blob/3ad963c848e352f40585c1d90a62faa69bf326c7/image.png)| ![Hugo Palomino](https://github.com/HugoPalomino3cm/Tarea-2-Spotifind-/blob/d48628f1a78cbea3c8fe0ca7ac8cb5dbf44ae92a/fotoMia.png) |
|:--:|:--:|
| **Felipe Romero** | **Hugo Palomino** |


---

## 🛠️ Requisitos Previos

- [Git](https://git-scm.com/) instalado.
- Compilador **GCC**.
- **PowerShell** (recomendado para Windows).
- Sistema operativo: Windows, Linux o macOS.
- [Visual Studio Code](https://code.visualstudio.com/) (recomendado).

---

## 📦 Clonar y Compilar el Proyecto

### 1. Clonar el Repositorio

Abre **Visual Studio Code** y sigue estos pasos:

- Abre la pestaña de **Control de Código Fuente** (ícono de Git).
- Haz clic en **"Clonar repositorio"** e ingresa la siguiente URL:

```bash
https://github.com/HugoPalomino3cm/Tarea-2-Spotifind-.git
```

- Elige una carpeta local para guardar el proyecto y ábrela en VSCode.

---

### 2. Compilar el Proyecto

Abre una terminal en VSCode (`Terminal -> New Terminal`) o usa PowerShell y ejecuta:

```bash
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
```

> ⚠️ Si estás usando PowerShell, añade esto para que se muestren bien los caracteres especiales:

```powershell
$OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::new()
```

---

### 3. Ejecutar el Programa

```bash
./tarea2
```

---

## 💡 Ejemplo de Uso

A continuación se muestra un ejemplo de cómo interactuar con Spotifind:

---

### 1) Cargar Canciones

**Descripción:** Carga el dataset de canciones desde un archivo CSV.  
**Nota:** Solo necesitas cargar las canciones una vez al iniciar el programa.

```text
Seleccione opción: 1
```

---

### 2) Buscar por Género

**Descripción:** Busca canciones por género musical.

```text
Seleccione opción: 2

Opciones:
  1) Buscar por género
  2) Mostrar todos los géneros disponibles
Ingrese su opción (1-2):

Seleccione opción: 1 (Buscar por género)

Ingrese: samba

```

---

### 3) Buscar por Artista

**Descripción:** Busca canciones por artista.

```text
Seleccione opción: 3
Ingrese artista: Kurt Cobain
```

---

### 4) Buscar por Tempo

**Descripción:** Busca canciones según el rango de BPM.

```text
Seleccione opción: 4
Seleccione categoría: 2 (Moderadas: 80-120 BPM)
```

---

### 5) Crear Lista de Reproducción

**Descripción:** Crea una nueva playlist vacía.

```text
Seleccione opción: 5
Ingrese el nombre de la nueva lista: Mis Favoritas 2019
Lista de reproducción 'Mis Favoritas 2019' creada exitosamente.
```

---

### 6) Agregar Canción a Lista

**Descripción:** Agrega una canción existente a una playlist usando su ID.

```text
Seleccione opción: 6
Ingrese el nombre de la nueva lista: Mis Favoritas 2019

══════════════════ Listas de Reproducción Disponibles ══════════════════
  1) Mis Favoritas 2019
════════════════════════════════════════════════════════════
Ingrese el nombre de la lista de reproducción: Mis Favoritas 2019
Ingrese el ID de la canción: 19
```

---

### 7) Mostrar Canciones de Lista

**Descripción:** Muestra las canciones dentro de una playlist.

```text
Seleccione opción: 7
Ingrese el nombre de la lista de reproducción: Mis Favoritas 2019

══════════════════ Lista de Canciones ══════════════════
Canción 1
  Título: Photograph
  Álbum: Cover Sessions, Vol. 4
  Género: Acoustic
  Tempo: 107.95 BPM
Artistas: Boyce Avenue, Bea Miller
-----------------------------------------------------
```

---

### 8) Salir

**Descripción:** Cierra el programa.

```text
Seleccione opción: 8
```

---

## ⚠️ Posibles Errores Comunes (TODO)

- Si el programa se ejecuta en un sistema Windows sin una terminal compatible con códigos ANSI (como `cmd.exe` sin configuración especial), los colores no se mostrarán correctamente y podrían verse como texto extraño (por ejemplo: `\033[1;32m`).
- Mezclar scanf con fgets puede dejar caracteres residuales en el buffer.


# Contribuciones

## Aportes individuales

### Hugo Palomino
- Diseñó la **interfaz visual** con colores ANSI.
- Implementó la **barra de progreso** (`mostrar_barra_progreso`).
- Desarrolló el **menú principal** (`mostrarMenuPrincipal`).
- Lideró `cargar_canciones` para una lectura eficiente del archivo CSV.

### Felipe Romero
- Optimizó `show_songs` y `show_artists` con **paginación** y presentación mejorada (relacionados con `buscar_por_genero` y `buscar_por_artista`).
- Implementó `capitalize_first` para géneros, asegurando que no se usen mayúsculas incorrectas.
- Aseguró la **modularidad** del código.

### Trabajo en conjunto
- Implementación de `liberar_memoria` para una gestión eficiente de recursos.
- Desarrollo de las funciones de búsqueda: `buscar_por_genero`, `buscar_por_artista`, `buscar_por_tempo`.
- Implementación de la gestión de listas de reproducción: `crear_lista_reproduccion`, `agregar_cancion_lista`, `mostrar_canciones_lista`.
- Colaboración en `mostrar_playlists`, garantizando un programa **funcional** y **visualmente atractivo**.

