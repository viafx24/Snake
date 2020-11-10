// le jeux marche mais comme d'hab pas de gestion de gagner/perdu
// un bug: si j'appuie trop vite sur plusieurs fleche, la taille du serpent chute. Ce n'est pas un bug voir plus loin.
// le jeu fonctionne bien: manque simplement un indicateur de la taille max obtenue: gestion points, gagner, perdu.
// pas de gestion des carrés verts dont je ne connais pas le rôle à la base.

#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int N=30,M=20;
int size=16; // taille en pixel d'un carré (rouge ou blanc).
int w = size*N; // ce qui nous permet de générer la grille et de connaitre la taille à donner à la fenètre
int h = size*M;

// num c'est la longeur du serpent, dir, un entier servant à récuperer la position de la touche fleche appuyé
int dir,num=4;

struct Snake 
{ int x,y;}  s[100];



struct Fruit
{ int x,y;} f;

// tout se passe dans cette fonction assez complexe.


void Tick()
 {

    // si on fait rien le serpent avance tout seul à chaque delay c'est à dire toute les 0.1 secondes

    // on decrement depuis la fin du serpent jusqu'à 1 et chaque case prend la position de la précedente.
    for (int i=num;i>0;--i)
     {s[i].x=s[i-1].x; s[i].y=s[i-1].y;}

    // il semble qu'on travaille toujours avec S[0] qui est la tête du serpent
    // le 1,2,3,4 corresponde à la fleche taper et donc aux directionx haut bas, gauche, droite.
    // c'est l'utilisateur qui induit une brisure de ligne mais avec les deux lignes du dessus, si on ne fait rien
    // le serpent redevient une ligne.

    if (dir==0) s[0].y+=1;      
    if (dir==1) s[0].x-=1;        
    if (dir==2) s[0].x+=1;         
    if (dir==3) s[0].y-=1;   


    // si le serpent arrive sur le fruit
    // on increment num (qui commence à 4) et on crée un nouveau fruit avec une position en x et y aleatoire.
    // ce num gère la taille du serpent et sert de limite à la boucle for qui dessine le serpent tout en bas.
    if ((s[0].x==f.x) && (s[0].y==f.y)) 
     {num++; f.x=rand()%N; f.y=rand()%M;}


// si le serpent depasse par le bas, revient en haut
// si le serpent depasse par la gauche, revient à droite
    if (s[0].x>N) s[0].x=0;  if (s[0].x<0) s[0].x=N;
    if (s[0].y>M) s[0].y=0;  if (s[0].y<0) s[0].y=M;
 

// ces lignes bizarre pourraient expliquer le comportement quand le serpent se croise (il perd sa taille)
// c'est ce que je croyais être un bug mais c'est sans doute la base du jeu. au niveau du croisement, le serpent 
// reprend cette taille là. Testons cela.
    for (int i=1;i<num;i++)
     if (s[0].x==s[i].x && s[0].y==s[i].y)  num=i;
 }

int main()
{  
    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake Game!");

    Texture t1,t2;
    t1.loadFromFile("images/white.png");
    t2.loadFromFile("images/red.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);

    Clock clock;
    float timer=0, delay=0.1;

    // initialisation de la position initial du fruit
    f.x=10;
    f.y=10; 
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds(); // on regarde combien de seconde depuis initilisation de clock
        clock.restart();// on remet clock à zero
        timer+=time; // on increment timer avec time

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)      
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) dir=1;   
        if (Keyboard::isKeyPressed(Keyboard::Right)) dir=2;    
        if (Keyboard::isKeyPressed(Keyboard::Up)) dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) dir=0;


        // si on depasse le delay, on appelle la fonction Tick.
        if (timer>delay) {timer=0; Tick();}

   ////// draw  ///////
        // triade classsique clear/draw/display

  
    window.clear();

    // dessine la grille.
    for (int i=0; i<N; i++) 
      for (int j=0; j<M; j++) 
        { sprite1.setPosition(i*size, j*size);  window.draw(sprite1); }


    // dessine le serpent: num correspond à sa longueur en carré.
    for (int i=0;i<num;i++)
        { sprite2.setPosition(s[i].x*size, s[i].y*size);  window.draw(sprite2); }
   
    // affichage du fruit
    sprite2.setPosition(f.x*size, f.y*size);  window.draw(sprite2);    

    window.display();
    }

    return 0;
}
