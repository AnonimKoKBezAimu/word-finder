#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

std::vector<std::string> file_matches;

struct Arguments
{
    std::string word;
    std::string path = ".";
    bool verbose = false;
    bool case_sensitive = false;
};

Arguments ParseArguments(int argc, char* arvg[]);
void PrintMatches(std::vector<std::string>& file_matches);
void SearchFile(const Arguments& args, const std::filesystem::path& path);
void SearchDirectory(const Arguments& args, const std::filesystem::path& path);

int main(int argc, char* argv[])
{
    Arguments args = ParseArguments(argc, argv); 
    
    SearchDirectory(args, std::filesystem::path(args.path));
    PrintMatches(file_matches);
}

Arguments ParseArguments(int argc, char* argv[])
{
    Arguments args;
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "--word")
        {
            args.word = argv[i+1];
        }
        else if (std::string(argv[i]) == "--path")
        {
            args.path = argv[i+1];
        }
        else if(std::string(argv[i]) == "--verbose")
        {
            args.verbose = true;
        }
        else if(std::string(argv[i]) == "--case-sensitive")
        {
            args.case_sensitive = true;
        }
    } 

    if (args.word.empty())
    {
        std::cout << "Word argument missing add --word <target-word>";
        std::exit(1);
    }
    else
    {
        return args;
    }
}

void SearchDirectory(const Arguments& args, const std::filesystem::path& path)
{
    if (args.verbose)  
    {
        std::cout << "Entering directory: " << args.path << '\n';
    } 
    
    for(const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            SearchDirectory(args, entry.path());
        }
        else if (entry.is_regular_file())
        {
            SearchFile(args, entry.path());
        }
        else
        {
            std::cout << "Unknown file type: " << entry.path().filename();
        }
    }
}

void SearchFile(const Arguments& args, const std::filesystem::path& path)
{
    if (args.verbose)  
    {
        std::cout << "Opening file: " << args.path << '\n';
    } 

    std::ifstream file(path);
    if(!file.is_open())  
    { 
        std::cout << "Cant open file: " << path << '\n';
        return;
    } 

    std::string word;
    while(file >> word)
    {
        if (!args.case_sensitive)
        {
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
        }
        if(word == args.word)
        {
            if (args.verbose)
            {
                std::cout << "Match found!" << '\n';
            }
            file_matches.push_back(path.string());
        }
    }
}

void PrintMatches(std::vector<std::string>& file_matches)
{
    for (std::string& match : file_matches)
    {
        std::cout << match << '\n';
    }
}