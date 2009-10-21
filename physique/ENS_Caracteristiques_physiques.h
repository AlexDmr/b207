//#ifndef __ALX_ENS_CARACTERE_PHYSIQUE_H__
//#define __ALX_ENS_CARACTERE_PHYSIQUE_H__


   double elasticite;
   double masse                   // En Kg.
        , rotation                // En radian par rapport au model (centre: barycentre).
        ;

  // Les caract�ristiques cin�tiques:
   alx_vecteur2D force;
   alx_vecteur2D acceleration     // L'accel�ration en m/s�
               , vitesse          // La vitesse en m/s
               , position;        // La derni�re position en m�tre par rapport au model

   alx_liste<alx_deplacement2D> L_dep; // Les positions (coordonn�es en m�tre).

   alx_point2D centre_rot;        // dans le rep�re du mod�le.
   double vitesse_rotation        // Dans le sens trigonom�trique.
        , acceleration_rot;       // Acc�l�ration angulaire.

//#endif