# Trabajo Práctico Especial

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
