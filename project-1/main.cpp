#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <queue>
#include <fstream>
#include <sstream>
#include <functional> // Required for std::greater
#include <stack>

typedef std::pair<std::pair<int, int>, std::string> Node;

int findRoute(std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> &cityMap, const std::string &startCity, const std::string &endCity, int limit);

int main(int argc, char *argv[])
{

    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> cityMap{};

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
        cityMap[cityA].push_back({distance, cityB});
        cityMap[cityB].push_back({distance, cityA});
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

    int result{-1};
    for (int limit{0}; limit < static_cast<int>(cityMap.size()); ++limit)
    {
        result = findRoute(cityMap, city, destination, limit);
        if (result != -1)
        {
            break;
        }
    }

    if (result != -1)
    {
        std::cout << "Distance: " << result << " km\n";
    }
    else
    {
        std::cout << "NO ROUTE\n";
    }
    return 0;
}

int findRoute(std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> &cityMap, const std::string &startCity, const std::string &endCity, int limit)
{

    int totalCities{static_cast<int>(cityMap.size())};

    std::priority_queue<Node,
                        std::vector<Node>,
                        std::greater<Node>>
        pq{};

    int level{0};
    pq.push({{0, level}, startCity});

    while (!pq.empty())
    {

        std::string currentCity{pq.top().second};
        int totalDistance{pq.top().first.first};
        int currentLevel{pq.top().first.second};

        if (currentLevel > limit)
        {
            break;
        }
        
        pq.pop();

        if (currentCity == endCity)
        {
            return totalDistance;
        }
        for (const auto &neighbor : cityMap[currentCity])
        {
            pq.push({{neighbor.first + totalDistance, level}, neighbor.second});
        }
        level++;
    }

    return -1;
}