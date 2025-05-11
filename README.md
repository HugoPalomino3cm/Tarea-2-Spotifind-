# Spotifind 🎵

![Logo de Spotify](https://upload.wikimedia.org/wikipedia/commons/1/19/Spotify_logo_without_text.svg)

## 👥 Integrantes

![Felipe Romero](falta)
**Felipe Romero**

---

![Hugo Palomino]([img/hugo.jpg](https://github.com/HugoPalomino3cm/Tarea-2-Spotifind-/blob/b4fc487b2f812e7d62d7843ac086905d7cbe7549/tralaleroPrueba.jpg))
**Hugo Palomino**


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
Ingrese género: pop
```

---

### 3) Buscar por Artista

**Descripción:** Busca canciones por artista.

```text
Seleccione opción: 3
Ingrese artista: Queen
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
Ingrese nombre lista: Mis Favoritas 2018
```

---

### 6) Agregar Canción a Lista

**Descripción:** Agrega una canción existente a una playlist usando su ID.

```text
Seleccione opción: 6
Seleccione lista: 1 (Mis Favoritas 2018)
Ingrese ID canción: 5
```

---

### 7) Mostrar Canciones de Lista

**Descripción:** Muestra las canciones dentro de una playlist.

```text
Seleccione opción: 7
Seleccione lista: 1 (Mis Favoritas 2018)
```

---

### 8) Salir

**Descripción:** Cierra el programa.

```text
Seleccione opción: 8
```

---

## ⚠️ Posibles Errores Comunes (TODO)

ñau amargaita


