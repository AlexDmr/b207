// On place ici quelques définitions de structures afin de rendre plus lisible
// le fichier "alx_entite2D.h"

typedef struct _ENS_CARACT_PHYSIQUE
{
 double masse            // En Kg.
      , rotation         // En degrès par rapport au model (centre: barycentre).
      ;
 alx_point2D barycentre; // par rapport au model

// Les caractéristiques cinétiques
 alx_vecteur2D vitesse   // La vitesse en m/s
             , position; // La position (coordonnées en mètre).
 double vitesse_rotation;// Dans le sens trigonométrique.
 
} ENS_Caracteristiques_physiques;

typedef struct _ENS_PRIMITIVE_2D
{
 list<alx_droite2D>  L_droite2D; // interdit pour les objets mobiles
 list<alx_segment2D> L_segment2D;
 list<alx_cercle2D>  L_cercle2D;
} ENS_primitive2D;

 