#include "../include/launcher.h"
#include <iostream>

void Launcher::Start(int argc, char *argv[]) {
    try {
        Args args(argc, argv);
        Applier applier(args.GetFilters());
        applier.ReadBMP(args.GetInFile());
        applier.ApplyFilters();
        applier.WriteBMP(args.GetOutFile());
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}
