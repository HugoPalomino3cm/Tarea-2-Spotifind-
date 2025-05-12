// Inclusión de bibliotecas estándar y personalizadas
#include "tdas/extra.h"      
#include "tdas/list.h"       
#include "tdas/map.h"        
#include "tdas/HashMap.h"    
#include <stdio.h>           
#include <stdlib.h>         
#include <string.h>          
#include <ctype.h>           

// Definición de colores ANSI para salida en consola con formato
#define ROJO    "\x1b[31m"   // Color rojo para mensajes de error
#define VERDE   "\x1b[32m"   // Color verde para mensajes de éxito
#define AMARILLO "\x1b[33m"  // Color amarillo para advertencias o separadores
#define AZUL    "\x1b[34m"   // Color azul para menús y prompts
#define MAGENTA "\x1b[35m"   // Color magenta para énfasis en listas
#define CYAN    "\x1b[36m"   // Color cian para información secundaria
#define RESET   "\x1b[0m"    // Restablece el color a predeterminado

// Estructura para representar una canción con todos sus atributos
typedef struct {
  char id[100];           // Identificador único de la canción
  List *artists;          // Lista de artistas para una canción (puede haber uno más de uno)
  char album_name[100];   // Nombre del álbum al que pertenece la canción
  char track_name[100];   // Nombre de la canción
  float tempo;            // Tempo de la canción en BPM (beats per minute)
  char track_genre[100];  // Género musical de la canción
} Song;

// Estructura para representar una lista de reproducción
typedef struct {
  char name[100];         // Nombre de la lista de reproducción
  List *songs;            // Lista de canciones incluidas en la playlist
} Playlist;

/* 
 * Función: mostrarMenuPrincipal
 * Descripción: Muestra el menú principal del programa con opciones numeradas
 *              para interactuar con las funcionalidades de Spotifind.
 *              Limpia la pantalla y utiliza colores para mejorar la presentación.
 */
void mostrarMenuPrincipal() {
  limpiarPantalla(); // Limpia la consola para una presentación limpia
  printf("%s╔════════════════════════════════════════╗\n", AZUL);
  printf("%s║            🎵 Spotifind 🎵             ║\n", AZUL);
  printf("%s╚════════════════════════════════════════╝\n", AZUL);
  
  // Opciones del menú con colores para destacarlas
  printf("%s  1) Cargar Canciones%s\n", VERDE, RESET);
  printf("%s  2) Buscar por género%s\n", VERDE, RESET);
  printf("%s  3) Buscar por artista%s\n", VERDE, RESET);
  printf("%s  4) Buscar por tempo%s\n", VERDE, RESET);
  printf("%s  5) Crear lista de reproducción%s\n", VERDE, RESET);
  printf("%s  6) Agregar canción a la lista%s\n", VERDE, RESET);
  printf("%s  7) Mostrar canciones de una lista%s\n", VERDE, RESET);
  printf("%s  8) Salir%s\n", ROJO, RESET);
  
  printf("%s══════════════════════════════════════════%s\n", AZUL, RESET);
}

/* 
 * Función: is_equal_str
 * Descripción: Compara dos cadenas de caracteres usando strcmp.
 *              Retorna 1 si son iguales, 0 si son distintas.
 * Parámetros:
 *   - key1: Primer puntero a una cadena
 *   - key2: Segundo puntero a una cadena
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/* 
 * Función: is_equal_int
 * Descripción: Compara dos enteros recibidos como punteros.
 *              Retorna 1 si son iguales, 0 si son distintos.
 * Parámetros:
 *   - key1: Primer puntero a un entero
 *   - key2: Segundo puntero a un entero
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

/* 
 * Función: to_lowercase
 * Descripción: Convierte una cadena de caracteres a minúsculas para facilitar comparaciones a futuro.
 *              Modifica la cadena original.
 * Parámetros:
 *   - str: Cadena a convertir
 */
void to_lowercase(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}

/* 
 * Función: capitalize_first
 * Descripción: Capitaliza la primera letra de una cadena y copia el resultado
 *              a una cadena de salida. Usada para formalizar los print en algunos casos
 * Parámetros:
 *   - str: Cadena de entrada
 *   - output: Cadena de salida donde se almacena el resultado
 */
void capitalize_first(char *str, char *output) {
  strcpy(output, str); // Copia la cadena original
  if (output[0]) {    // Evalpua si la cadena no está vacía
    output[0] = toupper(output[0]); // Capitaliza la primera letra
  }
}

/* 
 * Función: mostrar_barra_progreso
 * Descripción: Muestra una barra de progreso en consola que indica el avance
 *              de la tarea cargar canciones). Solo actualiza si el
 *              porcentaje cambia o al completar la tarea.
 * Parámetros:
 *   - actual: Número de elementos procesados
 *   - total: Número total de elementos
 */
