//#ifndef __ALX_ENS_CARACTERE_PHYSIQUE_H__
//#define __ALX_ENS_CARACTERE_PHYSIQUE_H__


   double elasticite;
   double masse                   // En Kg.
        , rotation                // En radian par rapport au model (centre: barycentre).
        ;

  // Les caractéristiques cinétiques:
   alx_vecteur2D force;
   alx_vecteur2D acceleration     // L'accelération en m/s²
               , vitesse          // La vitesse en m/s
               , position;        // La dernière position en mètre par rapport au model

   alx_liste<alx_deplacement2D> L_dep; // Les positions (coordonnées en mètre).

   alx_point2D centre_rot;        // dans le repère du modèle.
   double vitesse_rotation        // Dans le sens trigonométrique.
        , acceleration_rot;       // Accélération angulaire.

//#endif