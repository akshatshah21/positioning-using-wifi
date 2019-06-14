x1 =0.0
y1 =0.0
x2 =3.0
y2 =0.0
x3 =0.0
y3 =3.0

r1 =
r2 =
r3 =

xNr1 = y3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
xNr2 = y2*(r3*r3 - x3*x3 - y3*y3 -r1*r1)
xDr = 2*(y2*x3 - y3*x2)
x = (xNr1 - xNr2) / xDr


yNr1 = x3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
yNr2 = x2*(r3*r3 - x3*x3 - y3*y3 - r1*r1)
yDr = 2*(y3*x2 - y2*x3)

y = (yNr1 - yNr2) / yDr

print('Tag: (' + str(x) + ',' + str(y) + ')')
