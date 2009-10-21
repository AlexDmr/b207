#ifndef __LANGUE_NAT_H__
#define __LANGUE_NAT_H__

const unsigned int lang_fr = 1
                 , lang_en = 2
                 , lang_de = 3
                 , lang_it = 4
                 , lang_es = 5
                 , lang_ar = 6
                 , nb_lang = 7;

typedef char* P_char;
const P_char n_def_lang[7] = {"Cette langue n'est pas prise en compte"
                             ,"Aucune description n'a été fournit en français"
                             ,"No description given in english"
                             ,"Keine description in Deutsh"
                             ,"No description"
                             ,"No description"
                             ,"No description"};

#endif