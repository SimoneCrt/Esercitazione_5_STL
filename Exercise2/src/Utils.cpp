#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

bool ImportMesh(const std::string& file0D, const std::string& file1D, const std::string& file2D, PolygonalMesh& mesh)
{
    std::ifstream f0(file0D), f1(file1D), f2(file2D);
    if (!f0.is_open() || !f1.is_open() || !f2.is_open())
    {
        std::cerr << "Errore: impossibile aprire uno o più file." << std::endl;
        return false;
    }

    std::string line;

    // Procedo con la Lettura di Cell0D
    std::getline(f0, line); // salta header
    while (std::getline(f0, line))
    {
        std::stringstream ss(line); //estraggo i singoli dati separati da ";"
        Cell0D cell;  //dichiaro la variabile cell di tipo Cell0D
        std::string field;
        std::getline(ss, field, ';'); cell.id = std::stoi(field);
        std::getline(ss, field, ';'); cell.marker = std::stoi(field);
        std::getline(ss, field, ';'); cell.x = std::stod(field);
        std::getline(ss, field, ';'); cell.y = std::stod(field);
        mesh.Cell0Ds.push_back(cell);
    }

    // Procedo con la Lettura di Cell1D
    std::getline(f1, line); // salta header
    while (std::getline(f1, line))
    {
        std::stringstream ss(line);
        Cell1D cell;
        std::string field;
        std::getline(ss, field, ';'); cell.id = std::stoi(field);
        std::getline(ss, field, ';'); cell.marker = std::stoi(field);
        std::getline(ss, field, ';'); cell.origin = std::stoi(field);
        std::getline(ss, field, ';'); cell.end = std::stoi(field);
        mesh.Cell1Ds.push_back(cell);

        // Verifica lunghezza lato (richiesto dal testo)
        const Cell0D& p1 = mesh.Cell0Ds[cell.origin]; //accedo al punto di origine della mesh
        const Cell0D& p2 = mesh.Cell0Ds[cell.end];    //accedo al punto di fine della mesh
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        double length = std::sqrt(dx * dx + dy * dy); //distanza euclidea
        if (length == 0.0)
        {
            std::cerr << "Errore: lato con lunghezza nulla tra i punti "
                      << cell.origin << " e " << cell.end << std::endl;
        }
    }

    // Procedo con la Lettura di Cell2D
    std::getline(f2, line); // salta header

    int countZeroArea = 0; // contatore triangoli con area nulla

    while (std::getline(f2, line))
    {
        std::stringstream ss(line);
        Cell2D cell;
        std::string field;

        std::getline(ss, field, ';'); cell.id = std::stoi(field);
        std::getline(ss, field, ';'); cell.marker = std::stoi(field);
        std::getline(ss, field, ';'); int numVerts = std::stoi(field);

        for (int i = 0; i < numVerts; ++i)
        {
            std::getline(ss, field, ';');
            cell.vertices.push_back(std::stoi(field));
        }

        std::getline(ss, field, ';'); int numEdges = std::stoi(field);

        for (int i = 0; i < numEdges; ++i)
        {
            std::getline(ss, field, ';');
            cell.edges.push_back(std::stoi(field));
        }

        // Verifica area solo se è un triangolo
        if (cell.vertices.size() == 3)
        {
            const Cell0D& A = mesh.Cell0Ds[cell.vertices[0]];
            const Cell0D& B = mesh.Cell0Ds[cell.vertices[1]];
            const Cell0D& C = mesh.Cell0Ds[cell.vertices[2]];
            //calcolo area utilizzando formula area con determinante
            double area = 0.5 * std::abs(
                A.x * (B.y - C.y) +
                B.x * (C.y - A.y) +
                C.x * (A.y - B.y)
            );

            if (area == 0.0)
            {
                ++countZeroArea;
                std::cerr << "Attenzione: triangolo con area nulla. Vertici ID: "
                          << cell.vertices[0] << ", "
                          << cell.vertices[1] << ", "
                          << cell.vertices[2] << std::endl;
            }
        }

        //Aggiungi SEMPRE il triangolo
        mesh.Cell2Ds.push_back(cell);
    }

    if (countZeroArea > 0)
    {
        std::cout << "Totale triangoli con area nulla: " << countZeroArea << std::endl;
    }

    std::cout << "Importazione completata con successo." << std::endl;
    return true;
}





























