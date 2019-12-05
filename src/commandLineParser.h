#ifndef MOVIAC_COMMANDLINEPARSER_H
#define MOVIAC_COMMANDLINEPARSER_H

#include <string>

using namespace std;

struct CDLArguments {
    string sourceVideoPath = "";
    string sourcePosterPath = "";
    string destinationPosterPath = "";

    // True if command line wasn't parsed but there was no exception, such as printing help.
    bool noParse = false;
};

CDLArguments parseCommands(int argc, char *argv[]);

#endif //MOVIAC_COMMANDLINEPARSER_H
