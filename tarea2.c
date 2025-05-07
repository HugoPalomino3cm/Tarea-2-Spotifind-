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
void cargar_canciones(Map *canciones_byid, Map *canciones_bygenres, Map *canciones_byartist, Map *canciones_by_tempo) {
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
  
  int count = 0; // Contador para las canciones cargadas CREO QYE HAY QUE BORRAR PORQUE DEBE SER MODULAR PARA OTROS ARCHIVOS
  int lentas = 0, moderadas = 0, rapidas = 0;

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
    MapPair *genre_pair = map_search(canciones_bygenres, cancion->track_genre); //BUSCA SI YA EXISTE EL GENERO
    if (genre_pair == NULL) {
      List *new_list = list_create();
      if (new_list == NULL) { //MANEJO DE QUE NO SE HAYA PODIDO CREAR
        printf("Error: No se pudo crear una lista para el género %s.\n", cancion->track_genre);
        free(cancion);
        fclose(archivo);
        printf("Presione una tecla para continuar...\n");
        presioneTeclaParaContinuar();
        return;
      }
      list_pushBack(new_list, cancion);//AGREGAMOS LA CANCION AL FINAL DE LA LISTA
      map_insert(canciones_bygenres, cancion->track_genre, new_list); //AGREGAMOS LA LISTA AL MAPA 
    } else {
      List *genre_list = (List *)genre_pair->value; //EN CASO DE NO EXISTIR CREAMOS LA LISTA DEL GENERO
      list_pushBack(genre_list, cancion); //AGREGAMOS LA CANCION A LA LISTA
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

    char *categoria ;
    if(cancion->tempo < 80.0)
    {
      categoria = "Lentas" ;
      lentas++;
    }else if(cancion->tempo >= 80.0 && cancion->tempo <= 120.0)
    {
      categoria = "Moderadas" ;
      moderadas++;
    }else
    {
      categoria = "Rápidas" ;
      rapidas++;
    }
    MapPair *pair_tempo = map_search(canciones_by_tempo, categoria);
    if(pair_tempo == NULL)
    {
      List *new_list = list_create() ;
      if(new_list == NULL)
      {
        printf("ERROR: NO SE PUDO CREAR UNA LISTA PARA LA CATEGORÍA %s\n", categoria);
        free(cancion);
        fclose(archivo);
        presioneTeclaParaContinuar();
        return;
      }
      list_pushBack(new_list, cancion) ;
      map_insert(canciones_by_tempo, categoria, new_list);
    }
    else
    {
      list_pushBack((List *)pair_tempo->value, cancion);
    }

    count++;
  }


  fclose(archivo);

  // Mostrar mensaje de éxito o advertencia según el número de canciones cargadas 

  //BORRAR O HACER MODULAR YA QUE TIENE QUE ABRIR MULTIPLES ARCHIVOS CSV SEGUN PROFESOR
  if (count == 114000) {
    printf("Éxito: Se cargaron todas las canciones (%s%d%s) correctamente.\n", VERDE, count, RESETEAR);
    printf("Canciones por tempo: Lentas=%d, Moderadas=%d, Rápidas=%d\n", lentas, moderadas, rapidas);
  } else {
    printf("Advertencia: No se cargaron todas las canciones. Se cargaron %s%d%s de 114000.\n", ROJO, count, RESETEAR);
  }
  presioneTeclaParaContinuar();

}

void show_artists(List *list_artists)
{
  if(list_artists == NULL)
  {
    printf("Artistas: Desconocido\n");
    return;
  }

  printf("Artistas : ");
  char *artist = (char *)list_first(list_artists) ;
  if (artist == NULL)
  {
    printf("Desconocido\n");
    return;
  }
  while(artist != NULL)
  {
    printf("%s", artist);
    artist = (char *)list_next(list_artists);
    if (artist != NULL)
    {
      printf(", ");
    }
  }
  printf("\n");
}


void show_songs(List *songs)
{
  limpiarPantalla();
  if(songs == NULL)return;

  printf("\nLista de canciones\n");
  Song *cancion = (Song *)list_first(songs);
  int song_number = 1;

  if(cancion == NULL)
  {
    printf("%sNo se encontraron canciones.%s\n", ROJO, RESETEAR);
    presioneTeclaParaContinuar();
    return;
  }
  while(cancion != NULL)
  {
    printf("Cancion %d\n", song_number);
    printf("  Titulo : %s\n", cancion->track_name); //AGREGAR CONDICIONES PARA PONER DECSONOCIDO EN CASO DE Q NO EXISTA
    printf("  Album : %s\n", cancion->album_name);
    printf("  Genero : %s\n", cancion->track_genre);
    printf("  Tempo : %.2f BPM\n", cancion->tempo);

    show_artists(cancion->artists);
    printf("-------------------------------------\n");
    cancion = (Song *)list_next(songs);
    song_number++;

  }
  presioneTeclaParaContinuar();
}

