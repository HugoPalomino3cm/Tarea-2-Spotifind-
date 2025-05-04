#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de colores ANSI para salida en consola
#define ROJO "\033[0;31m"      // Rojo oscuro para mensajes de error o advertencia
#define VERDE "\033[0;32m"     // Verde oscuro para mensajes de éxito
#define AZUL "\033[0;34m"      // Azul oscuro para mensajes informativos
#define BLANCO "\033[1;37m"    // Blanco para mensaje de salida
#define RESETEAR "\033[0m"     // Resetea el color a predeterminado

typedef struct {
  char id[100];           // ID de la canción
  List *artists;          // Lista de artistas ya que pueden ser varios 
  char album_name[100];   // Nombre del álbum
  char track_name[100];   // Nombre de la pista
  float tempo;            // Tempo de la canción (en BPM)
  char track_genre[100];  // Género de la pista 
} Song;

// ESTRUCTURA CREADA PARA LA PLAYLIST PALTANEITOR (seccion de preguntas 5,6,7)
typedef struct {
  char name[100];          // Nombre de la lista de reproduccion
  List *songs;            // Lista de canciones en la lista de reproduccion
} Playlist;

/* AVISO IMPORTANTE NO BORRE VARIOS COMENTARIOS PERO MODIFIQUE LA WEAS ASI QUE PUEDE QUE SIGAN HABIENDO COMENTARIOS DEL CODIGO PROPUESTO PORSIACASO */

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  printf("%s╔════════════════════════════════════════╗\n", AZUL);
  printf("║            🎵 Spotifind 🎵             ║\n");
  printf("╚════════════════════════════════════════╝\n");
  printf("  1) Cargar Canciones\n");
  printf("  2) Buscar por género\n");
  printf("  3) Buscar por artista\n");
  printf("  4) Buscar por tempo\n");
  printf("  5) Crear lista de reproducción\n");
  printf("  6) Agregar canción a la lista\n");
  printf("  7) Mostrar canciones de una lista\n");
  printf("%s  8) Salir%s\n", ROJO, AZUL);
  printf("══════════════════════════════════════════%s\n", RESETEAR);
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/* YA MODIFIQUE EL ARCHIVO EN EL QUE SE TENIA QUE LEER */
void cargar_canciones(Map *canciones_byid, Map *canciones_bygenres, Map *canciones_byartist) {
  // Abrir el archivo CSV que contiene los datos de las canciones
  FILE *archivo = fopen("data/song_dataset_.csv", "r");
  if (archivo == NULL) {
    printf("Error: No se pudo abrir el archivo de datos.\n");
    return;
  }

  char **campos;

  // Leer la primera línea (encabezado) del archivo CSV
  campos = leer_linea_csv(archivo, ','); 
  if (campos == NULL) {
    printf("Error: No se pudo leer el encabezado del archivo.\n");
    fclose(archivo);
    printf("Presione una tecla para continuar...\n");
    presioneTeclaParaContinuar();
    return;
  }
  
  int count = 0; // Contador para las canciones cargadas
  // Leer cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Asignar memoria para una nueva estructura Song
    Song *cancion = (Song *)malloc(sizeof(Song));
    if (cancion == NULL) {
      printf("Error: No se pudo asignar memoria para la canción %s%d%s.\n", ROJO, count + 1, RESETEAR);
      fclose(archivo);
      return;
    }
    // Imprimir mensaje de progreso con solo el número en azul
    printf("Cargando canción %s%d%s...\n", AZUL, count + 1, RESETEAR);

    // Copia los campos con manejo seguro
    strncpy(cancion->id, campos[0] ? campos[0] : "", sizeof(cancion->id) - 1);
    cancion->id[sizeof(cancion->id) - 1] = '\0';

    // Manejo de artistas
    if (campos[2] == NULL || strlen(campos[2]) == 0) {
      cancion->artists = list_create();
    } else {
      cancion->artists = split_string(campos[2], ";");    // Asignamos nombre del o los artistas
      if (cancion->artists == NULL || list_first(cancion->artists) == NULL) {
        cancion->artists = list_create();
        char *artista_copy = strdup(campos[2]);
        list_pushBack(cancion->artists, artista_copy);
      }
    }

    strncpy(cancion->album_name, campos[3] ? campos[3] : "", sizeof(cancion->album_name) - 1);
    cancion->album_name[sizeof(cancion->album_name) - 1] = '\0';

    strncpy(cancion->track_name, campos[4] ? campos[4] : "", sizeof(cancion->track_name) - 1);
    cancion->track_name[sizeof(cancion->track_name) - 1] = '\0';

    cancion->tempo = (campos[18] && strlen(campos[18]) > 0) ? atof(campos[18]) : 0.0;

    strncpy(cancion->track_genre, campos[20] ? campos[20] : "", sizeof(cancion->track_genre) - 1);
    cancion->track_genre[sizeof(cancion->track_genre) - 1] = '\0';

    // Insertar la canción en el mapa usando el ID como clave
    map_insert(canciones_byid, cancion->id, cancion);
    
    // Insertar en el mapa por género
    MapPair *genre_pair = map_search(canciones_bygenres, cancion->track_genre);
    if (genre_pair == NULL) {
      List *new_list = list_create();
      if (new_list == NULL) {
        printf("Error: No se pudo crear una lista para el género %s.\n", cancion->track_genre);
        free(cancion);
        fclose(archivo);
        printf("Presione una tecla para continuar...\n");
        presioneTeclaParaContinuar();
        return;
      }
      list_pushBack(new_list, cancion);
      map_insert(canciones_bygenres, cancion->track_genre, new_list);
    } else {
      List *genre_list = (List *)genre_pair->value;
      list_pushBack(genre_list, cancion);
    }

    // Insertar en el mapa por artista
    char *artista = (char *)list_first(cancion->artists);
    while (artista != NULL) {
      MapPair *artist_pair = map_search(canciones_byartist, artista);
      if (artist_pair == NULL) {
        List *new_list = list_create();
        if (new_list == NULL) {
          printf("Error: No se pudo crear una lista para el artista %s.\n", artista);
          free(cancion);
          fclose(archivo);
          printf("Presione una tecla para continuar...\n");
          presioneTeclaParaContinuar();
          return;
        }
        list_pushBack(new_list, cancion);
        char *artista_copy = strdup(artista); // Crear una copia para el mapa
        map_insert(canciones_byartist, artista_copy, new_list);
      } else {
        List *artist_list = (List *)artist_pair->value;
        list_pushBack(artist_list, cancion);
      }
      artista = (char *)list_next(cancion->artists);
    }

    count++;
  }
  fclose(archivo);

  // Mostrar mensaje de éxito o advertencia según el número de canciones cargadas
  if (count == 114000) {
    printf("Éxito: Se cargaron todas las canciones (%s%d%s) correctamente.\n", VERDE, count, RESETEAR);
  } else {
    printf("Advertencia: No se cargaron todas las canciones. Se cargaron %s%d%s de 114000.\n", ROJO, count, RESETEAR);
  }
  printf("Presione una tecla para continuar...\n");
  presioneTeclaParaContinuar();
}

