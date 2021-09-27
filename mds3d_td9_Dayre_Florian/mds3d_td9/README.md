Florian DAYRE

Rapport

Ce TP ne m'a pas vraiment posé de problème étant donné que les différentes étapes étaient bien explicitées. Le principal challenge a été la compréhension de la documentation de surface_mesh et des différentes fonctions qui la composent. 

Une fois cela de passé j'ai pu réaliser les trois étapes assez rapidement et j'ai obtenu les résultats que l'on peut voir sur les captures préfixée par subdivide. J'ai juste eu un problème que l'on peut voir sur les captures "bug(1)" qui été dû au fait que j'avais pas enlevé du code la boucle for, qui parcourait toutes les faces, que j'avais mis en place pour tester l'étape 3.

Enfin, pour la gestion des bords il m'a suffit de rajouter deux conditions à l'étape 3 et 1 pour appliquer les nouvelles formules. Le "bug2" est juste dû au fait que dans l'étape 3 je n'utilisais pas le halfedge qui varie dans la condition alors qu'il le fallait.
