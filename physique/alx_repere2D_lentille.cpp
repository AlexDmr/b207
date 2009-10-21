#include "alx_repere2D_lentille.h"
#include "math_alex.cpp"

bool alx_repere2D_lentille::lentilles_initialisees = false;
double courbe_plate[2*2]
     , courbe_bombe[2*20]
     , courbe_test[2*8]
     , niveaux[8];

void Initialiser_courbes_lentilles()
{unsigned int i, pos;
 // Définition de la courbe plate, c'est à dire celle qui ne déforme rien!!! Un peu pour tester finalement...
 courbe_plate[0]=0; courbe_plate[1]=0;
 courbe_plate[2]=1; courbe_plate[3]=1;
 //Définition de la courbe bombée :
 // abscisse                 ordonnée
 courbe_bombe[0]  = 0;    courbe_bombe[1]  = 0;
 courbe_bombe[2]  = 0.05; courbe_bombe[3]  = 0.075;
 courbe_bombe[4]  = 0.1;  courbe_bombe[5]  = 0.2;
 courbe_bombe[6]  = 0.15; courbe_bombe[7]  = 0.4;
 courbe_bombe[8]  = 0.2;  courbe_bombe[9]  = 0.55;
 courbe_bombe[10] = 0.25; courbe_bombe[11] = 0.625;
 courbe_bombe[12] = 0.3;  courbe_bombe[13] = 0.68;
 courbe_bombe[14] = 0.35; courbe_bombe[15] = 0.725;
 courbe_bombe[16] = 0.4;  courbe_bombe[17] = 0.775;
 courbe_bombe[18] = 0.45; courbe_bombe[19] = 0.8125;
 courbe_bombe[20] = 0.5;  courbe_bombe[21] = 0.85;
 courbe_bombe[22] = 0.55; courbe_bombe[23] = 0.875;
 courbe_bombe[24] = 0.6;  courbe_bombe[25] = 0.9;
 courbe_bombe[26] = 0.65; courbe_bombe[27] = 0.922;
 courbe_bombe[28] = 0.7;  courbe_bombe[29] = 0.94;
 courbe_bombe[30] = 0.75; courbe_bombe[31] = 0.957;
 courbe_bombe[32] = 0.8;  courbe_bombe[33] = 0.97;
 courbe_bombe[34] = 0.85; courbe_bombe[35] = 0.98;
 courbe_bombe[36] = 0.9;  courbe_bombe[37] = 0.988;
 courbe_bombe[38] = 0.95; courbe_bombe[39] = 0.995;
 // Définission de la courbe de test
 pos = 0;
 for(i=0;i<8;i++)
  {courbe_test[pos++] = (double)(i)/7.0;
   courbe_test[pos++] = (double)(i)/7.0;}

// Définition des niveaux de détail standarts
 niveaux[0] = 0.0078125;
 niveaux[1] = 0.015625;
 niveaux[2] = 0.03125;
 niveaux[3] = 0.0625;
 niveaux[4] = 0.125;
 niveaux[5] = 0.25;
 niveaux[6] = 0.5;
 niveaux[7] = 1;
}

void Courbe_lentille_plate(info_courbe_lentille &rep) {rep.Tab(courbe_plate); rep.Nb(2);}
void Courbe_lentille_bombe(info_courbe_lentille &rep) {rep.Tab(courbe_bombe); rep.Nb(20);}
void Courbe_lentille_test (info_courbe_lentille &rep) {rep.Tab(courbe_test);  rep.Nb(8);}
double* Tab_niveaux_details(unsigned int n) {return &(niveaux[8-n]);}

//______________________________________________________________________________
//_____________________________ Les constructeurs ______________________________
//______________________________________________________________________________
alx_repere2D_lentille::alx_repere2D_lentille() : alx_repere2D()
                                               , nb_pt_courbe(0)
                                               , nb_pt_bordure(0)
                                               , nb_niveaux_details(0)
                                               , fct(NULL)
{courbe_unite = bordure = niveaux_details = (double*)NULL;

 lentille_sommets    = (double**)NULL;
 lentille_tex_coords = (double**)NULL;


 nb_couche_par_niveau = (unsigned int*)NULL;
 ordres_affichage_int = (unsigned int*)NULL;
 ordres_affichage_ext = (unsigned int*)NULL;
 Dimensions_effectives_texture(1,1);
 if(!lentilles_initialisees) Initialiser_courbes_lentilles();
 lentilles_initialisees = true;
 Nb_pix_par_unite( 1 );
}

