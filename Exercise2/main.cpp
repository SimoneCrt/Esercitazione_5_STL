#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <iostream>

int main()
{
    PolygonalMesh mesh; //creo oggetto mesh che contiene i dati della mesh
    if (!ImportMesh("Cell0Ds.csv", "Cell1Ds.csv", "Cell2Ds.csv", mesh))
    {
        std::cerr << "Failed to import mesh." << std::endl;
        return 1;
    }

    std::cout << "Mesh imported successfully!" << std::endl;

    // Esporta la mesh in formato UCD (per ParaView)
    Gedim::UCDUtilities exporter;

    Eigen::MatrixXd points(3, mesh.Cell0Ds.size()); //matrice points(3 cordinate e n elementi)
    Eigen::VectorXi pointMarkers(mesh.Cell0Ds.size());  //vettore marker
    
	//voglio riempire la matrice points e assegnare il marker corrispondente a ciascun punto
    for (size_t i = 0; i < mesh.Cell0Ds.size(); ++i)
    {
        points(0, i) = mesh.Cell0Ds[i].x;
        points(1, i) = mesh.Cell0Ds[i].y;
        points(2, i) = 0.0;
        pointMarkers(i) = mesh.Cell0Ds[i].marker;
    }
    //creo un vettore di triangoli e di materiali per l'esportazione
    std::vector<std::vector<unsigned int>> triangles; //vettore di vettori
    Eigen::VectorXi materials;

    for (const auto& cell : mesh.Cell2Ds)
    {
        if (cell.vertices.size() == 3)
            triangles.push_back(cell.vertices);
    }

    materials.resize(triangles.size()); //ridimensiono il vettore
    for (size_t i = 0; i < triangles.size(); ++i)
        materials[i] = mesh.Cell2Ds[i].marker;

    exporter.ExportPolygons("mesh_output.inp", points, triangles, {}, {}, materials);

    std::cout << "Mesh exported to mesh_output.inp" << std::endl;
    return 0;
}
































