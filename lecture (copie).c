#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
//#include <libgsl23>



struct wavfile //définit la structure de l entete d un wave
{
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
	int puissance=0; // rajouté
	int i=0;
	int taille=1; //variable qui correspondra par la suite a la longueur du tableau(puissance de 2)
	int nbech=0; //nombre d echantillons extraits du fichier audio
	char fichieraudio[100];
	printf ("entrer le nom du fichier audio a analyser :\n");
	scanf("%s", fichieraudio);
	printf ("nom du fichier : %s\n", fichieraudio);
	FILE *wav = fopen(fichieraudio,"rb"); //ouverture du fichier wave
	struct wavfile header; //creation du header
	if ( wav == NULL ) {
		printf("\nne peut pas ouvrir le fichier demande, verifier le nom\n");
		printf("ne pas oublier l'extention .wav\n");
		exit(1);}


//initialise le header par l'entete du fichier wave
//verifie que le fichier possede un entete compatible


	if ( fread(&header,sizeof(header),1,wav) < 1 ) {
		printf("\nne peut pas lire le header\n");
		exit(1); }
	if ( header.id[0] != 'R'|| header.id[1] != 'I'|| header.id[2] != 'F'|| header.id[3] != 'F' ){
		printf("\nerreur le fichier n'est pas un format wave valide\n");
		exit(1);}
	if (header.channels!=1) {
		printf("\nerreur : le fichier n'est pas mono\n");
		exit(1);
		}

	nbech=(header.bytes_in_data/header.bytes_by_capture);
	printf ("\nle fichier audio contient %d echantillons\n",nbech);
	while (nbech>taille) {
		taille=taille*2;
		puissance=puissance+1;
		}
	double **tab=NULL; //tableau de l'onde temporelle
	tab=malloc( (taille) * sizeof(double));
	if (tab == NULL) {
		exit(0);
	}
	for(i=0;i<(taille);i++) {
		tab[i]=malloc( 2 * sizeof(double));
		if (tab[i] == NULL) {
			exit(0);}
		}
	i=0;
	short value=0;
	FILE *dat=fopen("data.dat","w"); //fichier data des echantillons
	while( fread(&value,(header.bits_per_sample)/8,1,wav) ) { //lecture des echantillons et enregistrement dans le tableau
	tab[i][0]=value;
	i++;
	}
	printf("\nnombre d'echantillons lus : %d\n",i);
	printf("nombre de valeurs sauvegardees %d\n",i);
	for (i=0;i<taille;i++)
	{
	printf("%f,%f", tab[i][0], tab[i][1]);
	fprintf(dat,"%lf %lf\n", tab[i][0], tab[i][1]);
	}
	for(i=0;i<taille;i++)
	{
	free(tab[i]);
	tab[i] = NULL ;
	}
	fclose(wav);
	fclose(dat);
	printf("%d\n", puissance); // rajouté
	return puissance; // rajouté
	}
	
	
/*La fréquence d'échantillonage est dans la structure.
Le tableau tab est à utiliser dans la suite.
Il corespond au signal échantilloné en fonction du temps.
En utilisant la FFT, on crée le spectre du signal.
On va ensuite utiliser la méthode donnée en énoncé pour déterminer f0, en faisant varier f et en trouvant la somme maximale des amplitudes f, 2f... jusqu'à Fe/2.
On en déduit la fréquence fondamentale.*/

/*On travaille avec gsl_complex_packed_array tableau1 permet de construire le tableau nommé "tableau" qui contiendra les pts complexes de facon à ce que les indices paires correspondent à la partie réelle et les impaires à la partie imaginaire.

tableau2 vide créé
On parcourt  tableau1 en appliquant la fft à chaque point pair et en stockant le résultat dans tableau2.
Vérification résultat: on trace tableau2 de 0 à Fe/2 (donné dans structure)
On fait varier f de 0 à Fe/2, avec un pas de Fe/200.
*/





