Florian DAYRE 

Rapport 

1) Normal mapping sur un plan 

J’ai pas eu de réel difficulté sur cette partie. J’ai bien pensé à multiplier la normale par deux avant de lui retirer un pour la faire passer de l’intervalle [0,1] à [-1,1]. Au début, je pensais qu’il fallait faire l’inverse mais en testant je me suis rendu compte de l’erreur. On peut voir mes différents résultats sur les captures d'écran “plan(1,2,3,4).png” et sur “sphere.png” pour l’affichage sur autre chose qu’un plan. 

2) Calcul du repère tangent  

Cette partie a été un peu plus compliquée, j’ai du ré-écouter les explications données en cours et bien comprendre les formules qu’on a sur les différentes slides. J’ai ensuite suivi les étapes une par une, en oubliant pas de transmettre tangent et bitangent aux shaders pour ensuite les afficher. On peut voir les résultats que j’ai obtenus sur les captures “tangent.png” et “bitangent.png”. 

3) Normal mapping sur une sphère 

Je n’ai pas réussi à avoir le bon affichage sur cette dernière partie. Dans le vertex shader je pense bien à normaliser la tangente et cotangente avant de les transmettre au fragment shader. Ensuite dans le fragment shader, je récupère la texture earth\_normal comme j’ai fait avant, et je re normalise la normale, tangente et cotangente, avant de créer la matrice tbnv. Enfin, j’appelle la fonction blinn en pensant à multiplier la vue et la lumière par cette matrice. Cependant, je n’ai pas réussi à afficher la texture, j’obtiens le résultat “normalMapSpehere.png”. 