void buscar_por_tempo(Map *canciones_by_tempo) {
  limpiarPantalla();
  int opcion;
  
  /** Solicita al usuario la categoría de tempo */
  printf("Seleccione la categoría de tempo:\n");
  printf("1) Lentas (menos de 80 BPM)\n");
  printf("2) Moderadas (80-120 BPM)\n");
  printf("3) Rápidas (mayor a 120 BPM)\n");
  printf("Ingrese su opción (1-3): ");
  scanf("%d", &opcion);

  
  char *categoria ;
  // Define los rangos de tempo según la opción 
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
      printf("Opción inválida\n");
      presioneTeclaParaContinuar();
      return;
  }
  MapPair *searched = map_search(canciones_by_tempo, categoria);
  if (searched != NULL) {
    List *canciones = (List *)searched->value;
    printf("\nCanciones con categoría de TEMPO %s\n", categoria);
    show_songs(canciones);
  } else {
    printf("%sNo se encontraron canciones en la categoría %s%s\n", ROJO, categoria, RESETEAR);
    presioneTeclaParaContinuar();
  }
  presioneTeclaParaContinuar();
}

void buscar_por_genero(Map *canciones_bygenres) {
  limpiarPantalla();
  
  char genero[100];

  printf("Ingrese el género de la canción: ");
  printf("Búsquedas recomendadas: “acoustic”, “samba”, “soul”, “anime” \n") ; 
  scanf("%s", genero); 

  MapPair *searched_genre = map_search(canciones_bygenres, genero);
  if (searched_genre != NULL) {
    List *canciones = searched_genre->value;
    
    printf("\nCanciones del género %s:\n", genero);
    show_songs(canciones);
  }
  else {
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
    List *songs = (List *)pair->value;
    
    printf("\nCanciones del artista %s:\n", artista);
    show_songs(songs);
  } else {
    printf("No se encontraron canciones del artista %s\n", artista);
  }
  presioneTeclaParaContinuar();
}
//a
crear_lista_reproduccion(playlists){
  limpiarPantalla();

  char nombre[100];

  printf("Ingrese el nombre de la nueva lista");
  scanf("%[^\n]", nombre);

  Playlist *nueva_playlist = (Playlist *)malloc(sizeof(Playlist));
  if (nueva_playlist == NULL) {
      printf("%sError: No se pudo asignar memoria para la nueva lista de reproducción.%s\n", ROJO, RESETEAR);
      presioneTeclaParaContinuar();
      return;
  }

  // Inicializar los campos de la nueva playlist
  strncpy(nueva_playlist->name, nombre, sizeof(nueva_playlist->name) - 1);
  nueva_playlist->name[sizeof(nueva_playlist->name) - 1] = '\0';
  nueva_playlist->songs = list_create();
  if (nueva_playlist->songs == NULL) {
      printf("%sError: No se pudo crear la lista de canciones para la playlist.%s\n", ROJO, RESETEAR);
      free(nueva_playlist);
      presioneTeclaParaContinuar();
      return;
  }

  // Insertar la nueva playlist en el mapa
  map_insert(playlists, nueva_playlist->name, nueva_playlist);

  printf("%sLista de reproducción '%s' creada exitosamente.%s\n", VERDE, nombre, RESETEAR);
  presioneTeclaParaContinuar();

}


int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);

  Map *canciones_byartist = map_create(is_equal_str);
  Map *canciones_by_tempo = map_create(is_equal_str);
  Map *playlists = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_canciones(canciones_byid, canciones_bygenres,canciones_byartist, canciones_by_tempo);
      break;
    case '2':
      buscar_por_genero(canciones_bygenres);
      break;
    case '3':
    // podemos agregar de que si colocan un nombre que esta incompleto y hay varios nombres similares podemos darle opciones al usuario
      buscar_por_artista(canciones_byartist);
      break;
    case '4':
      buscar_por_tempo(canciones_by_tempo);
      break;
    case '5':
      crear_lista_reproduccion(playlists);
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