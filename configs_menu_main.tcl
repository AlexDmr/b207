# Labels on the zones
automate_main set_delta_for_label 2 7 -85
automate_main set_delta_for_label 3 -40 -90

automate_main set_delta_for_label 6 7 -85
automate_main set_delta_for_label 7 -60 -90

# Normal slices
[automate_main get_thumb_zone] Etirement 1 1
[automate_main get_thumb_zone] Origine 0 0
$poly_pour_main Rotation 0.1

# Medium slices
[automate_main get_thumb_zone] Etirement 1.1 1.1
[automate_main get_thumb_zone] Origine -20 -30
$poly_pour_main Rotation 0.1
# Bigger slices
[automate_main get_thumb_zone] Etirement 1.2 1.2
[automate_main get_thumb_zone] Origine -60 -80
$poly_pour_main Rotation 0.1

# Left hand
[automate_main get_thumb_zone] Origine 320 -135
$poly_pour_main Rotation -0.1
