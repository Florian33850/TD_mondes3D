

Florian DAYRE



Rapport

1) TD4

En ce qui concerne le TD4, j’en suis arrivé au bout, c'est-à-dire jusqu’à la mise en place du
système solaire.

a) Introduction / Transformations / Caméra

En ce qui concerne ces parties, je n’ai pas vraiment eu de problème de compréhension et
les quelques points bloquants ont été expliqués par vous pendant les phases de TP. Le
code de ces parties sont dans drawScene() et drawScene2D(). J’ai essayé de commenter
au mieux cette dernière fonction suivant le déroulé du TP.

b) Système solaire

Cette partie a été la plus compliquée pour moi. Cependant une fois que j’ai compris vraiment
comment utiliser les fonctions AngleAxis et Scalling, j’ai réussit à initialiser correctement les
différentes matrices dans la fonction Viewer::init() à partir des informations données dans le
schéma du TP.

Ensuite, j’ai créé la fonction drawScene3DSolarSysteme(), qui permet donc de gérer les
rotations de la lune et de la terre sur elle-même, ainsi que la rotation de la lune autour de la
terre et de la terre autour du soleil. J’ai également essayé de commenter au maximum pour
expliquer ma démarche.

Dans le cas de la rotation d’une planète autour d’elle même, je récupère le vecteur de
translation à partir de l’affine de la matrice de l’objet correspondant. Ensuite, comme vu pour
les parties précédentes, je réalise une translation de ce vecteur, suivi de la rotation de l’objet
suivant l’axe que je définis, suivi d’une translation inverse. Enfin, je mets en place cette
rotation en multipliant l’affine calculée précédemment par la matrice de l’objet.

Je n’ai pas fait de capture d’écrans de cette partie étant donnée que l'intérêt est de le voir en
action.

2) TD5

J’ai également pu finaliser le TD5 même si j’ai encore quelques bugs. Les modification de
code correspondantes à ce TD sont drawScene3DSolarSystem et drawScene3D où je fais
le calcul des normales. J’ai également deux nouveaux fichiers de shaders que sont
blinn.vert et blinn.frag.

a) Calcul de l’éclairage

Pour le calcul de l’éclairage, j’ai donc transmis les normales au shader via l’attribut
normal\_mat. Je calcule les normales à partir de la matrice de la caméra et de la matrice de
mon objet.

J’ai ensuite pu écrire la fonction blinn dans mon vertex shader, qui est maintenant dans le
fragment shader, à partir des formules qui nous étaient données.

Ensuite, j’ai calculé les vecteurs view\_pos, normal et light\_dir dans le main de mon fragment
shader, maintenant dans le vertex shader.

J’ai donc pu obtenir les résultats correspondant aux captures d’écran blinnDansVert(1) et (2)
qui correspond à l’éclairage avec la fonction blinn dans le vertex shader et j’ai également les
résultats lorsque je place cette fonction dans le fragment shader sur les captures
blinnDansFrag(1) et (2). On peut noter une amélioration significative de l’éclairage calculé
depuis le fragment shader.

J’ai également essayé d’afficher les résultats uniquement avec l’éclairage diffus (voir
diffuseColor.png) ou spéculaire (voir specularColor.png) mais je n’ai pas su comment
afficher l’éclairage ambiant. De plus, je n’ai pas réussi à afficher l’image sphere.obj avec les
couleurs bleutées comme indiqué, mais images étant toujours grises.

b) Calcul des normales

Pour le calcul des normales j’ai juste suivi le protocole indiqué dans le TP. J’ai donc
parcouru tous les sommets pour mettre leur normale à 0, puis j’ai ajouté la nouvelle normale
calculée à la normale des sommets de chaque face, pour enfin normalisé ces normales. j’ai
appelé la fonction computeNormals dans Viewer::init() juste après le chargement de l’objet à
afficher.

