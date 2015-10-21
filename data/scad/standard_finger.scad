s = 0.001;

length = 100;
width = 25;
depth = 20;
chf_depth = 1;
chf_angle = 45;
cut_pos = 75;
cut_depth = 15;
cut_angle = 90;
cut_tilt = 0;

/* Produces basic box finger */
module basicShape(length, width, depth)
{
	translate([0, -depth/2, 0]) cube([length, depth, width]);
}

/* Produces the chamfering geometry.
 * length -- length of the finger
 * start_depth -- distance from the finger plane at which the chamfer starts
 * angle --- chamfering angle
 */
module chamfering(length, start_depth, angle) {
	size = 1000;

	translate([length, 0, start_depth])
		rotate([0, 90-angle, 0])
			translate([0, 0, size/2])
				cube(size, center = true);
}

/* Produces the geometry for the cutout.
 * cut_pos -- position of the cutout
 * cut_depth -- depth of the cutout
 * cut_angle -- the angle of the cutout
 * cut_tilt -- the tilt of the cutout
 */
module cutout(cut_pos, cut_depth, cut_angle, cut_tilt) {
	size = 1000;

	s_x = tan(cut_angle /2 );

	translate([cut_pos, 0, cut_depth])
		rotate([0, 0, cut_tilt])
			rotate([0, 135, 0])
				translate([0, -size/2, 0])
					scale([s_x, 1, 1])
						cube(size);
}

/* Produces the finger geometry.
 * Parameters...
 */
module finger(length, width, depth, chf_depth, chf_angle, cut_pos, cut_depth, cut_angle, cut_tilt) {
	difference() {
		difference() {
			basicShape(length, width, depth);
			chamfering(length, depth - chf_depth * depth, chf_angle);
		}
		cutout(cut_pos, cut_depth, cut_angle, cut_tilt);
	}
}


scale(s) finger(length, width, depth, chf_depth, chf_angle, cut_pos, cut_depth, cut_angle, cut_tilt);
