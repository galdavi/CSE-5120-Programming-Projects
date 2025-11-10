#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <queue>
#include <fstream>
#include <sstream>

int findRoute(std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> &cityMap, const std::string &startCity, const std::string &endCity);

int main(int argc, char *argv[])
{

    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> cityMap{};

    std::string city{argv[2]};
    std::string destination{argv[3]};

    std::ifstream inputFile{argv[1]};
    if (!inputFile)
    {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }

    std::string strInput{};

    while (std::getline(inputFile, strInput))
    {
        if (strInput == "END OF INPUT")
        {
            break;
        }
        std::string cityA{};
        std::string cityB{};

        int distance{};

        std::stringstream ss(strInput);

        // Use the stringstream to parse the line and extract city names and distance
        ss >> cityA >> cityB >> distance;

        //
        cityMap[cityA].push_back({cityB, distance});
        cityMap[cityB].push_back({cityA, distance});
    }

    // for (auto const &[key, val] : cityMap)
    // {
    //     std::cout << key << " children: ";

    //     for (auto const &pair : val)
    //     {
    //         std::cout << "{" << pair.first << " distance: " << pair.second << "} ";
    //     }
    //     std::cout << "\n";
    // }

    std::cout << "Distance: " << findRoute(cityMap, city, destination) << " km\n";
    return 0;
}

int findRoute(std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> &cityMap, const std::string &startCity, const std::string &endCity)
{
    std::queue<std::string> q{};
    int totalDistance = 0;

    q.push(startCity);

    while (!q.empty())
    {
        std::string currentCity = q.front();
        q.pop();

        if (currentCity == endCity)
        {
            return totalDistance;
        }

        for (const auto &neighbor : cityMap[currentCity])
        {
            q.push(neighbor.first);
            totalDistance += neighbor.second;
        }
    }

    return -1;
    
}