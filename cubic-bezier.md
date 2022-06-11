# Cubic Bézier

Used in main program for handling curves like 

Per wikipedia:

**B**(t) = (1-t)<sup>3</sup>**P**<sub>0</sub> + 3(1-t)<sup>2</sup>t**P**<sub>1</sub> + 3(1-t)t<sup>2</sup>**P**<sub>2</sub> + t<sup>3</sup>**P**<sub>3</sub>, 0&le;t&le;1

<pre style="font-size:80%; background: white; color: #00C; padding: 0.4em;">
<b>typedef</b> <b>struct</b> _point_S {
    <b class="green">float</b> x, y;
} point;

point bezier(point P[4], <b class="green">float</b> frac) {
    point Q;
    <b class="green">float</b> ifrac = 1-frac;
    <b class="green">float</b> ifrac2 = ifrac*ifrac;
    <b class="green">float</b> frac2 = frac*frac;
    <b class="green">float</b> c0 = ifrac*ifrac2;
    <b class="green">float</b> c1 = 3*ifrac2*frac;
    <b class="green">float</b> c2 = 3*ifrac*frac2;
    <b class="green">float</b> c3 = frac*frac2;
    Q.x = c0*P[0].x + c1*P[1].x + c2*P[2].x + c3*P[3].x;
    Q.y = c0*P[0].y + c1*P[1].y + c2*P[2].y + c3*P[3].y;
    <b>return</b> Q;
}
</pre>