void buscar_por_tempo(Map *canciones_byid) {
  limpiarPantalla();
  int opcion;
  
  /** Solicita al usuario la categoría de tempo */
  printf("Seleccione la categoría de tempo:\n");
  printf("1) Lentas (menos de 80 BPM)\n");
  printf("2) Moderadas (80-120 BPM)\n");
  printf("3) Rápidas (mayor a 120 BPM)\n");
  printf("Ingrese su opción (1-3): ");
  scanf("%d", &opcion);

  float min_tempo, max_tempo;
  char categoria[20];
  
  /** Define los rangos de tempo según la opción */
  switch (opcion) {
    case 1:
      strcpy(categoria, "Lentas");
      min_tempo = 0.0;
      max_tempo = 80.0;
      break;
    case 2:
      strcpy(categoria, "Moderadas");
      min_tempo = 80.0;
      max_tempo = 120.0;
      break;
    case 3:
      strcpy(categoria, "Rápidas");
      min_tempo = 120.0;
      // segun yo esto lo tenemos que cambiar  lo de max_tempo pero de ahi lo vemos dea
      max_tempo = 9999.0; // Valor arbitrario alto
      break;
    default:
      printf("Opción inválida\n");
      return;
  }
}

void buscar_por_genero(Map *canciones_bygenres) {
  limpiarPantalla();
  
  char genero[100];

  printf("Ingrese el género de la canción: ");
  scanf("%s", genero); 

  MapPair *pair = map_search(canciones_bygenres, genero);
  if (pair != NULL) {
    List *canciones = pair->value;
    Song *cancion = list_first(canciones);
    
    printf("\nCanciones del género %s:\n", genero);
    while (cancion != NULL) {
      printf("ID: %s, Título: %s, Álbum: %s, Género: %s, Tempo: %.2f, Artistas: ",
             cancion->id, cancion->track_name, cancion->album_name,
             cancion->track_genre, cancion->tempo);
      // Imprimir lista de artistas
      char *artista = (char *)list_first(cancion->artists);
      while (artista != NULL) {
        printf("%s", artista);
        artista = (char *)list_next(cancion->artists);
        if (artista != NULL) printf(", ");
      }
      printf("\n");
      cancion = list_next(canciones);
    }
  } else {
    printf("No se encontraron canciones del género %s\n", genero);
  }
}

