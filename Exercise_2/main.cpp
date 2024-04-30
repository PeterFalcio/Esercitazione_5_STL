#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct Cell0D {
    int id;
    int marker;
    double x;
    double y;
};

struct Cell1D {
    int id;
    int marker;
    int origin;
    int end;
};

struct Cell2D {
    int id;
    int marker;
    int numVertices;
    std::vector<int> vertices;
    int numEdges;
    std::vector<int> edges;
};

double distance(double x1, double y1, double x2, double y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double triangleArea(double x1, double y1, double x2, double y2, double x3, double y3) {
    return 0.5 * std::abs((x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)));
}

int main() {
    // Read Cell0D data
    std::ifstream cell0D_file("Cell0Ds.csv");
    std::vector<Cell0D> cell0Ds;
    std::string line;
    getline(cell0D_file, line); // Skip header
    while (getline(cell0D_file, line)) {
        std::stringstream ss(line);
        Cell0D cell0D;
        char delim;
        ss >> cell0D.id >> delim >> cell0D.marker >> delim >> cell0D.x >> delim >> cell0D.y;
        cell0Ds.push_back(cell0D);
    }
    cell0D_file.close();

    // Read Cell1D data
    std::ifstream cell1D_file("Cell1Ds.csv");
    std::vector<Cell1D> cell1Ds;
    getline(cell1D_file, line); // Skip header
    while (getline(cell1D_file, line)) {
        std::stringstream ss(line);
        Cell1D cell1D;
        char delim;
        ss >> cell1D.id >> delim >> cell1D.marker >> delim >> cell1D.origin >> delim >> cell1D.end;
        cell1Ds.push_back(cell1D);
    }
    cell1D_file.close();

    // Read Cell2D data
    std::ifstream cell2D_file("Cell2Ds.csv");
    std::vector<Cell2D> cell2Ds;
    getline(cell2D_file, line); // Skip header
    while (getline(cell2D_file, line)) {
        std::stringstream ss(line);
        Cell2D cell2D;
        char delim;
        ss >> cell2D.id >> delim >> cell2D.marker >> delim >> cell2D.numVertices >> delim;
        for (int i = 0; i < cell2D.numVertices; ++i) {
            int vertex;
            ss >> vertex >> delim;
            cell2D.vertices.push_back(vertex);
        }
        ss >> cell2D.numEdges >> delim;
        for (int i = 0; i < cell2D.numEdges; ++i) {
            int edge;
            ss >> edge >> delim;
            cell2D.edges.push_back(edge);
        }
        cell2Ds.push_back(cell2D);
    }
    cell2D_file.close();

    // Test properties
    // Test all markers have been stored correctly
    for (const auto& cell0D : cell0Ds) {
        if (cell0D.marker < 0 || cell0D.marker > 8) {
            std::cerr << "Error: Invalid marker for Cell0D with ID " << cell0D.id << std::endl;
        }
    }
    for (const auto& cell1D : cell1Ds) {
        if (cell1D.marker < 0 || cell1D.marker > 8) {
            std::cerr << "Error: Invalid marker for Cell1D with ID " << cell1D.id << std::endl;
        }
    }
    for (const auto& cell2D : cell2Ds) {
        if (cell2D.marker < 0 || cell2D.marker > 8) {
            std::cerr << "Error: Invalid marker for Cell2D with ID " << cell2D.id << std::endl;
        }
    }

    // Test edges of triangles have non-zero length
    for (const auto& cell2D : cell2Ds) {
        for (int i = 0; i < cell2D.numVertices; ++i) {
            int v1 = cell2D.vertices[i];
            int v2 = cell2D.vertices[(i + 1) % cell2D.numVertices];
            double len = distance(cell0Ds[v1].x, cell0Ds[v1].y, cell0Ds[v2].x, cell0Ds[v2].y);
            if (len == 0) {
                std::cerr << "Error: Edge between vertices " << v1 << " and " << v2 << " has zero length." << std::endl;
            }
        }
    }

    // Test area of triangles is non-zero
    for (const auto& cell2D : cell2Ds) {
        double x1 = cell0Ds[cell2D.vertices[0]].x;
        double y1 = cell0Ds[cell2D.vertices[0]].y;
        double x2 = cell0Ds[cell2D.vertices[1]].x;
        double y2 = cell0Ds[cell2D.vertices[1]].y;
        double x3 = cell0Ds[cell2D.vertices[2]].x;
        double y3 = cell0Ds[cell2D.vertices[2]].y;
        double area = triangleArea(x1, y1, x2, y2, x3, y3);
        if (area == 0) {
            std::cerr << "Error: Triangle with vertices " << cell2D.vertices[0] << ", "
                      << cell2D.vertices[1] << ", " << cell2D.vertices[2] << " has zero area." << std::endl;
        }
    }

    return 0;
}
