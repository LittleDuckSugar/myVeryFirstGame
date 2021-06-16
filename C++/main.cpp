/* =======================================================================================================================
                                                TODO list (not ordered)

    * A real main menu (with map border)
    * Rewrite generate functions to allow text printing on screen with map behind
    * See for an ascii-art generator
    * Add levels with manipulation of both speed and number of obstacle generated
    * Remove or replace 'conio.h' header file because it's a Windows header file --> Currently used for input (move())
    * Generate new line when player is close to the botom and ask to get down (so he can never touch the bottom limit)
    * A real score handler (with storage on server ?) (with username and best scores list)
    * Support for Windows

========================================================================================================================== */


#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <thread>
#include <sys/ioctl.h>

// Windows header
#include <conio.h>

using namespace std;


class Game {
    public:

        // Constructor
        Game() {
            refreshSize();
            generateCleanMap();
        }
    
        // playGame launch the game
        void playGame() {
            // User input thread
            std::thread getUserPlace (&Game::move, this);

            while (play == true) {
                // refreshSize();
                if (gamePause == false) {
                    refreshScreen();
                    generateLine();
                }
                std::this_thread::sleep_for(speed);
            }

            cout << endl << "You loose" << endl;
            cout << "Score: " << points <<endl ;
            cout << "Hit return to close the game if needed" << endl;
            getUserPlace.join();
        }

    private:

        int lines, columns, placey, placex, points = 0;
        std::chrono::milliseconds speed = 600ms;
        vector<string> toPrint;
        // speed var must depends on serial communication
        bool play = true;
        bool gamePause = false;

        // refreshSize get terminal size
        void refreshSize() {
            struct winsize w;

            ioctl(fileno(stdout), TIOCGWINSZ, &w);
            columns = (int)(w.ws_col);
            lines = (int)(w.ws_row);
            
            placey = columns / 2;
            placex = lines / 2;
        }

        // move change the place
        void move() {
            int choice;
            while (play == true) {
                choice = getch();

                if (choice == 32) { // 'space' key
                    if (gamePause == true) {
                        gamePause = false;
                    } else {
                        cout << "Game paused" << endl;
                        cout << "Press 'Space' again to unpause" << endl;
                        gamePause = true;
                    }
                }

                if (gamePause == false) {
                    if (choice == 100) { // 'd' key
                        if (placey != columns -2) {
                            placey++;
                            refreshScreen();
                        }
                    }
                    if (choice == 113) { // 'q' key
                        if (placey != 1) {
                            placey--;
                            refreshScreen();
                        }
                    }
                    if (choice == 115) { // 's' key
                        if (placex != lines - 3) {
                            placex++;
                            points += 2;
                            refreshScreen();
                        }
                    }
                    if (choice == 122) { // 'z' key
                        if (placex != 0) {
                            placex--;
                            points--;
                            refreshScreen();
                        }
                    }
                }
            }
        }

        /* ========================================

        generate functions must be rewriten
        to allow strings to be print

        Maybe see ascii-art-generator (from Go)

        ======================================== */


        // generateLine generate a new line and append this line to toPrint
        void generateLine() {
            string toPush;
            int random;
            toPush = "|";
            for (int y = 0; y < columns -2; y++) {
                random = rand() % 10;
                if (random <= 8) {
                    toPush += " ";
                } else {
                    toPush += "-";
                }
            }
            toPush += "|";
            
            for (int x = 0; x<toPrint.size() - 1; x++) {
                toPrint[x] = toPrint[x+1];
            }
            toPrint.pop_back();

            points++;
            toPrint.push_back(toPush);
        }

        // generateCleanMap generate as much empti line as they are in the terminal and append it to toPrint
        void generateCleanMap() {
            string toBuild;
            for (int x = 0; x < lines - 2; x++) {
                toBuild = "|";
                for (int y = 0; y < columns -2; y++) {
                    toBuild +=  " ";
                }
                toBuild += "|";
                toPrint.push_back(toBuild);
                toBuild = "";
            }
        }

        // refreshScreen print to the terminal toPrint
        void refreshScreen() {
            bool rest;


            // Clear the screen
            cout << "\x1B[2J\x1B[H" << endl ;

            for (int x = 0; x < lines -2; x++) {
                if (x == placex) {
                    if (toPrint[placex][placey] == '-') {
                        play = false;
                        break;
                    } else {
                        for (int y = 0; y < columns; y++) {
                            if (y == placey) {
                                cout << '*';
                                rest = true;
                            } else {
                                if (rest == true) {
                                    cout << toPrint[x].substr(y);
                                    break;
                                }
                                cout << toPrint[x][y];
                            }
                        }
                        cout << endl;
                    }
                } else {
                    cout << toPrint[x] << endl;
                }
            }
        }
};


int main() { 
    char choice;
    
    Game myGame;

    cout << "Welcome to myVeryFirstGame !" << endl << endl;
    cout << "The goal of this game is to go deeper and deeper escaping '-' elements on screen" << endl;
    cout << "You can move using ZQSD" << endl;
    cout << "Z: Up" << endl;
    cout << "S: Down"<< endl;
    cout << "D: Right"<< endl;
    cout << "Q: Left"<< endl;
    cout << endl << "Need a break ? Press 'Space' to pause the game" << endl;
    cout << endl << "How to make points:" << endl;
    cout << "One auto line down -> 1 point" << endl; 
    cout << "One manual line down -> 2 points" << endl; 
    cout << "One line up -> -1 point" << endl;

    cout << endl << "Launch the game ? ('Y' for yes, 'N' for no)" << endl;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
            myGame.playGame();
    }

    cout << "Hope you enjoy!" << endl;

    return 0;
}