// GameTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define TESTLINEBORDER(game, player, test, mult) (game[i][j] == player && (game[i][j + (1*mult)] == player || game[i][j + (1*mult)] == test) && (game[i][j + (2*mult)] == player || game[i][j + (2*mult)] == test))
#define TESTCOLLUMNBORDER(game, player, test, mult) (game[i][j] == player && (game[i + (1*mult)][j] == player || game[i + (1*mult)][j] == test) && (game[i + (2*mult)][j] == player || game[i + (2*mult)][j] == test))
#define TESTDIAGONALBORDER(game, player, test, multX, multY) (game[i][j] == player && (game[i + (1*multY)][j + (1*multX)] == player || game[i + (1*multY)][j + (1*multX)] == test) && (game[i + (2*multY)][j + (2*multX)] == player || game[i + (2*multY)][j + (2*multX)] == test))
#define TESTLINEMIDDLE(game, player, test) (game[i][j] == player && (game[i][j + 1] == player || game[i][j + 1] == test) && (game[i][j - 1] == player || game[i][j - 1] == test))
#define TESTCOLLUMNMIDDLE(game, player, test) (game[i][j] == player && (game[i + 1][j] == player || game[i + 1][j] == test) && (game[i - 1][j] == player || game[i - 1][j] == test))
#define TESTDIAGONALMIDDLEDESC(game, player, test) (game[1][1] == player && (game[0][0] == player || game[0][0] == test) && (game[2][2] == player || game[2][2] == test))
#define TESTDIAGONALMIDDLEASC(game, player, test) (game[1][1] == player && (game[2][0] == player || game[2][0] == test) && (game[0][2] == player || game[0][2] == test))

#include <iostream>
#include <vector>

using tab = char[3][3];

struct node {
    tab game;
    int8_t rating;
    std::vector<node*> children;
    node *parent;
    char currentMove;
};

void minimax(node*);
int8_t rateGame(tab, char);
void printGame(tab, int);
void generateChildren(node*, char, char);
void generateTree(node*);
bool draw(tab, int);

static int games = 0;

int main()
{
    tab game = { ' ', ' ', ' ',
                'O', 'X', ' ',
                ' ', ' ', 'O' };

    node * root = new node;
    memcpy(&(root->game), game, 3 * 3 * sizeof(char));

    root->rating = rateGame(game, 'X');
    root->children = {};
    root->parent = nullptr;
    root->currentMove = 'O';

    generateTree(root);
    std::cout << +games << " possibilities generated." << std::endl;
    std::cout << "Start evaluation..." << std::endl;
    minimax(root);
}

void minimax(node *root) {
    if (!(root->children.empty()))
        for (int i = 0; i < root->children.size(); i++){
            minimax(root->children[i]);
        }

    while (root->parent != nullptr) {
        root = root->parent;
        root->rating = root->children[0]->rating;
        for (int i = 0; i < root->children.size(); i++){
            if (root->rating < (root->currentMove == 'X' ? 1 : -1) * root->children[i]->rating)
                root->rating = root->children[i]->rating;
        }
    }
}

void generateTree(node* root) {
    if (root->rating != 9 && root->rating != -9) {
        if(root->rating == 0) 
            if (!draw(root->game, root->rating)) {
                games++;
                return;
            }
        generateChildren(root, root->currentMove == 'X' ? 'O' : 'X', 'X');
        for (int i = 0; i < root->children.size(); i++) {
            generateTree(root->children[i]);
        }
    }
    games++;
    return;
}

void printGame(tab game, int rating) {

    std::cout << "---------" << std::endl;
    std::cout << "Game #" << games << " rating: " << rating << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << (game[i][j]==' '?'_': game[i][j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------" << std::endl;
}

void generateChildren(node *parent, char nextMove, char player) {
    if (parent->rating == 9) return;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (parent->game[i][j] == ' ') {
                tab *newGame = (tab*)malloc(3*3*sizeof(char));
                memcpy(newGame, &(parent->game), 3*3*sizeof(char));
                (*newGame)[i][j] = nextMove;
                node *child = new node;
                memcpy(&(child->game), newGame, 3 * 3 * sizeof(char));
                child->rating = rateGame(child->game, player);
                child->children = {};
                child->parent = parent;
                child->currentMove = nextMove;
                parent->children.push_back(child);
                //printGame(*newGame, child->rating);
            }
        }
    }
}