void mostrar_barra_progreso(int actual, int total) {
  static int ultimo_porcentaje_mostrado = -1; // Almacena el último porcentaje mostrado
  int ancho_barra = 30; // Ancho de la barra en caracteres
  float porcentaje = (float)actual / total; // Calcula el porcentaje de avance
  int porcentaje_entero = (int)(porcentaje * 100); // Convierte a entero

  if (actual == total) { // Forzar 100% al completar
    porcentaje_entero = 100;
  }

  // Actualizar solo si el porcentaje cambió o se completó la tarea
  if (porcentaje_entero != ultimo_porcentaje_mostrado || actual == total) {
    int completado = (int)((float)porcentaje_entero / 100 * ancho_barra); // Calcular bloques completados

    if (porcentaje_entero == 100) { // Asegurar barra llena al 100%
      completado = ancho_barra;
    }

    // Imprimir barra de progreso
    printf("\r%s[", CYAN);
    for (int i = 0; i < completado; i++) printf("🟩"); // Bloques completados
    for (int i = completado; i < ancho_barra; i++) printf("⬜"); // Bloques pendientes
    printf("] %3d%% [%d/%d]%s", porcentaje_entero, actual, total, RESET);
    fflush(stdout); // Forzar la impresión inmediata

    ultimo_porcentaje_mostrado = porcentaje_entero; // Actualizar porcentaje mostrado
  }
}

/* 
 * Función: cargar_canciones
 * Descripción: Carga canciones desde un archivo CSV y las organiza en mapas
 *              según ID, género, artista y tempo. Muestra una barra de progreso
 *              y estadísticas de carga.
 * Parámetros:
 *   - canciones_byid: Mapa para almacenar canciones por ID
 *   - canciones_bygenres: Mapa para almacenar listas de canciones por género
 *   - canciones_byartist: Mapa para almacenar listas de canciones por artista
 *   - canciones_by_tempo: Mapa para almacenar listas de canciones por categoría de tempo
 */
