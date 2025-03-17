#include <string>
#include <iostream>
#include <curl/curl.h>
#include <queue>
#include <unordered_set>
#include <bits/stdc++.h>
#include "rapidjson/document.h"

using namespace std;

size_t my_write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string* strang = (std::string*)userdata;
    for (size_t i = 0; i<nmemb; ++i) {
        strang->push_back(ptr[i]);
    }
    return nmemb;
}

vector<string> getNeighbors(const string& node)
{
    CURL *curl = curl_easy_init();
    vector<string> neighbors;

    std::string URL = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";
    char* encodeName = curl_easy_escape(curl, node.c_str(), node.size());
    std::string fullURL = URL + encodeName;
    curl_free(encodeName);

    std::string outstring;
    curl_easy_setopt(curl, CURLOPT_URL, fullURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&outstring);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_data);
    curl_easy_perform(curl);
    rapidjson::Document document;
    document.Parse(outstring.c_str());
    
    const rapidjson::Value& neighborsArray = document["neighbors"];
    for (rapidjson::SizeType i = 0; i < neighborsArray.Size(); i++)
    {
        neighbors.push_back(neighborsArray[i].GetString());
    }
    curl_easy_cleanup(curl);
    return neighbors;
}

int main(int argc, char* argv[]) {
    std::string startNode = argv[1];
    int maxDepth = stoi(argv[2]);
    curl_global_init(CURL_GLOBAL_ALL);

    queue<pair<string, int>> toVisit;
    unordered_set<string> visited;
    vector<string> bfsOrder;

    toVisit.push({startNode, 0});
    visited.insert(startNode);
    while (!toVisit.empty())
    {
        int currDepth = toVisit.front().second;
        std::string currentNode = toVisit.front().first;
        toVisit.pop();

        if (currDepth >= maxDepth)
        {
            continue;
        }
        bfsOrder.push_back(currentNode);
        vector<string> neighbors = getNeighbors(currentNode);

        for (const string& neighbor : neighbors)
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                toVisit.push({neighbor, currDepth + 1});
            }
        }
    }

    std::cout << "\nBFS Traversal Order: \n";
    for (const auto& node : bfsOrder)
    {
        std::cout << node << "\n" << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }
    curl_global_cleanup();
    return 0;
}