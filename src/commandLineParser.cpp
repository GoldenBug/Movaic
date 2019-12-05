#include <iostream>

#include "commandLineParser.h"
#include "boost/program_options.hpp"

using namespace boost::program_options;

CDLArguments parseCommands(int argc, char **argv) {

    CDLArguments parsedArguments;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Print help messages")
            ("poster,p", value<string>()->required(),
             "Path of the input poster that will be used as reference to create the output")
            ("video,v", value<string>()->required(),
             "Path of movie that will be used as a source of replacement frames")
            ("output,o", value<string>()->required(),
             "Path and filename of the output poster");

    variables_map vm;
    try {
        store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            cout << "Movaic (Movie Mosaic) is a command line application that can be used to create "
                    "picture mosaics using a source image and a video clip.\n" << endl;
            cout << desc << endl;

            parsedArguments.noParse = true;
            return parsedArguments;
        }

        notify(vm);

        parsedArguments.sourceVideoPath = vm["video"].as<string>();
        parsedArguments.sourcePosterPath = vm["poster"].as<string>();
        parsedArguments.destinationPosterPath = vm["output"].as<string>();

    }
    catch (error &e) {
        cerr << "ERROR: " << e.what() << endl << endl;
        cerr << desc << endl;
        throw(e);
    }

    return parsedArguments;
}