void buscar_por_artista(Map *canciones_byartist) {
  limpiarPantalla();
  
  char artista[100];
  printf("Ingrese el nombre del artista: ");
  scanf(" %[^\n]", artista); // Lee el nombre completo, incluyendo espacios

  MapPair *pair = map_search(canciones_byartist, artista);
  if (pair != NULL) {
    List *canciones = (List *)pair->value;
    Song *cancion = (Song *)list_first(canciones);
    
    printf("\nCanciones del artista %s:\n", artista);
    while (cancion != NULL) {
      printf("ID: %s, Título: %s, Álbum: %s, Género: %s, Tempo: %.2f, Artistas: ",
             cancion->id, cancion->track_name, cancion->album_name,
             cancion->track_genre, cancion->tempo);
      // Imprimir lista de artistas
      char *art = (char *)list_first(cancion->artists);
      while (art != NULL) {
        printf("%s", art);
        art = (char *)list_next(cancion->artists);
        if (art != NULL) printf(", ");
      }
      printf("\n");
      cancion = (Song *)list_next(canciones);
    }
  } else {
    printf("No se encontraron canciones del artista %s\n", artista);
  }
  presioneTeclaParaContinuar();
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);
  // creamos este mapa para las canciones para que a la hora de buscar a los artistas tenga una complejidad menor por eso lo hacemos 
  Map *canciones_byartist = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_canciones(canciones_byid, canciones_bygenres,canciones_byartist);
      break;
    case '2':
      buscar_por_genero(canciones_bygenres);
      break;
    case '3':
    // podemos agregar de que si colocan un nombre que esta incompleto y hay varios nombres similares podemos darle opciones al usuario
      buscar_por_artista(canciones_byartist);
      break;
    case '4':
      buscar_por_tempo(canciones_byid);
      break;
    case '5':
      //crear_lista_reproduccion(playlists);
      break;
    case '6':
      //agregar_cancion_lista(canciones_byid, playlists);
      break;
    case '7':
      //mostrar_canciones_lista(playlists);
      break;
    case '8':
      printf("%sSaliendo de la aplicación...%s\n", BLANCO, RESETEAR);
      break;
    default:
      printf("Opción inválida\n");
    }
    if (opcion != '8') {
      presioneTeclaParaContinuar();
    }

  } while (opcion != '8');

  return 0;
}