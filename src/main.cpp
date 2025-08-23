#include "core/Game.h"
#include "core/Player.h"
#include "ui/Display.h"
#include "ui/InputParser.h"
#include "utils/Utils.h"
#include "network/NetworkManager.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    Display display;
    InputParser parser;
    Game game;
    NetworkManager net;

    bool networkMode = false;
    bool isServer = false;

    // Command line options: --server <port> or --client <host> <port>
    if (argc >= 3) {
        std::string mode = argv[1];
        if (mode == "--server") {
            int port = std::stoi(argv[2]);
            if (!net.startServer(port)) {
                return 1;
            }
            networkMode = true;
            isServer = true;
        } else if (mode == "--client" && argc >= 4) {
            std::string host = argv[2];
            int port = std::stoi(argv[3]);
            if (!net.connectToServer(host, port)) {
                return 1;
            }
            networkMode = true;
            isServer = false;
        }
    }

    display.showWelcomeMessage();

    // Get player names and exchange if in network mode
    std::string whiteName;
    std::string blackName;
    if (networkMode) {
        if (isServer) {
            whiteName = parser.getPlayerName(PieceColor::WHITE);
            net.sendMessage(whiteName);
            net.receiveMessage(blackName);
        } else {
            blackName = parser.getPlayerName(PieceColor::BLACK);
            net.sendMessage(blackName);
            net.receiveMessage(whiteName);
        }
    } else {
        whiteName = parser.getPlayerName(PieceColor::WHITE);
        blackName = parser.getPlayerName(PieceColor::BLACK);
    }

    auto whitePlayer = std::make_unique<HumanPlayer>(whiteName, PieceColor::WHITE);
    auto blackPlayer = std::make_unique<HumanPlayer>(blackName, PieceColor::BLACK);
    game.setPlayer(PieceColor::WHITE, std::move(whitePlayer));
    game.setPlayer(PieceColor::BLACK, std::move(blackPlayer));

    while (!game.isGameOver()) {
        display.showBoard(game.getBoard());
        display.showGameStatus(game);

        bool myTurn = !networkMode ||
                      (networkMode && ((isServer && game.getCurrentPlayer() == PieceColor::WHITE) ||
                                       (!isServer && game.getCurrentPlayer() == PieceColor::BLACK)));

        std::string input;
        if (myTurn) {
            Player* currentPlayer = game.getPlayer(game.getCurrentPlayer());
            if (!currentPlayer) {
                display.displayError("No player found for current turn!");
                break;
            }
            display.displayPrompt(currentPlayer->getName() + "'s turn (" +
                                Utils::colorToString(game.getCurrentPlayer()) + "): ");
            input = parser.getUserInput();
        } else {
            display.displayMessage("Waiting for opponent...");
            if (!net.receiveMessage(input)) {
                display.displayError("Connection lost.");
                break;
            }
            display.displayMessage("Opponent played: " + input);
        }

        if (parser.isQuitCommand(input)) {
            if (networkMode && myTurn)
                net.sendMessage(input);
            display.displayMessage("Thanks for playing!");
            break;
        } else if (parser.isHelpCommand(input)) {
            if (myTurn)
                display.showHelpMessage();
            continue;
        }

        auto movePositions = parser.parseMove(input);
        if (movePositions) {
            Position from = movePositions->first;
            Position to = movePositions->second;

            if (game.makeMove(from, to)) {
                if (networkMode && myTurn)
                    net.sendMessage(input);
                display.displayMessage("Move executed: " +
                                     Utils::positionToSquare(from) + " to " +
                                     Utils::positionToSquare(to));
            } else {
                display.displayError("Invalid move! Try again.");
                continue;
            }
        } else {
            display.displayError("Invalid input format! Use 'e2e4' or type 'help'");
            continue;
        }
    }

    if (game.isGameOver()) {
        display.showBoard(game.getBoard());
        display.showGameOverMessage(game);
        display.showMoveHistory(game);
    }

    net.close();
    return 0;
}