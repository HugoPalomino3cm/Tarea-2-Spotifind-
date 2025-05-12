#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/HashMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definición de colores ANSI para salida en consola
#define ROJO    "\x1b[31m"
#define VERDE   "\x1b[32m"
#define AMARILLO "\x1b[33m"
#define AZUL    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

typedef struct {
  char id[100];           // ID de la canción 
  List *artists;          // Lista de artistas ya que pueden ser varios 
  char album_name[100];   // Nombre del álbum
  char track_name[100];   // Nombre de la pista
  float tempo;            // Tempo de la canción (en BPM)
  char track_genre[100];  // Género de la pista 
} Song;

typedef struct {
  char name[100];          // Nombre de la lista de reproduccion
  List *songs;            // Lista de canciones en la lista de reproduccion
} Playlist;

void mostrarMenuPrincipal() {
  limpiarPantalla();
  printf("%s╔════════════════════════════════════════╗\n", AZUL);
  printf("%s║            🎵 Spotifind 🎵             ║\n", AZUL);
  printf("%s╚════════════════════════════════════════╝\n", AZUL);
  
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

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

void to_lowercase(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}

//Función para capitalizar la primera letra de un string
void capitalize_first(char *str, char *output) {
  strcpy(output, str);
  if (output[0]) {
    output[0] = toupper(output[0]);
  }
}

void mostrar_barra_progreso(int actual, int total) {
  static int ultimo_porcentaje_mostrado = -1;
  int ancho_barra = 30;
  float porcentaje = (float)actual / total;
  int porcentaje_entero = (int)(porcentaje * 100);

  if (actual == total) {
    porcentaje_entero = 100;
  }

  if (porcentaje_entero != ultimo_porcentaje_mostrado || actual == total) {
    int completado = (int)((float)porcentaje_entero / 100 * ancho_barra);

    if (porcentaje_entero == 100) {
      completado = ancho_barra;
    }

   printf("\r%s[", CYAN);
    for (int i = 0; i < completado; i++) printf("🟩");
    for (int i = completado; i < ancho_barra; i++) printf("⬜");
    printf("] %3d%% [%d/%d]%s", porcentaje_entero, actual, total, RESET);
    fflush(stdout);

    ultimo_porcentaje_mostrado = porcentaje_entero;
  }
}

void cargar_canciones(HashMap *canciones_byid, HashMap *canciones_bygenres, HashMap *canciones_byartist, HashMap *canciones_by_tempo) {
  FILE *archivo = fopen("data/song_dataset_.csv", "r");
  if (archivo == NULL) {
    printf("%sError: No se pudo abrir el archivo de datos.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  int total_lineas = 0;
  char buffer[1024];
  int is_first_line = 1;
  while (fgets(buffer, sizeof(buffer), archivo)) {
    if (is_first_line) {
      is_first_line = 0;
      continue;
    }
    if (strlen(buffer) > 1 && buffer[0] != '\n' && strspn(buffer, " \t\n") != strlen(buffer)) {
      total_lineas++;
    }
  }
  if (total_lineas <= 0) {
    printf("%sError: El archivo está vacío o solo contiene el encabezado.%s\n", ROJO, RESET);
    fclose(archivo);
    presioneTeclaParaContinuar();
    return;
  }
  rewind(archivo);

  char **campos = leer_linea_csv(archivo, ',');
  if (campos == NULL) {
    printf("%sError: No se pudo leer el encabezado del archivo.%s\n", ROJO, RESET);
    fclose(archivo);
    presioneTeclaParaContinuar();
    return;
  }

  int count = 0;
  int lentas = 0, moderadas = 0, rapidas = 0;

  mostrar_barra_progreso(count, total_lineas);

  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    if (!campos[0] || strlen(campos[0]) == 0) {
      continue;
    }

    Song *cancion = (Song *)malloc(sizeof(Song));
    if (cancion == NULL) {
      printf("\n%sError: No se pudo asignar memoria para la canción %d.%s\n", ROJO, count + 1, RESET);
      fclose(archivo);
      presioneTeclaParaContinuar();
      return;
    }

    strncpy(cancion->id, campos[0] ? campos[0] : "", sizeof(cancion->id) - 1);
    cancion->id[sizeof(cancion->id) - 1] = '\0';

    if (campos[2] == NULL || strlen(campos[2]) == 0) {
      cancion->artists = list_create();
    } else {
      cancion->artists = split_string(campos[2], ";");
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

    //Se convierte track_genre a minúsculas antes de almacenar
    strncpy(cancion->track_genre, campos[20] ? campos[20] : "", sizeof(cancion->track_genre) - 1);
    cancion->track_genre[sizeof(cancion->track_genre) - 1] = '\0';
    to_lowercase(cancion->track_genre);

    insertMap(canciones_byid, cancion->id, cancion);
    
    Pair *genre_pair = searchMap(canciones_bygenres, cancion->track_genre);
    if (genre_pair == NULL) {
      List *new_list = list_create();
      if (new_list == NULL) {
        printf("\n%sError: No se pudo crear una lista para el género %s.%s\n", ROJO, cancion->track_genre, RESET);
        free(cancion);
        fclose(archivo);
        presioneTeclaParaContinuar();
        return;
      }
      list_pushBack(new_list, cancion);
      //Se inserta el género en minúsculas
      char *genre_copy = strdup(cancion->track_genre);
      insertMap(canciones_bygenres, genre_copy, new_list);
    } else {
      List *genre_list = (List *)genre_pair->value;
      list_pushBack(genre_list, cancion);
    }

    char *artista = (char *)list_first(cancion->artists);
    while (artista != NULL) {
      //Se convierte el nombre del artista a minúsculas para la clave
      char *artista_lower = strdup(artista);
      to_lowercase(artista_lower);
      Pair *artist_pair = searchMap(canciones_byartist, artista_lower);
      if (artist_pair == NULL) {
        List *new_list = list_create();
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
        list_pushBack(artist_list, cancion);
        free(artista_lower);
      }
      artista = (char *)list_next(cancion->artists);
    }

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
      List *new_list = list_create();
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
      list_pushBack((List *)pair_tempo->value, cancion);
    }

    count++;
    mostrar_barra_progreso(count, total_lineas);
  }

  fclose(archivo);

  mostrar_barra_progreso(count, count);

  printf("\n");
  printf("%sÉxito: Se cargaron %d canciones correctamente.%s\n", VERDE, count, RESET);
  printf("%sCanciones por tempo: Lentas=%d, Moderadas=%d, Rápidas=%d%s\n", CYAN, lentas, moderadas, rapidas, RESET);
  presioneTeclaParaContinuar();
}

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
      printf("%s, ", RESET);
    }
  }
  printf("%s\n", RESET);
}