alx_repere2D_lentille::~alx_repere2D_lentille()
{if(nb_pt_courbe)    delete[] courbe_unite;
 if(nb_pt_bordure)   delete[] bordure;
 if(niveaux_details) delete[] niveaux_details;
 Effacer_ordre_affichage();
 Effacer_lentille();
}

//______________________________________________________________________________
//_______________________________ Les méthodes _________________________________
//______________________________________________________________________________
void alx_repere2D_lentille::Tranche( const double *P
                                   , const double *C                  // Le centre
                                   , const unsigned int dec_bord      // Pour faire la différence en CE et CT pour la bordure
                                   , unsigned int &p1, unsigned int &p2 ) const   // Les deux point de la bordure de la tranche
{unsigned int a_p1;
 const double *B;
 double cx = C[dec_bord]
      , cy = C[dec_bord+1];
 alx_vecteur2D vcp(P[0]-cx, P[1]-cy);

 a_p1 = p1 = 0;
 p2 = nb_pt_bordure;

 // Recherche de la position de départ pour la vrai recherche...héhéhé..ahem
 B = &( bordure[(p1<<2)+dec_bord] );
 alx_vecteur2D vp1(B[0]-cx, B[1]-cy)
             , vp1g(vp1);
 vp1g.Perpendiculaire_g();
 alx_vecteur2D vg(vp1g)
             , v(vp1);

 while( vp1g.produit_scalaire(vcp) < 0 )
  {if( vp1.produit_scalaire(vg) > 0 )
    {a_p1 = p1;
     p1 += (p2-p1+1)/2;}
    else
     {unsigned int p1_tmp = p1;
      p1 = a_p1+(p2-a_p1+1)/2;
      if(p1_tmp!=0)
        p2 = p1_tmp;
     }
   //     DEBUG
   if(p1 >= p2)
   {unsigned int tmp = p1;
    /*p1 = p2;
    p2 = tmp;*/
   }
   // FIN_DEBUG
   B = &( bordure[(p1<<2)+dec_bord] );
   vp1.maj(B[0]-cx, B[1]-cy);
   vp1g.maj(vp1);
   vp1g.Perpendiculaire_g();
  }

 while( p1 < p2 )
  {if( vp1g.produit_scalaire(vcp) < 0 )
    {p2 = p1;
     p1 = a_p1;}
   a_p1 = p1;
   p1 += (p2-p1+1)/2;

   B = &( bordure[(p1<<2)+dec_bord] );
   vp1.maj(B[0]-cx, B[1]-cy);
   vp1g.maj(vp1);
   vp1g.Perpendiculaire_g();
  }

 p1 = Amax((int)(p1-1), 0); p1 %= nb_pt_bordure;
 p2 %= nb_pt_bordure;

//     DEBUG
if(p1 >= p2)
  {unsigned int tmp = p1;
   /*p1 = p2;
   p2 = tmp;*/
  }
// FIN_DEBUG

}

