#include "interface.h"
#include <iostream>

void cleanScreen(int I) {
    for (int i = 0; i < I; i++)
        std::cout << std::endl;
}


OnceAnnouncement::OnceAnnouncement(int I) {

    switch (I) {
        case 0:
            cout << " ██████╗██╗  ██╗███████╗███████╗███████╗" << endl;
            cout << "██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝" << endl;
            cout << "██║     ███████║█████╗  ███████╗███████╗" << endl;
            cout << "██║     ██╔══██║██╔══╝  ╚════██║╚════██║" << endl;
            cout << "╚██████╗██║  ██║███████╗███████║███████║" << endl;
            cout << " ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝" << endl;
            break;
        case 1:
            cout << " ▄████▄   ██░ ██ ▓█████   ██████   ██████ " << endl;
            cout << "▒██▀ ▀█  ▓██░ ██▒▓█   ▀ ▒██    ▒ ▒██    ▒ " << endl;
            cout << "▒▓█    ▄ ▒██▀▀██░▒███   ░ ▓██▄   ░ ▓██▄   " << endl;
            cout << "▒▓▓▄ ▄██▒░▓█ ░██ ▒▓█  ▄   ▒   ██▒  ▒   ██▒" << endl;
            cout << "▒ ▓███▀ ░░▓█▒░██▓░▒████▒▒██████▒▒▒██████▒▒" << endl;
            cout << "░ ░▒ ▒  ░ ▒ ░░▒░▒░░ ▒░ ░▒ ▒▓▒ ▒ ░▒ ▒▓▒ ▒ ░" << endl;
            cout << "  ░  ▒    ▒ ░▒░ ░ ░ ░  ░░ ░▒  ░ ░░ ░▒  ░ ░" << endl;
            cout << "░         ░  ░░ ░   ░   ░  ░  ░  ░  ░  ░  " << endl;
            cout << "░ ░       ░  ░  ░   ░  ░      ░        ░  " << endl;
            cout << "░                                         " << endl;
            cout << endl;
            break;
        case 2:
            cout << " ▄████████    ▄█    █▄       ▄████████    ▄████████    ▄████████ " << endl;
            cout << "███    ███   ███    ███     ███    ███   ███    ███   ███    ███ " << endl;
            cout << "███    █▀    ███    ███     ███    █▀    ███    █▀    ███    █▀  " << endl;
            cout << "███         ▄███▄▄▄▄███▄▄  ▄███▄▄▄       ███          ███        " << endl;
            cout << "███        ▀▀███▀▀▀▀███▀  ▀▀███▀▀▀     ▀███████████ ▀███████████ " << endl;
            cout << "███    █▄    ███    ███     ███    █▄           ███          ███ " << endl;
            cout << "███    ███   ███    ███     ███    ███    ▄█    ███    ▄█    ███ " << endl;
            cout << "████████▀    ███    █▀      ██████████  ▄████████▀   ▄████████▀  " << endl;
            break;
        case 3:
            cout << " ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄ " << endl;
            cout << "▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌" << endl;
            cout << "▐░█▀▀▀▀▀▀▀▀▀ ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ " << endl;
            cout << "▐░▌          ▐░▌       ▐░▌▐░▌          ▐░▌          ▐░▌          " << endl;
            cout << "▐░▌          ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ " << endl;
            cout << "▐░▌          ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌" << endl;
            cout << "▐░▌          ▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀█░▌ ▀▀▀▀▀▀▀▀▀█░▌" << endl;
            cout << "▐░▌          ▐░▌       ▐░▌▐░▌                    ▐░▌          ▐░▌" << endl;
            cout << "▐░█▄▄▄▄▄▄▄▄▄ ▐░▌       ▐░▌▐░█▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄█░▌ ▄▄▄▄▄▄▄▄▄█░▌" << endl;
            cout << "▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌" << endl;
            cout << " ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀ " << endl;
            break;
        case 4:

            cout << "Chess" << endl;
            break;
        case 5:
            cout
                    << "        /\\\\\\\\\\\\\\\\\\  /\\\\\\        /\\\\\\  /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\     /\\\\\\\\\\\\\\\\\\\\\\       /\\\\\\\\\\\\\\\\\\\\\\"
                    << endl;
            cout
                    << "      /\\\\\\////////  \\/\\\\\\       \\/\\\\\\ \\/\\\\\\///////////    /\\\\\\/////////\\\\\\   /\\\\\\/////////\\\\\\"
                    << endl;
            cout
                    << "     /\\\\\\/           \\/\\\\\\       \\/\\\\\\ \\/\\\\\\              \\//\\\\\\      \\///   \\//\\\\\\      \\///"
                    << endl;
            cout
                    << "     /\\\\\\             \\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\/\\\\\\\\\\\\\\\\\\\\\\       \\////\\\\\\           \\////\\\\\\"
                    << endl;
            cout
                    << "     \\/\\\\\\             \\/\\\\\\/////////\\\\\\ \\/\\\\\\///////           \\////\\\\\\           \\////\\\\\\"
                    << endl;
            cout
                    << "      \\//\\\\\\            \\/\\\\\\       \\/\\\\\\ \\/\\\\\\                     \\////\\\\\\           \\////\\\\\\"
                    << endl;
            cout
                    << "        \\///\\\\\\          \\/\\\\\\       \\/\\\\\\ \\/\\\\\\              /\\\\\\      \\//\\\\\\   /\\\\\\      \\//\\\\\\"
                    << endl;
            cout
                    << "           \\////\\\\\\\\\\\\\\\\\\ \\/\\\\\\       \\/\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\///\\\\\\\\\\\\\\\\\\\\\\/   \\///\\\\\\\\\\\\\\\\\\\\\\/"
                    << endl;
            cout
                    << "               \\/////////  \\///        \\///  \\///////////////    \\///////////       \\///////////"
                    << endl;
            break;
        case 6:
            cout
                    << "          _____                    _____                    _____                    _____                    _____          "
                    << endl;
            cout
                    << "         /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\         "
                    << endl;
            cout
                    << "        /::\\    \\                /::\\____\\                /::\\    \\                /::\\    \\                /::\\    \\        "
                    << endl;
            cout
                    << "       /::::\\    \\              /:::/    /               /::::\\    \\              /::::\\    \\              /::::\\    \\       "
                    << endl;
            cout
                    << "      /::::::\\    \\            /:::/    /               /::::::\\    \\            /::::::\\    \\            /::::::\\    \\      "
                    << endl;
            cout
                    << "     /:::/\\:::\\    \\          /:::/    /               /:::/\\:::\\    \\          /:::/\\:::\\    \\          /:::/\\:::\\    \\     "
                    << endl;
            cout
                    << "    /:::/  \\:::\\    \\        /:::/____/               /:::/__\\:::\\    \\        /:::/__\\:::\\    \\        /:::/__\\:::\\    \\    "
                    << endl;
            cout
                    << "   /:::/    \\:::\\    \\      /::::\\    \\              /::::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\   "
                    << endl;
            cout
                    << "  /:::/    / \\:::\\    \\    /::::::\\    \\   _____    /::::::\\   \\:::\\    \\    ___\\:::\\   \\:::\\    \\    ___\\:::\\   \\:::\\    \\  "
                    << endl;
            cout
                    << " /:::/    /   \\:::\\    \\  /:::/\\:::\\    \\ /\\    \\  /:::/\\:::\\   \\:::\\    \\  /\\   \\:::\\   \\:::\\    \\  /\\   \\:::\\   \\:::\\    \\ "
                    << endl;
            cout
                    << "/:::/____/     \\:::\\____\\/:::/  \\:::\\    /::\\____\\/:::/__\\:::\\   \\:::\\____\\/::\\   \\:::\\   \\:::\\____\\/::\\   \\:::\\   \\:::\\____\\"
                    << endl;
            cout
                    << "\\:::\\    \\      \\::/    /\\::/    \\:::\\  /:::/    /\\:::\\   \\:::\\   \\::/    /\\:::\\   \\:::\\   \\::/    /\\:::\\   \\:::\\   \\::/    /"
                    << endl;
            cout
                    << " \\:::\\    \\      \\/____/  \\/____/ \\:::\\/:::/    /  \\:::\\   \\:::\\   \\/____/  \\:::\\   \\:::\\   \\/____/  \\:::\\   \\:::\\   \\/____/ "
                    << endl;
            cout
                    << "  \\:::\\    \\                       \\::::::/    /    \\:::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\     "
                    << endl;
            cout
                    << "   \\:::\\    \\                       \\::::/    /      \\:::\\   \\:::\\____\\       \\:::\\   \\:::\\____\\       \\:::\\   \\:::\\____\\    "
                    << endl;
            cout
                    << "    \\:::\\    \\                      /:::/    /        \\:::\\   \\::/    /        \\:::\\  /:::/    /        \\:::\\  /:::/    /    "
                    << endl;
            cout
                    << "     \\:::\\    \\                    /:::/    /          \\:::\\   \\/____/          \\:::\\/:::/    /          \\:::\\/:::/    /     "
                    << endl;
            cout
                    << "      \\:::\\    \\                  /:::/    /            \\:::\\    \\               \\::::::/    /            \\::::::/    /      "
                    << endl;
            cout
                    << "       \\:::\\____\\                /:::/    /              \\:::\\____\\               \\::::/    /              \\::::/    /       "
                    << endl;
            cout
                    << "        \\::/    /                \\::/    /                \\::/    /                \\::/    /                \\::/    /        "
                    << endl;
            cout
                    << "         \\/____/                  \\/____/                  \\/____/                  \\/____/                  \\/____/         "
                    << endl;
            break;

        default:
            break;
    }

}

void OnceAnnouncement::init() {
    char anything;
    cout << "\t\tInput [anything] to continue:" << endl;
    cout << "\t\t\t\t";
    cin >> anything;
}

OnceAnnouncement::~OnceAnnouncement() {
    cout << "\n";
}
