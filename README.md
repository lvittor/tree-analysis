# Procesamiento de datos de árboles en el espacio público de una ciudad

## Objetivo

Se busca procesar los datos de los árboles de dos ciudades: Ciudad Autónoma de Buenos Aires, Argentina :argentina: y Vancouver, British Columbia, Canadá :canada:.  Ambos datos son extraídos de los respectivos portales de gobierno en formato CSV (https://data.buenosaires.gob.ar/dataset/arbolado-publico-lineal/archivo/ecf38a47-563f-42c1-9bd4-7cedf35d536b y https://opendata.vancouver.ca/explore/dataset/street-trees). Si bien se detalla el origen de los archivos, los mismos fueron modificados para simplificar su procesamiento. 

Esta implementación funciona también para procesar los datos de árboles de cualquier otra ciudad, manteniendo siempre la estructura de los archivos que se presenta a continuación. 

## Descripción funcional

El programa consiste en dos ejecutables distintos, uno para cada dataset. Cada ejecutable lee dos archivos CSV: uno con la información de los
árboles correspondientes a una ciudad y otro archivo con la información de los barrios de esa ciudad. En ambos el delimitador es un punto y coma (“;”). Se asume que el formato y contenido de los archivos es correcto.

### Datos de árboles de Buenos Aires :argentina: (arbolesBUE.csv) ​

```
❖ Cantidad de líneas: ​ 370.089 (incluyendo el encabezado)
❖ Campos Relevantes:
  ● comuna ​ : Nombre del barrio donde se encuentra el árbol.
  ● calle_nombre ​: Nombre de la calle donde se encuentra el árbol
  ● nombre_cientifico ​: Nombre científico del árbol
  ● diametro_altura_pecho ​: Diámetro del árbol a la altura del pecho
```
El archivo se compone de una primera línea de encabezado, con los títulos de cada campo.
De la segunda línea en adelante, cada línea representa un árbol conteniendo los datos de cada uno de
los campos, separados por “;”.

### Datos de barrios de Buenos Aires :argentina: (barriosBUE.csv) ​
```
❖ Cantidad de líneas: ​ 16 (incluyendo el encabezado)
❖ Campos:
  ● nombre ​:​ Nombre del barrio, para relacionarlo con el campo "comuna" de arbolesBUE.csv.
  ● habitantes ​:​ Cantidad de habitantes del barrio.
```
El archivo se compone de una primera línea de encabezado. De la segunda línea en adelante,
cada línea representa un barrio.

###  Datos de árboles de Vancouver :canada: (arbolesVAN.csv) ​

```
❖ Cantidad de líneas: ​ 146.746 (incluyendo el encabezado)
❖ Campos Relevantes:
  ● NEIGHBOURHOOD_NAME ​:​ Nombre del barrio donde se encuentra el árbol.
  ● STD_STREET ​: Nombre de la calle donde se encuentra el árbol
  ● COMMON_NAME ​: Nombre científico del árbol
  ● DIAMETER ​: Diámetro del árbol a la altura del pecho
```
El archivo se compone de una primera línea de encabezado, con los títulos de cada campo.
De la segunda línea en adelante, cada línea representa un árbol conteniendo los datos de cada uno de
los campos, separados por “,”.

### Datos de barrios de Vancouver :canada: (barriosVAN.csv) ​
```
❖ Cantidad de líneas: ​23 (incluyendo el encabezado)
❖ Campos:
  ● nombre ​:​ Nombre del barrio, para relacionarlo con el campo "NEIGHBOURHOOD_NAME" ​ de ​arbolesVAN.csv​.
  ● habitantes ​:​ Cantidad de habitantes del barrio.
```
El archivo se compone de una primera línea de encabezado. De la segunda línea en adelante,
cada línea representa un barrio.

## Queries 

Una vez recibido correctamente los path de los archivos CSV, el programa resuelve las consultas que se listan a continuación, dejando los resultados de cada
una en archivos queryN.csv y localizados en el mismo directorio que el ejecutable.

### Total de árboles por barrio (query1.csv)

Cada línea de la salida contiene, separados por “;” el nombre del barrio y el total de árboles pertenecientes a ese barrio.
Sólo se listan los barrios presentes en el archivo CSV de barrios.
El orden de impresión es descendente por cantidad de árboles y luego alfabético por nombre de barrio.

### Total de árboles por habitante (query2.csv)

Cada línea de la salida contiene, separados por “;” el nombre del barrio y el total de árboles por habitante (que consiste en el cociente entre el total de árboles de ese barrio y el número de habitantes del mismo).
Sólo se listan los barrios presentes en el archivo CSV de barrios.
El orden de impresión es descendente por el total de árboles por habitante y luego alfabético por nombre de barrio.
El total de árboles por habitante se imprime truncado a dos decimales.

### Diámetro promedio por especie de árbol (query3.csv)

Cada línea de la salida contiene, separados por “;” el nombre de la especie del árbol y el promedio del diámetro del árbol a la altura del pecho de esa especie.
El orden de impresión es descendente por diámetro y luego alfabético por nombre de la especie.
El promedio del diámetro del árbol a la altura del pecho se imprime truncado a dos decimales.

## Creación de los ejecutables

### Modo debug

```bash
  $make clean
  $make debug
```

### Modo producción

```bash
  $make clean
  $make all
```

## Uso de los ejecutables

El primer parámetro a pasarle al ejecutable debe ser el path al archivo de árboles y el segundo, al archivo de barrios.

Para el caso de BUE:
```bash
  $./arbolesADTBUE arbolesBUE.csv barriosBUE.csv
```
El ejecutable arbolesADTBUE tiene configurado que los datos se encuentran distribuidos de acuerdo a:

    barriosBUE.csv:
        * "nombre" en columna 1
        * "habitantes" en columna 2

    arbolesBUE.csv:
        * "comuna" en columna 3
        * "nombre_cientifico" en columna 8
        * "diametro_altura_pecho" en columna 12

Para el caso de VAN:
```bash
  $./arbolesADTVAN arbolesVAN.csv barriosVAN.csv
```
El ejecutable arbolesADTVAN tiene configurado los datos se encuentran distribuidos de acuerdo a:

    barriosBUE.csv:
        * "nombres" en columna 1
        * "habitantes" en columna 2

    arbolesBUE.csv:
        * "NEIGHBOURHOOD_NAME" en columna 13
        * "COMMON_NAME" en columna 7
        * "DIAMETER" en columna 16