//______________________________________________________________________________
void alx_repere2D_lentille::Triangle_contenant( const double *P
                                              , const double *C
                                              , const unsigned int dec_bord
                                              , const unsigned int p1, const unsigned int p2
                                              , const double **tab_tab
                                              , unsigned int &niv
                                              , unsigned int &T1, unsigned int &T2, unsigned int &T3 ) const
{double cx = C[dec_bord]
      , cy = C[dec_bord+1];
 alx_point2D pt(P[0], P[1])
           , pt1(bordure[(p1<<2)+dec_bord], bordure[(p1<<2)+dec_bord+1])
           , pt2(bordure[(p2<<2)+dec_bord], bordure[(p2<<2)+dec_bord+1])
           , pt_bord;
 alx_vecteur2D v_c_p(P[0]-cx, P[1]-cy)
             , v_pt(pt);
 alx_droite2D d(pt, v_c_p)
            , db(pt1, pt2);

 d.intersection(db, pt_bord);
 alx_point2D centre(cx, cy);
 double dist  = centre.distance( pt_bord );
 double alpha;
 if(dist != 0)
   alpha = 1 - (v_c_p.Norme()/dist);
  else alpha = -1;

 sabordage = false;
 if(alpha < 0)
  {sabordage = true;
   return;}

 // Avec alpha, on détermine où on se trouve par rapport à la courbe unité.
 unsigned int pos_courbe = 0
            , i = 0
            , dec = dec_bord>>1
            , indice_courbe = (pos_courbe<<1)+dec;
 while( (pos_courbe < nb_pt_courbe)
      &&(courbe_unite[indice_courbe] < alpha) ) {pos_courbe++;
                                                 indice_courbe = (pos_courbe<<1)+dec;
                                                }
 if(pos_courbe >= nb_pt_courbe) {
   pos_courbe = nb_pt_courbe-1;
   indice_courbe = (pos_courbe<<1)+dec;
  }
 /*if(pos_courbe != nb_pt_courbe-1)  {
   pos_courbe = pos_courbe; //Amax(0, (int)(pos_courbe)-1);
  }*/

 // XXX Test DEBUG
 if( pos_courbe == (nb_couche_par_niveau[i]-1) )
  {pos_courbe = pos_courbe;
  }
 // XXX

 while( (int)(pos_courbe)-(int)(nb_couche_par_niveau[i]) >= 0 )
   {pos_courbe-=nb_couche_par_niveau[i]; i++;}
 niv = i;
 if(niv > 0)
  niv = niv;
 const double *tab = tab_tab[i];

 if(niv == nb_niveaux_details) // On est sur la couche centrale
  {T2 = 0; // Le centre
   T1 = 2 + p1*2;
   T3 = 2 + p2*2;
   return;
  } else {if(pos_courbe>0)
            pos_courbe--;
         }

 // Sinon, on est sur une des couches externe.

 // On sait maintenant dans quel trapèze on se trouve.
 unsigned int trap_1 = p1*4 + (nb_pt_bordure*4)*pos_courbe
            , trap_2 = trap_1 + (nb_pt_bordure*4) + 2;

 // DEBUG
         if(trap_1>nb_couche_par_niveau[niv]*nb_pt_bordure*4)
          {trap_1++;
           trap_1--;}
 // FIN DEBUG
 pt1.maj(tab[trap_1], tab[trap_1+1]);
 pt2.maj(tab[trap_2], tab[trap_2+1]);
 v_c_p.maj(pt2, pt1);
 v_c_p.Perpendiculaire_g();
 v_pt.maj(pt2, pt);
 if( v_pt.produit_scalaire(v_c_p) > 0 )
  {trap_1+=4; if(trap_1%(nb_pt_bordure<<2)==0) trap_1 = Amax((unsigned int)0, trap_1-(nb_pt_bordure<<2));
   pt1.maj(tab[trap_1], tab[trap_1+1]);
   v_c_p.maj(pt2, pt1);
   v_c_p.Perpendiculaire_g();
   T1 = trap_1;
   T2 = trap_2;
   if( v_pt.produit_scalaire(v_c_p) > 0 ) // Cas du triangle à droite
    {T3 = trap_1 + (nb_pt_bordure<<2);
     return;}
   // Cas du triangle au centre.
   T3 = trap_2 - (nb_pt_bordure<<2) - 2;
   return;
  }
 // Cas du triangle à gauche.
 T1 = trap_1;
 T2 = trap_2;
 T3 = trap_2 - 2;
}

//______________________________________________________________________________
void alx_repere2D_lentille::Changer_coordonnees_inverse( alx_point2D &pt ) const
{// On sort du repère de la lentille
 // 1) On repasse en coordonnées du corp, puis en coordonnées texture
 alx_repere2D::Changer_coordonnees_inverse(pt);
 if( (Taille_x() == 0)
   ||(Taille_y() == 0)
   ||(bordure == NULL)||(courbe_unite == NULL)||(niveaux_details==NULL) ) return;
 pt.Etirer(1/Taille_x(), 1/Taille_y());

 // 2) A partir des coordonnées texture, on déduit les coordonnées spatiales.
 // CE : coordonnées espace, CT : coordonnées textures
   // a) en CT, on calcul le vecteur centre-pt.
   // b) On récupère la tranche où le point se trouve
   // c) On récupère la distance segment sur la rayon centre-pt-bordure
   // d) On identifie le quadrilatère contenant pt
   // e) On Interpolle dans la triangle contenant pt, pt est alors exprimé en CE
 double P[2];
   P[0] = pt.x;
   P[1] = pt.y;
 unsigned int p1, p2, t1, t2, t3, niv;
 Tranche( P
        , centre
        , 2        // décalage pour faire la différence en CE et CT pour la bordure
        , p1, p2 );
 Triangle_contenant( P
                   , centre
                   , 2
                   , p1, p2
                   , (const double **)lentille_tex_coords
                   , niv
                   , t1, t2, t3 );
 if(sabordage) return; // On est hors de la lentille

 double *P1 = &(lentille_sommets[niv][t1]), *T1 = &(lentille_tex_coords[niv][t1])
      , *P2 = &(lentille_sommets[niv][t2]), *T2 = &(lentille_tex_coords[niv][t2])
      , *P3 = &(lentille_sommets[niv][t3]), *T3 = &(lentille_tex_coords[niv][t3]);

 // 4) On a trouvé le bon triangle, on interpolle pour avoir la correspondance
 //    en coordonnées texture.
 Changer_coord( pt
              , T1, P1
              , T2, P2
              , T3, P3 );
}

