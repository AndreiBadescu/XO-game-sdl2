#include <SDL2/SDL.h>
#include <windows.h>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;
const int ONE_THIRD = SCREEN_WIDTH * 1/3;
const int TWO_THIRDS = SCREEN_WIDTH * 2/3;
const int sleep_time = 3000;

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

void drawCircle(int xc, int yc, int x, int y){
    SDL_RenderDrawPoint(renderer, xc+x, yc+y);
    SDL_RenderDrawPoint(renderer, xc-x, yc+y);
    SDL_RenderDrawPoint(renderer, xc+x, yc-y);
    SDL_RenderDrawPoint(renderer, xc-x, yc-y);
    SDL_RenderDrawPoint(renderer, xc+y, yc+x);
    SDL_RenderDrawPoint(renderer, xc-y, yc+x);
    SDL_RenderDrawPoint(renderer, xc+y, yc-x);
    SDL_RenderDrawPoint(renderer, xc-y, yc-x);
}

// Bresenham's algorithm
void circle(int xc, int yc, int r){
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
    while (y >= x){
        ++x;
        if (d > 0){
            --y;
            d = d + 4 * (x - y) + 10;
        }
        else{
            d = d + 4 * x + 6;
        }
        drawCircle(xc, yc, x, y);
    }
}

SDL_bool Check_for_winner(int a[3][3]){
    for(int i = 0; i < 3; ++i)
        if(a[i][0])
            if(a[i][0] == a[i][1] && a[i][1] == a[i][2]){
                int offset = SCREEN_WIDTH / 30;
                int lineX = i * ONE_THIRD + ONE_THIRD / 2;
                int lineY = offset;
                int line2X = lineX;
                int line2Y = SCREEN_WIDTH - offset;
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, lineX, lineY, line2X, line2Y);
                SDL_RenderPresent(renderer);
                return 1;
            }

    for(int i = 0; i < 3; ++i)
        if(a[0][i])
            if(a[0][i] == a[1][i] && a[1][i] == a[2][i]){
                int offset = SCREEN_WIDTH / 30;
                int lineX = offset;
                int lineY = i * ONE_THIRD + ONE_THIRD / 2;
                int line2X = SCREEN_WIDTH - offset;
                int line2Y = lineY;
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, lineX, lineY, line2X, line2Y);
                SDL_RenderPresent(renderer);
                return 1;
            }

    if(a[0][0] && a[0][0] == a[1][1] && a[1][1] == a[2][2]){
        int offset = SCREEN_WIDTH / 7;
        int lineX = ONE_THIRD / 2 - offset;
        int lineY = lineX;
        int line2X = SCREEN_WIDTH - lineX;
        int line2Y = line2X;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, lineX, lineY, line2X, line2Y);
        SDL_RenderPresent(renderer);
        return 1;
    }

    if(a[0][2] && a[0][2] == a[1][1] && a[1][1] == a[2][0]){
        int offset = SCREEN_WIDTH / 7;
        int lineX = SCREEN_WIDTH - ONE_THIRD / 2 + offset;
        int lineY = ONE_THIRD / 2 - offset;
        int line2X = ONE_THIRD / 2 - offset;
        int line2Y = SCREEN_WIDTH - ONE_THIRD / 2 + offset;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, lineX, lineY, line2X, line2Y);
        SDL_RenderPresent(renderer);
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) == 0) {
            int mat[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
            int squares_left = 9;
            time_t t;
            srand((unsigned) time(&t));

            SDL_bool done = SDL_FALSE;
            SDL_SetWindowTitle(window, "XO");

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, ONE_THIRD, 0, ONE_THIRD, SCREEN_HEIGHT);
            SDL_RenderDrawLine(renderer, TWO_THIRDS, 0, TWO_THIRDS, SCREEN_HEIGHT);
            SDL_RenderDrawLine(renderer, 0, ONE_THIRD, SCREEN_WIDTH, ONE_THIRD);
            SDL_RenderDrawLine(renderer, 0, TWO_THIRDS, SCREEN_WIDTH, TWO_THIRDS);

            SDL_RenderPresent(renderer);

            while(!done) {
                SDL_Event event;

                while(SDL_PollEvent(&event) && !done) {
                    switch (event.type){
                        case SDL_QUIT:
                            done = SDL_TRUE;
                            break;

                        case SDL_MOUSEBUTTONDOWN:
                            if(event.button.button == SDL_BUTTON_LEFT){
                                int mouseX,mouseY;
                                SDL_GetMouseState(&mouseX,&mouseY);

                                int squareX = mouseX / ONE_THIRD;
                                int squareY = mouseY / ONE_THIRD;

                                if(mat[squareX][squareY] == 0){
                                    //circle(squareX * ONE_THIRD + ONE_THIRD / 2, squareY * ONE_THIRD + ONE_THIRD / 2, ONE_THIRD * 2 / 5);
                                    int offset = SCREEN_WIDTH / 30;
                                    int lineX = squareX * ONE_THIRD + offset;
                                    int lineY = squareY * ONE_THIRD + offset;
                                    int line2X = (squareX + 1) * ONE_THIRD - offset;
                                    int line2Y = (squareY + 1) * ONE_THIRD - offset;
                                    SDL_RenderDrawLine(renderer, lineX, lineY, line2X, line2Y);
                                    SDL_RenderDrawLine(renderer, lineX, line2Y, line2X, lineY);
                                    SDL_RenderPresent(renderer);

                                    mat[squareX][squareY] = 1;
                                    --squares_left;
                                }
                                else{
                                    break;
                                }

                                if(Check_for_winner(mat)){
                                    done = SDL_TRUE;
                                    printf(" AI CASTIGAT!\n");
                                    Sleep(sleep_time);
                                    break;
                                }

                                if(squares_left == 0){
                                    done = SDL_TRUE;
                                    printf(" EGALITATE!\n");
                                    Sleep(sleep_time);
                                    break;
                                }

                                int computerX,computerY;
                                do{
                                    computerX = rand() % 3;
                                    computerY = rand() % 3;
                                }while(mat[computerX][computerY]);

                                circle(computerX * ONE_THIRD + ONE_THIRD / 2, computerY * ONE_THIRD + ONE_THIRD / 2, ONE_THIRD * 2 / 5);
                                Sleep(500);
                                SDL_RenderPresent(renderer);

                                mat[computerX][computerY] = 2;
                                --squares_left;

                                if(Check_for_winner(mat)){
                                    done = SDL_TRUE;
                                    printf(" COMPUTERUL A CASTIGAT!\n");
                                    Sleep(sleep_time);
                                    break;
                                }
                            }
                            break;
                    }
                }
            }
        }

        if(renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if(window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}


