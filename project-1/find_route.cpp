#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <fstream>
#include <sstream>
#include <functional>
#include <queue>

using adjacencyList = std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>; // city -> list of (distance, neighborCity)

struct Node
{
    int cost{0};
    std::string city{};
    std::string parent{};
    std::vector<Node> path{};
    // Overload the < operator for priority queue
    bool operator<(const Node &other) const
    {
        // For min-heap based on cost
        return cost > other.cost;
    }
};

std::vector<Node> findShortestPath(const adjacencyList &cityMap, const std::string &startCity, const std::string &endCity);
void displayResults(const std::vector<Node> &path);

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <start_city> <end_city>\n";
        return 1;
    }
    
    std::ifstream inputFile{argv[1]};
    if (!inputFile)
    {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }

    adjacencyList cityMap{};
    std::string strInput{};

    // Process input file line by line
    while (std::getline(inputFile, strInput))
    {
        if (strInput == "END OF INPUT")
        {
            break;
        }

        std::string cityA{};
        std::string cityB{};
        int distance{};

        // Parse the line
        std::stringstream ss(strInput);
        ss >> cityA >> cityB >> distance;

        // Populate the city map (undirected graph)
        cityMap[cityA].push_back({distance, cityB});
        cityMap[cityB].push_back({distance, cityA});
    }

    // Retrieve start and end cities from command line arguments
    std::string startCity{argv[2]};
    std::string endCity{argv[3]};
    

    int numberOfCities{static_cast<int>(cityMap.size())};
    std::vector<Node> shortestPath {findShortestPath(cityMap, startCity, endCity)};

    displayResults(shortestPath);

    return 0;
}

// Function to find the shortest path using Uniform Cost Search with depth limit, returns the path as a vector of Nodes
std::vector<Node> findShortestPath(const adjacencyList &cityMap, const std::string &startCity, const std::string &endCity)
{

    std::priority_queue<Node> pq{};

    // Initialize the start node
    Node startNode{0, startCity, "", {}};

    // If start and end cities are the same, return immediately
    if(startCity == endCity)
    {
        return {startNode, startNode};
    }

    startNode.path.push_back(startNode);
    

    // Initialize the priority queue with the start city
    pq.push(startNode);

    while (!pq.empty())
    {
        Node currNode = pq.top();
        pq.pop();

        // If we reached the destination city, return the path
        if (currNode.city == endCity)
        {
            return currNode.path;
        }

        if(cityMap.find(currNode.city) == cityMap.end())
        {
            continue; // No neighbors to explore
        }

        // Explore neighbors
        for (const auto &neighbor : cityMap.at(currNode.city))
        {
            bool inPath{false};

            for(const auto &nodeInPath : currNode.path)
            {
                if(nodeInPath.city == neighbor.second)
                {
                    inPath = true;
                    break;
                }
            }

            //Node is unique in path, proceed
            if(!inPath) 
            {
                // Create a new node for the neighbor (current cost, neighbor city, current city as parent, updated path)
                Node nextNode{neighbor.first + currNode.cost, neighbor.second, currNode.city, currNode.path};
                nextNode.path.push_back(nextNode);
                pq.push(nextNode);
            }
        }
    }
    return {};
}


// Function to display the results
void displayResults(const std::vector<Node> &path)
{
    // If the path is empty, no route was found
    if (path.empty())
    {
        std::cout << "distance: infinity\n";
        std::cout << "route:\nnone\n";
        return;
    }

    // Display total distance and route
    int totalDistance = path.back().cost;
    std::cout << "distance: " << totalDistance << " km\n";
    std::cout << "route:\n";

    // Display each segment of the route
    for (size_t i = 1; i < path.size(); ++i)
    {
        int segmentDistance = path[i].cost - path[i - 1].cost;
        std::cout << path[i - 1].city << " to " << path[i].city << ", " << segmentDistance << " km\n";
    }
}
