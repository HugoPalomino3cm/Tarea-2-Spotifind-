# Tarea-2-Spotifind-

![Logo del Proyecto](8bf28eb00e71fffd81e7339ce70b2a0cc51fac2b)

## Pasos para Compilar y Ejecutar el Programa

### Requisitos Previos
- **Git** instalado.
- **Compilador GCC**.
- **PowerShell** (para Windows; opcional para Linux/macOS).
- Sistema operativo: Windows, Linux o macOS.
- **Visual Studio Code** (recomendado).

### Descripción General del Sistema
Este sistema permite gestionar la atención de clientes en un servicio técnico, priorizando su atención según el nivel de urgencia del problema reportado.

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


## Uso del Programa
