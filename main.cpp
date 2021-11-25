#include "archiver.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <string>

bool ParseArguments(const std::vector<std::string_view>& arguments) {
    if (arguments.size() == 2 && arguments[1] == "-h") {
        std::cout << "Command usage:\n\n";
        std::cout << "archiver -c archive_name file1 [file2 ...] --- archive "
                     "files \"file1\" [\"file2\", ...] and "
                     "save the result in the file \"archive_name\".\n\n";
        std::cout << "archiver -d archive_name --- unarchive files from the "
                     "archive \"archive_name\" and put in the "
                     "current directory. Filenames will be preserved.\n\n";
        std::cout << "archiver -h --- show this menu.\n";
    } else if (arguments.size() >= 4 && arguments[1] == "-c") {
        std::vector<std::string_view> filenames(arguments.size() - 3);
        for (size_t i = 0; i < arguments.size() - 3; i++) {
            filenames[i] = arguments[i + 3];
        }
        Archive(arguments[2], filenames);
    } else if (arguments.size() == 3 && arguments[1] == "-d") {
        Unarchive(arguments[2]);
    } else {
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    std::vector<std::string_view> arguments(argc);
    for (int i = 0; i < argc; i++) {
        arguments[i] = argv[i];
    }

    try {
        if (ParseArguments(arguments)) {
            std::cout << "Command executed successfully.\n";
        } else {
            std::cout << "Wrong command usage. For help execute \"archiver -h\".\n";
        }
    } catch (const std::ios_base::failure& e) {
        std::cout << "File error: " << e.what() << ".\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Test error: " << e.what() << ".\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Arguments error: " << e.what() << ".\nFor help execute \"archiver -h\".\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << ".\n";
    }

    return 0;
}
