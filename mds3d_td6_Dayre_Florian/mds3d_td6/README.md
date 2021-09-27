

Florian DAYRE

Rapport

\1) Introduction

Pour cette partie, je n’ai pas vraiment eu de problèmes étant donné qu’il fallait surtout

s’inspirer de ce qui est fait dans la fonction Mesh::draw() pour la couleur et la normale, et

l’appliquer aux coordonnées de texture. J’ai juste bloqué sur le dernier argument de la

fonction glVertexAttribpointer(). Au début, je passais en argument 3\*sizeof(Vector3f) par

rapport à ce que je comprenais de ce qui était fait pour la couleur et la normale.

Cependant, je n’obtenais pas les bonnes couleurs (voir “3\*vec3.png”). Il s’est avéré qu’il

fallait plutôt réaliser un décalage de 2\*sizeof(Vector3f)+sizeof(Vector4f). j’obtiens donc le

bon résultat (voir “2\*vec3+vec4.png”). J’utilise les shaders de “intro.vert” et “intro.frag”.

\2) Placage de texture

Dans cette partie, j’ai réalisé tout ce qui était attendu. J’ai réalisé le chargement des textures

Viewer::init() et le passage de ces textures aux shader dans Viewer::drawScene() grâce aux

fonctions données dans le TD et la documentation fournie en lien. glActiveTexture() permet

de signaler la texture que l’on va manipuler à l’aide d’un numéro (GL\_TEXTURE0 pour la

texture numéro 0..etc). glbindTexture() permet d’indiquer la texture chargée que l’on

souhaite utiliser. Enfin, glUniform1i permet de transmettre la texture aux shaders. J’utilise les

shaders “placageText.vert” et “placageText.frag”.

Dans le fragment de shader je récupère donc les textures avec des uniform sampler2D. Je

peux ensuite créer un vec4 à l’aide de l’appelle à la fonction GL texture() qui prend en

paramètres le sampler2D et les coordonnées de texture. Il ne reste plus qu’à appliquer la

lumière à cette texture et à l’afficher. En ce qui concerne la superposition des textures, il faut

appeler la fonction GL mix() qui prend en paramètres les deux textures et un troisième

argument permettant de gérer comment se réalise la superposition. Par exemple, pour la

texture earth\_night, on superpose la texture earth et cette dernière que l’on paramètre par

rapport à la lumière. Ce qui permet d’appliquer la texture earth\_night sur l’espace non

éclairée par la lumière.

Mes résultats sont présents sur les captures d’écran “earth.png”, “earthCloud.png” et

“eathNight.png”.

\3) Filtrage

Pour cette dernière, j’ai pu réaliser les tests demandés. J’ai donc suivi les instructions du TP

qui étaient relativement détaillées et j’ai pu obtenir les résultats suivants :

\-

“linear.png” et “nearest.png” pour la texture “cow”. L’attribut nearest donne une

sensation de flou à l’objet.





\-

“linearMipmapNearest.png” et “nearestMipmapLinear.png” pour les objets “earth”,

“plane” et “cow”. J’ai l’impression que l’attribut linear\_mipmap\_nearest a tendance à

faire ressortir les polygones des objets, là où nearest\_mipmap\_linear a tendance à

lisser l’objet.

Pour la mipmap, on voit donc bien la modification des différentes couleurs sur chaque objet,

suivant si on s’en rapproche ou s’en s’éloigne. Du plus loin au plus proche, j’ai donc capturé

les résultats avec “cow(1).png” jusqu’à “cow(4).png”.

