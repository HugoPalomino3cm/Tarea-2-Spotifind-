# Tarea-2-Spotifind-

![Logo de Spotify](https://upload.wikimedia.org/wikipedia/commons/1/19/Spotify_logo_without_text.svg)

## Integrantes
1. Felipe Romero
2. Hugo Palomino

## Pasos para Compilar y Ejecutar el Programa

### Requisitos Previos
- **Git** instalado.
- **Compilador GCC**.
- **PowerShell** (para Windows; opcional para Linux/macOS).
- Sistema operativo: Windows, Linux o macOS.
- **Visual Studio Code** (recomendado).

## Descripción del Proyecto
Spotifind es un sistema de gestión de canciones que permite buscar y organizar música por diferentes criterios como género, artista y tempo. Además, permite crear listas de reproducción personalizadas.

### Instrucciones para Compilar

1. **Clonar el Repositorio en Visual Studio Code (VSCode):**
   - Asegúrate de tener la extensión de Git instalada en VSCode.
   - Haz clic en la pestaña "Source Control" (ícono de Git) en la barra lateral.
   - Selecciona "Clone Repository" e ingresa la URL del repositorio:
     ```bash
        https://github.com/HugoPalomino3cm/Tarea-2-Spotifind-.git
   - Elige una carpeta local para guardar el proyecto y abre la carpeta clonada en VSCode.

2. **Compilar el Programa:**
   - Abre una terminal en VSCode (`Terminal -> New Terminal`) o usa PowerShell.
   - Ejecuta los siguientes comandos en la terminal:
     - Para generar el ejecutable:  
       ```bash
          gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
       
     - Para mostrar correctamente las tildes (en PowerShell):
       ```bash
         $OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::new()
      
      - Para ejecutar el programa:
       ```bash
          ./tarea2

### FALTA AGREGAR POSIBLES FALLAS PALTADEUS LO ESCRIBO POR SI ESQ ME OLVIDO :V


### Ejemplo de Uso

Un ejemplo de cómo interactuar con el sistema de gestión musical Spotifind:

---

#### **1) Cargar Canciones**  
**Descripción:** Carga el dataset de canciones desde el archivo CSV.  
**Nota:** `Solo necesitas cargar las canciones una vez al iniciar el programa.`  
- **Seleccione opción:** 1  

---

#### **2) Buscar por Género**  
**Descripción:** Busca canciones por género musical.  
- **Seleccione opción:** 2  
- **Ingrese género:** pop

---

#### **3) Buscar por Artista**  
**Descripción:** Busca todas las canciones de un artista específico.  
- **Seleccione opción:** 3  
- **Ingrese artista:** Queen

---

#### **4) Buscar por Tempo**  
**Descripción:** Busca canciones por rango de tempo (BPM).  
- **Seleccione opción:** 4  
- **Seleccione categoría:** 2 (Moderadas: 80-120 BPM)

---

#### **5) Crear Lista de Reproducción**  
**Descripción:** Crea una nueva lista de reproducción vacía.  
- **Seleccione opción:** 5  
- **Ingrese nombre lista:** Mis Favoritas 2024

---

#### **6) Agregar Canción a Lista**  
**Descripción:** Añade una canción a una lista existente usando su ID.  
- **Seleccione opción:** 6  
- **Seleccione lista:** 1 (Mis Favoritas 2024)  
- **Ingrese ID canción:** 5

---

#### **7) Mostrar Canciones de Lista**  
**Descripción:** Muestra todas las canciones en una lista de reproducción.  
- **Seleccione opción:** 7  
- **Seleccione lista:** 1 (Mis Favoritas 2024)

---

#### **8) Salir**  
**Descripción:** Finaliza el programa.  
- **Seleccione opción:** 8  
