
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <gsl/gsl_fft_complex.h>

#define PI 3.14159

struct wavfile { //définit la structure de l entete d un wave
	char id[4]; // doit contenir "RIFF"
	int totallength; // taille totale du fichier moins 8 octets
	char wavefmt[8]; // doit etre "WAVEfmt "
	int format; // 16 pour le format PCM
	short pcm; // 1 for PCM format
	short channels; // nombre de channels
	int frequency; // frequence d echantillonage
	int bytes_per_second; // nombre de octets par secondes
	short bytes_by_capture; // nombre de bytes par echantillon
	short bits_per_sample; // nombre de bit par echantillon
	char data[4]; // doit contenir "data"
	int bytes_in_data; // nombre de bytes de la partie data
	};

	
int main() {

	char fichieraudio[100]; //chaine contenant le nom du fichier audio
	printf ("entrer le nom du fichier audio a analyser :\n");
	scanf("%s", fichieraudio);
	printf ("nom du fichier : %s\n", fichieraudio);
	FILE *wav = fopen(fichieraudio,"rb"); //ouverture du fichier wave, 		en mode "lecture"
	struct wavfile header; //creation du header
	if ( wav == NULL ) { //conditions à respecter sur le fichier
		printf("\nne peut pas ouvrir le fichier demande, verifier le 			nom\n");
		printf("ne pas oublier l'extention .wav\n");
		exit(1);
		}

//initialise le header par l'entete du fichier wave
//verifie que le fichier possede un entete compatible
	if ( fread(&header,sizeof(header),1,wav) < 1 ) {
		printf("\nne peut pas lire le header\n");
		exit(1);
		}
	//vérification que que l'identifiant soit "RIFF"
	if ( header.id[0] != 'R'|| header.id[1] != 'I'|| header.id[2] != 'F'||header.id[3] != 'F' ) {
		printf("\nerreur le fichier n'est pas un format wave valide\n");
		exit(1);
		}
	//On travaille avec des signaux mono (pas stéréos). On vérifie que le nombre de channels est bien égal à un
	if (header.channels!=1) {
		printf("\nerreur : le fichier n'est pas mono\n");
		exit(1);
		}

	int nbech=(header.bytes_in_data/header.bytes_by_capture); //nombre de bit du fichier audio dans la partie data
	printf ("\nle fichier audio contient %d echantillons\n",nbech);
	
	//définissons la "taille", une puissance de 2 correspondant à la taille du tableau
	int taille=1; //Avoir une puissance de 2 est nécessaire lors du calcul de la fft
	while (nbech>taille) {
		taille=taille*2;
		}
	
	//création du tableau de l'onde temporelle
	int i=0;
	double **tab=NULL;
	tab=malloc( (taille) * sizeof(double));
	if (tab == NULL) {
		exit(0);}
	for(i=0;i<(taille);i++) {
		tab[i]=malloc( 2 * sizeof(double));
		if (tab[i] == NULL) {
			exit(0); }
		}
	//C'est un tableau à "taille" lignes et 2 colonnes
	
	//remplissage du tableau avec la fonction fread (cf rapport)
	short value=0;
	i=0;
	while( fread(&value,(header.bits_per_sample)/8,1,wav) ) { //lecture des echantillons 	et enregistrement dans le tableau
		tab[i][0]=value;
		i++; 
		}
	printf("\nnombre d'echantillons lus : %d\n",i);
	printf("nombre de valeurs sauvegardees %d\n",i);
	
	
	FILE *dat=fopen("data.dat","w"); //création du fichier data des echantillons
	for (i=0;i<taille;i++) {
		fprintf(dat,"%lf %lf\n", tab[i][0], tab[i][1]); } 
		//remplissage de data.dat	







//Fenetrage de Hamming

	for (int i=0;i<taille;i++){
		tab[i][0]=tab[i][0]*(0.54-0.46*cos(2*PI*i/nbech));
		}
	

//il faut concaténer les 2 colonnes de tab pour avoir une liste avec d'abord les reels en position pair et complexes en impair

	double *tab_concat=malloc(taille*2*sizeof(double));
		for (int i=0; i<taille-1; i++) {
			tab_concat[2*i]=tab[i][0];
			tab_concat[2*i+1]=tab[i][1];
			}
		
		//libération de la mémoire
	for(i=0;i<taille;i++) {
		free(tab[i]);}
	free(tab);

	gsl_fft_complex_radix2_forward (tab_concat,1,taille);
	//

    
   //retrouver la fréquence fondamentale avec argmax

int Fe= header.frequency;
	int f=Fe/4410; //On prend un pas 10 Hz
	double sum; //sum est la somme de mon log
	double sum_max=0;
	int H=5; //4 harmoniques
	int f0=0;
	while (f<Fe/2) {
		sum=0;
		for (int h=1; h<=H; h++) {
		 //h est le nombre d'harmoniques considérées
			if(h*f < taille) {
				sum+= 2*log(abs(tab_concat[h*f]));	
			}
		}
		if (sum>sum_max) {
			f0=f;
			sum_max=sum;
		}
		f+=Fe/4410;
	}
	printf("fréquence fondamentale: %dHz\n", f0);

free(tab_concat);
//fermeture des fichiers utilisés
	fclose(wav);
	fclose(dat);  
	}