void show_songs(List *songs, int initial_page, int songs_per_page) {
  if (songs == NULL || list_first(songs) == NULL) {
    limpiarPantalla();
    printf("%sNo se encontraron canciones.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  //Se usa list_size para obtener el número total de canciones
  int total_songs = list_size(songs);
  int total_pages = (total_songs + songs_per_page - 1) / songs_per_page;

  int current_page = initial_page;
  char nav_option;

  //Bucle para manejar la navegación entre páginas
  do {
    limpiarPantalla();
    printf("\n%s══════════════════ Lista de Canciones ══════════════════%s\n", AZUL, RESET);

    //Mostrar canciones de la página actual
    Song *cancion = (Song *)list_first(songs);
    int song_number = 1;
    int current_song = 1;
    int start_song = (current_page - 1) * songs_per_page + 1;
    int end_song = current_page * songs_per_page;

    while (cancion != NULL && current_song < start_song) {
      cancion = (Song *)list_next(songs);
      current_song++;
      song_number++;
    }

    while (cancion != NULL && current_song <= end_song) {
      printf("%sCanción %d%s\n", MAGENTA, song_number, RESET);
      printf("%s  Título: %s%s%s\n", CYAN, VERDE, cancion->track_name[0] ? cancion->track_name : "Desconocido", RESET);
      printf("%s  Álbum: %s%s%s\n", CYAN, VERDE, cancion->album_name[0] ? cancion->album_name : "Desconocido", RESET);
      //Se capitaliza la primera letra del género
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

    // Mostrar información de página y opciones
    printf("%sPágina %d de %d%s\n", AZUL, current_page, total_pages, RESET);
    printf("%sOpciones:%s\n", AZUL, RESET);
    if (current_page > 1) {
      printf("%s  1) Página anterior%s\n", AZUL, RESET);
    }
    if (current_page < total_pages) {
      printf("%s  2) Página siguiente%s\n", AZUL, RESET);
    }
    printf("%s  3) Volver%s\n", ROJO, RESET);

    //Se captura la entrada del usuario
    printf("%sIngrese su opción: %s", AZUL, RESET);
    scanf(" %c", &nav_option);
    while (getchar() != '\n');

    //Se procesa la opción
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

void buscar_por_tempo(HashMap *canciones_by_tempo) {
  limpiarPantalla();
  int opcion;
  
  printf("%sSeleccione la categoría de tempo:%s\n", AZUL, RESET);
  printf("%s1) Lentas (menos de 80 BPM)%s\n", AZUL, RESET);
  printf("%s2) Moderadas (80-120 BPM)%s\n", AZUL, RESET);
  printf("%s3) Rápidas (mayor a 120 BPM)%s\n", AZUL, RESET);
  printf("%sIngrese su opción (1-3): %s", AZUL, RESET);
  scanf("%d", &opcion);
  while (getchar() != '\n');

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

  Pair *searched = searchMap(canciones_by_tempo, categoria);
  if (searched == NULL) {
    printf("%sNo se encontraron canciones en la categoría '%s'.%s\n", ROJO, categoria, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  List *canciones = (List *)searched->value;
  printf("\n%sCanciones con categoría de tempo '%s':%s\n", VERDE, categoria, RESET);
  show_songs(canciones, 1, 100);
}

//Opción para mostrar todos los géneros disponibles y búsqueda insensible a mayúsculas/minúsculas
void buscar_por_genero(HashMap *canciones_bygenres) {
  limpiarPantalla();
  
  char opcion;
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
        //Se capitaliza la primera letra del género
        char display_genre[100];
        capitalize_first((char *)current->key, display_genre);
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

  char genero[100];
  printf("%sIngrese el género de la canción: %s", AZUL, RESET);
  printf("%s(puede ver los géneros disponibles usando previamente la opción 2)%s\n", AZUL, RESET);
  scanf("%s", genero);
  while (getchar() != '\n');
  // Se convierte la entrada a minúsculas para la búsqueda
  to_lowercase(genero);

  Pair *searched_genre = searchMap(canciones_bygenres, genero);
  if (searched_genre != NULL) {
    List *canciones = (List *)searched_genre->value;
    // Se capitaliza la primera letra del género
    char display_genre[100];
    capitalize_first(genero, display_genre);
    printf("\n%sCanciones del género '%s':%s\n", VERDE, display_genre, RESET);
    show_songs(canciones, 1, 10);
  } else {
    printf("%sNo se encontraron canciones del género '%s'.%s\n", ROJO, genero, RESET);
    presioneTeclaParaContinuar();
  }
}

void buscar_por_artista(HashMap *canciones_byartist) {
  limpiarPantalla();
  
  char artista[100];
  printf("%sIngrese el nombre del artista: %s", AZUL, RESET);
  scanf(" %[^\n]", artista);
  while (getchar() != '\n');
  to_lowercase(artista);

  Pair *pair = searchMap(canciones_byartist, artista);
  if (pair != NULL) {
    List *songs = (List *)pair->value;
    printf("\n%sCanciones del artista '%s':%s\n", VERDE, artista, RESET);
    show_songs(songs, 1, 10);
  } else {
    printf("%sNo se encontraron canciones del artista '%s'.%s\n", ROJO, artista, RESET);
    presioneTeclaParaContinuar();
  }
}

void crear_lista_reproduccion(HashMap *playlists) {
  limpiarPantalla();

  char nombre[100];
  printf("%sIngrese el nombre de la nueva lista: %s", AZUL, RESET);
  scanf(" %[^\n]", nombre);
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

  Playlist *nueva_playlist = (Playlist *)malloc(sizeof(Playlist));
  if (nueva_playlist == NULL) {
    printf("%sError: No se pudo asignar memoria para la nueva lista de reproducción.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  strncpy(nueva_playlist->name, nombre, sizeof(nueva_playlist->name) - 1);
  nueva_playlist->name[sizeof(nueva_playlist->name) - 1] = '\0';
  nueva_playlist->songs = list_create();
  if (nueva_playlist->songs == NULL) {
    printf("%sError: No se pudo crear la lista de canciones para la playlist.%s\n", ROJO, RESET);
    free(nueva_playlist);
    presioneTeclaParaContinuar();
    return;
  }

  insertMap(playlists, nueva_playlist->name, nueva_playlist);
  printf("%sLista de reproducción '%s' creada exitosamente.%s\n", VERDE, nombre, RESET);
  presioneTeclaParaContinuar();
}

//Función para mostrar todas las playlists disponibles
void mostrar_playlists(HashMap *playlists) {
  printf("\n%s══════════════════ Listas de Reproducción Disponibles ══════════════════%s\n", AZUL, RESET);
  Pair *current = firstMap(playlists);
  int index = 1;
  if (current == NULL) {
    printf("%sNo hay listas de reproducción creadas.%s\n", ROJO, RESET);
  } else {
    while (current != NULL) {
      Playlist *playlist = (Playlist *)current->value;
      printf("%s  %d) %s%s%s\n", AZUL, index, MAGENTA, playlist->name, RESET);
      current = nextMap(playlists);
      index++;
    }
  }
  printf("%s════════════════════════════════════════════════════════════%s\n", AZUL, RESET);
}

void agregar_cancion_lista(HashMap *canciones_byid, HashMap *playlists) {
  limpiarPantalla();

  //Se muestran las playlists disponibles antes de pedir el nombre
  if (firstMap(playlists) == NULL) {
    printf("%sError: No hay listas de reproducción creadas.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  mostrar_playlists(playlists);

  char nombre_lista[100];
  printf("%sIngrese el nombre de la lista de reproducción: %s", AZUL, RESET);
  scanf(" %[^\n]", nombre_lista);
  while (getchar() != '\n');

  Pair *playlist_pair = searchMap(playlists, nombre_lista);
  if (playlist_pair == NULL) {
    printf("%sError: La lista '%s' no existe.%s\n", ROJO, nombre_lista, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  Playlist *playlist = (Playlist *)playlist_pair->value;

  char id_cancion[100];
  printf("%sIngrese el ID de la canción: %s", AZUL, RESET);
  scanf("%[^\n]", id_cancion);
  while (getchar() != '\n');

  Pair *song_pair = searchMap(canciones_byid, id_cancion);
  if (song_pair == NULL) {
    printf("%sError: La canción con ID '%s' no existe.%s\n", ROJO, id_cancion, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  Song *cancion = (Song *)song_pair->value;

  Song *current_song = (Song *)list_first(playlist->songs);
  while (current_song != NULL) {
    if (strcmp(current_song->id, cancion->id) == 0) {
      printf("%sAdvertencia: La canción '%s' ya está en la lista '%s'.%s\n", AMARILLO, cancion->track_name, playlist->name, RESET);
      presioneTeclaParaContinuar();
      return;
    }
    current_song = (Song *)list_next(playlist->songs);
  }

  list_pushBack(playlist->songs, cancion);
  printf("%sCanción '%s' agregada exitosamente a la lista '%s'.%s\n", VERDE, cancion->track_name, playlist->name, RESET);
  presioneTeclaParaContinuar();
}

void mostrar_canciones_lista(HashMap *playlists) {
  //Se muestran las playlists disponibles antes de pedir el nombre
  limpiarPantalla();

  if (firstMap(playlists) == NULL) {
    printf("%sError: No hay listas de reproducción creadas.%s\n", ROJO, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  mostrar_playlists(playlists);

  char nombre_lista[100];
  printf("%sIngrese el nombre de la lista de reproducción: %s", AZUL, RESET);
  scanf(" %[^\n]", nombre_lista);
  while (getchar() != '\n');

  Pair *playlist_pair = searchMap(playlists, nombre_lista);
  if (playlist_pair == NULL) {
    printf("%sError: La lista '%s' no existe.%s\n", ROJO, nombre_lista, RESET);
    presioneTeclaParaContinuar();
    return;
  }

  Playlist *playlist = (Playlist *)playlist_pair->value;
  printf("\n%sCanciones en la lista '%s':%s\n", VERDE, playlist->name, RESET);
  show_songs(playlist->songs, 1, 10);
}

//Función para liberar toda la memoria asignada
void liberar_memoria(HashMap *canciones_byid, HashMap *canciones_bygenres, HashMap *canciones_byartist, HashMap *canciones_by_tempo, HashMap *playlists) {
  Pair *pair = firstMap(canciones_bygenres);
  while (pair != NULL) {
    free(pair->key);
    list_clean((List *)pair->value);
    free(pair->value);
    pair = nextMap(canciones_bygenres);
  }
  free(canciones_bygenres);

  pair = firstMap(canciones_byartist);
  while (pair != NULL) {
    free(pair->key);
    list_clean((List *)pair->value);
    free(pair->value);
    pair = nextMap(canciones_byartist);
  }
  free(canciones_byartist);

  pair = firstMap(canciones_byid);
  while (pair != NULL) {
    Song *cancion = (Song *)pair->value;
    char *artista = (char *)list_first(cancion->artists);
    while (artista != NULL) {
      free(artista);
      artista = (char *)list_next(cancion->artists);
    }
    list_clean(cancion->artists);
    free(cancion->artists);
    free(cancion);
    pair = nextMap(canciones_byid);
  }
  free(canciones_byid);

  pair = firstMap(canciones_by_tempo);
  while (pair != NULL) {
    list_clean((List *)pair->value);
    free(pair->value);
    pair = nextMap(canciones_by_tempo);
  }
  free(canciones_by_tempo);

  pair = firstMap(playlists);
  while (pair != NULL) {
    Playlist *playlist = (Playlist *)pair->value;
    list_clean(playlist->songs);
    free(playlist->songs);
    free(playlist);
    pair = nextMap(playlists);
  }
  free(playlists);
}

int main() {
  char opcion;

  HashMap *canciones_byid = createMap(200000);
  HashMap *canciones_bygenres = createMap(200000);
  HashMap *canciones_byartist = createMap(200000);
  HashMap *canciones_by_tempo = createMap(3);
  HashMap *playlists = createMap(40000);

  do {
    mostrarMenuPrincipal();
    printf("%sIngrese su opción: %s", AZUL, RESET);
    scanf(" %c", &opcion);
    while (getchar() != '\n');

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
  

  //Llamada a la función de liberación de memoria
  liberar_memoria(canciones_byid, canciones_bygenres, canciones_byartist, canciones_by_tempo, playlists);

  return 0;
}