int8_t rateGame(tab game, char player) { //FUNÇÃO AVALIA
    uint8_t playerRating =  0, enemyRating = 0;
    char enemyChar = (player == 'X' ? 'O' : 'X');

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game[i][j] == ' ') continue;
            if (i == 0 && j == 0) {
                if (TESTDIAGONALBORDER(game, player, player, 1, 1)) return 9;
                else if (TESTDIAGONALBORDER(game, enemyChar, enemyChar, 1, 1)) return -9;
                else if (TESTDIAGONALBORDER(game, player, ' ', 1, 1)) playerRating++;
                else if (TESTDIAGONALBORDER(game, enemyChar, ' ', 1, 1)) enemyRating++;
            } else if(i == 0 && j == 2){
                if (TESTDIAGONALBORDER(game, player, player, 1, -1)) return 9;
                else if (TESTDIAGONALBORDER(game, enemyChar, enemyChar, 1, -1)) return -9;
                else if (TESTDIAGONALBORDER(game, player, ' ', 1, -1)) playerRating++;
                else if (TESTDIAGONALBORDER(game, enemyChar, ' ', 1, -1)) enemyRating++;
            } else if(i == 2 && j == 0){
                if (TESTDIAGONALBORDER(game, player, player, -1, 1))  return 9;
                else if (TESTDIAGONALBORDER(game, enemyChar, enemyChar, -1, 1))  return -9;
                else if (TESTDIAGONALBORDER(game, player, ' ', -1, 1)) playerRating++;
                else if (TESTDIAGONALBORDER(game, enemyChar,  ' ', -1, 1)) enemyRating++;
            } else if(i == 2 && j == 2){
                if (TESTDIAGONALBORDER(game, player, player, -1, -1))  return 9;
                else if (TESTDIAGONALBORDER(game, enemyChar, enemyChar, -1, -1))  return -9;
                else if (TESTDIAGONALBORDER(game, player, ' ', -1, -1)) playerRating++;
                else if (TESTDIAGONALBORDER(game, enemyChar, ' ', -1, -1)) enemyRating++;
            } else if (i == 1 && j == 1 && game[i][j] == player) {
                if (TESTDIAGONALMIDDLEASC(game, player, ' ')) playerRating++;
                if (TESTDIAGONALMIDDLEDESC(game, player, ' ')) playerRating++;
                if (TESTDIAGONALMIDDLEASC(game, player, player)) return 9;
                if (TESTDIAGONALMIDDLEDESC(game, player, player)) return 9;
            } else if (i == 1 && j == 1 && game[i][j] == enemyChar) {
                if (TESTDIAGONALMIDDLEASC(game, enemyChar, ' ')) enemyRating++;
                if (TESTDIAGONALMIDDLEDESC(game, enemyChar, ' ')) enemyRating++;
                if (TESTDIAGONALMIDDLEASC(game, enemyChar, enemyChar)) return -9;
                if (TESTDIAGONALMIDDLEDESC(game, enemyChar, enemyChar)) return -9;
            }
            switch (i) {
            case 0:
                if (TESTCOLLUMNBORDER(game, player, player, 1)) return 9;
                else if (TESTCOLLUMNBORDER(game, enemyChar, enemyChar, 1)) return -9;
                else if (TESTCOLLUMNBORDER(game, player, ' ', 1)) playerRating++;
                else if (TESTCOLLUMNBORDER(game, enemyChar,  ' ', 1)) enemyRating++;
                break;
            case 2:
                if (TESTCOLLUMNBORDER(game, player, player, -1)) return 9;
                else if (TESTCOLLUMNBORDER(game, enemyChar, enemyChar, -1)) return -9;
                else if (TESTCOLLUMNBORDER(game, player, ' ', -1)) playerRating++;
                else if (TESTCOLLUMNBORDER(game, enemyChar,  ' ', -1)) enemyRating++;
                break;
            default:
                if (TESTCOLLUMNMIDDLE(game, player, player)) return 9;
                else if (TESTCOLLUMNMIDDLE(game, enemyChar, enemyChar)) return -9;
                else if (TESTCOLLUMNMIDDLE(game, player, ' ')) playerRating++;
                else if (TESTCOLLUMNMIDDLE(game, enemyChar, ' ')) enemyRating++;
                break;
            }
            switch (j) {
            case 0:
                if (TESTLINEBORDER(game, player, player, 1)) return 9;
                else if (TESTLINEBORDER(game, enemyChar, enemyChar, 1)) return -9;
                else if (TESTLINEBORDER(game, player, ' ', 1)) playerRating++;
                else if (TESTLINEBORDER(game, enemyChar,  ' ', 1)) enemyRating++;
                break;
            case 2:
                if (TESTLINEBORDER(game, player, player, -1)) return 9;
                else if (TESTLINEBORDER(game, enemyChar, enemyChar, -1)) return -9;
                else if (TESTLINEBORDER(game, player, ' ', -1)) playerRating++;
                else if (TESTLINEBORDER(game, enemyChar,  ' ', -1)) enemyRating++;
                break;
            default:
                if (TESTLINEMIDDLE(game, player, player)) return 9;
                else if (TESTLINEMIDDLE(game, enemyChar, enemyChar)) return -9;
                else if (TESTLINEMIDDLE(game, player, ' ')) playerRating++;
                else if (TESTLINEMIDDLE(game, enemyChar, ' ')) enemyRating++;
                break;
            }
        }
    }
    return playerRating - enemyRating;
}

bool draw(tab game, int rating) {
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            if (game[i][j]) return 0; 
    return rating==0;
}
