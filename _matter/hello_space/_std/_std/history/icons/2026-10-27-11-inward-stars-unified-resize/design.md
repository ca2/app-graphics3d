# Revision 11: inward stars, one master for every size

The hi! image and the main star core/flare geometry remain unchanged from
revision 10. Star groups are translated only: (+55,+80), (-45,+25),
(-65,-60) in the 1280-unit canvas, giving centers (225,260), (805,170),
(1085,1100). The two upper horizontal glow axes are now 90 units apart
instead of 35, and the lower star is farther from the bottom/right edges.

Glow rays continue to the canvas boundaries. Their long gradient starts
below the white core brightness (0.64 opacity before bloom), declining
smoothly through 0.43,0.22,0.07 to 0.008 at the boundary. The established
letter-clearance mask keeps rays behind and clear of letter silhouettes.

The sparse tiny-star field is more visible: 13 small brighter points have
radii 4.6–6.1 units, with the remaining points at 1.7–3.2 units. This lets a
few points remain faintly visible around 64px while naturally fading at
32px and below. Some field positions were moved clear of shifted cores.

There are no optical-size variants in this revision. Every PNG and ICO
frame is an ordinary proportional resize of source.png, rendered once
from source.svg. Historical optical variants remain in revisions 09/10;
obsolete icon-small*.svg files were removed from the active main folder.

PNG exports are standard 8-bit RGBA. ICO frames use those exact pixels in standard 32-bit BGRA/DIB entries with transparency masks, avoiding palette-alpha conversion. The portable `export_ico.py ICON_FOLDER` script is retained here for reproduction.
