#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple> 
#include <fstream>
#include <sstream>
#include <functional>
#include <queue>


// Name alias for better readability of Node representation and graph structure
using Node = std::tuple<int, int, std::string>;     // (totalDistance, level, cityName)
using adjacencyList = std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>; // city -> list of (distance, neighborCity)

// Function prototypes
int findRoute(adjacencyList &cityMap, const std::string &startCity, const std::string &endCity, int limit);
void displayResult(int result, const std::string& startCity, const std::string& endCity);

int main(int argc, char *argv[])
{
    
    std::ifstream inputFile{argv[1]};
    if (!inputFile){
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }

    // Data structure to hold the city map
    adjacencyList cityMap{};
    
    
    std::string strInput{};

    // Process input file line by line
    while (std::getline(inputFile, strInput))
    {
        if(strInput == "END OF INPUT"){
            break;
        }

        std::string cityA{};
        std::string cityB{};
        int distance{};

        //Parse the line
        std::stringstream ss(strInput);
        ss >> cityA >> cityB >> distance;

        // Populate the city map (undirected graph)
        cityMap[cityA].push_back({distance, cityB});
        cityMap[cityB].push_back({distance, cityA});

    }
    
    // Retrieve start and end cities from command line arguments
    std::string startCity{argv[2]};
    std::string endCity{argv[3]};
    
    int result{-1};



    for(int limit{0}; limit < static_cast<int>(cityMap.size()); ++limit)
    {
        result = findRoute(cityMap, startCity, endCity, limit);
        if (result != -1)
        {
            break;
        }
    }
    
    
    displayResult(result, startCity, endCity);

    return 0;
}


int findRoute(adjacencyList &cityMap, const std::string &startCity, const std::string &endCity, int limit)
{
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minPQ{};

    minPQ.push(std::make_tuple(0, 0, startCity)); // (totalDistance, level, cityName)


    while (!minPQ.empty())
    {
        auto currentNode {minPQ.top()};
        minPQ.pop();

        int currentDistance {std::get<0>(currentNode)};
        int currentLevel {std::get<1>(currentNode)};
        std::string currentCity {std::get<2>(currentNode)};
        
        if (currentCity == endCity){
            return currentDistance;
        }


        if (currentLevel < limit){

            // Explore neighbor cities
            for (const auto& neighbor : cityMap[currentCity]){
                int neighborDistance {neighbor.first};
                std::string neighborCity {neighbor.second};
                
                // Push neighbor city into the priority queue with updated distance and level
                minPQ.push(std::make_tuple(currentDistance + neighborDistance, currentLevel + 1, neighborCity));
            }
        }
    }
    return -1;
}
// Function to display the result, follows output format specifications
void displayResult(int result, const std::string& startCity, const std::string& endCity){
    std::cout << "distance: ";
    if (result != -1){
        std::cout << result << " km\n";
    } else{
        std::cout << "infinity\n";
    }

    std::cout << "route:\n";
    if (result != -1){
        std::cout << startCity << " to " << endCity << ", " << result << " km\n";
    } else{
        std::cout << "none\n";
    }

}