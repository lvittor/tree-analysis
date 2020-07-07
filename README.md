Si se tiene en el .csv de arboles campos vacios con doble ";" seguidos, se puede modificar para que funcione bien el programa de la forma:

```bash
sed -i 's/;;/; ;/g' nombre-archivo-arbol.csv
```
