#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// initialise des constantes correspondantes a la taille de l'ecran
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

/*  A entrer dans le console:
    gcc GameOfLife.c -o GameOfLife -I include -L lib -lmingw32 -lSDL2main -lSDL2
    ./GameOfLife.exe
*/

// fonction permettant d'afficher un message d'erreur et de quitter le programme
void SDL_ExitWithError(const char *message){
    SDL_Log("ERREUR : %s\n",message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

// fonction permettant d'initialiser la matrice
int initialiseMatrice(char** matrice, int taille){
    // Initialise la matrice
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            matrice[i][j] = '.';
        }
    }
    return **matrice;
}

// fonction permettant d'initiliaser des cellules vivantes dans la matrice de maniere aleatoire
int initialiseCellulesVivantes(char** matrice, int taille){
    srand(time(NULL));
    for(int i = 0; i < taille; i++){
        for(int j = 0; j < taille; j++){

            // cree un nombre aleatoire
            int randomNum = rand() % 2;
            if(randomNum == 1){
                matrice[i][j] = '0';
            }
        }
    }
    return **matrice;
}

// fonction permettant d'initialiser la matrice
int initialiseMatriceNouvelle(char** matrice_nouvelle, int taille){
    // Initialise la matrice nouvelle
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            matrice_nouvelle[i][j] = '.';
        }
    }
    return **matrice_nouvelle;
}

// fonction permettant de verifier si des cellules doivent mourir et les tuer si c'est le cas
int tuerCellule(char** matrice, char** matrice_nouvelle , int cellules_voisines, int i, int j, int taille){
    // teste la position des cellules voisines avec les exceptions que representent les bords
    if (i != 0 && j != 0 && matrice[i - 1][j - 1] == '0') {
        // incremente de 1 le nombre de cellules voisines vivantes
        cellules_voisines++;
    }
    if (i != 0 && matrice[i - 1][j] == '0') {
        cellules_voisines++;
    }
    if (i != 0 && j != taille - 1 && matrice[i - 1][j + 1] == '0') {
        cellules_voisines++;
    }
    if (j != 0 && matrice[i][j - 1] == '0') {
        cellules_voisines++;
    }
    if (j != taille - 1 && matrice[i][j + 1] == '0') {
        cellules_voisines++;
    }
    if (i != taille - 1 && j > 0 && matrice[i + 1][j - 1] == '0') {
        cellules_voisines++;
    }
    if (i != taille - 1 && matrice[i + 1][j] == '0') {
        cellules_voisines++;
    }
    if (i != taille - 1 && j != taille - 1 && matrice[i + 1][j + 1] == '0') {
        cellules_voisines++;
    }

    // teste si la cellule reste en vie
    if (cellules_voisines == 2 || cellules_voisines == 3) {
        // rajoute la cellule vivante en position matrice[i][j] a matrice_nouvelle a la meme position
        matrice_nouvelle[i][j] = matrice[i][j];
    }

    // sinon si la cellule meurt
    else if (cellules_voisines < 2 || cellules_voisines > 3) {
        // remplace la cellule vivante par une cellule morte en position matrice[i][j] a matrice_nouvelle a la meme position
        matrice_nouvelle[i][j] = '-';
    }

    return **matrice_nouvelle;
}