//______________________________________________________________________________
void alx_repere2D_lentille::Changer_coordonnees( alx_point2D &pt ) const
{// On passe dans le repère de la lentille
 if( (Taille_x() == 0)
   ||(Taille_y() == 0)
   ||(bordure == NULL)||(courbe_unite == NULL)||(niveaux_details==NULL) ) return;

 double P[2];
 P[0] = pt.x;
 P[1] = pt.y;

 // 1) Repérer dans quelle tranche on se trouve, on identifie B1 et B2
 //  , les deux points de la bordure définissant cette tranche
 // 2) On calcul la distance relative du point au bord (par rapport à la longueur
 //    du rayon qui part du centre et passe par le point. C'est alpha.
 // 3) A partir de alpha on obtient les 5 points dans le trapèze à considérer.
 //    Cela dit on ne les considère que par 3.
 unsigned int p1, p2, t1, t2, t3, niv;
 Tranche( P
        , centre
        , 0        // décalage pour faire la différence en CE et CT pour la bordure
        , p1, p2 );

 Triangle_contenant( P
                   , centre
                   , 0
                   , p1, p2
                   , (const double **)lentille_sommets
                   , niv
                   , t1, t2, t3 );

 if(sabordage)
  {//if( fct ) fct(pt, param);
   return; // On est hors de la lentille
  }

 double *P1 = &(lentille_sommets[niv][t1]), *T1 = &(lentille_tex_coords[niv][t1])
      , *P2 = &(lentille_sommets[niv][t2]), *T2 = &(lentille_tex_coords[niv][t2])
      , *P3 = &(lentille_sommets[niv][t3]), *T3 = &(lentille_tex_coords[niv][t3]);

 // 4) On a trouvé le bon triangle, on interpolle pour avoir la correspondance
 //    en coordonnées texture.
 Changer_coord( pt
              , P1, T1
              , P2, T2
              , P3, T3 );
 // 5) Bon c'est bien gentil mais on a des coordonnées entre 0 et 1 là...vite la transfo!
 pt.Etirer(Taille_x()/Nb_pix_par_unite(), Taille_y()/Nb_pix_par_unite());
 alx_repere2D::Changer_coordonnees(pt);
 if( fct ) fct(pt, param);
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D_lentille::Courbe_unite(const double *C, const unsigned int nb)
{if(nb_pt_courbe!=nb)
  {if(courbe_unite) delete[] courbe_unite;
   courbe_unite = new double[2*nb];
   nb_pt_courbe = nb;
   icl.Tab(courbe_unite);
   icl.Nb(nb);}
 memcpy(courbe_unite, C, (2*nb*sizeof(double)));
}

//______________________________________________________________________________
void alx_repere2D_lentille::Bordure(const double *B, const unsigned int nb)
{if(nb_pt_bordure != nb)
  {if(bordure) delete[] bordure;
   bordure = new double[4*nb];
   nb_pt_bordure = nb;}
 memcpy(bordure, B, (4*nb)*sizeof(double));
}

//______________________________________________________________________________
void alx_repere2D_lentille::Niveaux_details(const double *niv, const unsigned int nb)
{if(nb_niveaux_details!=nb)
  {if(niveaux_details) delete[] niveaux_details;
   niveaux_details = new double[nb];
   nb_niveaux_details = nb;
   if(nb_couche_par_niveau) delete[] nb_couche_par_niveau;
   nb_couche_par_niveau = new unsigned int[nb];
  }

 memcpy(niveaux_details, niv, nb*sizeof(double));
}

//______________________________________________________________________________
//_______________________________La lentille____________________________________
//______________________________________________________________________________
void alx_repere2D_lentille::Effacer_ordre_affichage()
{if(ordres_affichage_int) delete[] ordres_affichage_int;
 if(ordres_affichage_ext) delete[] ordres_affichage_ext;
 ordres_affichage_int = (unsigned int*)NULL;
 ordres_affichage_ext = (unsigned int*)NULL;}

//______________________________________________________________________________
void alx_repere2D_lentille::Allouer_ordre_affichage()
{// La couche en rayon, centrale
 ordres_affichage_int = new unsigned int[2+nb_pt_bordure]; // centre plus les bords dont le premier répété deux fois
 // La couche en triangle
 ordres_affichage_ext = new unsigned int[2+(nb_pt_courbe-1)*(8*(nb_pt_bordure-1)+8)]; // On a besoin d'un point d'initialisation et de terminaison
}

//______________________________________________________________________________
void alx_repere2D_lentille::Generer_ordre_affichage()
{unsigned int i, j
            , pos    = 0
            , num_pt = 0
            , nb_pt_bord_total = 2*nb_pt_bordure;

 // Pour la couche centrale
 // C'est pipo, c'est la suite d'entier de 0 à nb_pt_bordure plus le premier répété à la fin
 for(i=0;i<=nb_pt_bordure;i++)
   ordres_affichage_int[i] = i;
 ordres_affichage_int[i] = 1;


 /*ordres_affichage_ext[pos++] = nb_pt_bord_total - 2;
 for(j=0; j<nb_pt_courbe-1; j++)        // Pour chaque point de la courbe sauf celui représentant le centre, on fait un anneau
  {for(i=0;i<nb_pt_bordure;i++)         //     Pour chaque point de la bordure de cette couche
    {ordres_affichage_ext[pos++] = num_pt;
     ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total;
     num_pt += 2;}
  }
 ordres_affichage_ext[pos++] = num_pt;

 return; //pour tester        */

 // Pour les couches externes
 ordres_affichage_ext[pos++] = nb_pt_bord_total-2; // Le point de départ
 ordres_affichage_ext[pos++] = 2*nb_pt_bord_total-1; // Le point de départ
 for(j=0; j<nb_pt_courbe-1; j++)        // Pour chaque point de la courbe sauf celui représentant le centre, on fait un anneau
  {for(i=0;i<nb_pt_bordure-1;i++)         //     Pour chaque point de la bordure de cette couche
    {ordres_affichage_ext[pos++] = num_pt;
     ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total;
     ordres_affichage_ext[pos++] = num_pt + 1;
     ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total + 2;
     ordres_affichage_ext[pos++] = num_pt + 1;
     ordres_affichage_ext[pos++] = num_pt + 1;
     ordres_affichage_ext[pos++] = num_pt;
     ordres_affichage_ext[pos++] = num_pt + 1;
     num_pt += 2;}

   // Le cas particulier du dernier, on a un changement de couche, on passe à la plus interne suivante
   // de plus les ordres d'affichages doivent suivrent...
   ordres_affichage_ext[pos++] = num_pt;
   ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total;
   ordres_affichage_ext[pos++] = num_pt + 1;
   ordres_affichage_ext[pos++] = num_pt + 2;
   ordres_affichage_ext[pos++] = num_pt + 2;
   ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total;
   ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total;
   ordres_affichage_ext[pos++] = num_pt + nb_pt_bord_total + 1;

   num_pt += 2;
  }
}

//______________________________________________________________________________
//_______________________________La lentille____________________________________
//______________________________________________________________________________
void alx_repere2D_lentille::Effacer_lentille()
{unsigned int i;
 if(lentille_tex_coords)
  {for(i=0; i<nb_niveaux_details+1; i++)
     delete[] lentille_tex_coords[i];
   delete[] lentille_tex_coords;
   lentille_tex_coords = (double**)NULL;
  }
 if(lentille_sommets)
  {for(i=0; i<nb_niveaux_details+1; i++)
     delete[] lentille_sommets[i];
   delete[] lentille_sommets;
   lentille_sommets = (double**)NULL;
  }
}

//______________________________________________________________________________
void alx_repere2D_lentille::Allouer_lentille()
{lentille_sommets    = new double*[nb_niveaux_details+1]; // le dernier étant pour la couche central, affichée en rayon
 lentille_tex_coords = new double*[nb_niveaux_details+1]; // le dernier étant pour la couche central, affichée en rayon

// Les couches en triangle
 unsigned int pos = 0;
 for(unsigned int i=0;i<nb_niveaux_details;i++)
  {// Compter le nombre de couche qui dépendent de ce niveau de détail
   nb_couche_par_niveau[i] = 0;
   while( (niveaux_details[i] > courbe_unite[pos<<1])
        &&(pos < nb_pt_courbe-2) )
    {nb_couche_par_niveau[i]++;
     pos++;}
   if(nb_couche_par_niveau[i]) nb_couche_par_niveau[i]++;

   // Allouer la place correspondante (2 coordonnées par point chaque tableaux, espace et texture)
   lentille_sommets[i]    = new double[(nb_couche_par_niveau[i]*(2*nb_pt_bordure))<<1];
   lentille_tex_coords[i] = new double[(nb_couche_par_niveau[i]*(2*nb_pt_bordure))<<1];
  }

// La couche en rayon, centrale
 lentille_sommets[nb_niveaux_details]    = new double[(1+nb_pt_bordure)<<1]; // centre plus les bords, 4 coordonnées par point
 lentille_tex_coords[nb_niveaux_details] = new double[(1+nb_pt_bordure)<<1]; // centre plus les bords, 4 coordonnées par point
}

//______________________________________________________________________________
void alx_repere2D_lentille::Generer_lentille()
{unsigned int i, j, k
            , pos, pos_courbe, pos_bord
            , niv = 0;
 double val_niv = 0
      , va_niv_prcdt = 0
      , pt_tmp[4]
      , delta_espace, delta_texture;


// Il faut générer en couche, de la plus externe à la plus interne.
 pos_courbe = 0;
 for(i=0;i<nb_niveaux_details;i++)
  {pos = 0;
   for(j=0;j<nb_couche_par_niveau[i];j++)
    {pos_bord = 0;
     for(k=0;k<nb_pt_bordure;k++)
      {// Le point sur la bordure.
       lentille_tex_coords[i][pos] = Interpolation_lineaire( bordure[pos_bord+2]
                                                           , centre[2]
                                                           , (courbe_unite[pos_courbe+1]-0.5*val_niv*Text_x()) / (1-val_niv) );
       lentille_sommets[i][pos++]  = Interpolation_lineaire( bordure[pos_bord]
                                                           , centre[0]
                                                           , courbe_unite[pos_courbe]);
       lentille_tex_coords[i][pos] = Interpolation_lineaire( bordure[pos_bord+3]
                                                           , centre[3]
                                                           , (courbe_unite[pos_courbe+1]-0.5*val_niv*Text_y()) / (1-val_niv) );
       lentille_sommets[i][pos++]  = Interpolation_lineaire( bordure[pos_bord+1]
                                                           , centre[1]
                                                           , courbe_unite[pos_courbe]);
       // Le point au centre du segment correspondant, utile pour simuler une
       // déformation par étirement en 4 points à l'aide d'une liste de triangle.
       delta_espace  = Interpolation_lineaire(courbe_unite[pos_courbe], courbe_unite[pos_courbe+2], 0.5);   //courbe_unite[pos_courbe];
       delta_texture = Interpolation_lineaire(courbe_unite[pos_courbe+1], courbe_unite[pos_courbe+3], 0.5); //courbe_unite[pos_courbe+1];
       Interpolation_lineaire_V( 4
                               ,&(bordure[pos_bord])
                               ,&(bordure[(pos_bord+4)%(4*nb_pt_bordure)])
                               , 0.5
                               , pt_tmp);
       lentille_tex_coords[i][pos] = Interpolation_lineaire( pt_tmp[2]
                                                           , centre[2]
                                                           , (delta_texture-0.5*val_niv*Text_x()) / (1-val_niv) );
       lentille_sommets[i][pos++]  = Interpolation_lineaire( pt_tmp[0]
                                                           , centre[0]
                                                           , delta_espace);
       lentille_tex_coords[i][pos] = Interpolation_lineaire( pt_tmp[3]
                                                           , centre[3]
                                                           , (delta_texture-0.5*val_niv*Text_y()) / (1-val_niv) );
       lentille_sommets[i][pos++]  = Interpolation_lineaire( pt_tmp[1]
                                                           , centre[1]
                                                           , delta_espace);

       pos_bord += 4;
      }
     pos_courbe += 2;
    }
   pos_courbe -= 2; // on se recalle pour générer les même point MAIS avec des coordonnées textures différentes.
   va_niv_prcdt = val_niv;
   niv++;
   val_niv      = niveaux_details[niv];
  }

// A la sortie de la bouvle, on a incrémenté niv une fois de trop, en effet
// la zone centrale correspond toujours au même niveau.
 val_niv = va_niv_prcdt;

// La couche interne est définie en rayon; on spécifie le centre et les bords.
 // On enregistre le centre.
 lentille_sommets[i][0]    = centre[0];
 lentille_sommets[i][1]    = centre[1];
 lentille_tex_coords[i][0] = centre[2];
 lentille_tex_coords[i][1] = centre[3];

 // On se fait la bordure
 pos = 2;
 pos_bord   = 0;
 pos_courbe = (nb_pt_courbe-2)<<1; // juste histoire d'en être bien sur...
     for(k=0;k<nb_pt_bordure;k++)
      {lentille_tex_coords[i][pos] = Interpolation_lineaire( bordure[pos_bord+2]
                                                           , centre[2]
                                                           , (courbe_unite[pos_courbe+1]-0.5*val_niv*Text_x()) / (1-val_niv) );
       lentille_sommets[i][pos++] = Interpolation_lineaire( bordure[pos_bord]
                                                          , centre[0]
                                                          , courbe_unite[pos_courbe]);
       lentille_tex_coords[i][pos] = Interpolation_lineaire( bordure[pos_bord+3]
                                                           , centre[3]
                                                           , (courbe_unite[pos_courbe+1]-0.5*val_niv*Text_y()) / (1-val_niv) );
       lentille_sommets[i][pos++] = Interpolation_lineaire( bordure[pos_bord+1]
                                                          , centre[1]
                                                          , courbe_unite[pos_courbe]);
       pos_bord += 4;
      }

} // Fin de la génération de la lentille

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_repere2D_lentille::Changer_coord( alx_point2D &pt
                                         , const double *P1, const double *T1
                                         , const double *P2, const double *T2
                                         , const double *P3, const double *T3 ) const
{alx_point2D pt_esp(pt) // Les coordonnées spatiales
           , p1(P1[0], P1[1])
           , p2(P2[0], P2[1])
           , p3(P3[0], P3[1]);
 // Vérifions qu'on a pas cliqué qur un sommet !!!
 if(pt.equivalent(p1, 0.0000001)) {pt.maj(T1); return;}
 if(pt.equivalent(p2, 0.0000001)) {pt.maj(T1); return;}
 if(pt.equivalent(p3, 0.0000001)) {pt.maj(T1); return;}

 // Passons à la suite...
 alx_vecteur2D v13(p1, p3);
 alx_droite2D d21(p2, p1)
            , d23(p2, p3)
            , dp_v13(pt, v13);
 alx_point2D p21, p23;

  /* //DEBUG
    alx_vecteur2D v12_n( *(d12.directeur) )
                , v13_n( *(d13.directeur) )
                , v23_n(v23);
    v12_n.Normer(1);
    v13_n.Normer(1);
    v23_n.Normer(1);
    if( ( absd(v23_n.produit_scalaire(v12_n)) > 0.95 )
      ||( absd(v23_n.produit_scalaire(v13_n)) > 0.95 ) )
     {p12.maj(p12);}
   // FIN DEBUG*/

 dp_v13.intersection(d21, p21);
 dp_v13.intersection(d23, p23);
 alx_vecteur2D v21n( *(d21.directeur) )
             , v_21_23n(p21, p23)
             , v_2_21(p2, p21)
             , v_21_pt(p21, pt);
 double dist21 = v21n.Norme()
      , dist   = v_21_23n.Norme();
 v21n.Normer(1);
 v_21_23n.Normer(1);

 double alpha21 = v21n.produit_scalaire(v_2_21) / dist21;
 double alpha;

 if(dist==0)
   alpha = 0;
  else alpha = v_21_23n.produit_scalaire( v_21_pt ) / dist;

 double text_p21[2], text_p23[2], text_pt[2];

 Interpolation_lineaire_V(2, T2, T1, alpha21, text_p21);
 Interpolation_lineaire_V(2, T2, T3, alpha21, text_p23);
 Interpolation_lineaire_V(2, text_p21, text_p23, alpha, text_pt);

 pt.maj(text_pt[0], text_pt[1]);
 // A la fin, on est passé des des coordonnées spatiales aux coordonnées texture pour pt ;o)
}
