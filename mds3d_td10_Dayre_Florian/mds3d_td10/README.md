

Florian DAYRE

README

\1) Rendu du bras articulé

Pour cette partie, je n’ai pas eu de réelle difficulté de compréhension. Je suis tout de même

passé par plusieurs étapes pour bien comprendre comment gérer la taille des différents

segments et l'enchaînement des deux rotations et de la translation. On peut voir les résultats

sur les captures d’écran numérotées de 1 à 4. Au final, je récupère les valeurs de phi et

thêta au début de la boucle for suivant l’indice i du segment. Ensuite, je réalise la première

rotation sur l’axe z suivant phi, puis la deuxième rotation sur l’axe y suivant thêta. Enfin, je

réalise la translation suivant la taille du segment courant.

\2) Inverse Kinematic

Pour cette partie je n’ai pas de capture d’écran étant donné que c’est une animation. Au

début de la fonction updateAndDrawScene() je refait la même boucle que présentée

précédemment afin de pouvoir récupérer à la fin le point correspondant à l’extrémité du bras

en réalisant une ultime translation sur la matrice M. Ensuite, je refais de nouveau une boucle

similaire sauf que cette fois je calcule la matrice jacobienne, avec ri qui correspond au

vecteur allant de l'extrémité du bras vers l'extrémité du segment courant et wi et wi+1

correspondant respectivement à l’axe z et y du repère courant. Enfin, avec cette matrice, je

peux calculer le gradient G et Q en suivant les formules données dans le document.

Mes principaux problèmes pour cette partie ont été des erreurs de concentration. Par

exemple, je me suis trompé de sens pour le calcul de ri ou je n’ai pas effectué les rotations

dans le même ordre que je l’avais fait précédemment.

\3) Pour la dernière partie je n'ai pas réussit à afficher le cylindre autour de mon bras en repartant des shaders donnés. Je n'ai pas pu effectuer la suite.

