=================================================
   _____                   _           
  / ____|                 | |          
 | (___    _ __     __ _  | | __   ___ 
  \___ \  | '_ \   / _` | | |/ /  / _ \
  ____) | | | | | | (_| | |   <  |  __/
 |_____/  |_| |_|  \__,_| |_|\_\  \___|
                                       
=================================================

Snake pour microcontr�leur C8051F020
D�cembre 2013 -- Bastien Cl�ment

=================================================

P�riph�riques utilis�s:

  * Matrice � LEDs 8x8
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

  [F]   --  Active / d�sactive la pause

  [E]   --  Active le mode pas � pas
            et effectue un pas

  [A]   --  Active / d�sactive les murs

  [C]   --  R�initialisation sur l'affichage
            de l'�cran des scores.

Fonctionnement des murs:

  Les murs (bords de l'�cran) ne sont en principe
  pas mortels. Si le serpent tente de d�passer le
  bord de l'�cran,  il ressort du c�t� oppos�, si
  la place le lui permet.

  Lorsque les murs sont "activ�s" (touche [A]) du
  clavier,  tenter de d�passer le bord de l'�cran
  provoque la fin de la partie.

  Il n'y a pas de t�moin visuel indiquant  l'�tat
  actuel des murs. Cependant,  la  partie  d�bute
  toujours avec les murs d�sactiv�s et  un  effet
  de flash indique chaque changement de mode.