void cargar_canciones(HashMap *canciones_byid, HashMap *canciones_bygenres, HashMap *canciones_byartist, HashMap *canciones_by_tempo) {
  // Se abre el archivo CSV en modo lectura
  FILE *archivo = fopen("data/song_dataset_.csv", "r");
  if (archivo == NULL) {
    printf("%sError: No se pudo abrir el archivo de datos.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  // Contar líneas válidas para calcular el total (excluyendo encabezado y líneas vacías)
  int total_lineas = 0;
  char buffer[1024];
  int is_first_line = 1;
  while (fgets(buffer, sizeof(buffer), archivo)) {
    if (is_first_line) {
      is_first_line = 0;
      continue; // Saltar el encabezado
    }
    if (strlen(buffer) > 1 && buffer[0] != '\n' && strspn(buffer, " \t\n") != strlen(buffer)) {
      total_lineas++; // Contar líneas no vacías
    }
  }
  if (total_lineas <= 0) {
    printf("%sError: El archivo está vacío o solo contiene el encabezado.%s\n", ROJO, RESET);
    fclose(archivo);
    presioneTeclaParaContinuar();
    return;
  }
  rewind(archivo); // Volver al inicio del archivo

  // Leer el encabezado del CSV
  char **campos = leer_linea_csv(archivo, ',');
  if (campos == NULL) {
    printf("%sError: No se pudo leer el encabezado del archivo.%s\n", ROJO, RESET);
    fclose(archivo);
    presioneTeclaParaContinuar();
    return;
  }

  // Se inicializan contadores para mostrar al final una cantidad de canciones cargadas
  int count = 0; // Canciones procesadas
  int lentas = 0, moderadas = 0, rapidas = 0; // Contadores de canciones por categoría de tempo

  mostrar_barra_progreso(count, total_lineas); // Mostrar barra inicial

  // Se procesan las líneas del CSV
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    if (!campos[0] || strlen(campos[0]) == 0) {
      continue; // Se saltan las líneas vacías o sin ID
    }

    // Se asigna memoria para una nueva canción
    Song *cancion = (Song *)malloc(sizeof(Song));
    if (cancion == NULL) {
      printf("\n%sError: No se pudo asignar memoria para la canción %d.%s\n", ROJO, count + 1, RESET);
      fclose(archivo);
      presioneTeclaParaContinuar();
      return;
    }

    // Copiar ID de la canción (asegurando terminación nula de caracteres)
    strncpy(cancion->id, campos[0] ? campos[0] : "", sizeof(cancion->id) - 1);
    cancion->id[sizeof(cancion->id) - 1] = '\0';

    // Se procesa la lista de artistas
    if (campos[2] == NULL || strlen(campos[2]) == 0) {
      cancion->artists = list_create(); // Se crea una lista vacía si no hay artistas
    } else {
      cancion->artists = split_string(campos[2], ";"); // Se separan artistas por ';'
      if (cancion->artists == NULL || list_first(cancion->artists) == NULL) {
        cancion->artists = list_create();
        char *artista_copy = strdup(campos[2]); // Se usa el campo completo si split falla
        list_pushBack(cancion->artists, artista_copy);
      }
    }

    // Se copia el nombre del álbum
    strncpy(cancion->album_name, campos[3] ? campos[3] : "", sizeof(cancion->album_name) - 1);
    cancion->album_name[sizeof(cancion->album_name) - 1] = '\0';

    // Se copia el nombre de la pista
    strncpy(cancion->track_name, campos[4] ? campos[4] : "", sizeof(cancion->track_name) - 1);
    cancion->track_name[sizeof(cancion->track_name) - 1] = '\0';

    // Se convierte a tempo a float (0.0 si no está disponible)
    cancion->tempo = (campos[18] && strlen(campos[18]) > 0) ? atof(campos[18]) : 0.0;

    // Se copia género y convertir a minúsculas
    strncpy(cancion->track_genre, campos[20] ? campos[20] : "", sizeof(cancion->track_genre) - 1);
    cancion->track_genre[sizeof(cancion->track_genre) - 1] = '\0';
    to_lowercase(cancion->track_genre);

    // Se inserta la canción en el mapa por ID
    insertMap(canciones_byid, cancion->id, cancion);
    
    // Organizar por género utilizando la función searchmap para búsquedas optimizadas
    Pair *genre_pair = searchMap(canciones_bygenres, cancion->track_genre);
    if (genre_pair == NULL) {
      List *new_list = list_create(); // Crear nueva lista para el género en caso de no existir ya en el mapa
      if (new_list == NULL) {
        printf("\n%sError: No se pudo crear una lista para el género %s.%s\n", ROJO, cancion->track_genre, RESET);
        free(cancion);
        fclose(archivo);
        presioneTeclaParaContinuar();
        return;
      }
      list_pushBack(new_list, cancion); 
      char *genre_copy = strdup(cancion->track_genre); // Se copia el género en minúsculas para facilitar futuras comparaciones
      insertMap(canciones_bygenres, genre_copy, new_list); //En caso de no existir el género se inserta en el mapa
    } else {
      List *genre_list = (List *)genre_pair->value;
      list_pushBack(genre_list, cancion); // Se añade a la lista existente 
    }

    // Organizar por artista
    char *artista = (char *)list_first(cancion->artists);
    while (artista != NULL) {
      char *artista_lower = strdup(artista); // Copiar artista
      to_lowercase(artista_lower); // Convertir a minúsculas
      Pair *artist_pair = searchMap(canciones_byartist, artista_lower); // Se busca el artista en el mapa, en caso de no existir se crea una lista nueva dentro del mapa
      if (artist_pair == NULL) {
        List *new_list = list_create(); // Crear nueva lista para el artista
        if (new_list == NULL) {
          printf("\n%sError: No se pudo crear una lista para el artista %s.%s\n", ROJO, artista, RESET);
          free(cancion);
          free(artista_lower);
          fclose(archivo);
          presioneTeclaParaContinuar();
          return;
        }
        list_pushBack(new_list, cancion);
        insertMap(canciones_byartist, artista_lower, new_list);
      } else {
        List *artist_list = (List *)artist_pair->value;
        list_pushBack(artist_list, cancion); // En caso contrario se añade a la lista existente
        free(artista_lower); // Se libera copia
      }
      artista = (char *)list_next(cancion->artists);
    }

    // Se organiza por categoría de tempo
    char *categoria;
    if (cancion->tempo < 80.0) {
      categoria = "Lentas";
      lentas++;
    } else if (cancion->tempo >= 80.0 && cancion->tempo <= 120.0) {
      categoria = "Moderadas";
      moderadas++;
    } else {
      categoria = "Rápidas";
      rapidas++;
    }
    Pair *pair_tempo = searchMap(canciones_by_tempo, categoria);
    if (pair_tempo == NULL) {
      List *new_list = list_create(); // Se crea una nueva lista para la categoría
      if (new_list == NULL) {
        printf("\n%sERROR: NO SE PUDO CREAR UNA LISTA PARA LA CATEGORÍA %s%s\n", ROJO, categoria, RESET);
        free(cancion);
        fclose(archivo);
        presioneTeclaParaContinuar();
        return;
      }
      list_pushBack(new_list, cancion);
      insertMap(canciones_by_tempo, categoria, new_list);
    } else {
      list_pushBack((List *)pair_tempo->value, cancion); // En caso de que ya exista solamente se ingresa 
    }

    count++;
    mostrar_barra_progreso(count, total_lineas); // Actualizar barra de progreso
  }

  fclose(archivo); // Cerrar archivo

  mostrar_barra_progreso(count, count); // Mostrar barra al 100%

  // Se muestra un resumen de carga
  printf("\n");
  printf("%sÉxito: Se cargaron %d canciones correctamente.%s\n", VERDE, count, RESET);
  printf("%sCanciones por tempo: Lentas=%d, Moderadas=%d, Rápidas=%d%s\n", CYAN, lentas, moderadas, rapidas, RESET);
  presioneTeclaParaContinuar();
}

/* 
 * Función: show_artists
 * Descripción: Muestra la lista de artistas de una canción, separados por comas.
 *              Maneja casos donde la lista está vacía o es nula.
 * Parámetros:
 *   - list_artists: Lista de artistas de una canción (cadenas)
 */
void show_artists(List *list_artists) {
  if (list_artists == NULL) {
    printf("%sArtistas: %sDesconocido%s\n", CYAN, AMARILLO, RESET);
    return;
  }

  printf("%sArtistas: ", CYAN);
  char *artist = (char *)list_first(list_artists);
  if (artist == NULL) {
    printf("%sDesconocido%s\n", AMARILLO, RESET);
    return;
  }
  while (artist != NULL) {
    printf("%s%s", VERDE, artist);
    artist = (char *)list_next(list_artists);
    if (artist != NULL) {
      printf("%s, ", RESET); // Separador entre artistas
    }
  }
  printf("%s\n", RESET);
}

/* 
 * Función: show_songs
 * Descripción: Muestra una lista de canciones por páginas, con detalles como título,
 *              álbum, género, tempo y artistas. Permite navegar entre páginas.
 *              Función creada para optimizar líneas de codigo en funciones donde
 *              se deba mostrar por pantalla los detalles de las canciones.
 * Parámetros:
 *   - songs: Lista de canciones a mostrar
 *   - initial_page: Página inicial para mostrar
 *   - songs_per_page: Número de canciones por página
 */
void show_songs(List *songs, int initial_page, int songs_per_page) {
  //Se evalúa si la lista o el primer elemento es nulo para finalizar la función
  if (songs == NULL || list_first(songs) == NULL) { 
    limpiarPantalla();
    printf("%sNo se encontraron canciones.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  // Se calcula el total de canciones y páginas
  int total_songs = list_size(songs);
  int total_pages = (total_songs + songs_per_page - 1) / songs_per_page;

  int current_page = initial_page;
  char nav_option;

  // Bucle para navegar entre páginas
  do {
    limpiarPantalla();
    printf("\n%s══════════════════ Lista de Canciones ══════════════════%s\n", AZUL, RESET);

    // Mostrar canciones de la página actual
    Song *cancion = (Song *)list_first(songs);
    int song_number = 1;
    int current_song = 1;
    int start_song = (current_page - 1) * songs_per_page + 1;
    int end_song = current_page * songs_per_page;

    // Avanzar hasta la primera canción de la página
    while (cancion != NULL && current_song < start_song) {
      cancion = (Song *)list_next(songs);
      current_song++;
      song_number++;
    }

    // Mostrar canciones de la página actual
    while (cancion != NULL && current_song <= end_song) {
      printf("%sCanción %d%s\n", MAGENTA, song_number, RESET);
      printf("%s  Título: %s%s%s\n", CYAN, VERDE, cancion->track_name[0] ? cancion->track_name : "Desconocido", RESET);
      printf("%s  Álbum: %s%s%s\n", CYAN, VERDE, cancion->album_name[0] ? cancion->album_name : "Desconocido", RESET);
      // Se capitaliza la primera letra del género para mejorar presentación
      char display_genre[100];
      capitalize_first(cancion->track_genre, display_genre);
      printf("%s  Género: %s%s%s\n", CYAN, VERDE, display_genre[0] ? display_genre : "Desconocido", RESET);
      printf("%s  Tempo: %s%.2f BPM%s\n", CYAN, VERDE, cancion->tempo, RESET);
      show_artists(cancion->artists);
      printf("%s-----------------------------------------------------%s\n", AMARILLO, RESET);
      cancion = (Song *)list_next(songs);
      current_song++;
      song_number++;
    }

    // Mostrar información de paginación y opciones
    printf("%sPágina %d de %d%s\n", AZUL, current_page, total_pages, RESET);
    printf("%sOpciones:%s\n", AZUL, RESET);
    if (current_page > 1) {
      printf("%s  1) Página anterior%s\n", AZUL, RESET);
    }
    if (current_page < total_pages) {
      printf("%s  2) Página siguiente%s\n", AZUL, RESET);
    }
    printf("%s  3) Volver%s\n", ROJO, RESET);

    // Capturar opción del usuario
    printf("%sIngrese su opción: %s", AZUL, RESET);
    scanf(" %c", &nav_option);
    while (getchar() != '\n');

    // Procesar navegación
    if (nav_option == '1' && current_page > 1) {
      current_page--;
    } else if (nav_option == '2' && current_page < total_pages) {
      current_page++;
    } else if (nav_option != '3') {
      printf("%sOpción inválida.%s\n", ROJO, RESET);
      presioneTeclaParaContinuar();
    }
  } while (nav_option != '3');
}

/* 
 * Función: buscar_por_tempo
 * Descripción: Permite buscar canciones por categoría de tempo (Lentas,
 *              Moderadas, Rápidas) y muestra los resultados usando show_songs.
 * Parámetros:
 *   - canciones_by_tempo: Mapa con listas de canciones por categoría de tempo
 */
void buscar_por_tempo(HashMap *canciones_by_tempo) {
  limpiarPantalla();
  int opcion;
  
  // Mostrar opciones de tempo
  printf("%sSeleccione la categoría de tempo:%s\n", AZUL, RESET);
  printf("%s1) Lentas (menos de 80 BPM)%s\n", AZUL, RESET);
  printf("%s2) Moderadas (80-120 BPM)%s\n", AZUL, RESET);
  printf("%s3) Rápidas (mayor a 120 BPM)%s\n", AZUL, RESET);
  printf("%sIngrese su opción (1-3): %s", AZUL, RESET);
  scanf("%d", &opcion);
  while (getchar() != '\n');

  // Seleccionar categoría según opción
  char *categoria;
  switch (opcion) {
    case 1:
      categoria = "Lentas";
      break;
    case 2:
      categoria = "Moderadas";
      break;
    case 3:
      categoria = "Rápidas";
      break;
    default:
      printf("%sOpción inválida.%s\n", ROJO, RESET);
      presioneTeclaParaContinuar();
      return;
  }

  // Buscar canciones en la categoría
  Pair *searched = searchMap(canciones_by_tempo, categoria);
  if (searched == NULL) {
    printf("%sNo se encontraron canciones en la categoría '%s'.%s\n", ROJO, categoria, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  List *canciones = (List *)searched->value;
  printf("\n%sCanciones con categoría de tempo '%s':%s\n", VERDE, categoria, RESET);
  show_songs(canciones, 1, 100); // Mostrar hasta 100 canciones por página
}

/* 
 * Función: buscar_por_genero
 * Descripción: Permite buscar canciones por género o mostrar todos los géneros
 *              disponibles. La búsqueda es insensible a mayúsculas/minúsculas
 *              para controlar el manejo de errores y la búsqueda sea eficiente
 *              Ejemplo: rock sería lo mismo que rOCK
 * Parámetros:
 *   - canciones_bygenres: Mapa con listas de canciones por género
 */
void buscar_por_genero(HashMap *canciones_bygenres) {
  limpiarPantalla();
  
  char opcion;
  // Mostrar opciones: buscar o listar géneros
  printf("%sOpciones:%s\n", AZUL, RESET);
  printf("%s  1) Buscar por género%s\n", AZUL, RESET);
  printf("%s  2) Mostrar todos los géneros disponibles%s\n", AZUL, RESET);
  printf("%sIngrese su opción (1-2): %s", AZUL, RESET);
  scanf(" %c", &opcion);
  while (getchar() != '\n');

  if (opcion == '2') {
    limpiarPantalla();
    printf("\n%s══════════════════ Géneros Disponibles ══════════════════%s\n", AZUL, RESET);
    Pair *current = firstMap(canciones_bygenres);
    if (current == NULL) {
      printf("%sNo hay géneros disponibles.%s\n", ROJO, RESET);
    } else {
      int index = 1;
      while (current != NULL) {
        char display_genre[100];
        capitalize_first((char *)current->key, display_genre); // Capitalizar para mostrar
        printf("%s  %d) %s%s%s\n", AZUL, index, MAGENTA, display_genre, RESET);
        current = nextMap(canciones_bygenres);
        index++;
      }
    }
    printf("%s════════════════════════════════════════════════════════════%s\n", AZUL, RESET);
    presioneTeclaParaContinuar();
    return;
  } else if (opcion != '1') {
    printf("%sOpción inválida.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  // Buscar por género
  char genero[100];
  printf("%sIngrese el género de la canción: %s", AZUL, RESET);
  printf("%s(puede ver los géneros disponibles usando previamente la opción 2)%s\n", AZUL, RESET);
  scanf("%s", genero);
  while (getchar() != '\n');
  to_lowercase(genero); // Convertir entrada a minúsculas para mejorar la búsqueda 

  Pair *searched_genre = searchMap(canciones_bygenres, genero); //Búsqueda del Pair en el mapa 
  if (searched_genre != NULL) {
    List *canciones = (List *)searched_genre->value;
    char display_genre[100];
    capitalize_first(genero, display_genre); // Se capitaliza la primera letra 
    printf("\n%sCanciones del género '%s':%s\n", VERDE, display_genre, RESET);
    show_songs(canciones, 1, 10); // Mostrar hasta 10 canciones por página
  } else {
    printf("%sNo se encontraron canciones del género '%s'.%s\n", ROJO, genero, RESET);
    presioneTeclaParaContinuar();
  }
}

/* 
 * Función: buscar_por_artista
 * Descripción: Busca canciones por nombre de artista (insensible a mayúsculas/
 *              minúsculas) y muestra los resultados. Se tranforma la entrada del usuario
 *              a todo minúsculas para mejorar la búsqueda de artistas en el mapa.
 *              Ejemplo: Taylor Swift sería lo mismo que tAyLoR SwIFT
 * Parámetros:
 *   - canciones_byartist: Mapa con listas de canciones por artista
 */
void buscar_por_artista(HashMap *canciones_byartist) {
  limpiarPantalla();
  
  char artista[100];
  printf("%sIngrese el nombre del artista: %s", AZUL, RESET);
  scanf(" %[^\n]", artista); // Se lee línea completa
  while (getchar() != '\n');
  to_lowercase(artista); // Se convierte a minúsculas

  Pair *pair = searchMap(canciones_byartist, artista);
  if (pair != NULL) {
    List *songs = (List *)pair->value;
    printf("\n%sCanciones del artista '%s':%s\n", VERDE, artista, RESET);
    show_songs(songs, 1, 10); // Mostrar hasta 10 canciones por página
  } else {
    printf("%sNo se encontraron canciones del artista '%s'.%s\n", ROJO, artista, RESET);
    presioneTeclaParaContinuar();
  }
}

/* 
 * Función: crear_lista_reproduccion
 * Descripción: Crea una nueva lista de reproducción con un nombre único.
 * Parámetros:
 *   - playlists: Mapa donde se almacenan las listas de reproducción
 */
void crear_lista_reproduccion(HashMap *playlists) {
  limpiarPantalla();

  char nombre[100];
  printf("%sIngrese el nombre de la nueva lista: %s", AZUL, RESET);
  scanf(" %[^\n]", nombre); // Leer línea completa
  while (getchar() != '\n');

  if (strlen(nombre) == 0) {
    printf("%sError: El nombre de la lista no puede estar vacío.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  if (searchMap(playlists, nombre) != NULL) {
    printf("%sError: Ya existe una lista de reproducción con el nombre '%s'.%s\n", ROJO, nombre, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  // Se asigna memoria para la nueva playlist
  Playlist *nueva_playlist = (Playlist *)malloc(sizeof(Playlist));
  if (nueva_playlist == NULL) {
    printf("%sError: No se pudo asignar memoria para la nueva lista de reproducción.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  // Se inicializa la playlist
  strncpy(nueva_playlist->name, nombre, sizeof(nueva_playlist->name) - 1);
  nueva_playlist->name[sizeof(nueva_playlist->name) - 1] = '\0';
  nueva_playlist->songs = list_create();
  if (nueva_playlist->songs == NULL) {
    printf("%sError: No se pudo crear la lista de canciones para la playlist.%s\n", ROJO, RESET);
    free(nueva_playlist);
    presioneTeclaParaContinuar();
    return;
  }

  // Se inserta la playlist en el mapa de playlists
  insertMap(playlists, nueva_playlist->name, nueva_playlist);
  printf("%sLista de reproducción '%s' creada exitosamente.%s\n", VERDE, nombre, RESET);
  presioneTeclaParaContinuar();
}

/* 
 * Función: mostrar_playlists
 * Descripción: Muestra todas las listas de reproducción disponibles con un índice.
 * Se crea para que al usuario se le pueda facilitar ver que playlists tiene 
 * disponibles y no tener que estar preocupado si se equivoca en algun carácter
 * ya que mientras ingresa el nombre de una playlist podrá ver como se llama.
 * Parámetros:
 *   - playlists: Mapa con las listas de reproducción
 */
void mostrar_playlists(HashMap *playlists) {
  printf("\n%s══════════════════ Listas de Reproducción Disponibles ══════════════════%s\n", AZUL, RESET);
  //Se inicializa el current para recorrer el mapa
  Pair *current = firstMap(playlists);
  int index = 1;
  if (current == NULL) {
    printf("%sNo hay listas de reproducción creadas.%s\n", ROJO, RESET);
  } else {
    while (current != NULL) {
      Playlist *playlist = (Playlist *)current->value;
      printf("%s  %d) %s%s%s\n", AZUL, index, MAGENTA, playlist->name, RESET);
      //Pasa al siguiente elemento del mapa
      current = nextMap(playlists);
      index++;
    }
  }
  printf("%s════════════════════════════════════════════════════════════%s\n", AZUL, RESET);
}

/* 
 * Función: agregar_cancion_lista
 * Descripción: Agrega una canción existente a una lista de reproducción,
 *              verificando que no esté duplicada.
 *              Se le pide al usuario el nombre de la lista mientras puede ver
 *              las opciones por pantalla y poder agregar una canción según su ID.
 * Parámetros:
 *   - canciones_byid: Mapa con canciones por ID
 *   - playlists: Mapa con listas de reproducción
 */
void agregar_cancion_lista(HashMap *canciones_byid, HashMap *playlists) {
  limpiarPantalla();

  // Verificar si hay playlists
  if (firstMap(playlists) == NULL) {
    printf("%sError: No hay listas de reproducción creadas.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  mostrar_playlists(playlists); // Mostrar playlists disponibles

  char nombre_lista[100];
  printf("%sIngrese el nombre de la lista de reproducción: %s", AZUL, RESET);
  scanf(" %[^\n]", nombre_lista);
  while (getchar() != '\n');

  // Buscar playlist
  Pair *playlist_pair = searchMap(playlists, nombre_lista);
  if (playlist_pair == NULL) {
    printf("%sError: La lista '%s' no existe.%s\n", ROJO, nombre_lista, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  Playlist *playlist = (Playlist *)playlist_pair->value;

  // Se solicita ID de la canción
  char id_cancion[100];
  printf("%sIngrese el ID de la canción: %s", AZUL, RESET);
  scanf("%[^\n]", id_cancion);
  while (getchar() != '\n');

  // Buscar canción
  Pair *song_pair = searchMap(canciones_byid, id_cancion);
  if (song_pair == NULL) {
    printf("%sError: La canción con ID '%s' no existe.%s\n", ROJO, id_cancion, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  Song *cancion = (Song *)song_pair->value;

  // Verificar si la canción ya está en la playlist
  Song *current_song = (Song *)list_first(playlist->songs);
  while (current_song != NULL) {
    if (strcmp(current_song->id, cancion->id) == 0) {
      printf("%sAdvertencia: La canción '%s' ya está en la lista '%s'.%s\n", AMARILLO, cancion->track_name, playlist->name, RESET);
      presioneTeclaParaContinuar();
      return;
    }
    current_song = (Song *)list_next(playlist->songs);
  }

  // Agregar canción a la playlist
  list_pushBack(playlist->songs, cancion);
  printf("%sCanción '%s' agregada exitosamente a la lista '%s'.%s\n", VERDE, cancion->track_name, playlist->name, RESET);
  presioneTeclaParaContinuar();
}

/* 
 * Función: mostrar_canciones_lista
 * Descripción: Muestra las canciones de una lista de reproducción seleccionada.
 * Parámetros:
 *   - playlists: Mapa con listas de reproducción
 */
void mostrar_canciones_lista(HashMap *playlists) {
  limpiarPantalla();

  // Verificar si hay playlists
  if (firstMap(playlists) == NULL) {
    printf("%sError: No hay listas de reproducción creadas.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  mostrar_playlists(playlists); // Mostrar playlists disponibles

  char nombre_lista[100];
  printf("%sIngrese el nombre de la lista de reproducción: %s", AZUL, RESET);
  scanf(" %[^\n]", nombre_lista);
  while (getchar() != '\n');

  // Buscar playlist
  Pair *playlist_pair = searchMap(playlists, nombre_lista);
  if (playlist_pair == NULL) {
    printf("%sError: La lista '%s' no existe.%s\n", ROJO, nombre_lista, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  Playlist *playlist = (Playlist *)playlist_pair->value;
  printf("\n%sCanciones en la lista '%s':%s\n", VERDE, playlist->name, RESET);
  show_songs(playlist->songs, 1, 10); // Mostrar hasta 10 canciones por página
}

/* 
 * Función: liberar_memoria
 * Descripción: Libera toda la memoria asignada para los mapas y listas,
 *              incluyendo canciones, artistas, géneros, tempos y playlists.
 * Parámetros:
 *   - canciones_byid: Mapa con canciones por ID
 *   - canciones_bygenres: Mapa con listas por género
 *   - canciones_byartist: Mapa con listas por artista
 *   - canciones_by_tempo: Mapa con listas por tempo
 *   - playlists: Mapa con listas de reproducción
 */
void liberar_memoria(HashMap *canciones_byid, HashMap *canciones_bygenres, HashMap *canciones_byartist, HashMap *canciones_by_tempo, HashMap *playlists) {
  // Liberar canciones por género
  Pair *pair = firstMap(canciones_bygenres);
  while (pair != NULL) {
    free(pair->key); // Liberar clave (género)
    list_clean((List *)pair->value); // Limpiar lista de canciones
    free(pair->value); // Liberar lista
    pair = nextMap(canciones_bygenres);
  }
  free(canciones_bygenres);

  // Liberar canciones por artista
  pair = firstMap(canciones_byartist);
  while (pair != NULL) {
    free(pair->key); // Liberar clave (artista)
    list_clean((List *)pair->value); // Limpiar lista de canciones
    free(pair->value); // Liberar lista
    pair = nextMap(canciones_byartist);
  }
  free(canciones_byartist);

  // Liberar canciones por ID
  pair = firstMap(canciones_byid);
  while (pair != NULL) {
    Song *cancion = (Song *)pair->value;
    char *artista = (char *)list_first(cancion->artists);
    while (artista != NULL) {
      free(artista); // Liberar cada artista
      artista = (char *)list_next(cancion->artists);
    }
    list_clean(cancion->artists); // Limpiar lista de artistas
    free(cancion->artists); // Liberar lista
    free(cancion); // Liberar estructura de canción
    pair = nextMap(canciones_byid);
  }
  free(canciones_byid);

  // Liberar canciones por tempo
  pair = firstMap(canciones_by_tempo);
  while (pair != NULL) {
    list_clean((List *)pair->value); // Limpiar lista de canciones
    free(pair->value); // Liberar lista
    pair = nextMap(canciones_by_tempo);
  }
  free(canciones_by_tempo);

  // Liberar playlists
  pair = firstMap(playlists);
  while (pair != NULL) {
    Playlist *playlist = (Playlist *)pair->value;
    list_clean(playlist->songs); // Limpiar lista de canciones
    free(playlist->songs); // Liberar lista
    free(playlist); // Liberar estructura de playlist
    pair = nextMap(playlists);
  }
  free(playlists);
}

/* 
 * Función: main
 * Descripción: Función principal del programa. Inicializa los mapas, muestra
 *              el menú principal y procesa las opciones del usuario hasta que
 *              se selecciona salir.
 * Retorno: 0 al finalizar correctamente
 */
int main() {
  char opcion;

  // Inicializar mapas con capacidades iniciales
  HashMap *canciones_byid = createMap(200000); // Mapa para canciones por ID
  HashMap *canciones_bygenres = createMap(200000); // Mapa para canciones por género
  HashMap *canciones_byartist = createMap(200000); // Mapa para canciones por artista
  HashMap *canciones_by_tempo = createMap(3); // Mapa para canciones por tempo (3 categorías)
  HashMap *playlists = createMap(40000); // Mapa para listas de reproducción

  // Bucle principal del programa
  do {
    mostrarMenuPrincipal(); // Mostrar menú
    printf("%sIngrese su opción: %s", AZUL, RESET);
    scanf(" %c", &opcion);
    while (getchar() != '\n');

    // Procesar opción seleccionada
    switch (opcion) {
    case '1':
      cargar_canciones(canciones_byid, canciones_bygenres, canciones_byartist, canciones_by_tempo);
      break;
    case '2':
      buscar_por_genero(canciones_bygenres);
      break;
    case '3':
      buscar_por_artista(canciones_byartist);
      break;
    case '4':
      buscar_por_tempo(canciones_by_tempo);
      break;
    case '5':
      crear_lista_reproduccion(playlists);
      break;
    case '6':
      agregar_cancion_lista(canciones_byid, playlists);
      break;
    case '7':
      mostrar_canciones_lista(playlists);
      break;
    case '8':
      printf("%sSaliendo del programa...%s\n", VERDE, RESET);
      presioneTeclaParaContinuar();
      break;
    default:
      printf("%sOpción inválida.%s\n", ROJO, RESET);
      presioneTeclaParaContinuar();
    }
  } while (opcion != '8');
  
  // Liberar toda la memoria asignada
  liberar_memoria(canciones_byid, canciones_bygenres, canciones_byartist, canciones_by_tempo, playlists);

  return 0;
}