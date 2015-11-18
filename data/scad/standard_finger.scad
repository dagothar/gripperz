s = 1;

length = 0.1;
width = 0.025;
depth = 0.020;
chfdepth = 0;
chfangle = 45;
cutpos = 0.075;
cutdepth = 0.0;
cutangle = 90;
cuttilt = 0;

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
 * cutpos -- position of the cutout
 * cutdepth -- depth of the cutout
 * cutangle -- the angle of the cutout
 * cuttilt -- the tilt of the cutout
 */
module cutout(cutpos, cutdepth, cutangle, cuttilt) {
	size = 1000;

	s_x = tan(cutangle /2);

	translate([cutpos, 0, cutdepth])
		rotate([0, 0, cuttilt])
			rotate([0, 135, 0])
				translate([0, -size/2, 0])
					scale([s_x, 1, 1])
						cube(size);
}

/* Produces the finger geometry.
 * Parameters...
 */
module finger(length, width, depth, chfdepth, chfangle, cutpos, cutdepth, cutangle, cuttilt) {
	difference() {
		difference() {
			basicShape(length, width, depth);
			chamfering(length, width - width * chfdepth, chfangle);

echo(depth - chfdepth);
		}
		cutout(cutpos, cutdepth, cutangle, cuttilt);
	}
}

scale(s)
	finger(length, width, depth, chfdepth, chfangle, length-tcpoffset+cutpos, cutdepth, cutangle, cuttilt);
