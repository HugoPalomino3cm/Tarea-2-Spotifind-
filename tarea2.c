#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  puts("========================================");
  puts("     Base de Datos de canciones");
  puts("========================================");

  puts("1) Cargar Canciones");
  puts("2) Buscar por género");
  puts("3) Buscar por artista");
  puts("4) Buscar por tempo");
  puts("5) Crear lista de reproducción");  // nos piden que la usuaria ingrese la velocidad de las canciones deseadas (lentas (80 BPM),moderadas (80 y 120BPM) y rapidas (mayor a 120BPM))
  puts("6) Agregar cancion a la lista");  // si es que existe la lista deaaa nasheeeee
  puts("7) Mostrar canciones de una lista");  //si es que existe hay que verificar de que exista
  puts("8) Salir");
}

/** NO BORRE LOS COMENTARIOS DE ESTAS FUNCIONES, PERO EN MI HUMILDE OPINION BORREMOS LOS COMENTARIOS DE ESTA WEAS 
me dan toc**/

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}


/* YA MODIFIQUE EL ARCHIVO EN EL QUE SE TENIA QUE LEER */

void cargar_canciones(Map *canciones_byid, Map *canciones_bygenres) {
  
  FILE *archivo = fopen("data/song_dataset_.csv", "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos;

   // YA REVISE Y SI SE SEPARAN POR COMAS :V 

  campos = leer_linea_csv(archivo, ','); 
  if (campos == NULL) {
    printf("Error: No se pudo leer el encabezado del archivo.\n");
    fclose(archivo);
    printf("Presione una tecla para continuar...\n");
    while (getchar() != '\n');
    getchar();
    return;
  }
  
  int count = 0; // Contador para las canciones cargadas
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // ACA ESTOY AÑADIENDO LAS COSAS ACA SI ESTOY MODIFICANDO YO PALTADEUSSS

    Song *cancion = (Song *)malloc(sizeof(Song));
    if (cancion == NULL) {
      printf("Error: No se pudo asignar memoria para la canción %d.\n", count + 1);
      fclose(archivo);
      printf("Presione una tecla para continuar...\n");
      while (getchar() != '\n');
      getchar();
      return;
    }
    printf("Memoria asignada para canción %d\n", count + 1); // Printeo solo cuando se asigna memoria

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

    cancion->tempo = (campos[18] && strlen(campos[18]) > 0) ? atof(campos[18]) : 0.0;  // la funcion atof es para una cadena que en (si es un numero pero en string) pasarlo a float lo convertimos de cadena a float

    strncpy(cancion->track_genre, campos[20] ? campos[20] : "", sizeof(cancion->track_genre) - 1);
    cancion->track_genre[sizeof(cancion->track_genre) - 1] = '\0';

    // si es que se te ocurre podriamos añadir algo demas ya que hay muchas secciones que no ocupamos una que se me ocurre es (popularity)
    // o también duration podriamos hacer alguna wea exótica con 

    // Inserta la canción en el mapa usando el ID como clave
    map_insert(canciones_byid, cancion->id, cancion);
    
    // Busca el género en el mapa canciones_bygenres
    MapPair *genre_pair = map_search(canciones_bygenres, cancion->track_genre);
    if (genre_pair == NULL) {
      List *new_list = list_create();
      if (new_list == NULL) {
        printf("Error: No se pudo crear una lista para el género %s.\n", cancion->track_genre);
        free(cancion);
        fclose(archivo);
        printf("Presione una tecla para continuar...\n");
        while (getchar() != '\n');
        getchar();
        return;
      }
      list_pushBack(new_list, cancion);
      map_insert(canciones_bygenres, cancion->track_genre, new_list);
    } else {
      List *genre_list = (List *)genre_pair->value;
      list_pushBack(genre_list, cancion);
    }
    count++;
  }
  fclose(archivo); // cerramos el archivo despues despues de leer todas las lineas

  // Verifica si se cargaron todas las canciones (146 en tu CSV)
  if (count == 114000) {
    printf("Se cargaron todas las canciones (%d) exitosamente.\n", count);
  } else {
    printf("No se cargaron todas las canciones. Se cargaron %d de 114000.\n", count);
  }
  while (getchar() != '\n');
  getchar();
}


/**
 * Busca por artista
 */
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

  printf("Ingrese el género de la cancion: ");
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

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_canciones(canciones_byid, canciones_bygenres);
      break;
    case '2':
      buscar_por_genero(canciones_bygenres);
      break;
    case '3':
      //buscar_por_artista(canciones_byid);
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
      //printf("Saliendo de la aplicación...\n");
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
