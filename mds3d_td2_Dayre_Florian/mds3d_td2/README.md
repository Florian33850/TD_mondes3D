Florian DAYRE

Rapport TD2

Avancé :

Je suis donc arrivé au bout du TD mais je n’ai pas eu le temps d’implémenter une
optimisation de la BVH. En effet, avec ma BVH j’arrive à environ 3,5 millions de triangles
intersectés pour un temps de 0,5 secondes. Dans mon dossier de captures d’écran il y a les
résultats visuels obtenus sur les images “twBVH”, “killerooBVH” et “killerooGlassBVH”.

Difficultés :

Pour la première partie sur Mesh, je n’ai pas eu vraiment de problèmes puisqu’il s’agissait
surtout de coder des formules qu’on nous avait donné. J’ai juste eu des oublis comme par
exemple de normaliser la normale, ce qui donnait le résultat de la capture
“MeshSansNormalisation”. J’ai également eu une erreur de lecture pour l’implémentation de
ma condition if dans Mesh::intersectFace où je tester pour u\*v au lieu de u+v, ce qui a donné
le résultat “MeshErreurCalcul”.

Je suis ensuite passé à l’implémentation de la BVH qui a été beaucoup plus compliquée. La
principale difficulté a été de bien comprendre le fonctionnement de la structure Node.

En ce qui concerne la méthode BVH::buildNode, les étapes étaient bien expliquées donc j’ai
compris assez vite ce qu’il fallait faire. Pour la première étape, j’ai eu du mal à comprendre
ce que la méthode AlignedBox::extend faisait alors je mettais un peu embêté à calculer à la
main les dimensions de la AlignedBox3d. Finalement, j’ai compris qu’il suffisait de passer
des points en paramètre et que la box sa calculait d'elle-même. Le reste de la fonction a été
plutôt intuitif.

Ensuite pour la méthode BVH::intersect je me contente de vérifier que le rayon intersecte
bien la boite englobante et si c’est le cas j’appelle la méthode BVH::intersectNode.

Enfin, pour cette méthode, je me contente de tester si le nœud que je récupère est une
feuille, si oui j’intersecte les faces du nœud sinon j’explore les deux fils. C’est là où
l’optimisation n’est pas présente puisque dans tous les cas je vais explorer les deux fils sans
vérifier si leur boîte englobante est touchée par le rayon.

Une fois, ces fonctions réalisées j’ai donc pu tester et sur l’image tw.scn le personnage ne
s’affichait pas. Après plusieurs tests je me suis rendu compte qu'à la fin de buildNode, je ne
mettais pas le nœud construit dans m\_nodes. Cela faisait que quand je faisais des print
dans buildNode tout se passait bien mais quand j’ai testé en dehors, tous mes nœuds
étaient vide. Après avoir corrigé cela, tout a fonctionné normalement ce qui fait que j’ai pas
de capture d’écrans de problèmes graphique sur cette partie.