// fonction permettant de verifier si des cellules doivent revivre et les reanimer si c'est le cas
int reanimerCellule(char** matrice, char** matrice_nouvelle , int cellules_voisines, int i, int j, int taille){

    // teste la position des cellules voisines avec les exceptions que representent les bords
    if (i != 0 && j != 0 && matrice[i - 1][j - 1] == '0') {
        // incremente de 1 le nombre de cellules voisines vivantes
        cellules_voisines++;
    }
    if (i != 0 && matrice[i - 1][j] == '0') {
        cellules_voisines++;
    }
    if (i != 0 && j != taille - 1 && matrice[i - 1][j + 1] == '0') {
        cellules_voisines++;
    }
    if (j != 0 && matrice[i][j - 1] == '0') {
        cellules_voisines++;
    }
    if (j != taille - 1 && matrice[i][j + 1] == '0') {
        cellules_voisines++;
    }
    if (i != taille - 1 && j > 0 && matrice[i + 1][j - 1] == '0') {
        cellules_voisines++;
    }
    if (i != taille - 1 && matrice[i + 1][j] == '0') {
        cellules_voisines++;
    }
    if (i != taille - 1 && j != taille - 1 && matrice[i + 1][j + 1] == '0') {
        cellules_voisines++;
    }

    // teste si le nombre de cellules voisines vivantes vaut 3
    if (cellules_voisines == 3) {
        // remplace une cellule morte par une cellule vivantes aux coordonnes dans matrice_nouvelle[i][j] 
        matrice_nouvelle[i][j] = '0';
    }

    return **matrice_nouvelle;
}


int main(int argc, char **argv){

    // initialise des pointeurs
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // initialise des variables pour les tailles a l'ecran
    int grid_cell_size = 20;
    int grid_width = SCREEN_WIDTH;
    int grid_height = SCREEN_HEIGHT;

    // initialise la taille des cellules
    SDL_Rect grid_cursor = {
        .w = grid_cell_size-1,
        .h = grid_cell_size-1,
    };

    // Lancement SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_ExitWithError("Initialisation SDL");
    }

    // Creation fenetre + rendu
    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) !=0){
        SDL_ExitWithError("Impossible de cree la fenetre et le rendu");
    }

    // initialise des boolean avec le SDL
    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;


    // initialise un pointeur vers un tableau de caracteres
    char** matrice;

    // initialise la taille tu tableau
    int taille = ((SCREEN_WIDTH-1)/grid_cell_size)*sizeof(char);
    // Alloue dynamiquement la memoire pour la matrice
    matrice = (char**)malloc(taille*sizeof(char*));
    for (int i = 0; i < taille; i++) {
        matrice[i] = (char*)malloc(taille* sizeof(char));
    }

    // appelle la fonction servant a initialise le tableau matrice
    initialiseMatrice(matrice, taille);

    // appelle la fonciton servant a initialise les cellules vivantes dans le tableau
    initialiseCellulesVivantes(matrice, taille);

