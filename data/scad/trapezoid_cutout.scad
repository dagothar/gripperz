s = 1;

length = 0.1;
width = 0.025;
depth = 0.020;
chfdepth = 0;
chfangle = 45;
cutpos = 0.075;
cutdepth = 0.01;
cutwidth = 0.01;
cutangle1 = 10;
cutangle2 = 50;
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

    // todo: cut out the chamfers at ends
	translate([length, 0, start_depth])
		rotate([0, 90-angle, 0])
			translate([0, 0, size/2])
				cube(size, center = true);
}

/* Produces the slot geometry for the cutout.
 * cutdepth -- cutout depth
 * cutwidth -- width of the bottom part of the slot
 * cutangle1 -- angle of the wall closer to the base
 * cutangle2 -- angle of the wall closer to the tip
 */
module slot(cutdepth, cutwidth, cutangle1, cutangle2) {
    size = 1000;
    
    l1 = cutdepth * tan(cutangle1);
    l2 = cutdepth * tan(cutangle2);
    
    w = cutwidth + l1 + l2;
    dw = l2/2 - l1/2;
    
    translate([dw, 0, 0])
        difference() {
            difference() {
                cube([w, 2*cutdepth, size], center = true);
                translate([-cutwidth/2 - l1/2, 0, 0])
                    rotate([0, 0, 90-cutangle1])
                        # translate([0, 0, -size/2]) cube(size);
            };
            translate([cutwidth/2 + l2/2, 0, 0])
                rotate([0, 0, cutangle2])
                    # translate([0, 0, -size/2]) cube(size);
        };
}

/* Produces the geometry for the cutout.
 * cutpos -- position of the cutout
 * cutdepth -- depth of the cutout
 * cutangle -- the angle of the cutout
 * cuttilt -- the tilt of the cutout
 */
module cutout(cutpos, cutdepth, cutwidth, cutangle1, cutangle2, cuttilt) {

	translate([cutpos, 0, 0])
		rotate([0, 0, cuttilt])
			rotate([90, 0, 0])
				slot(cutdepth, cutwidth, cutangle1, cutangle2);
}

/* Produces the finger geometry.
 * Parameters...
 */
module finger(length, width, depth, chfdepth, chfangle, cutpos, cutdepth, cutwidth, cutangle1, cutangle2, cuttilt) {
	difference() {
		difference() {
			basicShape(length, width, depth);
			chamfering(length, width - width * chfdepth, chfangle);
		}
		cutout(cutpos, cutdepth, cutwidth, cutangle1, cutangle2, cuttilt);
	}
}

scale(s)
	finger(length, width, depth, chfdepth, chfangle, cutpos, cutdepth, cutwidth, cutangle1, cutangle2, cuttilt);
