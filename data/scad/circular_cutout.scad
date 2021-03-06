s = 1;

length = 0.1;
width = 0.025;
depth = 0.020;
chfdepth = 0;
chfangle = 45;
tcpoffset = 0.075;
cutpos = 0.0;
cutoffset = 0.0;
cutdepth = 0.01;
cutwidth = 0.02;
cutangle = 0;

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
 * cutpos -- the position of the cutout
 * cutoffset -- the offset along the y axis
 * cutdepth -- the depth of the cutout
 * cutdiameter -- the diameter of the cutout
 */
module cutout(cutpos, cutoffset, cutdepth, cutdiameter, cutangle) {
	size = 1000;

	r = cutdiameter/2;
	dr = 2 * cutdepth * tan(cutangle/2);

	translate([cutpos, cutoffset, 0])
		cylinder(h = 2*cutdepth, r1 = r+dr, r2 = r, $fn = 100, center = true);
}

/* Produces the finger geometry.
 * Parameters...
 */
module finger(length, width, depth, chfdepth, chfangle, cutpos, cutoffset, cutdepth, cutdiameter, cutangle) {
	difference() {
		difference() {
			basicShape(length, width, depth);
			chamfering(length, width - width * chfdepth, chfangle);

echo(depth - chfdepth);
		}
		cutout(cutpos, cutoffset, cutdepth, cutdiameter, cutangle);
	}
}

scale(s)
	finger(length, width, depth, chfdepth, chfangle, length-tcpoffset+cutpos, cutoffset, cutdepth, cutwidth, cutangle);