// _____________________________________________________________________________________________________________________________________________
//On initialise une première fois le programme pour afficher le premier tour du jeu
    int PAUSE = 2;
    while (!quit) {
        SDL_Event event;
        //Le SDL tente de trouver un des évenement
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN :
                switch (event.key.keysym.sym){
                    case SDLK_SPACE:
                    //Si la barre d'espace est appuyé le jeu se met en pause
                        PAUSE = 1;
                        SDL_Delay(200);
                        SDL_PumpEvents();
                        SDL_RenderPresent(renderer);
                        break;
                }      

                //Permet de détecter si la souris est au dessus de la fenetre et d'eviter le crash du programme  
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                        mouse_hover = SDL_TRUE;
                    else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                        mouse_hover = SDL_FALSE;
                    break;

                //Ferme la fenetre si l'utilisateur appui sur la croix
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
            }
            
        }
        // Etablie la couleur du fond d'ecran et le dessine
        SDL_SetRenderDrawColor(renderer, 233, 233, 233, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Etablie la couleur des lignes entre les cellules
        SDL_SetRenderDrawColor(renderer, 95, 95, 95, SDL_ALPHA_OPAQUE);

        // Dessine les lignes verticales
        for (int x = 0; x < 1 + grid_width * grid_cell_size; x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
        }

        // Dessine les lignes horizontales
        for (int y = 0; y < 1 + grid_height * grid_cell_size; y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
        }





        // Alloue dynamiquement la memoire pour la matrice_nouvelle
        char** matrice_nouvelle = (char**)malloc(taille * sizeof(char*));
        for (int i = 0; i < taille; i++) {
            matrice_nouvelle[i] = (char*)malloc(taille * sizeof(char));
        }
        
        // appelle la fonction servant a initialise le tableau matrice_nouvelle
        initialiseMatriceNouvelle(matrice_nouvelle, taille);

        //Met en pause le programme sur la barre d'espace a été pressé auparavant, enleve la pause si réappuyé
        while(PAUSE == 1){
            while (SDL_PollEvent(&event)) {
                switch (event.type) {   
                    case SDL_KEYDOWN :
                        switch (event.key.keysym.sym)
                        {
                            //si la barre d'espace est pressée, le script reprend
                        case SDLK_SPACE:
                            PAUSE = 0;
                            break;
                        }
                        //Le script reste figé si la barre d'espace n'est pas pressé au moins une fois
                    default:
                        SDL_Delay(1);
                        SDL_PumpEvents();
                }    
            }
        }

        // parcourt la taille du tableau
        for (int i = 0; i < taille; i++) {
            for (int j = 0; j < taille; j++) {
                // initialise un compteur du nombre de cellule voisines vivantes au debut de la boucle pour chaque element
                int cellules_voisines = 0;
                
                // teste si l'element en position matrice[i][j] contient une cellule vivante
                if (matrice[i][j] == '0') {
                    // appelle la fonction testant si la cellule doit mourir et la tue si oui
                    tuerCellule(matrice, matrice_nouvelle , cellules_voisines, i, j, taille);                   
                }

                // teste si la cellule est morte
                else if(matrice[i][j] == '.' || matrice[i][j] == '-'){
                    // appelle la fonction testant si la cellule doit revivre et la reanime si oui
                    reanimerCellule(matrice, matrice_nouvelle , cellules_voisines, i, j, taille);
                }
            }
        }

        // libère la memoire de la matrice
        for (int i = 0; i < taille; i++) {
            free(matrice[i]);
        }

        // libère la matrice de la memoire
        free(matrice);

        // attribue les elements de la nouvelle matrice a l'ancienne
        matrice = matrice_nouvelle;

        // dessine les cellules vivantes a l'ecran
        for(int i = 0; i < taille; i++){
            for(int j = 0; j < taille; j++){

                // teste si la cellule est vivante
                if(matrice[i][j] == '0'){

                    // etablie la position de la cellule a dessiner
                    grid_cursor.x = j*grid_cell_size+1; //colonne
                    grid_cursor.y = i*grid_cell_size+1; //ligne
                    // etablie la couleur de la cellule a dessiner
                    SDL_SetRenderDrawColor(renderer, 105, 150, 105, SDL_ALPHA_OPAQUE);
                    // dessine la cellule a l'ecran
                    SDL_RenderFillRect(renderer, &grid_cursor);
                }

                // UTILISABLE POUR CHANGER LA COULEUR DES CELLULES QUI VIENNENT DE MOURIR
                // teste si la cellule est vivante
                // else if(matrice[i][j] == '-'){   
                //     // etablie la position de la cellule a dessiner
                //     grid_cursor.x = j*grid_cell_size+1; //colonne
                //     grid_cursor.y = i*grid_cell_size+1; //ligne
                //     // etablie la couleur de la cellule a dessiner
                //     SDL_SetRenderDrawColor(renderer, 182, 182, 182, SDL_ALPHA_OPAQUE);
                //     // dessine la cellule a l'ecran
                //     SDL_RenderFillRect(renderer, &grid_cursor);
                // }
            }
        }

        if(PAUSE == 2)
            PAUSE =1;
        //Met un délai de X millisecondes avant d'afficher le nouveau tableau ainsi que de relancer la boucle
        SDL_Delay(625);
        SDL_RenderPresent(renderer);
    }

/*___________________________________________________________2_____________________________________________________________________*/

    // permet de quitter le programme et sa fenetre et vide la memoire de ceux-ci
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);

    return EXIT_SUCCESS;  // return 0;
}

