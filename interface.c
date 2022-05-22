#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define f0 27.5  
//définition de la fréquence de la plus basse touche (en Hz)

int trouver_la_touche(double f){
	int n=round(12.0*log2(f/f0))+1;
	return n;
	}


int setWindowColor(SDL_Renderer *renderer, SDL_Color color) {
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
	}


int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *tmp=NULL;
    SDL_Surface *surface=NULL;
    
    
    int statut = EXIT_FAILURE;
    SDL_Color orange={255,127,40,255};
    SDL_Rect dest={0,0,0,0};

    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
        }
    int WIDTH=1100;
    int HEIGHT=200;
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(NULL == window){
    	fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
        }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
	goto Quit;
    	}
    

	surface= SDL_LoadBMP("piano.bmp");//la surface récupere l'image du piano

	if(NULL == surface) {
    		fprintf(stderr, "Erreur surface : %s", SDL_GetError());
    		goto Quit;}

	tmp = SDL_CreateTextureFromSurface(renderer, surface);//on met cette image sur une texture
	if(NULL == tmp) {
    		fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
    		goto Quit;
    		}
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
SDL_TEXTUREACCESS_TARGET, surface->w, surface->h); //nouvelle texture qu'on pourra modifier de meme dimension que la surface
	if(NULL == texture) {
    		fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
    		goto Quit;
		}
//SDL_SetRenderTarget(renderer, texture); /* La cible de rendu est maintenant texture. */


//test avec f=423Hz
	int f=423;
	int n = trouver_la_touche(f);//donne le numéro de la note
	int taille_touche=WIDTH/52;


//On cherche maintenant l'emplacement sur le clavier sachant qu'il y a 12 touches dans une gamme 
//(on doit maintenant prendre en compte les touches noires)

	int q=n/12; //Le quotient de la division euclidienne correspond au numéro de la gamme
	int r=n%12; //le reste correspond au numéro de la touche dans la gamme
//Pour faciliter le positionnement de la touche dans la gamme on introduit un tableau. On sait que le clavier commence au La (la gamme commence par La) qui correspond au 0, le La dièse au 0.5 et ainsi de suite


	double L[12]={0,0.5,1,2, 2.5,3,3.5,4,5,5.5,6,6.5};
//on obtient alors facilement le positionnement dans la gamme
	double h=L[r];
	int H=h*10;
	SDL_Rect touche;
	if (H%2==0) 
		{SDL_Rect touche = {(7*q+h)*taille_touche+5,0,taille_touche,WIDTH};}
	else {SDL_Rect touche = {(7*q+h+0.3)*taille_touche+5,0,taille_touche-7,WIDTH*0.67};
	}

	SDL_RenderCopy(renderer, tmp, NULL, NULL); /* On copie tmp sur texture */
	SDL_DestroyTexture(tmp);
	SDL_FreeSurface(surface);

	SDL_SetRenderDrawColor(renderer, 255,0,0,0);

	SDL_RenderFillRect(renderer, &touche);
	SDL_RenderPresent(renderer);
    
	SDL_Delay(10000);//durée d'affichage de la fenetre
    		statut = EXIT_SUCCESS;
 
	Quit:
    	if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    	if(NULL != window)
        SDL_DestroyWindow(window);
    	if(NULL != texture) SDL_DestroyTexture(texture);
    	SDL_Quit();
    	return statut ;
	}

//gcc ... 'sdl2-config --cflags --libs'
//gcc ... $(sdl2-config --cflags --libs)

