if {[info exists noeud_partage]} {} else {source groupe.tcl}
proc etire_fond args {}
N_i_mere Afficher_souris

load FFMPEG_for_TCL.dll
  ffmpeg_init
set img [B_image]
  $img Inverser_y 1
  $noeud_partage Ajouter_fils $img

#cd {C:\Alexandre\Pol\Heaven}
#set id [FFMPEG_Open_video_stream {Heaven.avi}]

cd {C:\Documents and Settings\ademeure\Desktop\TMP\Incomes\videos}
set id [FFMPEG_Open_video_stream {Spaceballs___State_of_the_Art_(Amiga_Demo_Scene).avi}]
FFMPEG_set_Synchronisation_threshold $id 0.5

set t  [FFMPEG_startAcquisition $id]
set tx [FFMPEG_Width $id]
set ty [FFMPEG_Height $id]

set buf [FFMPEG_Get_a_buffer $t]

set rap_img [B_rappel [Interp_TCL]]
set start_ms [N_i_mere ms]; set frame_num 0
set time_frame [expr int(1000/[FFMPEG_getFramerate $id])]
  $rap_img Texte "set ms \[N_i_mere ms\]; set num \[expr int((\$ms - \$start_ms)/\$time_frame)\]; if {\$num != \$frame_num} {set frame_num \$num; FFMPEG_getImage $id $buf; $img maj_raw $tx $ty [GL_rvb] 3 $buf;}"

# Sound
 set sample_rate [FFMPEG_Sound_sample_rate $id]
 #set cb_audio [Get_B207_FMOD_Stream_Info_audio]
 set cb_audio [Get_FFMPEG_FMOD_Stream_Info_audio]
   set canal_audio 0
   set buf_len 8192
   set L_infos_B207_sound [FFMPEG_Info_for_sound_CB $id]
   if {[FFMPEG_Nb_channels $id] == 2} {set mono_stereo [FSOUND_Stereo]} else {set mono_stereo [FSOUND_Mono]}
 
 set B207_audio_stream [N_i_mere Nouveau_flux $cb_audio $canal_audio $buf_len [expr $mono_stereo | [FSOUND_signed] | [FSOUND_16b]] $sample_rate $L_infos_B207_sound]
 FFMPEG_set_Debug_mode $id 0
 FFMPEG_Audio_buffer_size $id
 puts "Info audio :\n  - Sample rate : $sample_rate\n  - Mono([FSOUND_Mono])/Stereo([FSOUND_Stereo]) : $mono_stereo "
  
# Gogogo!!!
N_i_mere abonner_a_fin_simulation [$rap_img Rappel]

proc gogo_video {} {
 global Noeud_scene
 global f_obs
 ci Add_daughters_R [list [CometVideo c_vid n d] [CometVideo c_vid2 n d]]
 #ci Add_daughters_R [list [CometVideo c_vid n d] ]
 $Noeud_scene Ajouter_fils_au_debut $f_obs
 cd {C:\Documents and Settings\ademeure\Desktop\TMP\Incomes\videos}
 c_vid  set_video_source {Spaceballs___State_of_the_Art_(Amiga_Demo_Scene).avi} 0
 cd {C:\Alexandre\Pol\Heaven}
 c_vid2 set_video_source {Heaven.avi} 1
 set prim [[CSS++ cr "#c_vid2->PMs.PM_BIGre"] get_prim_handle]
   $prim Origine 300 300
}

 ci Add_daughters_R [list [CometVideo c_vid n d] [CometVideo c_vid2 n d]]
 cd {C:\Alexandre\Pol\Heaven}
 c_vid2 set_video_source {Heaven.avi} 1


Vos informations de compte TrackMania Forever:

Votre login: ademeure
Votre mot de passe: hehehe
Votre clé: TMNF-MMK7-EDAA-CXHR-M8D
