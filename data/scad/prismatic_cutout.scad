s = 1;

length = 0.1;
width = 0.025;
depth = 0.020;
chfdepth = 0;
chfangle = 45;
tcpoffset = 0.025;
cutpos = 0.0;
cutdepth = 0.01;
cutangle = 90;
cuttilt = 0;
cutfill = 0;

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
module cutout(cutpos, cutdepth, cutangle, cuttilt, cutfill) {
	size = 1000;

	s_x = tan(cutangle /2);

	difference () {
        translate([cutpos, 0, cutdepth])
            rotate([0, 0, cuttilt])
                scale([s_x, 1, 1])
                    rotate([0, 135, 0])
                        translate([0, -size/2, 0])
                            cube(size);
        translate([0, 0, size/2+cutdepth-cutfill])
            cube(size, center = true);
    }
}

/* Produces the finger geometry.
 * Parameters...
 */
module finger(length, width, depth, chfdepth, chfangle, cutpos, cutdepth, cutangle, cuttilt, cutfill) {
	difference() {
		difference() {
			basicShape(length, width, depth);
			chamfering(length, width - width * chfdepth, chfangle);
		}
		cutout(cutpos, cutdepth, cutangle, cuttilt, cutfill);
	}
}

scale(s)
	finger(length, width, depth, chfdepth, chfangle, length-tcpoffset+cutpos, cutdepth, cutangle, cuttilt, cutfill);
