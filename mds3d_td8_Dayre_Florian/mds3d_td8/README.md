

Florian DAYRE

\1) Cylindre

README

a) Aligné sur un axe

Pour cette partie, je suis repartie des formules du diaporama du cours et j’y suis allé petit à

petit, en testant des formules, pour être sûr de bien comprendre comment cela fonctionnait.

J’ai laissé les différentes formules que j’ai testé dans le vertex shader, dans la fonction

cylinder. On peut voir les résultats que j’ai obtenus sur les captures préfixées de “cylindre”,

numérotées de 1 à 5..

b) Aligné sur un axe quelconque

Je n’ai pas réussi à obtenir le même résultat que précédemment avec un axe quelconque.

Mes résultats correspondent à “cylindre(6)” et “cylindre(7).

\2) Courbes de Bézier

a) Courbe planaire et sans repliement

Pour la fonction bezier, je me suis contenté de renvoyer le coefficient B[u] de bézier à partir

de la formule du cours. Pour cylBezierXY, j’ai repris ce que j’avais fait pour la dernière partie

de la partie Cylindre mais avec le calcul du coefficient B[u].

b) Suivi de la trajectoire de la courbe

J’ai tenté de calculer le vecteur tangent dans la fonction bezier mais j’ai pas réussi à l’utiliser

ensuite.

