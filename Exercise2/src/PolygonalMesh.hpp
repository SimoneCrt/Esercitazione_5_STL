#pragma once
#include <vector>
//creo una struttura che mi rappresenti un punto
struct Cell0D
{
    unsigned int id;
    int marker;
    double x, y;
};
//creo una struttura che mi rappresenti un segmento(linea)
struct Cell1D
{
    unsigned int id;
    int marker;
    unsigned int origin, end;
};
//creo una struttura che mi rappresenti un poligono
struct Cell2D
{
    unsigned int id;
    int marker;
    std::vector<unsigned int> vertices; //vettore che contiene gli id dei punti
    std::vector<unsigned int> edges;    //vettore che contiene gli id dei segmenti
};
//creo struttura che aggrega tutta la mesh
struct PolygonalMesh
{
    std::vector<Cell0D> Cell0Ds; //vettore che contiene tutti i punti
    std::vector<Cell1D> Cell1Ds; //vettore che contiene tutti i lati
    std::vector<Cell2D> Cell2Ds; //vettore che contiene tutti i poligoni
};

































