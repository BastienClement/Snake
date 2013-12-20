=================================================
   _____                   _           
  / ____|                 | |          
 | (___    _ __     __ _  | | __   ___ 
  \___ \  | '_ \   / _` | | |/ /  / _ \
  ____) | | | | | | (_| | |   <  |  __/
 |_____/  |_| |_|  \__,_| |_|\_\  \___|
                                       
=================================================

Snake pour microcontrôleur C8051F020
Décembre 2013 -- Bastien Clément

=================================================

Périphériques utilisés:

  * Matrice à LEDs 8x8
  * Clavier matriciel 4x4

Configuration des ports:

  <P1>  --  Matrice LEDs / J1 (data)
  <P2>  --  Matrice LEDs / J2 (ctrl)
  <P7>  --  Clavier

Utilisation du clavier:

  [2]   --  Haut
  [4]   --  Gauche
  [5]   --  Bas
  [6]   --  Droite

  [F]   --  Active / désactive la pause

  [E]   --  Active le mode pas à pas
            et effectue un pas

  [A]   --  Active / désactive les murs

  [C]   --  Réinitialisation sur l'affichage
            de l'écran des scores.

Fonctionnement des murs:

  Les murs (bords de l'écran) ne sont en principe
  pas mortels. Si le serpent tente de dépasser le
  bord de l'écran,  il ressort du côté opposé, si
  la place le lui permet.

  Lorsque les murs sont "activés" (touche [A]) du
  clavier,  tenter de dépasser le bord de l'écran
  provoque la fin de la partie.

  Il n'y a pas de témoin visuel indiquant  l'état
  actuel des murs. Cependant,  la  partie  débute
  toujours avec les murs désactivés et  un  effet
  de flash indique chaque changement de mode.
