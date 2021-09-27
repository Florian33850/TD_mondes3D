Florian DAYRE 

Rapport TD1 

1. Avancé 

J’ai réalisé toute la partie “obligatoire” du TD et les différentes parties fonctionnent de la manière attendue. Les résultats obtenus sont bien conformes aux différentes captures d’écrans du TD. 

Il manque juste, pour la partie sur les rebonds multiples, le fait de récupérer dans le code l’attribut maxRecursion de l’intégrateur “whitted”. En effet, dans mon code je le fixe manuellement à 4. 

2. Difficultés rencontrées 

Durant ce TD il m’a fallu me rappeler et/ou retrouver des formules de base, que ce soit pour des calculs d’angle, de vecteur, de distance entre des points...etc dont je ne me souvenais plus.  

Un autre problème que j’ai rencontré est au niveau du debugage de mon code lorsque l’affichage des différents fichiers ne correspondait pas au capture d'écrans données dans le TD. Plusieurs fois je me suis retrouvé dans une situation où un vecteur n’était pas dans le bon sens, et c’est plutôt difficile de se rendre compte que le problème vient de là, à partir de notre image qui ne s’affiche pas correctement. 

Dans la partie de calcul de l’éclairage, pour la formule de la partie spéculaire j’ai dû utiliser la formule avec cos(alpha), car la formule avec max me provoquait des erreurs comme on peut le voir sur la capture “directerreur(1)”. 

Enfin, dans la partie des rebonds multiples, j’avais commencé par calculer la couleur r avec ma boucle récursive puis je multipliait par la réflectivité du matériau. Or cela ne renvoyait pas le bon résultat comme on peut le voir avec “whittedErreur(1)”, les reflets étant bien trop coloré. J’ai donc ensuite appliqué à la couleur, la multiplication par la réflectivité, directement dans ma boucle récursive, ce qui m’a permis d’obtenir des reflets beaucoup plus fins. 

Une fois ces difficultés surmontées et étant donné que l’on est bien dirigé dans le TD, il m’a suffit d’implémenter les différentes formules données sans trop de problèmes. 
